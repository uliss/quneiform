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

#ifndef RFRMTDEF_H_
#define RFRMTDEF_H_

#include <cstddef>

namespace CIF {

const char TIRE = 0x97; //'-'

enum rfrmt_fragment_output_t
{
    //Fragment output types
    FOT_SINGLE = 0,
    FOT_FRAME = 1,
    FOT_SINGLE_IN_COLUMN = 2
};

//Fragment types
enum rfrmt_fragment_t
{
    FT_TEXT = 0,
    FT_FRAME = 1,
    FT_TABLE = 2,
    FT_PICTURE = 3
};

//HorizontalColumn types
enum
{
    HC_SingleTerminal = 0,
    HC_AllTerminal = 1,
    HC_FrameAndColumn = 2,
    HC_AllFrame = 3,
    HC_ONLY_PICTURE_TABLE = 4
};

const int MaxFontSize = 72;
const int DefFontSize = 24;
const size_t MAX_BUFFER_SIZE = 2048;
const int ChangedKeglSize = 6;

//       wFont bits
enum
{
    TG_EDW_SERIF = 1, //сериф.
    TG_EDW_GELV = 2, //без сериф.
    TG_EDW_BOLD = 4,
    TG_EDW_ITALIC = 16,
    TG_EDW_UNDERLINE = 64,
    TG_EDW_NARROW = 128,
// user defined
};

const float TwipsToEMU_Koef = (360000 * 2.54) / 1440;
const int K_TwipsInInch = 1440;

// Size A4
const int DefaultWidthPage = 11906;
const int DefaultHeightPage = 16838;

const int DefMargL = 1800; // Left   margin in twips    (the default is 1800).
const int DefMargR = 1800; // Right  margin in twips    (the default is 1800).
const int DefMargT = 1440; // Top    margin in twips    (the default is 1440).
const int DefMargB = 1440; // Bottom margin in twips    (the default is 1440).

enum rfrmt_align_t
{
    RTF_TP_LEFT_ALLIGN = 0, //- выравниваение по левому краю
    RTF_TP_RIGHT_ALLIGN = 1,//- выравнивание по правому краю
    RTF_TP_LEFT_AND_RIGHT_ALLIGN = 2, //- выравнивание по ширине
    RTF_TP_CENTER = 3, //- выравнивание по центру
    RTF_TP_ONE = 4, //- каждая строка состоит из одного слова, выравнивание левому краю
    RTF_TP_TYPE_LINE = 5
//- список
};

enum
{
    NOBOLD = 0x0004,
    NOCURSIV = 0x0008,
    NOSIZE = 0x0020
};

// Formatting Mode
enum rfrmt_mode_t
{
    USE_NONE = 0x0040, // no formatting
    USE_FRAME_AND_COLUMN = 0x0001, // use columns & frames
    USE_FRAME = 0x0002
// use only frames
};

}

#endif /* RFRMTDEF_H_ */
