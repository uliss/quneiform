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

#ifndef FONT_H_
#define FONT_H_

namespace CIF
{

const int MaxFontSize = 72;
const int DefFontSize = 24;
const int ChangedKeglSize = 6;
const unsigned char TIRE = 0x97; //'-'

typedef short font_number;

enum format_align_t
{
    //- выравниваение по левому краю
    FORMAT_ALIGN_LEFT = 0,
    //- выравнивание по правому краю
    FORMAT_ALIGN_RIGHT = 1,
    //- выравнивание по ширине
    FORMAT_ALIGN_JUSTIFY = 2,
    //- выравнивание по центру
    FORMAT_ALIGN_CENTER = 3,
    //- каждая строка состоит из одного слова, выравнивание левому краю
    FORMAT_ALIGN_ONE = 4,
    //- список
    FORMAT_ALIGN_LIST = 5
};

// Font bits
enum
{
    FORMAT_FONT_SERIF = 1,
    FORMAT_FONT_SANS = 2,
    FORMAT_FONT_BOLD = 4,
    FORMAT_FONT_ITALIC = 16,
    FORMAT_FONT_UNDERLINE = 64,
    // user defined
    FORMAT_FONT_NARROW = 128
};

class KEGL
{
    public:
        short OldKegl;
        short NewKegl;
        short Count;
};

int fontName(font_number fontNumber);

}

#endif /* FONT_H_ */
