/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <cassert>
#include <cstddef>

#include "imageformats.h"

static const char * image_formats[] = {
    "bmp", //
    "gif",
    "jpg",
    "png",
    "pnm",
    "tiff",
    "unknown",
    "xpm"
};

namespace cf {

const char *  imageFormatToString(image_format_t f) {
    int idx = f - 1;
    assert(idx >= 0);
    assert(idx < FORMAT_XPM);
    return image_formats[idx];
}

}

