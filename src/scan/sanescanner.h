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

#ifndef SANESCANNER_H
#define SANESCANNER_H

#include <vector>
#include <string>

#include "iscanner.h"
#include "common/image.h"

namespace cf {

class SaneScanner : public IScanner
{
public:
    typedef std::vector<std::string> DeviceList;
public:
    SaneScanner();
    ~SaneScanner();

    bool close();
    DeviceList listDevices() const;
    bool open(const std::string& device);
    ImagePtr start();
private:
    bool isOpened() const;
    bool readLine(uchar * buffer, size_t maxSize);
private:
    void * scanner_handle_;
};

}

#endif // SANESCANNER_H
