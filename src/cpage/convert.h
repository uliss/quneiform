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

#ifndef CONVERT_H
#define CONVERT_H

#include "table.h"
#include "tableclass.h"

void DefConvertInit();
uint32_t DefConvertBlock( uint32_t dwContext,
                          Handle TypeIn, void * lpDataIn, uint32_t SizeIn,
                          Handle TypeOut, void * LpDataOut, uint32_t SizeOut);
uint32_t DefConvertPage( uint32_t dwContext,
                         Handle TypeIn, void * lpDataIn, uint32_t SizeIn,
                         Handle TypeOut, void * LpDataOut, uint32_t SizeOut);

uint32_t TYPE_DESK_to_CPAGE_TABLE(TABLE_DESC * lpDataIn, uint32_t SizeIn, CPAGE_TABLE * LpDataOut, uint32_t SizeOut);
uint32_t CPAGE_TABLE_to_TYPE_DESK( CPAGE_TABLE * lpDataIn, uint32_t SizeIn, TABLE_DESC * LpDataOut, uint32_t SizeOut);
uint32_t TYPE_PICTURE_to_CPAGE_PICTURE(POLY_ * lpDataIn, uint32_t SizeIn, CPAGE_PICTURE * LpDataOut, uint32_t SizeOut);
uint32_t CPAGE_PICTURE_to_TYPE_PICTURE( CPAGE_PICTURE * lpDataIn, uint32_t SizeIn, POLY_ * LpDataOut, uint32_t SizeOut);


#endif // CONVERT_H
