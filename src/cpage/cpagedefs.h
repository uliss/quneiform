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
typedef int CDataType;

enum cpage_block_flag_t {
    CPAGE_BLOCK_USER = 0x1,  //выделен пользователем
    CPAGE_BLOCK_UNCERTAIN = 0x2  //грязь или нет ?
};

enum {
    CPAGE_MAXCORNER = 1000
};

#endif // CPAGEDEFS_H
