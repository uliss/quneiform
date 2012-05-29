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

#include "sanescanner.h"
#include "common/debug.h"
#include "rdib/ctdib.h"

#define SCANNER_ERROR Debug() << "[Error] " << BOOST_CURRENT_FUNCTION << " "
#define SCANNER_DEBUG Debug() << "[Debug] " << BOOST_CURRENT_FUNCTION << " "

namespace cf {

static void * scan_alloc(uint32_t sz) {
    return calloc(1, sz);
}

static void scan_free(void * mem) {
    free(mem);
}

static void * scan_lock(void * mem) { return mem; }
static void scan_unlock(void*){}

static bool initDIB(CTDIB& image, int width, int height, int depth) {
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

static std::string optionTypeToString(const SANE_Option_Descriptor * d)
{
    switch(d->type) {
    case SANE_TYPE_BOOL:
        return "bool";
    case SANE_TYPE_INT:
        return "int";
    case SANE_TYPE_FIXED:
        return "fixed";
    case SANE_TYPE_STRING:
        return "string";
    case SANE_TYPE_BUTTON:
        return "button";
    case SANE_TYPE_GROUP:
        return "group";
    default:
        return "";
    }
}

static std::string optionUnitToString(SANE_Unit u)
{
    switch(u) {
    case SANE_UNIT_DPI:
        return "dpi";
    case SANE_UNIT_MM:
        return "mm";
    case SANE_UNIT_PERCENT:
        return "%";
    case SANE_UNIT_MICROSECOND:
        return "us";
    case SANE_UNIT_BIT:
        return "bit";
    case SANE_UNIT_PIXEL:
        return "px";
    default:
        return "";
    }
}

SaneScanner::SaneScanner() :
    scanner_handle_(NULL)
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

    sane_close((SANE_Handle)scanner_handle_);
    scanner_handle_ = NULL;
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

    scanner_handle_ = h;

    SCANNER_DEBUG << "scanner opened: '" << device << "'\n";
    return true;
}

ImagePtr SaneScanner::start()
{
    if(!isOpened())
        return ImagePtr();

    printOptions();

    SANE_Status s = sane_start((SANE_Handle) scanner_handle_);
    switch(s) {
        case SANE_STATUS_GOOD:
        break;
    default:
        SCANNER_ERROR << sane_strstatus(s) << "\n";
        return ImagePtr();
    }

    SANE_Parameters params;
    s = sane_get_parameters((SANE_Handle) scanner_handle_, &params);

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
    if(!initDIB(image, params.pixels_per_line, params.lines, params.depth)) {
        SCANNER_ERROR << "can't init dib\n";
        return ImagePtr();
    }

    const size_t buffer_size = params.bytes_per_line;
    uchar buffer[buffer_size];

    assert(image.GetLineWidthInBytes() >= buffer_size);
    int line_counter = 0;

    while(readLine(buffer, buffer_size)) {
        void * line = image.GetPtrToLine(line_counter++);
        memcpy(line, buffer, buffer_size);
        if(!line)
            break;
    }

    return toPointer(image);
}

bool SaneScanner::isOpened() const
{
    return scanner_handle_ != NULL;
}

int SaneScanner::optionCount() const
{
    if(!isOpened())
        return 0;

    SANE_Int total;
    SANE_Int info;
    SANE_Status s = sane_control_option((SANE_Handle) scanner_handle_,
                                        0,
                                        SANE_ACTION_GET_VALUE,
                                        (void*) &total,
                                        &info);

    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR << '\'' << sane_strstatus(s) << "'\n";
        return 0;
    }

    return total;
}

std::string SaneScanner::optionBoolToString(const void * descr, int i) const
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) descr;

    if(d->size > sizeof(SANE_Word))
        return "";

    SANE_Word value;
    SANE_Status s = sane_control_option((SANE_Handle) scanner_handle_, i, SANE_ACTION_GET_VALUE, &value, 0);
    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "can't get option " << i << ": '" << sane_strstatus(s) << "'\n";
        return "error";
    }

    return value == SANE_FALSE ? "no" : "yes";
}

std::string SaneScanner::optionFixedToString(const void *descr, int i) const
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) descr;
    boost::scoped_array<SANE_Fixed> value(new SANE_Fixed[d->size]);
    SANE_Status s = sane_control_option((SANE_Handle) scanner_handle_, i, SANE_ACTION_GET_VALUE, value.get(), NULL);

    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "can't get option " << i << ": '" << sane_strstatus(s) << "'\n";
        return "error";
    }

    std::ostringstream buf;
    buf << SANE_UNFIX(value[0]) << optionUnitToString(d->unit);
    return buf.str();
}

std::string SaneScanner::optionIntToString(const void *descr, int i) const
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) descr;
    boost::scoped_array<SANE_Int> value(new SANE_Int[d->size]);
    SANE_Status s = sane_control_option((SANE_Handle) scanner_handle_, i, SANE_ACTION_GET_VALUE, value.get(), NULL);

    if(s != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "can't get option " << i << ": '" << sane_strstatus(s) << "'\n";
        return "error";
    }

    std::ostringstream buf;
    buf << value[0] << optionUnitToString(d->unit);
    return buf.str();
}

std::string SaneScanner::optionStringToString(const void *descr, int i) const
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) descr;

    if(!d)
        return "";

    if(d->type != SANE_TYPE_STRING)
        return "";

    boost::scoped_array<char> value(new char[d->size]);

    SANE_Status rc = sane_control_option((SANE_Handle) scanner_handle_, i, SANE_ACTION_GET_VALUE, value.get(), NULL);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "can't get option " << i << ": '" << sane_strstatus(rc) << "'\n";
        return "error";
    }

    return value.get();
}

std::string SaneScanner::optionToString(const void *descr, int i) const
{
    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) descr;
    if(!d)
        return "";

    switch(d->type) {
    case SANE_TYPE_BOOL:
        return optionBoolToString(d, i);
    case SANE_TYPE_INT:
        return optionIntToString(d, i);
    case SANE_TYPE_FIXED:
        return optionFixedToString(d, i);
    case SANE_TYPE_STRING:
        return optionStringToString(d, i);
    default:
        return "";
    }
}

void SaneScanner::printOptions() const
{
    if(!isOpened())
        return;

    SANE_Int total = 0;
    SANE_Status rc = sane_control_option((SANE_Handle) scanner_handle_, 0, SANE_ACTION_GET_VALUE, &total, 0);

    if(rc != SANE_STATUS_GOOD) {
        SCANNER_ERROR << "error: " << sane_strstatus(rc) << "\n";
        return;
    }

    for(SANE_Int i = 1; i < total; i++) {
        const SANE_Option_Descriptor * d = sane_get_option_descriptor((SANE_Handle) scanner_handle_, i);
        if(!d)
            continue;

        if(d->type == SANE_TYPE_GROUP || d->type == SANE_TYPE_BUTTON)
            continue;

        printOption(d, i);
    }
}

void SaneScanner::printOption(const void * opt, int i) const
{
    if(!opt)
        return;

    const SANE_Option_Descriptor * d = (const SANE_Option_Descriptor*) opt;

    if(!SANE_OPTION_IS_ACTIVE(d->cap))
        return;

    Debug() << "\t name:        " << d->name << "\n"
            << "\t title:       " << d->title << "\n"
            //<< "\t description: " << d->desc << "\n"
            //<< "\t type:        " << optionTypeToString(d) << "\n"
            << "\t value:       " << optionToString(d, i) << "\n\n";
}

bool SaneScanner::readLine(uchar * buffer, size_t maxSize)
{
    SANE_Status s = SANE_STATUS_GOOD;

    SANE_Int len = 0;
    s = sane_read((SANE_Handle) scanner_handle_, (SANE_Byte*) buffer, maxSize, &len);

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
