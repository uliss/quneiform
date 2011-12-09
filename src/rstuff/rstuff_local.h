/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef RSTUFF_LOCAL_H
#define RSTUFF_LOCAL_H

#include "cttypes.h"
#include "rstuff_struct.h"

Bool32 RSTUFF_Init(uint16_t wHeightCode);
Bool32 RSTUFF_Done();

/*  Описание функций  */
Bool32 RSTUFF_RSNormVerify(PRSPreProcessImage);
Bool32 RSTUFF_RSNormRemoveLines(PRSPreProcessImage);
Bool32 RSTUFF_RSLayout(PRSPreProcessImage);

#endif // RSTUFF_LOCAL_H
