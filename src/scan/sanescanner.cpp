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
#include <boost/current_function.hpp>

#include "sanescanner.h"
#include "common/debug.h"
#include "rdib/ctdib.h"

#define SCANNER_ERROR Debug() << "[Error] " << BOOST_CURRENT_FUNCTION << " "
#define SCANNER_DEBUG Debug() << "[Debug] " << BOOST_CURRENT_FUNCTION << " "

namespace cf {

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
    return true;
}

ImagePtr SaneScanner::start()
{
    if(!isOpened())
        return ImagePtr();

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
    if(!image.CreateDIBBegin(params.pixels_per_line, params.lines, params.depth)) {
        SCANNER_ERROR << "can't create dib\n";
        return ImagePtr();
    }

    if(!image.CreateDIBEnd()) {
        SCANNER_ERROR << "can't finish dib\n";
        return ImagePtr();
    }

    return ImagePtr();
}

bool SaneScanner::isOpened() const
{
    return scanner_handle_ != NULL;
}

}
