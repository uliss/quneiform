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

#ifndef BLOCKTYPE_H
#define BLOCKTYPE_H

/* do not change values! */
enum BlockType {
    BLOCK_PICTURE      = 0,
    BLOCK_CHAR         = 1,
    BLOCK_LINE         = 2,
    BLOCK_PARAGRAPH    = 3,
    BLOCK_COLUMN       = 4,
    BLOCK_SECTION      = 5,
    BLOCK_LAYOUT_TEXT  = 6,
    BLOCK_LAYOUT_IMAGE = 7,
    BLOCK_LAYOUT_AREA  = 8,
    BLOCK_LAYOUT_TABLE = 9
};

#endif // BLOCKTYPE_H
