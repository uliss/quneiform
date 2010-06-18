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

#ifndef CRTFFUNC_H_
#define CRTFFUNC_H_

#include "cfcompat.h"
#include "creatertf.h"
#include "crtfstruct.h"

namespace CIF
{
struct SectorInfo;
}

Bool CheckLines(RECT* Rect, Bool FlagVer, CIF::SectorInfo *SectorInfo);
int16_t GetRealSizeKegl(const char * str, int16_t width, int16_t FontPointSize, int16_t FontNumber);
int16_t GetRealSize(const char* str, int16_t len, int16_t FontSize, int16_t FontNumber,
        int16_t* strHeight);

Bool PageTree(FILE *InFileName, CIF::CRtfPage* RtfPage);

void RtfUnionRect_CRect_CRect(RECT *s1, RECT *s2);
void RtfAssignRect_CRect_Rect16(RECT *s1, Rect16 *s2);
void RtfAssignRect_CRect_CRect(RECT *s1, RECT *s2);

#endif /* CRTFFUNC_H_ */
