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

class RtfSectorInfo;

extern uint32_t RtfWriteMode;
extern uint32_t FlagMode;

void Put(const char *Data);
void PutC(char sym);
void PutCom(const char *Command, int32_t value, int16_t space);
void PutChar(uchar sym);
Bool CheckLines(RECT* Rect, Bool FlagVer, RtfSectorInfo *SectorInfo) ;

void RtfUnionRect_CRect_CRect(tagRECT *s1, tagRECT *s2);
void RtfAssignRect_CRect_Rect16(tagRECT *s1, Rect16 *s2);
void RtfCalcRectSizeInTwips(tagRECT *s1, float Twips);
void RtfAssignRect_CRect_CRect(tagRECT *s1, tagRECT *s2);

#endif /* CRTFFUNC_H_ */
