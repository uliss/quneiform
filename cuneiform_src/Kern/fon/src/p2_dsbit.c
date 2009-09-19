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

// use weighted distance
#define _WEIDIST_

#include <string.h>

#include "fon.h"
#include "sfont.h"
extern int Num11[256];
/**************************************/
// Bitmap distance from b1 to b2
// black point == 1 !
static int16_t DistanceBitDLL(uchar *b1, int16_t xbyte1, int16_t yrow1, uchar *b2,
		int16_t xbyte2, int16_t yrow2, int16_t porog) {
	register int16_t i, j;
	int16_t xbyte = MIN(xbyte1, xbyte2);
	int16_t yrow = MIN(yrow1, yrow2);
	int16_t dist;

	for (i = 0, dist = 0; i < yrow; i++, b1 += xbyte1, b2 += xbyte2) {
		//memset(btmp,0,xbyte*sizeof(ulong));
		for (j = 0; j < xbyte; j++)
			dist += Num11[b1[j] ^ b2[j]];
		if (dist >= porog)
			return dist;

		// если первый символ шире
		for (j = xbyte; j < xbyte1; j++)
			dist += Num11[b1[j]];
		if (dist >= porog)
			return dist;

		// если второй символ шире
		for (j = xbyte; j < xbyte2; j++)
			dist += Num11[b2[j]];
		if (dist >= porog)
			return dist;
	}

	// if yrow2 < yrow1  - первый символ выше
	for (i = yrow; i < yrow1; i++, b1 += xbyte1) {
		for (j = 0; j < xbyte1; j++)
			dist += Num11[b1[j]];
		if (dist >= porog)
			return dist;
	}

	//  если второй символ выше
	for (i = yrow; i < yrow2; i++, b2 += xbyte2) {
		for (j = 0; j < xbyte2; j++)
			dist += Num11[b2[j]];
		if (dist >= porog)
			return dist;
	}

	return dist;
}
/**************************************/
int16_t DistanceBitFull(uchar *b1, int16_t xbyte1, int16_t yrow1, uchar *b2,
		int16_t xbyte2, int16_t yrow2) {
	register int16_t i, j;
	int16_t xbyte = MIN(xbyte1, xbyte2);
	int16_t yrow = MIN(yrow1, yrow2);
	int16_t dist;

	for (i = 0, dist = 0; i < yrow; i++, b1 += xbyte1, b2 += xbyte2) {
		for (j = 0; j < xbyte; j++)
			dist += Num11[b1[j] ^ b2[j]];
		// если первый символ шире
		for (j = xbyte; j < xbyte1; j++)
			dist += Num11[b1[j]];
		// если второй символ шире
		for (j = xbyte; j < xbyte2; j++)
			dist += Num11[b2[j]];
	}

	// if yrow2 < yrow1  - первый символ выше
	for (i = yrow; i < yrow1; i++, b1 += xbyte1) {
		for (j = 0; j < xbyte1; j++)
			dist += Num11[b1[j]];
	}

	//  если второй символ выше
	for (i = yrow; i < yrow2; i++, b2 += xbyte2) {
		for (j = 0; j < xbyte2; j++)
			dist += Num11[b2[j]];
	}

	return dist;
}
/**************************************/
// move bitmaps left-right on mov bit ( <8!)
void MoveRightBmp(uchar *buf, int16_t mov, int16_t xbyte, int16_t yrow) {
	register int16_t i, j;
	int16_t mov1 = 8 - mov;

	for (i = 0; i < yrow; i++, buf += xbyte) {
		for (j = xbyte - 1; j > 0; j--)
			buf[j] = (buf[j] >> mov) | (buf[j - 1] << mov1);
		buf[0] >>= mov;
	}
}
////////////
void MoveLeftBmp(uchar *buf, int16_t mov, int16_t xbyte, int16_t yrow) {
	register int16_t i, j;
	int16_t mov1 = 8 - mov;

	for (i = 0; i < yrow; i++, buf += xbyte) {
		for (j = 0; j < xbyte - 1; j++)
			buf[j] = (buf[j] << mov) | (buf[j + 1] >> mov1);
		buf[xbyte - 1] <<= mov;
	}
}
////////////
// tbuf -temporary buffer, in it - moved b1 right,left
int16_t CheckCenterSymbol(uchar *b1, int16_t xbyte, int16_t yrow, uchar *b2,
		uchar *tbuf, int16_t xbit2, int16_t yrow2, int16_t *sdvigx, int16_t *sdvigy,
		int16_t firdist) {
	register int16_t i;
	int16_t dist, bestdist;
	int16_t xbyte2 = (xbit2 >> 3) + 1; // bytes in row
	int16_t bestx = 0;
	int16_t besty = 0;
	int16_t tx;

	*sdvigx = 0;
	*sdvigy = 0;

	bestdist = DistanceBitFull(b1, xbyte, yrow, b2, xbyte2, yrow2);
	if (bestdist == 0)
		return 0;
	for (tx = 0;;) {
		// distance to original bitmap
		if (tx) {
			dist = DistanceBitDLL(b1, xbyte, yrow, b2, xbyte2, yrow2, bestdist);
			if (tx == 1)
				dist += firdist;
			if (dist < bestdist) {
				bestdist = dist;
				bestx = tx;
				besty = 0;
				if (dist == 0)
					break;
			}
		}

		// move b2 - down (or etalon -up)
		dist = DistanceBitDLL(b1 + xbyte, xbyte, (int16_t) (yrow - 1), b2, xbyte2,
				yrow2, bestdist);
		if (tx == 1)
			dist += firdist;
		if (dist < bestdist) { // add first row
			for (i = 0; i < xbyte; i++)
				dist += Num11[b1[i]];
			if (dist < bestdist) {
				bestdist = dist;
				bestx = tx;
				besty = 1;
				if (dist == 0)
					break;
			}
		}

		// move b2 - up
		dist = DistanceBitDLL(b1, xbyte, yrow, b2 + xbyte2, xbyte2,
				(int16_t) (yrow2 - 1), bestdist);
		if (tx == 1)
			dist += firdist;
		if (dist < bestdist) {
			for (i = 0; i < xbyte; i++)
				dist += Num11[b2[i]];
			if (dist < bestdist) {
				bestdist = dist;
				bestx = tx;
				besty = -1;
				if (dist == 0)
					break;
			}
		}

		if (tx == 0) {
			b1 = tbuf;
			tx = -1;
		} else {
			if (tx != -1)
				break;
			tx = 1;
			if (firdist >= bestdist)
				break;
			b1 += xbyte * yrow;
		}
	} // end tx

	*sdvigx = bestx;
	*sdvigy = besty;
	return bestdist;
}
///////////////
// move left/right on 1 etalon
// return - summa 1 im first column
int16_t MakeMoved(uchar *etalon, int16_t xbyte, int16_t yrow, uchar *tmpbuf) {
	int16_t i, j;

	memcpy(tmpbuf, etalon, xbyte * yrow);
	MoveRightBmp(tmpbuf, 1, xbyte, yrow);

	tmpbuf += xbyte * yrow;
	memcpy(tmpbuf, etalon, xbyte * yrow);
	MoveLeftBmp(tmpbuf, 1, xbyte, yrow);

	// calc 1-s in first column
	for (i = 0, j = 0; i < yrow; i++, etalon += xbyte)
		if (*etalon & 128)
			j++;
	return j;
}
////////////////
