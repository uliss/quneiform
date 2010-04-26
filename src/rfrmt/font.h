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

enum rtf_align_t
{
    //- выравниваение по левому краю
    RTF_TP_LEFT_ALLIGN = 0,
    //- выравнивание по правому краю
    RTF_TP_RIGHT_ALLIGN = 1,
    //- выравнивание по ширине
    RTF_TP_LEFT_AND_RIGHT_ALLIGN = 2,
    //- выравнивание по центру
    RTF_TP_CENTER = 3,
    //- каждая строка состоит из одного слова, выравнивание левому краю
    RTF_TP_ONE = 4,
    //- список
    RTF_TP_TYPE_LINE = 5
};

//       wFont bits
enum
{
    TG_EDW_SERIF = 1, //сериф.
    TG_EDW_GELV = 2, //без сериф.
    TG_EDW_BOLD = 4,
    TG_EDW_ITALIC = 16,
    TG_EDW_UNDERLINE = 64,
    TG_EDW_NARROW = 128
// user defined
};

class KEGL
{
    public:
        short OldKegl;
        short NewKegl;
        short Count;
};

class FONT
{
    public:
        char *family;
        char *name;
        short Bold, Italic, Underline;
};

class FONT_COD
{
    public:
        const char *name;
        const char *family;
};

int fontName(font_number fontNumber);

}

#endif /* FONT_H_ */
