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
#include <list>
#include <boost/current_function.hpp>
#include <boost/scoped_array.hpp>

#include "scan_debug.h"
#include "sanescanner.h"
#include "scanoptioninfo.h"
#include "scanoptionvalue.h"
#include "common/ctdib.h"

#define SCANNER_ERROR_STATUS(status) {\
    if(status != SANE_STATUS_GOOD) \
        SCANNER_ERROR << ": '" << sane_strstatus(status) << "'\n";\
}

namespace cf {

typedef std::vector<uchar> BufferLine;
typedef boost::shared_ptr<BufferLine> BufferLinePtr;
typedef std::vector<BufferLinePtr> Buffer;

static inline uint saneDepthToBit(uint depth)
{
    switch(depth) {
    case 1:
        return 1;
    case 8:
        return 24;
    default:
        return 24;
    }
}

static bool initDIB(CTDIB& image, int width, int height, uint depth)
{

    if(!image.createBegin(width, height, saneDepthToBit(depth)))
        return false;

    if(depth == 1)
        image.makeBlackAndWhitePallete();

    if(!image.createEnd())
        return false;

    return true;
}

static ImagePtr toPointer(const CTDIB& ctdib)
{
    const size_t size = ctdib.dibSize();

    BitmapPtr dib = NULL;
    ctdib.bitmap(&dib);

    uchar * buf = (uchar*) malloc(size);
    memcpy(buf, dib, size);

    Image * i = new Image(buf, size, Image::AllocatorMalloc);
    i->setSize(Size(ctdib.width(), ctdib.height()));
    return ImagePtr(i);
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
                  << "\t stride: \t" << params.bytes_per_line << " bytes\n"
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

IScanner::DeviceList SaneScanner::listDevices() const
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
        cfError(MODULE_SCAN) << sane_strstatus(s);
        return ImagePtr();
    }

    if(params.pixels_per_line <= 0) {
        SCANNER_ERROR << "invalid image width: " << params.pixels_per_line;
        return ImagePtr();
    }

    if(params.lines < -1) {
        SCANNER_ERROR << "invalid image height: " << params.lines;
        return ImagePtr();
    }
    else if(params.lines == -1) {
        return handScannerScan(params.format,
                               params.pixels_per_line,
                               params.bytes_per_line,
                               (uint) params.depth);
    }

    return normalScannerScan(params.format,
                             params.pixels_per_line,
                             params.lines,
                             params.bytes_per_line,
                             (uint) params.depth);
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

#define CHECK_OPTION_INDEX(idx) {\
    if(!idx) {\
        SCANNER_ERROR << "invalid option index: " << idx << "\n";\
        return false;\
    }\
    if(!isOptionSettable(idx)) {\
        SCANNER_ERROR << "read-only option: " << idx << "\n";\
        return false;\
    }\
}

bool SaneScanner::setBackendOption(const std::string &name, bool v)
{
    int option_idx = optionIndex(name);
    CHECK_OPTION_INDEX(option_idx);

    SANE_Int info;
    SANE_Word value = v ? SANE_TRUE : SANE_FALSE;
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_, option_idx, SANE_ACTION_SET_VALUE, &value, &info);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "option '" << name << "'' failed: " << sane_strstatus(rc) << "\n";
        return false;
    }

    if(info == SANE_INFO_RELOAD_OPTIONS) {
        SCANNER_DEBUG << "option reload required\n";
        clearOptions();
        fillDeviceOptions();
    }

    return true;
}

bool SaneScanner::setBackendOption(const std::string& name, int v)
{
    int option_idx = optionIndex(name);
    CHECK_OPTION_INDEX(option_idx);

    SANE_Int info;
    SANE_Word value = v;
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_, option_idx, SANE_ACTION_SET_VALUE, &value, &info);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "option " << name << " failed: " << sane_strstatus(rc) << "\n";
        return false;
    }

    if(info == SANE_INFO_RELOAD_OPTIONS) {
        clearOptions();
        fillDeviceOptions();
    }

    return true;
}

bool SaneScanner::setBackendOption(const std::string& name, float v)
{
    int option_idx = optionIndex(name);
    CHECK_OPTION_INDEX(option_idx);

    SANE_Int info;
    SANE_Word value = SANE_FIX(v);
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_, option_idx, SANE_ACTION_SET_VALUE, &value, &info);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "option " << name << " failed: " << sane_strstatus(rc) << "\n";
        return false;
    }

    if(info == SANE_INFO_RELOAD_OPTIONS) {
        clearOptions();
        fillDeviceOptions();
    }

    return true;
}

bool SaneScanner::setBackendOption(const std::string& name, const std::string& v)
{
    int option_idx = optionIndex(name);
    CHECK_OPTION_INDEX(option_idx);

    const SANE_Option_Descriptor * d = sane_get_option_descriptor((SANE_Handle) scanner_,
                                                                  option_idx);
    if(!d) {
        SCANNER_ERROR << "can't get option descriptor\n";
        return false;
    }

    SANE_Int info;
    std::string value = v;
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_,
                                         option_idx,
                                         SANE_ACTION_SET_VALUE,
                                         (void*) value.c_str(),
                                         &info);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "option " << name << " failed: " << sane_strstatus(rc) << "\n";
        return false;
    }

    if(info == SANE_INFO_RELOAD_OPTIONS) {
        clearOptions();
        fillDeviceOptions();
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

static inline BufferLinePtr makeBufferLine(uchar * data, size_t size)
{
    return BufferLinePtr(new BufferLine(data, data + size));
}

ImagePtr SaneScanner::handScannerScan(int format, int width, int lineByteWidth, uint depth)
{
    if(width <= 0 || lineByteWidth <= 0)
        return ImagePtr();

    const size_t buffer_size = (size_t) lineByteWidth;
    uchar buffer[buffer_size];
    uint height = 0;

    Buffer raw_data;

    while(readLine(buffer, buffer_size)) {
        height++;
        raw_data.push_back(makeBufferLine(buffer, buffer_size));
    }

    if(height == 0) {
        SCANNER_ERROR << "scan error\n";
        return ImagePtr();
    }

    CTDIB image;
    if(!initDIB(image, width, height, (uint) depth)) {
        SCANNER_ERROR << "can't init dib\n";
        return ImagePtr();
    }

    assert(image.lineWidthInBytes() >= buffer_size);

    for(uint i = 0; i < height; i++) {
        void * line = image.lineAt(i);
        if(!line)
            break;

        uchar * data = &(raw_data.at(i)->front());
        memcpy(line, data, buffer_size);
    }

    return toPointer(image);
}

static void setDibLine(int format, int depth, uchar * dest, uchar * buffer, size_t buffer_size)
{
    uint bitDepth = saneDepthToBit(depth);
    switch(format) {
    case SANE_FRAME_GRAY:
        if(bitDepth == 24 && depth == 8) {
            for(size_t i = 0; i < buffer_size; i++) {
                dest[i * 3 + 0] = buffer[i];
                dest[i * 3 + 1] = buffer[i];
                dest[i * 3 + 2] = buffer[i];
            }
        }
        else if(bitDepth == 24 && depth == 16) {
// TODO:
        }
        else if(bitDepth == 1 && depth == 1) {
            memcpy(dest, buffer, buffer_size);
        }
        break;
    case SANE_FRAME_RGB:
        if(bitDepth == 24 && depth == 8) {
            memcpy(dest, buffer, buffer_size);
        }
        break;
    }
}

ImagePtr SaneScanner::normalScannerScan(int format, int width, int height, int lineByteWidth, uint depth)
{
    if(width <= 0 || height <= 0 || lineByteWidth <= 0)
        return ImagePtr();

    CTDIB image;
    if(!initDIB(image, width, height, depth)) {
        SCANNER_ERROR << "can't init dib\n";
        return ImagePtr();
    }

    const size_t buffer_size = (size_t) lineByteWidth;
    uchar buffer[buffer_size];

    uint line_counter = 0;

    try {
        while(readLine(buffer, buffer_size)) {
            void * line = image.lineAt(line_counter);
            if(!line)
                break;

            setDibLine(format, depth, (uchar*) line, buffer, buffer_size);
            line_counter++;
        }
    }
    catch(Exception& e) {
        SCANNER_ERROR << e.what() << "\n";
        return ImagePtr();
    }

    return toPointer(image);
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
    default: {
        std::ostringstream buf;
        buf << "read error: " << sane_strstatus(s);
        SCANNER_ERROR << buf.str() << "\n";
        throw Exception(buf.str());
    }
    }
}

}
