/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <cstring>
#include "bigimage.h"
#include "ccom/ccom.h"
#include "cpage/cpage.h"

namespace CIF {

BigImage::BigImage(Handle CPage) :
    ccom_(NULL) {
    PAGEINFO info;
    GetPageInfo(CPage, &info);
    setImageName(info.szImageName);
    incline_ = info.Incline2048;
}

BigImage::~BigImage() {
    if (ccom_)
        CCOM_DeleteContainer(ccom_);
}

CCOM_handle BigImage::ccom() const {
    return ccom_;
}

const char * BigImage::imageName() const {
    return image_name_;
}

int BigImage::incline() const {
    return incline_;
}

void BigImage::setCCOM(CCOM_handle ccom) {
    ccom_ = ccom;
}

void BigImage::setImageName(const char * name) {
    strncpy(image_name_, name, CPAGE_MAXNAME - 1);
    image_name_[CPAGE_MAXNAME - 1] = '\0';
}

}
