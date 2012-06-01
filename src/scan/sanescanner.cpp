/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <sane/sane.h>
#include <sstream>
#include <boost/current_function.hpp>
#include <boost/scoped_array.hpp>

#include "scan_debug.h"
#include "sanescanner.h"
#include "scanoptioninfo.h"
#include "scanoptionvalue.h"
#include "rdib/ctdib.h"

#define SCANNER_ERROR_STATUS(status) {\
    if(status != SANE_STATUS_GOOD) \
        SCANNER_ERROR << ": '" << sane_strstatus(status) << "'\n";\
}

namespace cf {

static void * scan_alloc(uint32_t sz) {
    return calloc(1, sz);
}

static void scan_free(void * mem) {
    free(mem);
}

static void * scan_lock(void * mem) { return mem; }
static void scan_unlock(void*){}

static bool initDIB(CTDIB& image, int width, int height, uint depth) {
    if(!image.SetExternals(scan_alloc, scan_free, scan_lock, scan_unlock))
        return false;

    if(!image.CreateDIBBegin(width, height, depth))
        return false;

    if(!image.CreateDIBEnd())
        return false;

    return true;
}

static ImagePtr toPointer(const CTDIB& dib)
{
    const size_t size = dib.GetImageSizeInBytes();
    uchar * buf = (uchar*) malloc(size);
    memcpy(buf, dib.GetPtrToHeader(), size);
    Image * image = new Image(buf, size, Image::AllocatorMalloc);
    image->setSize(Size(dib.GetImageWidth(), dib.GetImageHeight()));
    return ImagePtr(image);
}

static inline const char * toString(SANE_Frame f)
{
    switch(f) {
    case SANE_FRAME_GRAY:
        return "gray";
    case SANE_FRAME_RGB:
        return "rgb";
    case SANE_FRAME_RED:
        return "red";
    case SANE_FRAME_GREEN:
        return "green";
    case SANE_FRAME_BLUE:
        return "blue";
    default:
        return "unknown";
    }
}

static void printSaneParams(const SANE_Parameters& params)
{
    SCANNER_DEBUG << "boost parameters: \n"
                  << "\t bit depth: \t" << params.depth << "\n"
                  << "\t stride: \t" << params.bytes_per_line << "\n"
                  << "\t lines: \t" << params.lines << "px\n"
                  << "\t width: \t" << params.pixels_per_line << "px\n"
                  << "\t format: \t" << toString(params.format) << "\n";
}

SaneScanner::SaneScanner() :
    scanner_(NULL)
{
    SANE_Int version = 0;
    SANE_Status s = sane_init(&version, NULL);

    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "can't initialize sane library:\n"
                      << "\t" << sane_strstatus(s) << "\n";
    }
    else {
        SCANNER_DEBUG << "version: " << version << "\n";
    }
}

SaneScanner::~SaneScanner()
{
    close();
    sane_exit();
}

bool SaneScanner::close()
{
    if(!isOpened())
        return false;

    sane_close((SANE_Handle)scanner_);
    scanner_ = NULL;
    return true;
}

SaneScanner::DeviceList SaneScanner::listDevices() const
{
    const SANE_Device ** device_list = NULL;
    SANE_Status s = sane_get_devices(&device_list, SANE_TRUE);

    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "get devices failed:\n"
                      << "\t" << sane_strstatus(s) << "\n";
        return DeviceList();
    }

    DeviceList res;

    for(int i = 0; device_list[i] != NULL; i++)
        res.push_back(device_list[i]->name);

    return res;
}

bool SaneScanner::open(const std::string& device)
{
    close();

    SANE_Handle h = NULL;
    SANE_Status s = sane_open(device.c_str(), &h);

    switch(s) {
    case SANE_STATUS_GOOD:
        break;
    default:
        SCANNER_ERROR << "error:\n"
                      << "\t\'" << sane_strstatus(s) << "\', for device: \'"
                      << device << "\'\n";

        return false;
    }

    sane_set_io_mode(h, SANE_TRUE);

    scanner_ = h;

    SCANNER_DEBUG << "scanner opened: '" << device << "'\n";

    fillDeviceOptions();

    return true;
}

ImagePtr SaneScanner::start()
{
    if(!isOpened())
        return ImagePtr();

    SANE_Status s = sane_start((SANE_Handle) scanner_);
    switch(s) {
        case SANE_STATUS_GOOD:
        break;
    default:
        SCANNER_ERROR << sane_strstatus(s) << "\n";
        return ImagePtr();
    }

    SANE_Parameters params;
    s = sane_get_parameters((SANE_Handle) scanner_, &params);

    switch(s) {
        case SANE_STATUS_GOOD:
        printSaneParams(params);
        sleep(1);
        break;
    default:
        Debug() << sane_strstatus(s) << "\n";
        return ImagePtr();
    }

    if(params.pixels_per_line <= 0) {
        SCANNER_ERROR << "invalid image width: " << params.pixels_per_line << "\n";
        return ImagePtr();
    }

    if(params.lines <= 0) {
        SCANNER_ERROR << "invalid image height: " << params.lines << "\n";
        return ImagePtr();
    }

    CTDIB image;
    if(!initDIB(image, params.pixels_per_line, params.lines, (uint) params.depth)) {
        SCANNER_ERROR << "can't init dib\n";
        return ImagePtr();
    }

    const size_t buffer_size = (size_t) params.bytes_per_line;
    uchar buffer[buffer_size];

    assert(image.GetLineWidthInBytes() >= buffer_size);
    uint line_counter = 0;

    while(readLine(buffer, buffer_size)) {
        void * line = image.GetPtrToLine(line_counter++);
        memcpy(line, buffer, buffer_size);
        if(!line)
            break;
    }

    return toPointer(image);
}

Rect SaneScanner::scanArea() const
{
    if(!isOpened())
        return Rect(-1, -1, -1, -1);

    float tl_x = 0;
    float tl_y = 0;
    float br_x = 0;
    float br_y = 0;

    if(option("tl-x", &tl_x) &&
        option("tl-y", &tl_y) &&
        option("br-x", &br_x) &&
        option("br-y", &br_y))
    {
        return Rect(Point(tl_x, tl_y), Point(br_x, br_y));
    }

    return Rect();
}

bool SaneScanner::setScanArea(const Rect& area)
{
    if(!isOpened()) {
        SCANNER_ERROR << "scanner not opened\n";
        return false;
    }

    if(setOption("tl-x", (float) area.leftTop().x()) &&
        setOption("tl-y", (float) area.leftTop().y()) &&
        setOption("br-x", (float) area.rightBottom().x()) &&
        setOption("br-y", (float) area.rightBottom().y()))
    {
        return true;
    }
    else
        return false;
}

bool SaneScanner::setBackendOption(const std::string& name, float v)
{
    int option_idx = optionIndex(name);

    if(!option_idx) {
        SCANNER_ERROR << "invalid option index: " << option_idx << "\n";
        return false;
    }

    if(!isOptionSettable(option_idx))
        return false;

    SANE_Int info;
    SANE_Word value = SANE_FIX(v);
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_, option_idx, SANE_ACTION_SET_VALUE, &value, &info);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR_STATUS(rc);
        return false;
    }

    return true;
}

static inline bool isRealValueType(SANE_Value_Type t)
{
    return t != SANE_TYPE_BUTTON && t != SANE_TYPE_GROUP;
}

static inline ScanOptionInfo::Type SaneTypeToCommon(SANE_Value_Type t)
{
    switch(t) {
    case SANE_TYPE_BOOL:
        return ScanOptionInfo::BOOL;
    case SANE_TYPE_FIXED:
        return ScanOptionInfo::FLOAT;
    case SANE_TYPE_INT:
        return ScanOptionInfo::INT;
    case SANE_TYPE_STRING:
        return ScanOptionInfo::STRING;
    default:
        return ScanOptionInfo::UNKNOWN;
    }
}

static inline ScanOptionInfo::Unit SaneUnitToCommon(SANE_Unit u)
{
    switch(u) {
    case SANE_UNIT_PIXEL:
        return ScanOptionInfo::UNIT_PIXEL;
    case SANE_UNIT_MM:
        return ScanOptionInfo::UNIT_MM;
    case SANE_UNIT_DPI:
        return ScanOptionInfo::UNIT_DPI;
    case SANE_UNIT_PERCENT:
        return ScanOptionInfo::UNIT_PERCENT;
    case SANE_UNIT_BIT:
        return ScanOptionInfo::UNIT_BIT;
    case SANE_UNIT_MICROSECOND:
        return ScanOptionInfo::UNIT_MICROSECOND;
    case SANE_UNIT_NONE:
    default:
        return ScanOptionInfo::UNIT_NONE;
    }
}

static void setInfoOptionRange(const SANE_Option_Descriptor * d, ScanOptionInfo * info)
{
    info->setConstraint(ScanOptionInfo::RANGE);

    if(d->type == SANE_TYPE_INT) {
        info->setRangeMaxValue(d->constraint.range->max);
        info->setRangeMinValue(d->constraint.range->min);
    }

    if(d->type == SANE_TYPE_FIXED) {
        info->setRangeMaxValue((float) SANE_UNFIX(d->constraint.range->max));
        info->setRangeMinValue((float) SANE_UNFIX(d->constraint.range->min));
    }
}

static void setInfoOption(const SANE_Option_Descriptor * d, ScanOptionInfo * info, int idx)
{
    if(!d || !info)
        return;

    info->setTitle(d->title);
    info->setDescription(d->desc);
    info->setType(SaneTypeToCommon(d->type));
    info->setUnit(SaneUnitToCommon(d->unit));
    info->setIndex(idx);

    switch(d->constraint_type) {
    case SANE_CONSTRAINT_RANGE:
        setInfoOptionRange(d, info);
        break;
    case SANE_CONSTRAINT_STRING_LIST:
        info->setConstraint(ScanOptionInfo::LIST);
        info->clearAllowedValues();

        for (size_t i = 0; d->constraint.string_list[i]; ++i)
            info->appendAllowedValue(d->constraint.string_list[i]);

        break;
    case SANE_CONSTRAINT_WORD_LIST: {
        info->setConstraint(ScanOptionInfo::LIST);
        info->clearAllowedValues();

        int len = d->constraint.word_list[0];
        for(int i = 1; i <= len; i++) {
            if(d->type == SANE_TYPE_INT)
                info->appendAllowedValue((int) d->constraint.word_list[i]);
            else if(d->type == SANE_TYPE_FIXED)
                info->appendAllowedValue((float) SANE_UNFIX(d->constraint.word_list[i]));
        }
    }
        break;
    default:
        break;
    }
}

bool SaneScanner::setValueOption(const void * descr, int idx, ScanOptionValue * value)
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor *) descr;

    if(!SANE_OPTION_IS_ACTIVE(d->cap))
        return false;

    char buffer[d->size];

    SANE_Int info = 0;
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_,
                                         idx,
                                         SANE_ACTION_GET_VALUE,
                                         buffer,
                                         &info);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR_STATUS(rc);
        return false;
    }

    switch(d->type) {
    case SANE_TYPE_BOOL: {
        SANE_Bool v = *(SANE_Bool*) buffer;
        value->set((bool) v);
    }
        break;
    case SANE_TYPE_INT:
        value->set((int) *(SANE_Int*) buffer);
        break;
    case SANE_TYPE_FIXED:
        value->set((float) SANE_UNFIX(*(SANE_Word*) buffer));
        break;
    case SANE_TYPE_STRING: {
        value->set(std::string(buffer));
        break;
    }
    default:
        break;
    }

    return true;
}

void SaneScanner::addOption(const void * descr, int idx)
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) descr;

    if(!isRealValueType(d->type))
        return;

    ScanOption option(d->name);
    option.setEnabled(SANE_OPTION_IS_ACTIVE(d->cap));

    setInfoOption(d, option.info(), idx);
    bool rc = setValueOption(d, idx, option.value());

    if(rc)
        opts_.push_back(option);
}

void SaneScanner::fillDeviceOptions()
{
    if(!isOpened())
        return;

    int total = optionCount();

    for(int i = 1; i < total; i++) {
        const SANE_Option_Descriptor * d = sane_get_option_descriptor((SANE_Handle) scanner_, i);
        if(!d)
            continue;

        addOption(d, i);
    }
}

bool SaneScanner::isOpened() const
{
    return scanner_ != NULL;
}

bool SaneScanner::isOptionSettable(int idx) const
{
    const SANE_Option_Descriptor * d = sane_get_option_descriptor((SANE_Handle) scanner_, idx);

    if(!d) {
        SCANNER_ERROR << "can't get option descriptor\n";
        return false;
    }

    if(!SANE_OPTION_IS_SETTABLE(d->cap)) {
        SCANNER_ERROR << "option '" << d->name << "' is not settable\n";
        return false;
    }

    return true;
}

int SaneScanner::optionCount() const
{
    if(!isOpened())
        return 0;

    SANE_Int total;
    SANE_Status s = sane_control_option((SANE_Handle) scanner_, 0, SANE_ACTION_GET_VALUE, &total, 0);

    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR_STATUS(s);
        return 0;
    }

    return total;
}

int SaneScanner::optionIndex(const std::string &name) const
{
    OptionIteratorConst it = findOption(name);

    if(it == opts_.end()) {
        SCANNER_ERROR << "option not found: " << name << "\n";
        return -1;
    }

    return it->info()->index();
}

bool SaneScanner::readLine(uchar * buffer, size_t maxSize)
{
    SANE_Status s = SANE_STATUS_GOOD;

    SANE_Int len = 0;
    s = sane_read((SANE_Handle) scanner_, (SANE_Byte*) buffer, (SANE_Int) maxSize, &len);

    switch(s) {
    case SANE_STATUS_GOOD:
        return true;
    case SANE_STATUS_EOF:
        return false;
    default:
        SCANNER_ERROR << "read error: " << sane_strstatus(s) << "\n";
        return false;
    }
}

}
