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

#ifndef IMAGEFORMATS_H_
#define IMAGEFORMATS_H_

#include "globus.h"

namespace cf
{

enum image_format_t
{
    FORMAT_BMP = 1,
    FORMAT_GIF,
    FORMAT_JPEG,
    FORMAT_PNG,
    FORMAT_PNM,
    FORMAT_TIFF,
    FORMAT_UNKNOWN,
    FORMAT_XPM
};

FUN_EXPO__ const char * imageFormatToString(image_format_t f);

}

#endif /* IMAGEFORMATS_H_ */
