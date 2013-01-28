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

#ifndef CPAGEDEFS_H
#define CPAGEDEFS_H

#include "cttypes.h"

namespace cf {
namespace cpage {
class BackupPage;
class Block;
class Picture;

typedef cf::cpage::BackupPage * PageHandle;
}
}

typedef cf::cpage::PageHandle CPageHandle;
typedef cf::cpage::Block * CBlockHandle;
typedef int32_t CDataType;

enum cpage_block_flag_t {
    CPAGE_BLOCK_USER = 0x1,  //выделен пользователем
    CPAGE_BLOCK_UNCERTAIN = 0x2  //грязь или нет ?
};

enum {
    CPAGE_MAXCORNER = 1000
};

enum
{
    CPAGE_MAXNAME = 260
};

//константы аттрибутов фрагментов (к текстовому фрагменту и к ячейке таблицы) типа: негатив - позитив для представлени
enum block_light_t {
    CPAGE_BLOCK_NEGATIVE = 1,
    CPAGE_BLOCK_POSITIVE = 0
};

//константы ориентации содержимого фрагментов (к текстовому фрагменту и к ячейке таблицы)
enum block_orientation_t {
    CPAGE_ORIENT_LEFTRIGHT = 0,
    CPAGE_ORIENT_UPDOWN    = 1,
    CPAGE_ORIENT_DOWNUP    = 2
};

//Значения нижеследующих констант нельзя менять. Они используются для побитового сравнени
enum {
    VISIBLE_LINE = 0,
    HIDE_LINE    = 1,
    GRAYED_LINE  = 2,
    DOTTED_LINE  = 4,
    DASHED_LINE  = 8,
    DOUBLED_LINE = 16,
    NORMAL_LINE  = 0    // дублирует
};

#endif // CPAGEDEFS_H
