/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 ����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
 ��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

 * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
 ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
 ����� �� ��������.
 * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
 ������ ����������, ������������ ��� ���������������, ������ �����������
 ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
 ����������� ����� �� ��������.
 * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
 ���� ������������ � �������� �������� ��������� �/��� �����������
 ���������, ���������� �� ���� ��, ��� ���������������� �����������
 ����������.

 ��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
 ��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
 ������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
 ������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
 ����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
 ��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
 ������������� ������, ��������� � �������������� ��� ���������� ����������
 ������������� ������������� ��������� (������� ������ ������, ��� ������,
 ������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
 ������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
 �� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
 �������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

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

// - for AddClusters
#define _ADDCLU_
// use special razmaz for small bitmaps
#define _SPECSMALL_
#define SMALLPOROG 18

#include "fon.h"
#include "sfont.h"
#include <string.h>

extern int Num11[256];
/**************************************/
//
//  ���� ��⠥� - ࠧ��� ���⨭�� ������
//  � MAXSINT !
//  ��ୠ� �窠 == 1  !!!
//
int16_t MoveUpDownBitmap(uchar *bSource, int16_t xbyte, int16_t yrow, uchar *bDest) {
	register int16_t i;
	uchar *b1;
	uchar *b2;
	int16_t size = xbyte * yrow;

	// ࠧ����� ����
	b2 = bDest + xbyte;
	b1 = bSource;
	for (i = 0; i < size; i++)
		b2[i] |= b1[i];

	// ⥯��� �����
	b2 = bDest;
	b1 = bSource + xbyte;
	size -= xbyte;
	for (i = 0; i < size; i++)
		b2[i] |= b1[i];

	return 0;
}
/***********************/
int16_t MoveLeftRightBitmap(uchar *bSource, int16_t xbyte, int16_t yrow) {
	register int16_t i, j;
	uchar stay, stay1;
	uchar stay2;
	uchar *b1;

	for (i = 0, b1 = bSource; i < yrow; i++, b1 += xbyte) {
		// ���� �� ������ ��ப�
		for (j = 0, stay1 = 0; j < xbyte; j++) {
			// �������� last bit
			stay = b1[j] & 1;

			// first bit from next byte
			stay2 = (j + 1 < xbyte ? (b1[j + 1] & 128) >> 7 : 0);

			b1[j] |= (b1[j] >> 1) | (b1[j] << 1) | stay1 | stay2;

			// ��࠭���� last bit - ������ � ᫥���騩 ����
			stay1 = stay << 7;
		}
	}

	return 0;
}
/***********************/
static uchar const maska[8] = { 0, 128, 192, 224, 240, 248, 252, 254 };

static Bool VeryFat(uchar *bSource, int16_t xbit, int16_t yrow, int16_t xbyte) {
	int32_t i = (int32_t) xbyte * yrow, summa;

	for (summa = 0; i > 0; i--, bSource++)
		summa += Num11[*bSource];
	if (summa * 100 >= (int32_t) xbit * yrow * 75)
		return TRUE;
	return FALSE;
}
//////////////////////
int16_t Razmaz(uchar *bSource, uchar *bDest, int16_t xbyte, int16_t xbit, int16_t yrow,
		uchar fat) {
	int16_t i, j;
	uchar mas;
	uchar *b1;

	//  ���㫨�  墮�� � ���筨�� - ���� �� �� �����?
	j = xbit & 7;
	if (j != 0) {
		mas = maska[j];
		for (i = 0, b1 = bSource + (xbit >> 3); i < yrow; i++, b1 += xbyte)
			*b1 &= mas;
	}

	// �᫨ ���� ��譨� ���� � ���筨�� - ���㫨�
	else {
		for (i = 0, b1 = bSource + xbyte - 1; i < yrow; i++, b1 += xbyte)
			*b1 = 0;
	}

	// if(!fat)
	fat = VeryFat(bSource, xbit, yrow, xbyte);

	// ���㫨� ��᫥���� ��ப� '⮫�⮣�'
	memset(bDest + xbyte * yrow, 0, xbyte);

#ifdef _SPECSMALL_
	// not add angles
	if (yrow <= SMALLPOROG || fat) {
		// ᪮���㥬
		memcpy(bDest, bSource, xbyte * yrow);
		MoveLeftRightBitmap(bDest, xbyte, yrow);
		MoveUpDownBitmap(bSource, xbyte, yrow, bDest);
	} else {
#endif
		// ᪮���㥬
		memcpy(bDest, bSource, xbyte * yrow);
		// � ������� ࠧ�����
		//  �����-����
		MoveUpDownBitmap(bSource, xbyte, yrow, bDest);
		// � ��ࠢ�-�����
		MoveLeftRightBitmap(bDest, xbyte, (int16_t) (yrow + 1));
#ifdef _SPECSMALL_
	}
#endif

	// reverse thick
	j = xbyte * (yrow + 1);
	for (i = 0; i < j; i++)
		bDest[i] = ~bDest[i];
	return fat;
}
//////////////////////////
#ifdef _ADDCLU_
/**************************************/
//
//  ���� ��⠥� - ࠧ��� ���⨭�� ������
//  � MAXSINT !
//  ��ୠ� �窠 == 1  !!!
//
static uchar tmpbuf[(WR_MAX_WIDTH >> 3) * WR_MAX_HEIGHT];
int16_t MoveUpDownBitmap2(int16_t xbyte, int16_t yrow, uchar *bDest) {
	int16_t i;
	uchar *b1;
	uchar *b2;
	int16_t size = xbyte * yrow;
	uchar *bSource = tmpbuf;

	memcpy(bSource, bDest + xbyte, xbyte * yrow);

	// ࠧ����� ����
	b2 = bDest + 2* xbyte ;
	b1 = bSource;
	for (i = 0; i < size; i++)
		b2[i] |= b1[i];

	// ⥯��� �����
	b2 = bDest;
	b1 = bSource;
	for (i = 0; i < size; i++)
		b2[i] |= b1[i];

	return 0;
}
/////////////////////
static int16_t MoveUpDownNoAngle(uchar *bSource, int16_t xbyte1, int16_t xbyte2,
		int16_t yrow, uchar *bDest) {
	int16_t i, j;
	uchar *b1;
	uchar *b2;
	uchar stay;
	int16_t xbyte = MIN(xbyte1, xbyte2);

	// ࠧ����� ����
	b2 = bDest;
	b1 = bSource;
	for (i = 0, stay = 0; i < yrow; i++, b1 += xbyte1, b2 += xbyte2) {
		for (j = 0, stay = 0; j < xbyte; j++) {
			b2[j] |= (b1[j] >> 1) | stay;
			stay = (b1[j] & 1) << 7;
		}
		if (xbyte < xbyte2)
			b2[j] |= stay;
	}

	b2 = bDest + 2* xbyte2 ;
	b1 = bSource;
	for (i = 0, stay = 0; i < yrow; i++, b1 += xbyte1, b2 += xbyte2) {
		for (j = 0, stay = 0; j < xbyte; j++) {
			b2[j] |= (b1[j] >> 1) | stay;
			stay = (b1[j] & 1) << 7;
		}
		if (xbyte < xbyte2)
			b2[j] |= stay;
	}

	return 0;
}
/***********************/
// bSource - wide picture ! (if xbits in real picture xbits+2 in razmaz!)
static int16_t MoveLeftRightBitmap2(uchar *bSource, int16_t xbyte, int16_t yrow) {
	int16_t i, j;
	uchar stay, stay1;
	uchar *b1;

	for (i = 0, b1 = bSource; i < yrow; i++, b1 += xbyte) {
		// ���� �� ������ ��ப�
		for (j = 0, stay1 = 0; j < xbyte; j++) {
			// ��������  last bits
			stay = ((b1[j] & 1) << 7) | ((b1[j] & 3) << 6);

			b1[j] |= (b1[j] >> 1) | (b1[j] >> 2) | stay1; // | stay2;

			// ��࠭���� last bits - ������ � ᫥���騩 ����
			stay1 = stay;
		}
	}

	return 0;
}
/***********************/
int16_t Razmaz2(uchar *bSource, uchar *bDest, int16_t xbit, int16_t yrow, int16_t porogX,
		int16_t porogY) {
	register int16_t i, j;
	uchar mas;
	uchar *b1, *b2;
	int16_t xbyte = (xbit + 7) >> 3; // bytes in row bSource
	int16_t xbyte2 = (xbit + 9) >> 3; // bytes in row bDest

	//  ���㫨�  墮�� � ���筨�� - ���� �� �� �����?
	j = xbit & 7;
	if (j != 0) {
		mas = maska[j];
		for (i = 0, b1 = bSource + (xbit >> 3); i < yrow; i++, b1 += xbyte)
			*b1 &= mas;
	}

	// ᪮���㥬
	for (i = 0, b2 = bDest + xbyte2, b1 = bSource; i < yrow; i++, b1 += xbyte, b2
			+= xbyte2)
		memcpy(b2, b1, xbyte);

	// �᫨ ���� ��譨� ���� � Dest - ���㫨�
	if (xbyte < xbyte2) {
		for (i = 0, b1 = bDest + xbyte2 + xbyte; i < yrow; i++, b1 += xbyte2)
			*b1 = 0;
	}

	// ���㫨� first & ��᫥���� ��ப� '⮫�⮣�'
	memset(bDest, 0, xbyte2);
	memset(bDest + xbyte2 * (yrow + 1), 0, xbyte2);

	MoveLeftRightBitmap2(bDest + xbyte2, xbyte2, yrow);

	// now - add angles? not for SMALL, yes for BIG
	if (xbit > porogX && yrow > porogY)
		MoveUpDownBitmap2(xbyte2, yrow, bDest);
	else
		MoveUpDownNoAngle(bSource, xbyte, xbyte2, yrow, bDest);

	//#ifdef _SPECSMALL_
	// not add angles

	return 0; //  � ����� �� 0 ?
}
//////////////////////////
/***********************/
int16_t Razmaz2xByte(uchar *bSource, uchar *bDest, int16_t xbyteAll, int16_t xbit,
		int16_t yrow, int16_t porogX, int16_t porogY) {
	register int16_t i, j;
	uchar mas;
	uchar *b1, *b2;
	int16_t xbyte = (xbit + 7) >> 3; // actual bytes in row bSource
	int16_t xbyte2 = (xbit + 9) >> 3; // bytes in row bDest

	//  ���㫨�  墮�� � ���筨�� - ���� �� �� �����?
	j = xbit & 7;
	if (j != 0) {
		mas = maska[j];
		for (i = 0, b1 = bSource + (xbit >> 3); i < yrow; i++, b1 += xbyteAll)
			*b1 &= mas;
	}

	// ᪮���㥬
	for (i = 0, b2 = bDest + xbyte2, b1 = bSource; i < yrow; i++, b1
			+= xbyteAll, b2 += xbyte2)
		memcpy(b2, b1, xbyte);

	// �᫨ ���� ��譨� ���� � Dest - ���㫨�
	if (xbyte < xbyte2) {
		for (i = 0, b1 = bDest + xbyte2 + xbyte; i < yrow; i++, b1 += xbyte2)
			*b1 = 0;
	}

	// ���㫨� first & ��᫥���� ��ப� '⮫�⮣�'
	memset(bDest, 0, xbyte2);
	memset(bDest + xbyte2 * (yrow + 1), 0, xbyte2);

	MoveLeftRightBitmap2(bDest + xbyte2, xbyte2, yrow);

	// now - add angles? not for SMALL, yes for BIG
	if (xbit > porogX && yrow > porogY)
		MoveUpDownBitmap2(xbyte2, yrow, bDest);
	else
		MoveUpDownNoAngle(bSource, xbyteAll, xbyte2, yrow, bDest);

	//#ifdef _SPECSMALL_
	// not add angles

	return 0; //  � ����� �� 0 ?
}
//////////////////////////
//
// move right+down on 1 pixel
//
static void MoveDownBitmap(uchar *bSource, int16_t xbyte, int16_t yrow, uchar *bDest,
		int16_t xbyte2) {
	int16_t i;
	uchar *b1;
	uchar *b2;

	// ࠧ����� ����
	b2 = bDest + xbyte;
	b1 = bSource;
	for (; yrow > 0; yrow--, b1 += xbyte, b2 += xbyte2)
		for (i = 0; i < xbyte; i++)
			b2[i] |= b1[i];

	return;
}
/***********************/
static void MoveRightBitmap(uchar *b1, int16_t xbyte, int16_t yrow) {
	int16_t j;
	uchar stay1;
	uchar stay2;

	for (; yrow > 0; yrow--, b1 += xbyte) {
		for (j = 0, stay2 = 0; j < xbyte; j++) {
			stay1 = b1[j] & 1;
			b1[j] |= (b1[j] >> 1) | stay2;
			stay2 = stay1 << 7;
		}
	}

	return;
}
/***********************/
int16_t RazmazHalf(uchar *bSource, uchar *bDest, int16_t xbit, int16_t yrow) {
	int16_t i, j;
	uchar mas;
	uchar *b1, *b2;
	int16_t xbyte = (xbit + 7) >> 3;
	int16_t xbyte2 = (xbit >> 3) + 1;

	j = xbit & 7;
	if (j != 0) {
		mas = maska[j];
		for (i = 0, b1 = bSource + (xbit >> 3); i < yrow; i++, b1 += xbyte)
			*b1 &= mas;
	}

	for (i = 0, b2 = bDest, b1 = bSource; i < yrow; i++, b1 += xbyte, b2
			+= xbyte2)
		memcpy(b2, b1, xbyte);

	// �᫨ ���� ��譨� ���� � Dest - ���㫨�
	if (xbyte < xbyte2) {
		for (i = 0, b1 = bDest + xbyte2 - 1; i < yrow; i++, b1 += xbyte2)
			*b1 = 0;
	}

	// ���㫨� ��᫥���� ��ப� '⮫�⮣�'
	memset(bDest + xbyte2 * yrow, 0, xbyte2);

	MoveDownBitmap(bSource, xbyte, yrow, bDest, xbyte2);
	MoveRightBitmap(bDest, xbyte2, yrow);

	return 0; //  � ����� �� 0 ?
}
//////////////////////////
//////////////////////
// distance from bitmap to raster
// dist1 = minimal distance not used as invalid  (-summa/fill)
int16_t DistBitRas(puchar r, int16_t w, int16_t h, // b/w bitmap
		pchar wr, int16_t ww, int16_t wh, char dist1, // grey raster
		int16_t xo, int16_t yo, int16_t bdist) {
	pchar curr;
	int16_t i, j;
	uchar cbyte, cc;
	int16_t rbyte;
	int16_t dist;
	int16_t stx, sty, startx, starty;
	int16_t lastx, lasty;
	puchar rr;

	startx = (WR_MAX_WIDTH - w) / 2; // start b/w
	starty = (WR_MAX_HEIGHT - h) / 2;
	stx = (WR_MAX_WIDTH - ww) / 2; // start grey
	sty = (WR_MAX_HEIGHT - wh) / 2;

	startx += xo;
	starty += yo; // variation

	sty -= starty; // start grey - from b/w
	stx -= startx;
	startx = starty = 0;

	dist = 0;
	rbyte = (w + 7) >> 3;
	lasty = MIN(h, sty + wh); // last in intersection
	lastx = MIN(w, stx + ww);

	if (sty < 0)
		wr -= sty * ww;
	if (stx < 0)
		wr -= stx; // point to start intersection

	// first sty rows - may be empty in wel
	for (i = 0; i < sty; i++, r += rbyte)
		for (j = 0; j < rbyte; j++)
			dist += Num11[r[j]];

	if (dist >= bdist)
		return bdist;

	for (; i < lasty; i++, wr += ww, r += rbyte) {
		cc = 128;
		rr = r;
		cbyte = *rr;
		for (j = 0; j < stx; j++, cc >>= 1) {
			if (cc == 0) {
				cc = 128;
				cbyte = *(++rr);
			}
			if (cbyte & cc)
				dist++;
		}

		curr = wr; // current row in weighted
		for (; j < lastx; j++, cc >>= 1, curr++) {
			if (cc == 0) {
				cc = 128;
				cbyte = *(++rr);
			}
			if ((cbyte & cc) && *curr < dist1)
				dist++;
		}
		if (dist >= bdist)
			return dist;

		// now - last bits (only in b/w)
		for (; j < w; j++, cc >>= 1) {
			if (cc == 0) {
				cc = 128;
				cbyte = *(++rr);
			}
			if (cbyte & cc)
				dist++;
		}
		if (dist >= bdist)
			return dist;
	} // end i

	// last rows - may be empty in wel
	for (; i < h; i++, r += rbyte)
		for (j = 0; j < rbyte; j++)
			dist += Num11[r[j]];

	return dist;
}
////////////////////////
//
// for razmaz   -  �饬 �窨, ����� �뫠��� � wel
//
int16_t DistRasBit(puchar r, int16_t w, int16_t h, // b/w bitmap
		pchar wr, int16_t ww, int16_t wh, char wei, // grey raster
		int16_t xo, int16_t yo, int16_t bdist) {
	pchar curr;
	int16_t i, j;
	uchar cbyte, cc, initCC;
	int16_t rbyte;
	int16_t dist;
	int16_t stx, sty, startx, starty;
	int16_t lastx, lasty;
	puchar rr;

	startx = (WR_MAX_WIDTH - w) / 2;
	starty = (WR_MAX_HEIGHT - h) / 2;
	stx = (WR_MAX_WIDTH - ww) / 2;
	sty = (WR_MAX_HEIGHT - wh) / 2;

	startx += xo;
	starty += yo; // variation
	rbyte = (w + 7) >> 3;

	// to coordinates based on weighted raster
	startx -= stx;
	starty -= sty;
	stx = sty = 0;
	lastx = MIN(ww, startx + w);
	lasty = MIN(wh, starty + h);
	dist = 0;

	if (starty < 0)
		r -= starty * rbyte; // first few rows from r not useful

	if (startx < 0) {
		i = -startx;
		r += i >> 3;
		initCC = 128 >> (i & 7);
	} else
		initCC = 128; // from first bit

	// add upper lines
	for (i = 0, curr = wr; i < starty; i++, curr += ww)
		for (j = 0; j < ww; j++)
			if (curr[j] > wei)
				dist++;
	if (dist >= bdist)
		return dist;

	// now - start inside frame
	for (; i < lasty; i++, curr += ww, r += rbyte) {
		for (j = 0; j < startx; j++)
			if (curr[j] > wei)
				dist++;
		if (dist >= bdist)
			return dist;

		cc = initCC;
		rr = r;
		cbyte = *rr;
		for (; j < lastx; j++, cc >>= 1) {
			if (cc == 0) {
				cc = 128;
				cbyte = *(++rr);
			}
			if (((cbyte & cc) == 0) && curr[j] > wei)
				dist++;
		} // end j
		if (dist >= bdist)
			return dist;

		// last points in row
		for (; j < ww; j++)
			if (curr[j] > wei)
				dist++;
		if (dist >= bdist)
			return dist;

	} // end i

	// add down lines
	for (; i < wh; i++, curr += ww)
		for (j = 0; j < ww; j++)
			if (curr[j] > wei)
				dist++;

	return dist;
}
///////////////

#endif
///////////

//
// return  % weighted > porog
//  (fill*100) /(sx*sy)
//
static int16_t CountFill(welet *wel, int16_t porog) {
	int16_t i, j;
	char *rr;
	int16_t sx = wel->w;
	int16_t sy = wel->h;
	int16_t fx = sx; // first x
	int16_t lx = 0; // last  x
	int16_t fy = sy; // first y
	int16_t ly = 0; // last  y
	int16_t summa; // how many weighted points > porog
	int16_t startx = (WR_MAX_WIDTH - wel->w) / 2;
	int16_t starty = (WR_MAX_HEIGHT - wel->h) / 2;

	rr = wel->raster + starty * WR_MAX_WIDTH + startx;

	for (i = 0, summa = 0; i < sy; i++, rr += WR_MAX_WIDTH) {
		for (j = 0; j < sx; j++) {
			if (rr[j] <= porog)
				continue;
			// check new bounds
			if (j < fx)
				fx = j;
			if (j > lx)
				lx = j;
			if (i < fy)
				fy = i;
			if (i > ly)
				ly = i;
			summa++;
		}
	}

	if (summa <= 0)
		return 0;
	return ((summa * 100l) / ((long) (lx - fx + 1) * (ly - fy + 1)));
}
/////////////////
int16_t TestFat(welet *wel, int16_t porog, int16_t porog_fat) {
	int16_t i;
	//int16_t last=wel->weight/3;
	int16_t last = wel->weight - 1;

	/*  if( wel->weight == 2)
	 {
	 if(porog==1) return porog;
	 i= CountFill(wel,porog);
	 if( i < _POROG_FAT_ ) return porog;
	 last= CountFill(wel,1);
	 // don't remove too many !
	 if( i < last+7  ) porog=1;
	 else porog=0;
	 wel->porog = porog;
	 return porog;
	 }
	 */
	for (; porog <= last; porog++) {
		if ((i = CountFill(wel, porog)) < porog_fat)
			break;
	}

	if (porog > last)
		porog = last;
	if (i == 0 && porog > 0)
		porog--;
	wel->porog = (char) porog;
	return porog;
}
///////////
