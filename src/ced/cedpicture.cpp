/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#include "cedpicture.h"

namespace CIF
{

CEDPicture::CEDPicture() :
    align_(ED_ALIGN_MIDDLE), number_(0) {
}

ed_align_t CEDPicture::alignment() const {
    return align_;
}

ImagePtr CEDPicture::image() {
    return image_;
}

bool CEDPicture::isPicture() const {
    return true;
}

int CEDPicture::pictureNumber() const {
    return number_;
}

void CEDPicture::setAlignment(ed_align_t align) {
    align_ = align;
}

void CEDPicture::setImage(Image * image) {
    setImage(ImagePtr(image));
}

void CEDPicture::setImage(ImagePtr image) {
    image_ = image;
}

void CEDPicture::setPictureNumber(int number) {
    number_ = number;
}

}
