/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
/*  interface my        */
#include "gystogra.h"
/*  interface our util  */
#include "skew1024.h"
/*---------------------------------------------------------------------------*/
Bool MakeTopBotGysts (Rect16 *pRc, int nRc, int32_t Skew, int MaxSize,
				Un_GYST *pBegGt, Un_GYST *pEndGt)
{
	int MinBeg, MaxBeg, MinEnd, MaxEnd, i, End;
	long dy, ddy;
	int32_t x, yBeg, yEnd;
	int32_t SkewSquar;
	int *pBegSig, *pEndSig;
	SkewSquar = Skew*Skew;
	pBegGt->nElem = nRc;
	pEndGt->nElem = nRc;
	pBegSig = pBegGt->Signal;
	pEndSig = pEndGt->Signal;
	/*  Предельные значения проекций  */
	x    = (pRc[0].left + pRc[0].right + 1)/2;
	dy  =( (-Skew*x+0x200) >> 10 );
	yBeg =  pRc[0].top;
	yEnd =  pRc[0].bottom;
	ddy =( (SkewSquar*yBeg+0x100000) >> 21 );
	yBeg += dy;
	yBeg -= ddy;
	ddy =( (SkewSquar*yEnd+0x100000) >> 21 );
	yEnd += dy;
	yEnd -= ddy;
	MinBeg = yBeg;
	MaxBeg = yBeg;
	MinEnd = yEnd;
	MaxEnd = yEnd;
	for (i=1; i<nRc; i++)
	{
		x    = (pRc[i].left + pRc[i].right + 1)/2;
		dy  =( (-Skew*x+0x200) >> 10 );
		yBeg =  pRc[i].top;
		yEnd =  pRc[i].bottom;
		ddy =( (SkewSquar*yBeg+0x100000) >> 21 );
		yBeg += dy;
		yBeg -= ddy;
		ddy =( (SkewSquar*yEnd+0x100000) >> 21 );
		yEnd += dy;
		yEnd -= ddy;
		if (MinBeg > yBeg) MinBeg = yBeg;
		if (MaxBeg < yBeg) MaxBeg = yBeg;
		if (MinEnd > yEnd) MinEnd = yEnd;
		if (MaxEnd < yEnd) MaxEnd = yEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	pBegGt->Shift = MinBeg;
	pBegGt->End   = MaxBeg - MinBeg;
	pEndGt->Shift = MinEnd;
	pEndGt->End   = MaxEnd - MinEnd;
	End = pBegGt->End;
	if (End < pEndGt->End) End = pEndGt->End;
	for (i=0; i<=End; i++)
	{
		pBegSig[i] = 0;
		pEndSig[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		x    = (pRc[i].left + pRc[i].right + 1)/2;
		dy  =( (-Skew*x+0x200) >> 10 );
		yBeg =  pRc[i].top;
		yEnd =  pRc[i].bottom;
		ddy =( (SkewSquar*yBeg+0x100000) >> 21 );
		yBeg += dy;
		yBeg -= ddy;
		ddy =( (SkewSquar*yEnd+0x100000) >> 21 );
		yEnd += dy;
		yEnd -= ddy;
		pBegSig[yBeg - MinBeg]++;
		pEndSig[yEnd - MinEnd]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MakeLefRigGysts (Rect16 *pRc, int nRc, int32_t Skew, int MaxSize,
				Un_GYST *pBegGt, Un_GYST *pEndGt)
{
	int MinBeg, MaxBeg, MinEnd, MaxEnd, i, End;
	long dx, ddx;
	int32_t y, xBeg, xEnd;
	int32_t SkewSquar;
	int *pBegSig, *pEndSig;
	SkewSquar = Skew*Skew;
	pBegGt->nElem = nRc;
	pEndGt->nElem = nRc;
	pBegSig = pBegGt->Signal;
	pEndSig = pEndGt->Signal;
	/*  Предельные значения проекций  */
	y    = (pRc[0].top + pRc[0].bottom + 1)/2;
	dx = ( (-Skew*y+0x200) >> 10 );
	xBeg =  pRc[0].left;
	xEnd =  pRc[0].right;
	ddx =( (SkewSquar*xBeg+0x100000) >> 21 );
	xBeg -= dx;
	xBeg -= ddx;
	ddx =( (SkewSquar*xEnd+0x100000) >> 21 );
	xEnd -= dx;
	xEnd -= ddx;
	MinBeg = xBeg;
	MaxBeg = xBeg;
	MinEnd = xEnd;
	MaxEnd = xEnd;
	for (i=1; i<nRc; i++)
	{
		y    = (pRc[i].top + pRc[i].bottom + 1)/2;
		dx = ( (-Skew*y+0x200) >> 10 );
		xBeg =  pRc[i].left;
		xEnd =  pRc[i].right;
		ddx =( (SkewSquar*xBeg+0x100000) >> 21 );
		xBeg -= dx;
		xBeg -= ddx;
		ddx =( (SkewSquar*xEnd+0x100000) >> 21 );
		xEnd -= dx;
		xEnd -= ddx;
		if (MinBeg > xBeg) MinBeg = xBeg;
		if (MaxBeg < xBeg) MaxBeg = xBeg;
		if (MinEnd > xEnd) MinEnd = xEnd;
		if (MaxEnd < xEnd) MaxEnd = xEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	pBegGt->Shift = MinBeg;
	pBegGt->End   = MaxBeg - MinBeg;
	pEndGt->Shift = MinEnd;
	pEndGt->End   = MaxEnd - MinEnd;
	End = pBegGt->End;
	if (End < pEndGt->End) End = pEndGt->End;
	for (i=0; i<=End; i++)
	{
		pBegSig[i] = 0;
		pEndSig[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		y    = (pRc[i].top + pRc[i].bottom + 1)/2;
		dx = ( (-Skew*y+0x200) >> 10 );
		xBeg =  pRc[i].left;
		xEnd =  pRc[i].right;
		ddx =( (SkewSquar*xBeg+0x100000) >> 21 );
		xBeg -= dx;
		xBeg -= ddx;
		ddx =( (SkewSquar*xEnd+0x100000) >> 21 );
		xEnd -= dx;
		xEnd -= ddx;
		pBegSig[xBeg - MinBeg]++;
		pEndSig[xEnd - MinEnd]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MakeTopMidBotGysts (Rect16 *pRc, int nRc, int32_t Skew, int MaxSize,
				Un_GYST *pBegGt, Un_GYST *pMidGt, Un_GYST *pEndGt)
{
	int MinBeg, MaxBeg, MinMid, MaxMid, MinEnd, MaxEnd, i, End;
	long dy, ddy;
	int32_t x, yBeg, yMid, yEnd;
	int32_t SkewSquar;
	int *pBegSig, *pMidSig, *pEndSig;
	SkewSquar = Skew*Skew;
	pBegGt->nElem = nRc;
	pMidGt->nElem = nRc;
	pEndGt->nElem = nRc;
	pBegSig = pBegGt->Signal;
	pMidSig = pMidGt->Signal;
	pEndSig = pEndGt->Signal;
	/*  Предельные значения проекций  */
	x    = (pRc[0].left + pRc[0].right + 1)/2;
	dy  =( (-Skew*x+0x200) >> 10 );
	yBeg =  pRc[0].top;
	yMid = (pRc[0].top  + pRc[0].bottom + 1)/2;
	yEnd =  pRc[0].bottom;
	ddy =( (SkewSquar*yBeg+0x100000) >> 21 );
	yBeg += dy;
	yBeg -= ddy;
	ddy =( (SkewSquar*yMid+0x100000) >> 21 );
	yMid += dy;
	yMid -= ddy;
	ddy =( (SkewSquar*yEnd+0x100000) >> 21 );
	yEnd += dy;
	yEnd -= ddy;
	MinBeg = yBeg;
	MaxBeg = yBeg;
	MinMid = yMid;
	MaxMid = yMid;
	MinEnd = yEnd;
	MaxEnd = yEnd;
	for (i=1; i<nRc; i++)
	{
		x    = (pRc[i].left + pRc[i].right + 1)/2;
		dy  =( (-Skew*x+0x200) >> 10 );
		yBeg =  pRc[i].top;
		yMid = (pRc[i].top  + pRc[i].bottom + 1)/2;
		yEnd =  pRc[i].bottom;
		ddy =( (SkewSquar*yBeg+0x100000) >> 21 );
		yBeg += dy;
		yBeg -= ddy;
		ddy =( (SkewSquar*yMid+0x100000) >> 21 );
		yMid += dy;
		yMid -= ddy;
		ddy =( (SkewSquar*yEnd+0x100000) >> 21 );
		yEnd += dy;
		yEnd -= ddy;
		if (MinBeg > yBeg) MinBeg = yBeg;
		if (MaxBeg < yBeg) MaxBeg = yBeg;
		if (MinMid > yMid) MinMid = yMid;
		if (MaxMid < yMid) MaxMid = yMid;
		if (MinEnd > yEnd) MinEnd = yEnd;
		if (MaxEnd < yEnd) MaxEnd = yEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxMid-MinMid>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	pBegGt->Shift = MinBeg;
	pBegGt->End   = MaxBeg - MinBeg;
	pMidGt->Shift = MinMid;
	pMidGt->End   = MaxMid - MinMid;
	pEndGt->Shift = MinEnd;
	pEndGt->End   = MaxEnd - MinEnd;
	End = pBegGt->End;
	if (End < pMidGt->End) End = pMidGt->End;
	if (End < pEndGt->End) End = pEndGt->End;
	for (i=0; i<=End; i++)
	{
		pBegSig[i] = 0;
		pMidSig[i] = 0;
		pEndSig[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		x    = (pRc[i].left + pRc[i].right + 1)/2;
		dy  =( (-Skew*x+0x200) >> 10 );
		yBeg =  pRc[i].top;
		yMid = (pRc[i].top  + pRc[i].bottom + 1)/2;
		yEnd =  pRc[i].bottom;
		ddy =( (SkewSquar*yBeg+0x100000) >> 21 );
		yBeg += dy;
		yBeg -= ddy;
		ddy =( (SkewSquar*yMid+0x100000) >> 21 );
		yMid += dy;
		yMid -= ddy;
		ddy =( (SkewSquar*yEnd+0x100000) >> 21 );
		yEnd += dy;
		yEnd -= ddy;
		pBegSig[yBeg - MinBeg]++;
		pMidSig[yMid - MinMid]++;
		pEndSig[yEnd - MinEnd]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MakeLefMidRigGysts (Rect16 *pRc, int nRc, int32_t Skew, int MaxSize,
				Un_GYST *pBegGt, Un_GYST *pMidGt, Un_GYST *pEndGt)
{
	int MinBeg, MaxBeg, MinMid, MaxMid, MinEnd, MaxEnd, i, End;
	long dx, ddx;
	int32_t y, xBeg, xMid, xEnd;
	int32_t SkewSquar;
	int *pBegSig, *pMidSig, *pEndSig;
	SkewSquar = Skew*Skew;
	pBegGt->nElem = nRc;
	pMidGt->nElem = nRc;
	pEndGt->nElem = nRc;
	pBegSig = pBegGt->Signal;
	pMidSig = pMidGt->Signal;
	pEndSig = pEndGt->Signal;
	/*  Предельные значения проекций  */
	y    = (pRc[0].top + pRc[0].bottom + 1)/2;
	dx = ( (-Skew*y+0x200) >> 10 );
	xBeg =  pRc[0].left;
	xMid = (pRc[0].left + pRc[0].right + 1)/2;
	xEnd =  pRc[0].right;
	ddx =( (SkewSquar*xBeg+0x100000) >> 21 );
	xBeg -= dx;
	xBeg -= ddx;
	ddx =( (SkewSquar*xMid+0x100000) >> 21 );
	xMid -= dx;
	xMid -= ddx;
	ddx =( (SkewSquar*xEnd+0x100000) >> 21 );
	xEnd -= dx;
	xEnd -= ddx;
	MinBeg = xBeg;
	MaxBeg = xBeg;
	MinMid = xMid;
	MaxMid = xMid;
	MinEnd = xEnd;
	MaxEnd = xEnd;
	for (i=1; i<nRc; i++)
	{
		y    = (pRc[i].top + pRc[i].bottom + 1)/2;
		dx = ( (-Skew*y+0x200) >> 10 );
		xBeg =  pRc[i].left;
		xMid = (pRc[i].left + pRc[i].right + 1)/2;
		xEnd =  pRc[i].right;
		ddx =( (SkewSquar*xBeg+0x100000) >> 21 );
		xBeg -= dx;
		xBeg -= ddx;
		ddx =( (SkewSquar*xMid+0x100000) >> 21 );
		xMid -= dx;
		xMid -= ddx;
		ddx =( (SkewSquar*xEnd+0x100000) >> 21 );
		xEnd -= dx;
		xEnd -= ddx;
		if (MinBeg > xBeg) MinBeg = xBeg;
		if (MaxBeg < xBeg) MaxBeg = xBeg;
		if (MinMid > xMid) MinMid = xMid;
		if (MaxMid < xMid) MaxMid = xMid;
		if (MinEnd > xEnd) MinEnd = xEnd;
		if (MaxEnd < xEnd) MaxEnd = xEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxMid-MinMid>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	pBegGt->Shift = MinBeg;
	pBegGt->End   = MaxBeg - MinBeg;
	pMidGt->Shift = MinMid;
	pMidGt->End   = MaxMid - MinMid;
	pEndGt->Shift = MinEnd;
	pEndGt->End   = MaxEnd - MinEnd;
	End = pBegGt->End;
	if (End < pMidGt->End) End = pMidGt->End;
	if (End < pEndGt->End) End = pEndGt->End;
	for (i=0; i<=End; i++)
	{
		pBegSig[i] = 0;
		pMidSig[i] = 0;
		pEndSig[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		y    = (pRc[i].top + pRc[i].bottom + 1)/2;
		dx = ( (-Skew*y+0x200) >> 10 );
		xBeg =  pRc[i].left;
		xMid = (pRc[i].left + pRc[i].right + 1)/2;
		xEnd =  pRc[i].right;
		ddx =( (SkewSquar*xBeg+0x100000) >> 21 );
		xBeg -= dx;
		xBeg -= ddx;
		ddx =( (SkewSquar*xMid+0x100000) >> 21 );
		xMid -= dx;
		xMid -= ddx;
		ddx =( (SkewSquar*xEnd+0x100000) >> 21 );
		xEnd -= dx;
		xEnd -= ddx;
		pBegSig[xBeg - MinBeg]++;
		pMidSig[xMid - MinMid]++;
		pEndSig[xEnd - MinEnd]++;
	}
	return TRUE;
}
int ScoreComp (const Rect16 *pRcReg, const int32_t Skew, const Rect16 *pRc, const int nRc)
{
	int i, k;
	Point32 PosIdeal;
	k = 0;
	for (i=0; i<nRc; i++)
	{
		if (pRc[i].right - pRc[i].left < 2)
			continue;
		if (pRc[i].right - pRc[i].left > 100)
			continue;
		if (pRc[i].bottom - pRc[i].top < 2)
			continue;
		if (pRc[i].bottom - pRc[i].top > 100)
			continue;
		PosIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		PosIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (PosIdeal, -Skew);
		if (PosIdeal.x > pRcReg->right)
			continue;
		if (PosIdeal.x < pRcReg->left)
			continue;
		if (PosIdeal.y > pRcReg->bottom)
			continue;
		if (PosIdeal.y < pRcReg->top)
			continue;
		k++;
	}
	return k;
}
/*---------------------------------------------------------------------------*/
void MakeNormVertGyst (const Rect16 *pRcReg, const int32_t Skew, const Rect16 *pRc, const int nRc, int *Sig)
{
	int i, k;
	Point32 BegDirIdeal;
	Point32 EndDirIdeal;
	for (i=0; i<nRc; i++)
	{
		if (pRc[i].right - pRc[i].left < 2)
			continue;
		if (pRc[i].right - pRc[i].left > 100)
			continue;
		if (pRc[i].bottom - pRc[i].top < 2)
			continue;
		if (pRc[i].bottom - pRc[i].top > 100)
			continue;
		BegDirIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		BegDirIdeal.y = pRc[i].top;
		Deskew (BegDirIdeal, -Skew);
		if (BegDirIdeal.x > pRcReg->right)
			continue;
		if (BegDirIdeal.x < pRcReg->left)
			continue;
		if (BegDirIdeal.y >= pRcReg->bottom)
			continue;
		if (BegDirIdeal.y < pRcReg->top)
			BegDirIdeal.y = pRcReg->top;
		EndDirIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		EndDirIdeal.y = pRc[i].bottom;
		Deskew (EndDirIdeal, -Skew);
		if (EndDirIdeal.y <= pRcReg->top)
			continue;
		if (EndDirIdeal.y > pRcReg->bottom)
			EndDirIdeal.y = pRcReg->bottom;
		for (k=BegDirIdeal.y; k<=EndDirIdeal.y; k++)
			Sig[k - pRcReg->top]++;
	}
}
/*---------------------------------------------------------------------------*/
Bool MakeVertGysts (Rect16 *pRc, int nRc, int32_t Skew, int Amnist, int MaxSize, Un_GYST *pVerGt, int *pWhatDo)
{
	int MinBeg, MaxBeg, MinEnd, MaxEnd, CurBeg, CurEnd, i, End, k, iFirst;
	Point32 BegDirIdeal;
	Point32 EndDirIdeal;
	iFirst = -1;
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		iFirst = i;
		break;
	}
	if (iFirst==-1)
		return FALSE;
	/*  Предельные значения проекций  */
	BegDirIdeal.x = (int)(.5 * (pRc[iFirst].left + pRc[iFirst].right + 1));
	BegDirIdeal.y = pRc[iFirst].top;
	Deskew (BegDirIdeal, -Skew);
	MinBeg = BegDirIdeal.y;
	MaxBeg = BegDirIdeal.y;
	EndDirIdeal.x = (int)(.5 * (pRc[iFirst].left + pRc[iFirst].right + 1));
	EndDirIdeal.y = pRc[iFirst].bottom;
	Deskew (EndDirIdeal, -Skew);
	MinEnd = EndDirIdeal.y;
	MaxEnd = EndDirIdeal.y;
	for (i=iFirst+1; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		BegDirIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		BegDirIdeal.y = pRc[i].top;
		Deskew (BegDirIdeal, -Skew);
		CurBeg = BegDirIdeal.y;
		EndDirIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		EndDirIdeal.y = pRc[i].bottom;
		Deskew (EndDirIdeal, -Skew);
		CurEnd = EndDirIdeal.y;
		if (MinBeg > CurBeg) MinBeg = CurBeg;
		if (MaxBeg < CurBeg) MaxBeg = CurBeg;
		if (MinEnd > CurEnd) MinEnd = CurEnd;
		if (MaxEnd < CurEnd) MaxEnd = CurEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	if (MinBeg>MinEnd)
		return FALSE;
	if (MaxBeg>MaxEnd)
		return FALSE;
	pVerGt->Shift = MinBeg;
	pVerGt->End   = MaxEnd - MinBeg;
	pVerGt->nElem = nRc;
	End = pVerGt->End;
	for (i=0; i<=End; i++)
	{
		pVerGt->Signal[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		BegDirIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		BegDirIdeal.y = pRc[i].top;
		Deskew (BegDirIdeal, -Skew);
		CurBeg = BegDirIdeal.y;
		EndDirIdeal.x = (int)(.5 * (pRc[i].left + pRc[i].right + 1));
		EndDirIdeal.y = pRc[i].bottom;
		Deskew (EndDirIdeal, -Skew);
		CurEnd = EndDirIdeal.y;
		for (k=CurBeg + Amnist; k<=CurEnd - Amnist; k++)
			pVerGt->Signal[k - MinBeg]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
void MakeNormHoriGyst (const Rect16 *pRcReg, const int32_t Skew, const Rect16 *pRc, const int nRc, int *Sig)
{
	int i, k;
	Point32 BegDirIdeal;
	Point32 EndDirIdeal;
	for (i=0; i<nRc; i++)
	{
		if (pRc[i].right - pRc[i].left < 2)
			continue;
		if (pRc[i].right - pRc[i].left > 100)
			continue;
		if (pRc[i].bottom - pRc[i].top < 2)
			continue;
		if (pRc[i].bottom - pRc[i].top > 100)
			continue;
		BegDirIdeal.x = pRc[i].left;
		BegDirIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (BegDirIdeal, -Skew);
		if (BegDirIdeal.y > pRcReg->bottom)
			continue;
		if (BegDirIdeal.y < pRcReg->top)
			continue;
		if (BegDirIdeal.x >= pRcReg->right)
			continue;
		if (BegDirIdeal.x < pRcReg->left)
			BegDirIdeal.x = pRcReg->left;
		EndDirIdeal.x = pRc[i].right;
		EndDirIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (EndDirIdeal, -Skew);
		if (EndDirIdeal.x <= pRcReg->left)
			continue;
		if (EndDirIdeal.x > pRcReg->right)
			EndDirIdeal.x = pRcReg->right;
		for (k=BegDirIdeal.x; k<=EndDirIdeal.x; k++)
			Sig[k - pRcReg->left]++;
	}
}
/*---------------------------------------------------------------------------*/
Bool MakeHoriGysts (Rect16 *pRc, int nRc, int32_t Skew, int MaxSize, Un_GYST *pHorGt, int *pWhatDo)
{
	int MinBeg, MaxBeg, MinEnd, MaxEnd, CurBeg, CurEnd, i, End, k, iFirst;
	Point32 BegDirIdeal;
	Point32 EndDirIdeal;
	iFirst = -1;
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		iFirst = i;
		break;
	}
	if (iFirst==-1)
		return FALSE;
	/*  Предельные значения проекций  */
	BegDirIdeal.x = pRc[iFirst].left;
	BegDirIdeal.y = (int)(.5 * (pRc[iFirst].top + pRc[iFirst].bottom + 1));
	Deskew (BegDirIdeal, -Skew);
	MinBeg = BegDirIdeal.x;
	MaxBeg = BegDirIdeal.x;
	EndDirIdeal.x = pRc[iFirst].right;
	EndDirIdeal.y = (int)(.5 * (pRc[iFirst].top + pRc[iFirst].bottom + 1));
	Deskew (EndDirIdeal, -Skew);
	MinEnd = EndDirIdeal.x;
	MaxEnd = EndDirIdeal.x;
	for (i=iFirst+1; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		BegDirIdeal.x = pRc[i].left;
		BegDirIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (BegDirIdeal, -Skew);
		CurBeg = BegDirIdeal.x;
		EndDirIdeal.x = pRc[i].right;
		EndDirIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (EndDirIdeal, -Skew);
		CurEnd = EndDirIdeal.x;
		if (MinBeg > CurBeg) MinBeg = CurBeg;
		if (MaxBeg < CurBeg) MaxBeg = CurBeg;
		if (MinEnd > CurEnd) MinEnd = CurEnd;
		if (MaxEnd < CurEnd) MaxEnd = CurEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	if (MinBeg>MinEnd)
		return FALSE;
	if (MaxBeg>MaxEnd)
		return FALSE;
	pHorGt->Shift = MinBeg;
	pHorGt->End   = MaxEnd - MinBeg;
	pHorGt->nElem = nRc;
	End = pHorGt->End;
	for (i=0; i<=End; i++)
	{
		pHorGt->Signal[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		BegDirIdeal.x = pRc[i].left;
		BegDirIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (BegDirIdeal, -Skew);
		CurBeg = BegDirIdeal.x;
		EndDirIdeal.x = pRc[i].right;
		EndDirIdeal.y = (int)(.5 * (pRc[i].top + pRc[i].bottom + 1));
		Deskew (EndDirIdeal, -Skew);
		CurEnd = EndDirIdeal.x;
		for (k=CurBeg; k<=CurEnd; k++)
			pHorGt->Signal[k - MinBeg]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MakeHoriSrez (Rect16 *pRcId, int nRc, int BegSrez, int EndSrez, int MaxSize, Un_GYST *pHorGt, int *pWhatDo)
{
	int MinBeg, MaxBeg, MinEnd, MaxEnd, CurBeg, CurEnd, i, End, k, iFirst;
	iFirst = -1;
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		iFirst = i;
		break;
	}
	if (iFirst==-1)
		return FALSE;
	/*  Предельные значения проекций  */
	MinBeg = pRcId[iFirst].left;
	MaxBeg = MinBeg;
	MinEnd = pRcId[iFirst].right;
	MaxEnd = MinEnd;
	for (i=iFirst+1; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		CurBeg = pRcId[i].left;
		CurEnd = pRcId[i].right;
		if (MinBeg > CurBeg) MinBeg = CurBeg;
		if (MaxBeg < CurBeg) MaxBeg = CurBeg;
		if (MinEnd > CurEnd) MinEnd = CurEnd;
		if (MaxEnd < CurEnd) MaxEnd = CurEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	if (MinBeg>MinEnd)
		return FALSE;
	if (MaxBeg>MaxEnd)
		return FALSE;
	pHorGt->Shift = MinBeg;
	pHorGt->End   = MaxEnd - MinBeg;
	pHorGt->nElem = nRc;
	End = pHorGt->End;
	for (i=0; i<=End; i++)
	{
		pHorGt->Signal[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		if (pRcId[i].top>=EndSrez)
			continue;
		if (pRcId[i].bottom<=BegSrez)
			continue;
		CurBeg = pRcId[i].left;
		CurEnd = pRcId[i].right;
		for (k=CurBeg; k<=CurEnd; k++)
			pHorGt->Signal[k - MinBeg]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool MakeVertSrez (Rect16 *pRcId, int nRc, int BegSrez, int EndSrez, int MaxSize, Un_GYST *pVerGt, int *pWhatDo)
{
	int MinBeg, MaxBeg, MinEnd, MaxEnd, CurBeg, CurEnd, i, End, k, iFirst;
	iFirst = -1;
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		iFirst = i;
		break;
	}
	if (iFirst==-1)
		return FALSE;
	/*  Предельные значения проекций  */
	MinBeg = pRcId[iFirst].top;
	MaxBeg = MinBeg;
	MinEnd = pRcId[iFirst].bottom;
	MaxEnd = MinEnd;
	for (i=iFirst+1; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		CurBeg = pRcId[i].top;
		CurEnd = pRcId[i].bottom;
		if (MinBeg > CurBeg) MinBeg = CurBeg;
		if (MaxBeg < CurBeg) MaxBeg = CurBeg;
		if (MinEnd > CurEnd) MinEnd = CurEnd;
		if (MaxEnd < CurEnd) MaxEnd = CurEnd;
	}
	if (MaxBeg-MinBeg>=MaxSize)
		return FALSE;
	if (MaxEnd-MinEnd>=MaxSize)
		return FALSE;
	if (MinBeg>MinEnd)
		return FALSE;
	if (MaxBeg>MaxEnd)
		return FALSE;
	pVerGt->Shift = MinBeg;
	pVerGt->End   = MaxEnd - MinBeg;
	pVerGt->nElem = nRc;
	End = pVerGt->End;
	for (i=0; i<=End; i++)
	{
		pVerGt->Signal[i] = 0;
	}
	for (i=0; i<nRc; i++)
	{
		if (pWhatDo[i]!=1)
			continue;
		if (pRcId[i].left>=EndSrez)
			continue;
		if (pRcId[i].right<=BegSrez)
			continue;
		CurBeg = pRcId[i].top;
		CurEnd = pRcId[i].bottom;
		for (k=CurBeg; k<=CurEnd; k++)
			pVerGt->Signal[k - MinBeg]++;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool FindNextHole (Un_GYST *pDarkGt, int Beg, int End, int *NewBeg, int *NewEnd)
{
	int i;
	Bool ret;
	if (Beg>End)
		return FALSE;
	ret = FALSE;
	for (i=Beg; i<=End; i++)
	{
		if (i<pDarkGt->Shift)
			continue;
		if (i>pDarkGt->Shift+pDarkGt->End)
			break;
		if (pDarkGt->Signal[i-pDarkGt->Shift]>0)
			continue;
		*NewBeg = i;
		ret = TRUE;
		break;
	}
	if (!ret)
		return FALSE;
	for (i=*NewBeg; i<=End; i++)
	{
		if (i>pDarkGt->Shift+pDarkGt->End)
			break;
		if (pDarkGt->Signal[i-pDarkGt->Shift]>0)
			break;
		*NewEnd = i;
		continue;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
Bool FindNextHoleWithBound (int MaxSig, Un_GYST *pDarkGt, int Beg, int End, int *NewBeg, int *NewEnd, int MinLent)
{
	int i, Beg_C, End_C;
	Bool ret;
	if (Beg > End)
		return FALSE;
	Beg_C = Beg;
	if (Beg_C < pDarkGt->Shift)
		Beg_C = pDarkGt->Shift;
	End_C = End;
	if (End_C > pDarkGt->Shift+pDarkGt->End)
		End_C = pDarkGt->Shift+pDarkGt->End;
	if (Beg_C > End_C)
		return FALSE;
	while (Beg_C <= End_C)
	{
		ret = FALSE;
		for (i=Beg_C; i<=End_C; i++)
		{
			if (pDarkGt->Signal[i-pDarkGt->Shift]>MaxSig)
				continue;
			*NewBeg = i;
			ret = TRUE;
			break;
		}
		if (!ret)
			return FALSE;
		*NewEnd = *NewBeg;
		for (i=*NewBeg; i<=End_C; i++)
		{
			if (pDarkGt->Signal[i-pDarkGt->Shift]>MaxSig)
				break;
			*NewEnd = i;
			continue;
		}
		if (*NewEnd-*NewBeg>=MinLent)
			return TRUE;
		Beg_C = *NewEnd + 1;
	}
	return FALSE;
}
/*---------------------------------------------------------------------------*/
Bool FindNormNextHoleWithBound (int *pSig, int LenSig, int Beg, int End
						, int *NewBeg, int *NewEnd, int MaxSig, int MinLent)
{
	int i, Beg_C, End_C;
	Bool ret;
	if (Beg > End)
		return FALSE;
	Beg_C = Beg;
	if (Beg_C < 0)
		Beg_C = 0;
	End_C = End;
	if (End_C > LenSig - 1)
		End_C = LenSig - 1;
	if (Beg_C > End_C)
		return FALSE;
	while (Beg_C <= End_C)
	{
		ret = FALSE;
		for (i=Beg_C; i<=End_C; i++)
		{
			if (pSig[i] > MaxSig)
				continue;
			*NewBeg = i;
			ret = TRUE;
			break;
		}
		if (!ret)
			return FALSE;
		*NewEnd = *NewBeg;
		for (i=*NewBeg; i<=End_C; i++)
		{
			if (pSig[i] > MaxSig)
				break;
			*NewEnd = i;
			continue;
		}
		if (*NewEnd-*NewBeg>=MinLent)
			return TRUE;
		Beg_C = *NewEnd + 1;
	}
	return FALSE;
}
/*---------------------------------------------------------------------------*/
Bool FindMainHole (int Beg, int End, int MaxSig, Un_GYST *pOrtGt, int *NewBeg, int *NewEnd, int *NewMax)
{
	int CurBeg, CurEnd, i, BegPos;
	Bool ret;
	ret = FindNextHoleWithBound (MaxSig, pOrtGt, Beg, End, &CurBeg, &CurEnd, 0);
	if (!ret)
		return FALSE;
	*NewBeg = CurBeg;
	*NewEnd = CurEnd;
	BegPos = *NewEnd + 1;
	while (1)
	{
		ret = FindNextHoleWithBound (MaxSig, pOrtGt, BegPos, End, &CurBeg, &CurEnd, 0);
		if (!ret)
			break;
		BegPos = CurEnd + 1;
		if (*NewEnd-*NewBeg > CurEnd-CurBeg)
			continue;
		*NewBeg = CurBeg;
		*NewEnd = CurEnd;
	}
	*NewMax = pOrtGt->Signal[*NewBeg-pOrtGt->Shift];
	for (i=*NewBeg; i<=*NewEnd; i++)
		if (*NewMax < pOrtGt->Signal[i-pOrtGt->Shift])
			*NewMax = pOrtGt->Signal[i-pOrtGt->Shift];
	return TRUE;
}
/*---------------------------------------------------------------------------*/
