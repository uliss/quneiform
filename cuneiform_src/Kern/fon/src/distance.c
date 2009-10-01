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

#define _NOT_ALL_      // не делать полный поиск
// #define _NEED_SCALE_   // масштабировать под стандартную высоту
#define STANDARD_HEIGHT 38  // стандартная высота
//#define _TEST_ADD_FAT_

#define OLEG_ACC 0

//#define CHECK_EQUAL

// размазывать мелких на полпиксела
//#define _USE_HALF_

#define MAX_ALT              5

#define POROG_HALF_WIDTH    4      // 21
#define POROG_HALF_HEIGHT   13     // 23
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef _GETTIME_
#include <time.h>
#endif

#include "minmax.h"
#include "fon.h"
#include "sfont.h"
#include "fonrec.h"
#include "ctbdefs.h"
#include "ligas.h"    // 13.09.2000
extern uchar language;

uint16_t cmp(puchar r, uint16_t fullwb, uint16_t w, uint16_t h, welet * wl);
int16_t RazmazHalf(uchar *bSource, uchar *bDest, int16_t xbit, int16_t yrow);
///////////////////////
//
// штрафуем дальние точки
//

#ifdef OLEG_ACC
static int sign_tab[513] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 0
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 1
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 2
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 3
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 4
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 5
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 6
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 7
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 8
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 9
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // a
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // b
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // c
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // d
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // e
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // f
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static int *sign;
#include "array1.h"
static int DistToWelet(puchar r, int fullByte, int w, int h, welet * wl,
		int xo, int yo, int porog, int dist1) {
	pchar curr, currn, currh;
	int rbyte;
	puchar rr, rn;
	int dist;

	curr = wl->raster + WR_MAX_WIDTH * ((WR_MAX_HEIGHT - h) / 2)
			+ (WR_MAX_WIDTH - w) / 2;
	curr += xo + WR_MAX_WIDTH * yo; // variation
	currh = curr + WR_MAX_WIDTH * h;
	rbyte = (w + 7) >> 3;
	sign = &sign_tab[256] - dist1;
	for (dist = 0; curr < currh; curr += WR_MAX_WIDTH, r += fullByte) {
		if (dist + 8 >= porog) {
			for (currn = curr, rr = r, rn = r + rbyte; rr != rn; rr++, currn
					+= 8) {
				dist += bit_distance[*rr](currn);

				if (dist > porog)
					return dist;
			} // end rr
		} else {
			switch (rbyte) {
			case 1:
				dist += bit_distance[*r](curr);
				break;
			case 2:
				dist += bit_distance[*r](curr) + bit_distance[*(r + 1)](curr
						+ 8);
				break;
			case 3:
				dist += bit_distance[*r](curr) + bit_distance[*(r + 1)](curr
						+ 8) + bit_distance[*(r + 2)](curr + 16);
				break;
			case 4:
				dist += bit_distance[*r](curr) + bit_distance[*(r + 1)](curr
						+ 8) + bit_distance[*(r + 2)](curr + 16)
						+ bit_distance[*(r + 3)](curr + 24);
				break;
			default:
				for (currn = curr, rr = r, rn = r + rbyte; rr != rn; rr++, currn
						+= 8) {
					dist += bit_distance[*rr](currn);
				} // end rr
				break;
			}
		}
	} // end currh

	return (dist);
}
///////////////////////
//
// штрафуем точки cluster's
//
static int sig_tab[513] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // f
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 0
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 1
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 1
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 7
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // b
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // c
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // d
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // e
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 // f
		};
///////////////
int16_t DistWeletRazmaz(puchar r, int fullByte, int w, int h, welet * wl, int xo,
		int yo, int porog, int wei) {
	int ww = wl->w, hh = wl->h;
	pchar curr;
	int i, j;
	uchar cbyte;
	int dist;
	int startx = (WR_MAX_WIDTH - w) / 2;
	int starty = (WR_MAX_HEIGHT - h) / 2;
	int stx = (WR_MAX_WIDTH - ww) / 2; // start cluster
	int sty = (WR_MAX_HEIGHT - hh) / 2;
	int lasty, lastx, lastx8;
	uchar maskaStart, maskaHvost;
	uchar *rr;

	// test
	// wei=(wl->weight)/3;  // better on sb0_037
	//bit_dist_level=wei;
	sign = &sig_tab[256] - wei;

	startx += xo;
	starty += yo; // variation

	curr = wl->raster + sty * WR_MAX_WIDTH;
	dist = 0;

	// add upper lines
	for (i = sty; i < starty; i++, curr += WR_MAX_WIDTH)
		for (j = stx; j < stx + ww; j++)
			if (curr[j] > wei)
				dist++;
	if (dist > porog)
		return dist;

	lasty = MIN(starty + h, sty + hh);
	lastx = MIN(startx + w, stx + ww);

	if (starty < sty)
		r += (sty - starty) * fullByte;

	if (startx < stx) {
		j = (stx - startx) >> 3;
		r += j;
		startx += j;
		maskaStart = 0xFF << (8 - stx + startx);
	} else
		maskaStart = 0; // from first bit

	lastx8 = startx + ((lastx - startx) & 0xfff8);
	lastx = stx + ww;

	if (lastx < lastx8 + 8)
		maskaHvost = 0xFF << (lastx8 + 8 - lastx);
	else
		maskaHvost = 0xFF;

	// now - start inside frame
	for (; i < lasty; i++, curr += WR_MAX_WIDTH, r += fullByte) {
		for (j = stx; j < startx; j++) {
			if (curr[j] > wei)
				dist++;
		}
		if (dist > porog)
			return dist;

		rr = r;
		cbyte = ~((*rr) | maskaStart);

		for (j = startx; j < lastx8; j += 8) {
			dist += bit_distance[cbyte](curr + j);
			cbyte = ~(*(++rr));
		} // end j
		if (dist > porog)
			return dist;

		//  реально лишнее (>lastx) не используется - maska!

		if (lastx8 < lastx) {
			cbyte &= maskaHvost;
			dist += bit_distance[cbyte](curr + j);
			if (dist > porog)
				return dist;
			j += 8;
		}

		// last points in row
		for (; j < lastx; j++) {
			if (curr[j] > wei)
				dist++;
		}
		if (dist > porog)
			return dist;
	} // end i

	// add down lines
	for (; i < sty + hh; i++, curr += WR_MAX_WIDTH)
		for (j = stx; j < stx + ww; j++)
			if (curr[j] > wei)
				dist++;

	return (dist);
}
////////////////////

#else
static int DistToWelet(puchar r,int fullByte,int w,int h,welet * wl,
		int xo,int yo,int porog,int dist1)
{
	pchar curr;
	int i,j,jj;
	uchar cbyte,cc;
	int rbyte;
	int dist;

	curr = wl->raster + WR_MAX_WIDTH*((WR_MAX_HEIGHT-h)/2) +
	(WR_MAX_WIDTH-w)/2;
	curr += xo + WR_MAX_WIDTH * yo; // variation

	rbyte=(w+7)>>3;

	for(i=0,dist=0; i < h;i++,curr += WR_MAX_WIDTH,r+=fullByte)
	{
		for(j=jj=0;j<rbyte;j++)
		{
			cbyte=r[j];
			if(cbyte == 0) {jj+=8;continue;}

			for(cc=128;cc;cc>>=1,jj++)
			{
				if( cbyte & cc )
				{
					if( curr[jj] < dist1 )
					dist+=2;
					else if( curr[jj] == dist1 )
					dist++;
				}
			} // end cc

			if( dist > porog ) return dist;
		} // end j
	} // end i

	return (dist);
}

///////////////////////
//
// штрафуем точки cluster's
//
int16_t DistWeletRazmaz(puchar r,int fullByte,int w,int h,welet * wl,
		int xo,int yo, int porog,int wei)
{
	int16_t ww=wl->w, hh=wl->h;
	pchar curr;
	int16_t i,j;
	uchar cbyte,cc;
	int16_t rbyte;
	int dist;
	int startx=(WR_MAX_WIDTH-w)/2;
	int starty=(WR_MAX_HEIGHT-h)/2;
	int stx=(WR_MAX_WIDTH-ww)/2; // start cluster
	int sty=(WR_MAX_HEIGHT-hh)/2;
	int lasty,lastx;
	uchar initCC;
	uchar *rr;

	// test
	//  wei=(wl->weight)/3;  // better on sb0_037
	// wei=wl->weight>>1;     // old version

	startx += xo;
	starty += yo; // variation
	rbyte=(w+7)>>3;

	curr = wl->raster+sty*WR_MAX_WIDTH;
	dist=0;

	// add upper lines
	for(i=sty;i<starty;i++,curr+=WR_MAX_WIDTH)
	for(j=stx;j<stx+ww;j++) if(curr[j]> wei) dist++;
	if(dist > porog) return dist;

	lasty=MIN(starty+h,sty+hh);
	lastx=MIN(startx+w,stx+ww);
	if( starty < sty) r+=(sty-starty)*fullByte;

	if(startx < stx)
	{	initCC=128>>((stx-startx)&7);
		r+=(stx-startx)>>3;
	}
	else initCC=128; // from first bit

	// now - start inside frame
	for(; i < lasty;i++,curr +=WR_MAX_WIDTH,r+=fullByte)
	{
		for(j=stx;j<startx;j++) if(curr[j] > wei ) dist++;
		if(dist > porog ) return dist;

		cc=initCC;
		rr=r;
		cbyte=*rr;
		for(;j<lastx;j++,cc>>=1)
		{
			if(cc==0) {cc=128;cbyte=*(++rr);}
			if( ((cbyte & cc)==0) && curr[j]>wei ) dist++;
		} // end j
		if(dist > porog) return dist;

		// last points in row
		for(;j<stx+ww;j++) if(curr[j] > wei ) dist++;
		if(dist > porog) return dist;
	} // end i

	// add down lines
	for(;i<sty+hh;i++,curr+=WR_MAX_WIDTH)
	for(j=stx;j<stx+ww;j++) if(curr[j]> wei) dist++;

	return (dist);
}
///////////////
#endif
///////////

int distOne(uchar *buf, uchar *bufrazmaz, int w, int h, int bestdist,
		welet *wel, int x, int y, int countRazmaz) {
	int dist, j;
	int dist1, wei;
	int w2, h2;

#ifdef _USE_HALF_
	//  на каких-то отлично, но на иных бяка
	if( w < POROG_HALF_WIDTH || h < POROG_HALF_HEIGHT )
	{
		//return  255 - cmp( buf, (w+7)>>3, w, h, wel);

		//dist1 = wel->weight/10;
		//dist1 = wel->weight/3;

		dist1 = wel->summa/wel->fill;

		// not-weighted distance == 2
		//if(dist1 < 64 ) dist1 *=2;
		// else dist1=127;

		dist1 = -dist1;

		wei = wel->weight/3; //  /10
		w2 = w + 1;
		h2 = h + 1;

		//		wei = 	wel->weight/3;
		//		bufrazmaz = buf;
		//		w2 = w;
		//		h2 = h;
	}

	else // standard
#endif

	{
		dist1 = wel->summa / wel->fill;

		// not-weighted distance == 2
		if (dist1 < 64)
			dist1 *= 2;
		else
			dist1 = 127;

		dist1 = -dist1;

		wei = (wel->weight) / 3; // better on sb0_037

		w2 = w + 2;
		h2 = h + 2;
	}

	dist = DistToWelet(buf, (w + 7) >> 3, w, h, wel, x, y, bestdist - 1, dist1);
	if (dist >= bestdist)
		return dist;

	if (countRazmaz <= 0)
		countRazmaz = 1;

#ifdef _USE_HALF_
	if( w < POROG_HALF_WIDTH || h < POROG_HALF_HEIGHT )
	j= DistWeletRazmaz(bufrazmaz,(w>>3)+1,w+1,h+1,wel,x,y,(bestdist-dist-1)*countRazmaz,wei);
	else
#endif

	j = DistWeletRazmaz(bufrazmaz, (w2 + 7) >> 3, w2, h2, wel, x, y, (bestdist
			- dist - 1) * countRazmaz, wei);

	return dist + ((j + countRazmaz - 1) / countRazmaz);
}
////////////
int distWelet(uchar *buf, uchar *bufraz, int w, int h, welet * wl, int porog,
		int countRazmaz) {
	uint16_t best, east, west, north, south, center;
	int lbest; // local best
	int bound = 140; //2*MIN(50,w+h);
	int initPorog = porog;

	best = east = west = north = south = center = lbest = 0;

	// center - special threshold??? check !!!
	center = best = distOne(buf, bufraz, w, h, bound, wl, 0, 0, countRazmaz);
	// center =best=distOne(buf,bufraz,w,h,porog,wl,0,0);
	if (best == 0)
		return best;

	// test - if very bad, go away ... !!!
	if (best >= bound)
		return porog + 1;

	if (best < porog)
		porog = best;

	if ((south = distOne(buf, bufraz, w, h, porog, wl, 0, 1, countRazmaz))
			< best)
		best = south;
	if (best == 0)
		return best;
	if (best < porog)
		porog = best;
	if ((north = distOne(buf, bufraz, w, h, porog, wl, 0, -1, countRazmaz))
			< best)
		best = north;
	if (best == 0)
		return best;
	if (best < porog)
		porog = best;
	if ((east = distOne(buf, bufraz, w, h, porog, wl, 1, 0, countRazmaz))
			< best)
		best = east;
	if (best == 0)
		return best;
	if (best < porog)
		porog = best;
	if ((west = distOne(buf, bufraz, w, h, porog, wl, -1, 0, countRazmaz))
			< best)
		best = west;

	if (best == 0)
		return best;
	if (best < porog)
		porog = best;

	if (best < north && best < south && best < east && best < west)
		return best;

	// nothing good ?
	if (best >= initPorog)
		return best;

	center = best; // to save old best for compare
	// where try move ? - now two positions may be
	if (north == center || east == center) {
		if ((lbest = distOne(buf, bufraz, w, h, porog, wl, 1, -1, countRazmaz))
				< best) {
			best = lbest;
			if (best < porog)
				porog = best;
		}
	}
	if (east == center || south == center) {
		if ((lbest = distOne(buf, bufraz, w, h, porog, wl, 1, 1, countRazmaz))
				< best) {
			best = lbest;
			if (best < porog)
				porog = best;
		}
	}
	if (south == center || west == center) {
		if ((lbest = distOne(buf, bufraz, w, h, porog, wl, -1, 1, countRazmaz))
				< best) {
			best = lbest;
			if (best < porog)
				porog = best;
		}
	}
	if (west == center || north == center) {
		if ((lbest = distOne(buf, bufraz, w, h, porog, wl, -1, -1, countRazmaz))
				< best)
			best = lbest;
	}

	return best;
}

///////////////////////////
// 0xba - обозначение для i с приклееной точкой
static const char Palki[] = "!1Iil"; //  \xba";

/////////////////
static int LookBestClusters(int w, int h, uchar *buf, uchar *bufrazmaz,
		int NumClus, int porog, welet *wel, RECRESULT *recres, int maxNames,
		int nInCTB, int col, int row, int countRazmaz) {
	int i, j;
	int dist;
	int num = 0;

	for (i = num = 0; i < NumClus; i++, wel++) {
		if (wel->invalid)
			continue;
		if (!(wel->attr & FON_CLU_SOLID))
			continue;

		// не распознавать себя ?
		if (wel->weight == 1 && (wel->nInCTB == nInCTB ||
		//wel->sr_col == col && wel->sr_row==row
				abs(col - wel->sr_col) <= 1 && abs(row - wel->sr_row) <= 1))
			//	  wel-> work < 230 )
			continue;
		//   dist=(255 - wel->work + 4)/(2*STRAFPOINT) ;

		if (wel->num < 0) // не из нашего алфавита
			continue;

		// check for height likeness
		j = abs(wel->mh - h);

		if (strchr(Palki, wel->let) || wel->let == liga_i || (language
				== LANG_TURKISH && // 30.05.2002 E.P.
				(wel->let == i_sans_accent || wel->let == II_dot_accent))) {
			if (j * 5 > MIN(h, wel->mh))
				continue;
		} else {
			if (j > 2 && j * 4 > MAX(h, wel->mh))
				continue;
		}

		j = abs(wel->mw - w);
		if (j > 3 && j * 3 >= MAX(w, wel->mw))
			continue;

		dist = distWelet(buf, bufrazmaz, w, h, wel, porog + 1, countRazmaz);

		if (dist <= porog) {
			num = AddVersion(recres, (uchar) wel->let, (uchar)(255 - dist), i,
					num, maxNames);
			if (num >= maxNames) {
				porog = 255 - recres[maxNames - 1].prob - 1;
				if (porog < 0)
					break;
			}
			//	if(dist==0) break;
		}

	}

	for (i = 0; i < num; i++) {
		j = 255 - recres[i].prob;
		j = 255 - STRAFPOINT * j;
		// очень слабые не нужны
		if (j <= 0)
			break;
		// for every point - add straf
		recres[i].prob = (uchar) j;
	}
	num = i;

	return num;
}

///////////////////////

int AddTestAlt(uchar prob, int numAlt, FonTestInfo *attr, welet *wel,
		int nClust) {
	uchar flg = CTB_PRINT_FONT;
	int j, k;

	if (wel->attr & FON_CLU_ITALIC)
		flg |= CTB_PRINT_ITALIC;
	if (wel->attr & FON_CLU_BOLD)
		flg |= CTB_PRINT_BOLD;
	if (wel->attr & FON_CLU_SERIF)
		flg |= CTB_PRINT_SERIFIC;
	if (wel->attr & FON_CLU_GELV)
		flg |= CTB_PRINT_GELV;
	if (wel->attr & FON_CLU_NARROW)
		flg |= CTB_PRINT_NARROW;

	for (j = 0; j < numAlt; j++) {
		if (attr[j].flag == flg) {
			if (attr[j].prob < prob) {
				attr[j].prob = prob;
				attr[j].kegl = wel->kegl;
				attr[j].nClust = nClust;
			}
			return numAlt;
		}
		if (prob > attr[j].prob)
			break;
	}

	if (j >= MAXCHECKALT)
		return numAlt;

	// was such attr ?
	for (k = j + 1; k < numAlt; k++) {
		if (attr[k].flag == flg)
			break;
	}
	if (k == MAXCHECKALT)
		k--;
	else if (k > numAlt)
		numAlt++;

	if (j < k)
		memmove(attr + j + 1, attr + j, sizeof(FonTestInfo) * (k - j));

	attr[j].prob = prob;
	attr[j].flag = flg;
	attr[j].kegl = wel->kegl;
	attr[j].nClust = nClust;

	return numAlt;
}
/////////////
static int16_t CheckLetter(int16_t w, int16_t h, uchar *buf, uchar *bufrazmaz,
		FONBASE *fbase, int porog, int let, FonTestInfo *attr, int16_t nInCTB) {
	int16_t dist;
	int num = 0, i;
	int numAlt = 0;
	welet *wel;

	if (let < 0 || let > 255)
		return -1;

	i = fbase->first[let];
	for (num = 0; i > 0; i = wel->next) {
		wel = fbase->start + i - 1;

		if (wel->let != let)
			continue;
		if (wel->invalid)
			continue;
		if (!(wel->attr & FON_CLU_SOLID))
			continue;
		if (wel->weight == 1 && wel->nInCTB == nInCTB)
			continue;

		num++;
		dist = distWelet(buf, bufrazmaz, w, h, wel, porog + 1, 1);

		if (dist <= porog) {
			uchar prob = MAX(0, 255 - STRAFPOINT * dist);
			numAlt = AddTestAlt(prob, numAlt, attr, wel, i);
		}

	}

	if (num <= 0)
		return -1; // no such letter
	return numAlt;
}

///////////////////////
static uchar buf[REC_MAX_RASTER_SIZE];
static uchar bufrazmaz[REC_MAX_RASTER_SIZE];
static uchar const mask0[8] = { 255, 128, 192, 224, 240, 248, 252, 254 };
//////////////
int16_t RecogClu(uchar *rast, int16_t xbyte, int16_t xbit, int16_t yrow, RECRESULT *recres,
		int16_t maxNames, welet *wl, int numWel, int porog, int nInCTB,
		int16_t col, int16_t row, int32_t countRazmaz) {
	int i;
	int rbyte = (xbit + 7) >> 3;
	uchar *b1;
	uchar hvost = mask0[xbit & 7];

	if (wl == NULL || numWel <= 0)
		return 0;
	if (xbyte * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / xbyte;

	for (i = 0, b1 = buf; i < yrow; i++, b1 += rbyte, rast += xbyte) {
		memcpy(b1, rast, rbyte);
		b1[rbyte - 1] &= hvost;
	}

#ifdef _USE_HALF_
	if( xbit < POROG_HALF_WIDTH || yrow < POROG_HALF_HEIGHT )
	RazmazHalf(buf,bufrazmaz,xbit,yrow);
	else
#endif
	Razmaz2(buf, bufrazmaz, xbit, yrow, 0, POROG_ANGLES);

	if (porog < 0)
		porog = MIN(50, xbit + yrow + 4);

	if (maxNames > MAX_ALT)
		maxNames = MAX_ALT;

	i = LookBestClusters(xbit, yrow, buf, bufrazmaz, numWel, porog, wl, recres,
			maxNames, nInCTB, col, row, countRazmaz);

#ifdef CHECK_EQUAL
	if( i > 1 &&
			recres[0].prob - 3 <= recres[1].prob )
	{
		int CheckRaster(puchar r,uint16_t fullBytes,uint16_t w,uint16_t h,
				welet *wel,int inBase,int let);
		int prob0,prob1;
		prob0= CheckRaster(buf,rbyte,xbit,yrow,wl,numWel,recres[0].name);
		prob1= CheckRaster(buf,rbyte,xbit,yrow,wl,numWel,recres[1].name);
		if( prob1 > prob0 )
		{
			RECRESULT tmpRec=recres[0];
			recres[0]=recres[1];
			recres[1]=tmpRec;

		}

		if(prob1 != prob0 )
		{	int j=1;
			for(;j<i;j++)
			recres[j].prob--;
		}
	}
#endif

#ifdef _TEST_ADD_FAT_
	{
		static int AddTestRecogCollection( uchar *rast,int xbit,int yrow,
				int num,RECRESULT *recres, welet *wl,int numWel );
		i=AddTestRecogCollection ( buf,(int)xbit,(int)yrow,
				i, recres, wl,(int)numWel);
	}
#endif

	return i;
}
///////////////////////////
int16_t CheckClu(uchar *rast, int16_t xbyte, int16_t xbit, int16_t yrow, FONBASE *fbase,
		int let, FonTestInfo *attr, int16_t nInCTB) {
	int i;
	int rbyte = (xbit + 7) >> 3;
	uchar *b1;
	int porog;
	uchar hvost = mask0[xbit & 7];

	if (fbase == NULL || fbase->inBase <= 0)
		return -1;
	if (xbyte * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / xbyte;

	for (i = 0, b1 = buf; i < yrow; i++, b1 += rbyte, rast += xbyte) {
		memcpy(b1, rast, rbyte);
		b1[rbyte - 1] &= hvost;
	}

#ifdef _USE_HALF_
	if( xbit < POROG_HALF_WIDTH || yrow < POROG_HALF_HEIGHT )
	RazmazHalf(buf,bufrazmaz,xbit,yrow);
	else
#endif
	Razmaz2(buf, bufrazmaz, xbit, yrow, 0, (int16_t) POROG_ANGLES);

	//porog=MAX(xbit,yrow);
	porog = MIN(50, xbit + yrow);

	i
			= CheckLetter(xbit, yrow, buf, bufrazmaz, fbase, porog, let, attr,
					nInCTB);
	return i;
}
///////////////////////////
int CompareCluster(uchar *rast, int xbyte, int xbit, int yrow, welet *wel,
		int movex, int movey) {
	int i;
	int rbyte = (xbit + 7) >> 3;
	uchar *b1;
	uchar hvost = mask0[xbit & 7];

	if (wel == NULL)
		return 0;
	if (xbyte * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / xbyte;

	// to static buffer
	for (i = 0, b1 = buf; i < yrow; i++, b1 += rbyte, rast += xbyte) {
		memcpy(b1, rast, rbyte);
		b1[rbyte - 1] &= hvost;
	}

#ifdef _USE_HALF_
	if( xbit < POROG_HALF_WIDTH || yrow < POROG_HALF_HEIGHT )
	RazmazHalf(buf,bufrazmaz,(int16_t)xbit,(int16_t)yrow);
	else
#endif
	Razmaz2(buf, bufrazmaz, (int16_t) xbit, (int16_t) yrow, (int16_t) 0,
			(int16_t) POROG_ANGLES);

	i = distOne(buf, bufrazmaz, xbit, yrow, 85, wel, movex, movey, 1);
	return MAX(0, 255 - STRAFPOINT * i);
}
///////////////////////////
#ifdef _TEST_ADD_FAT_

static int DistToWeletReal(puchar r,int fullByte,int w,int h,welet * wl,
		int xo,int yo,int porog)
{
	pchar curr;
	int i,j,jj;
	uchar cbyte,cc;
	int rbyte;
	int dist;

	// dist1 = wl->summa/wl->fill;

	curr = wl->raster + WR_MAX_WIDTH*((WR_MAX_HEIGHT-h)/2) +
	(WR_MAX_WIDTH-w)/2;
	curr += xo + WR_MAX_WIDTH * yo; // variation

	rbyte=(w+7)>>3;

	for(i=0,dist=0; i < h;i++,curr += WR_MAX_WIDTH,r+=fullByte)
	{
		for(j=jj=0;j<rbyte;j++)
		{
			cbyte=r[j];
			if(cbyte == 0) {jj+=8;continue;}

			for(cc=128;cc;cc>>=1,jj++)
			{
				if( cbyte & cc )
				{
					if( curr[jj] < 0 )
					dist+=2;
				}
				else
				if( curr[jj] > 0 ) dist++;
			} // end cc

			if( dist > porog ) return dist;
		} // end j
	} // end i

	return (dist);
}

///////////////////////
static int AddTestRecogCollection( uchar *rast,int xbit,int yrow,
		int num,RECRESULT *recres,
		welet *wl,int numWel )
{
	int rbyte=(xbit+7)>>3;
	int straf[16];
	int iSame,i,best,j;
	RECRESULT recTmp;

	if(num <= 1)
	return num;

	if(recres[0].prob < 250)
	return num;

	if(recres[1].prob < recres[0].prob)
	return num;

	for(iSame=0,best=0;iSame<num;iSame++)
	{
		if(iSame >= 16)
		break;
		if(recres[iSame].prob < recres[0].prob)
		break;
		straf[iSame]= DistToWeletReal(rast,rbyte,xbit,yrow,
				wl+recres[iSame].nClust, 0,0,100);
		if(straf[iSame] < straf[best])
		best=iSame;
	}

	for(i=0;i<iSame;i++)
	{
		straf[i]-=straf[best];
		if(straf[i]>30)
		straf[i]=30;
		if(straf[i]<recres[i].prob)
		recres[i].prob-=straf[i];
		else
		recres[i].prob=1;
	}

	// not optimal !!!
	for(i=0,j=1;i<iSame-1;)
	{
		for(j=i+1;j<iSame;j++)
		{
			if(recres[i].prob < recres[j].prob)
			{
				recTmp=recres[i];
				recres[i]=recres[j];
				recres[j]=recTmp;
				break;
			}
		}
		if(j>=iSame)
		i++;
		// else - new recres[i]
	}

	for(i=iSame;i<num;i++)
	recres[i].prob=MIN(recres[i].prob,recres[iSame-1].prob-1);

	return num;
}
///////////////////
#endif
/////////////////
// for handprinted recognition
/////////////////
//
// не штрафовать точки на расстоянии <= bound
//
#define STRAF_NOTFAR 2
#define STRAF_FAR    4
#define STRAF_CLUFAR 1

static int distOkr1(puchar r, int w, int h, welet * wl, int xo, int yo,
		int porog, int bound) {
	pchar curr;
	int i, j, jj;
	uchar cbyte, cc;
	int rbyte;
	int dist1, dist2;
	int dist;

	dist1 = wl->summa / wl->fill;
	dist1 *= bound;
	if (dist1 > 127)
		dist1 = 127;

	// not-weighted distance
	if (dist1 < 64)
		dist2 = dist1 * 2;
	else
		dist2 = 127;

	dist1 = -dist1;
	dist2 = -dist2;

	curr = wl->raster + WR_MAX_WIDTH * ((WR_MAX_HEIGHT - h) / 2)
			+ (WR_MAX_WIDTH - w) / 2;
	curr += xo + WR_MAX_WIDTH * yo; // variation

	rbyte = (w + 7) >> 3;

	for (i = 0, dist = 0; i < h; i++, curr += WR_MAX_WIDTH, r += rbyte) {
		for (j = jj = 0; j < rbyte; j++) {
			cbyte = r[j];
			if (cbyte == 0) {
				jj += 8;
				continue;
			}

			for (cc = 128; cc; cc >>= 1, jj++) {
				if (cbyte & cc) {
					if (curr[jj] < dist2)
						dist += STRAF_FAR;
					else if (curr[jj] < dist1)
						dist += STRAF_NOTFAR;
				}
			} // end cc

			if (dist > porog)
				return dist;
		} // end j
	} // end i

	return (dist);
}

///////////////////////
///////////////////////
//
// штрафуем точки cluster's
//
int16_t distOkr2(puchar r, int w, int h, welet * wl, int xo, int yo, int porog,
		int proc) {
	int16_t ww = wl->w, hh = wl->h;
	pchar curr;
	int16_t i, j;
	uchar cbyte, cc;
	int16_t rbyte;
	int dist;
	int startx = (WR_MAX_WIDTH - w) / 2;
	int starty = (WR_MAX_HEIGHT - h) / 2;
	int stx = (WR_MAX_WIDTH - ww) / 2; // start cluster
	int sty = (WR_MAX_HEIGHT - hh) / 2;
	int wei;
	int lasty, lastx;
	uchar initCC;
	uchar *rr;

	wei = wl->weight;
	wei = (wei * proc) / 100;

	startx += xo;
	starty += yo; // variation
	rbyte = (w + 7) >> 3;

	curr = wl->raster + sty * WR_MAX_WIDTH;
	dist = 0;

	// add upper lines
	for (i = sty; i < starty; i++, curr += WR_MAX_WIDTH)
		for (j = stx; j < stx + ww; j++)
			if (curr[j] > wei)
				dist++;
	if (dist > porog)
		return dist;

	lasty = MIN(starty + h, sty + hh);
	lastx = MIN(startx + w, stx + ww);
	if (starty < sty)
		r += (sty - starty) * rbyte;

	if (startx < stx) {
		initCC = 128 >> ((stx - startx) & 7);
		r += (stx - startx) >> 3;
	} else
		initCC = 128; // from first bit

	// now - start inside frame
	for (; i < lasty; i++, curr += WR_MAX_WIDTH, r += rbyte) {
		for (j = stx; j < startx; j++)
			if (curr[j] > wei)
				dist++;
		if (dist > porog)
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
		if (dist > porog)
			return dist;

		// last points in row
		for (; j < stx + ww; j++)
			if (curr[j] > wei)
				dist++;
		if (dist > porog)
			return dist;
	} // end i

	// add down lines
	for (; i < sty + hh; i++, curr += WR_MAX_WIDTH)
		for (j = stx; j < stx + ww; j++)
			if (curr[j] > wei)
				dist++;

	return (dist);
}
///////////////
static int distOneOkr(puchar r, puchar razmaz, int w, int h, int porog,
		welet * wel, int x, int y, int bound, int proc) {
	int dist, j;

	dist = distOkr1(r, w, h, wel, x, y, porog - 1, bound);
	if (dist >= porog)
		return dist;

	j = distOkr2(razmaz, w + 2, h + 2, wel, x, y, porog - dist - 1, proc);

	return dist + j;
}
///////////////////////////

static int distWeletOkr(uchar *buf, uchar *razmaz, int w, int h, welet * wl,
		int porog, int okr, int proc) {
	uint16_t best, east, west, north, south, center;
	int lbest; // local best
	int bound = 200; //2*MIN(50,w+h);
	int initPorog = porog;
	int ne, es, sw, wn;

	best = east = west = north = south = center = lbest = 0;

	// center - special threshold??? check !!!
	center = best = distOneOkr(buf, razmaz, w, h, bound, wl, 0, 0, okr, proc);
	// center =best=distOne(buf,bufraz,w,h,porog,wl,0,0);
	if (best == 0)
		return best;

	// test - if very bad, go away ... !!!
	// if(best >= bound ) return porog+1;

	if (best < porog)
		porog = best;

	if ((south = distOneOkr(buf, razmaz, w, h, porog, wl, 0, 1, okr, proc))
			< best)
		best = south;
	if (best == 0)
		return best;
	if (best < porog)
		porog = best;
	if ((north = distOneOkr(buf, razmaz, w, h, porog, wl, 0, -1, okr, proc))
			< best)
		best = north;
	if (best == 0)
		return best;
	if (best < porog)
		porog = best;
	if ((east = distOneOkr(buf, razmaz, w, h, porog, wl, 1, 0, okr, proc))
			< best)
		best = east;
	if (best == 0)
		return best;
	if (best < porog)
		porog = best;
	if ((west = distOneOkr(buf, razmaz, w, h, porog, wl, -1, 0, okr, proc))
			< best)
		best = west;

	if (best == 0)
		return best;
	if (best < porog)
		porog = best;

	// if(best < north && best < south && best < east && best < west)
	//	 return best;

	// nothing good ?
	if (best >= initPorog) {
		north = south = east = west = best;
		//	 return best;
	} else if (best < north && best < south && best < east && best < west)
		return best;

	center = best; // to save old best for compare
	// where try move ? - now two positions may be
	ne = es = sw = wn = 0;

	if (north == center || east == center) {
		if ((lbest = distOneOkr(buf, razmaz, w, h, porog, wl, 1, -1, okr, proc))
				< best) {
			best = lbest;
			ne = 1;
			if (best < porog)
				porog = best;
		}
	}
	if (east == center || south == center) {
		if ((lbest = distOneOkr(buf, razmaz, w, h, porog, wl, 1, 1, okr, proc))
				< best) {
			best = lbest;
			if (best < porog)
				porog = best;
			ne = 0;
			es = 1;
		}
	}
	if (south == center || west == center) {
		if ((lbest = distOneOkr(buf, razmaz, w, h, porog, wl, -1, 1, okr, proc))
				< best) {
			best = lbest;
			if (best < porog)
				porog = best;
			ne = es = 0;
			sw = 1;
		}
	}
	if (west == center || north == center) {
		if ((lbest
				= distOneOkr(buf, razmaz, w, h, porog, wl, -1, -1, okr, proc))
				< best) {
			best = lbest;
			wn = 1;
			ne = es = sw = 0;
		}
	}

	{
		int movX = 0, movY = 0;
		if (ne) {
			movX = 2;
			movY = -2;
		} else if (es) {
			movX = 2;
			movY = 2;
		} else if (sw) {
			movX = -2;
			movY = 2;
		} else if (wn) {
			movX = -2;
			movY = -2;
		} else if (south == best)
			movY = 2;
		else if (north == best)
			movY = -2;
		else if (east == best)
			movX = 2;
		else if (west == best)
			movX = -2;

		if ((lbest = distOneOkr(buf, razmaz, w, h, porog, wl, movX, movY, okr,
				proc)) < best)
			best = lbest;
	}
	return best;
}

///////////////////////////
///////////////////////////
// 0xba - обозначение для i с приклееной точкой
//static const char Palki[]="!1Iil\xba";
/////////////////
static int LookBestOkr(int w, int h, uchar *buf, uchar *razmaz, int NumClus,
		int porog, welet *wel, RECRESULT *recres, int maxNames, int nInCTB,
		int col, int row, int okr, int proc) {
	int i, j;
	int dist;
	int num = 0;

	for (i = num = 0; i < NumClus; i++, wel++) {
		if (wel->invalid)
			continue;
		if (!(wel->attr & FON_CLU_SOLID))
			continue;

		// не распознавать себя ?
		if (wel->weight == 1 && (wel->nInCTB == nInCTB ||
		//wel->sr_col == col && wel->sr_row==row
				abs(col - wel->sr_col) <= 1 && abs(row - wel->sr_row) <= 1))
			//	  wel-> work < 230 )
			continue;
		//   dist=(255 - wel->work + 4)/(2*STRAFPOINT) ;

		if (wel->num < 0) // не из нашего алфавита
			continue;

		// check for height likeness
		j = abs(wel->mh - h);
		if (j > 2 && j * 3 > MAX(h, wel->mh))
			continue;

		j = abs(wel->mw - w);
		if (j > 3 && j * 3 >= MAX(w, wel->mw))
			continue;

		dist = distWeletOkr(buf, razmaz, w, h, wel, porog + 1, okr, proc);

		if (dist <= porog) {
			num = AddVersion(recres, (uchar) wel->let, (uchar)(255 - dist), i,
					num, maxNames);
			if (num >= maxNames) {
				porog = 255 - recres[maxNames - 1].prob - 1;
				if (porog < 0)
					break;
			}
			//	if(dist==0) break;
		}

	}

	return num;
}

///////////////////////
int RecogCluOkr(uchar *rast, int16_t xbyte, int16_t xbit, int16_t yrow,
		RECRESULT *recres, int16_t maxNames, welet *wl, int numWel, int porog,
		int nInCTB, int16_t col, int16_t row, int okr, int proc) {
	int i;
	int rbyte = (xbit + 7) >> 3;
	uchar *b1;
	uchar hvost = mask0[xbit & 7];

	if (wl == NULL || numWel <= 0)
		return 0;
	if (xbyte * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / xbyte;

	for (i = 0, b1 = buf; i < yrow; i++, b1 += rbyte, rast += xbyte) {
		memcpy(b1, rast, rbyte);
		b1[rbyte - 1] &= hvost;
	}

	Razmaz2(buf, bufrazmaz, xbit, yrow, 0, POROG_ANGLES);

	//porog=MAX(xbit,yrow);
	if (porog < 0)
		porog = 200;

	if (maxNames > MAX_ALT)
		maxNames = MAX_ALT;
	i = LookBestOkr(xbit, yrow, buf, bufrazmaz, numWel, porog, wl, recres,
			maxNames, nInCTB, col, row, okr, proc);

	return i;
}
///////////////////////////
int CompareClusterOkr(uchar *rast, int xbyte, int xbit, int yrow, welet *wel,
		int movex, int movey, int okr, int proc, int *dist1, int *dist2) {
	int i;
	int rbyte = (xbit + 7) >> 3;
	uchar *b1;
	uchar hvost = mask0[xbit & 7];

	if (wel == NULL)
		return 0;
	if (xbyte * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / xbyte;

	// to static buffer
	for (i = 0, b1 = buf; i < yrow; i++, b1 += rbyte, rast += xbyte) {
		memcpy(b1, rast, rbyte);
		b1[rbyte - 1] &= hvost;
	}
	Razmaz2(buf, bufrazmaz, (int16_t) xbit, (int16_t) yrow, 0, POROG_ANGLES);

	i = distOneOkr(buf, bufrazmaz, xbit, yrow, 250, wel, movex, movey, okr,
			proc);
	*dist1 = distOkr1(buf, xbit, yrow, wel, movex, movey, 250, okr);
	*dist2 = distOkr2(bufrazmaz, xbit + 2, yrow + 2, wel, movex, movey, 250,
			proc);
	return 255 - i;
}
///////////////////////////
// сколько точек вылазит
static int distOkr(puchar r, int w, int h, welet * wl, int xo, int yo,
		int porog) {
	pchar curr;
	int i, j, jj;
	uchar cbyte, cc;
	int rbyte;
	int dist;

	curr = wl->raster + WR_MAX_WIDTH * ((WR_MAX_HEIGHT - h) / 2)
			+ (WR_MAX_WIDTH - w) / 2;
	curr += xo + WR_MAX_WIDTH * yo; // variation

	rbyte = (w + 7) >> 3;

	for (i = 0, dist = 0; i < h; i++, curr += WR_MAX_WIDTH, r += rbyte) {
		for (j = jj = 0; j < rbyte; j++) {
			cbyte = r[j];
			if (cbyte == 0) {
				jj += 8;
				continue;
			}

			for (cc = 128; cc; cc >>= 1, jj++) {
				if (cbyte & cc) {
					if (curr[jj] < 0)
						dist++;
				}
			} // end cc

			if (dist > porog)
				return dist;
		} // end j
	} // end i

	return (dist);
}

///////////////////////
//#define KOEFF 4
static int LookBestInner(int w, int h, uchar *buf, uchar *razmaz, int NumClus,
		welet *wel, RECRESULT *recres, int maxNames, int nInCTB, int16_t *col,
		int16_t *row) {
	int i, j, k;
	int dist;
	int num = 0;
	int x, y;
	int porog;
	int dists[9];
	int best;
	int startx, starty;
	int stepx, stepy;
	int lastx, lasty;
	int brow, bcol;
	float KOEFF = MAX(1.0f, (w * h) / 250.0f);

	for (i = num = 0; i < NumClus; i++, wel++) {
		if (wel->invalid)
			continue;
		if (!(wel->attr & FON_CLU_SOLID))
			continue;

		// не распознавать себя ?

		if (wel->num < 0) // не из нашего алфавита
			continue;

		if (wel->mh * 2 < h || wel->mw * 2 < w)
			continue;

		porog = 240;

#ifdef _NOT_ALL_
		best = -1;
		for (k = 0; k < 9; k++)
			dists[k] = porog;

		for (y = -1, k = 0; y <= 1; y++) {
			for (x = -1; x <= 1; x++, k++) {
				dist = (int) (distOkr(buf, w, h, wel, x, y, (int) (porog
						* KOEFF)) / KOEFF);
				if (dist >= porog)
					continue;
				j = distOkr2(razmaz, w + 2, h + 2, wel, x, y, porog - dist, 30);
				dist += j;

				dists[k] = dist;

				if (dist < porog) {
					porog = dist;
					best = k;
					brow = y;
					bcol = x;
					if (porog == 0)
						break;
				}
			}
		}

		if (porog == 0)
			goto addvers;

		switch (best) {
		case 0:
			startx = starty = -2;
			stepx = stepy = -1;
			lastx = lasty = -5;
			break;
		case 1:
			startx = -2;
			starty = -2;
			stepx = 1;
			stepy = -1;
			lastx = 3;
			lasty = -5;
			break;
		case 2:
			startx = 2;
			starty = -2;
			stepx = 1;
			stepy = -1;
			lastx = 5;
			lasty = -5;
			break;
		case 3:
			startx = -2;
			starty = -2;
			stepx = -1;
			stepy = 1;
			lastx = -5;
			lasty = 3;
			break;
		case 4:
			goto addvers;
		case 5:
			startx = 2;
			starty = -2;
			stepx = 1;
			stepy = 1;
			lastx = 5;
			lasty = 3;
			break;
		case 6:
			startx = -2;
			starty = 2;
			stepx = -1;
			stepy = 1;
			lastx = -5;
			lasty = 5;
			break;
		case 7:
			startx = -2;
			starty = 2;
			stepx = 1;
			stepy = 1;
			lastx = 3;
			lasty = 5;
			break;
		case 8:
			startx = 2;
			starty = 2;
			stepx = 1;
			stepy = 1;
			lastx = 5;
			lasty = 5;
			break;
		default:
			continue;
		}
#else
		startx= -4;
		starty= -4;
		stepx = 1;
		stepy = 1;
		lastx = 5;
		lasty = 5;
#endif

		for (x = startx; x != lastx; x += stepx) {
			for (y = starty; y != lasty; y += stepy) {
				dist = (int) (distOkr(buf, w, h, wel, x, y, (int) (porog
						* KOEFF)) / KOEFF);
				if (dist >= porog)
					continue;
				j = distOkr2(razmaz, w + 2, h + 2, wel, x, y, porog - dist, 30);
				dist += j;
				if (dist < porog) {
					porog = dist;
					brow = y;
					bcol = x;
					if (porog == 0)
						break;
				}
			}
			if (porog == 0)
				break;
		} //  end x

		addvers: if (porog >= 240)
			continue;

		if (num <= 0 || recres[0].prob < 255 - porog) {
			*row = brow;
			*col = bcol;
		}

		num = AddVersion(recres, (uchar) wel->let, (uchar)(255 - porog), i,
				num, maxNames);

	}

	return num;
}
///////////////////////
static int ScaleSymbol(uchar *inbuf, int fullByte, int allSizeX, int allSizeY,
		uchar *outbuf, int newX, int newY);

int RecogCluInner(uchar *rast, int16_t xbyte, int16_t xbit, int16_t yrow,
		RECRESULT *recres, int16_t maxNames, welet *wl, int numWel, int nInCTB,
		int16_t *col, int16_t *row) {
	int i;
	int rbyte = (xbit + 7) >> 3;
	uchar *b1;
	uchar hvost = mask0[xbit & 7];
	Bool32 needCopy = TRUE;

	if (wl == NULL || numWel <= 0)
		return 0;
	if (xbit <= 0 || yrow <= 0)
		return 0;

	if (xbyte * yrow > REC_MAX_RASTER_SIZE)
		yrow = REC_MAX_RASTER_SIZE / xbyte;

#ifdef _NEED_SCALE_
	// need scale ?
	if( *col > 0 || *row > 0 )
	{
		int newX=*col,newY=*row;

		if( newY <= 0 )
		newY = (yrow*newX)/xbit;

		if( newY <= 0)
		return 0;

		// our symbol inside full frame must be STANDARD_HEIGHT
		// STANDARD_WIDTH - don't use
		newX = (xbit*STANDARD_HEIGHT)/newY;
		newY = (yrow*STANDARD_HEIGHT)/newY;

		if( newX != xbit || newY != yrow )
		{
			int AddBitmapToSnap(uchar *buf,int xbit,int yrow,int name,int dist);
			i= ScaleSymbol(rast,xbyte,xbit, yrow, buf, newX,newY);
			if( i < 0 )
			return i;
			needCopy = FALSE;
			xbit = newX;
			yrow = newY;
			// to view ...
			//	 AddBitmapToSnap(buf,xbit, yrow,0,newY);
			//	 FONShowSnap();
		}
	}
#endif

	if (needCopy) {
		for (i = 0, b1 = buf; i < yrow; i++, b1 += rbyte, rast += xbyte) {
			memcpy(b1, rast, rbyte);
			b1[rbyte - 1] &= hvost;
		}
	}

	Razmaz2(buf, bufrazmaz, xbit, yrow, 0, POROG_ANGLES);

	if (maxNames > MAX_ALT)
		maxNames = MAX_ALT;
	i = LookBestInner(xbit, yrow, buf, bufrazmaz, numWel, wl, recres, maxNames,
			nInCTB, col, row);

	return i;
}
///////////////////////////
static uchar *tmpBufScale = NULL;

void EndScale(void) {
	if (tmpBufScale)
		free(tmpBufScale);

	tmpBufScale = NULL;
}
/////////////
static int ScaleSymbol(uchar *inbuf, int fullByte, int allSizeX, int allSizeY,
		uchar *outbuf, int newX, int newY) {
	int allSize;
	int i, j;
	uchar cc;
	uchar *tbuf, *cbuf;
	int bigStep, oneStep;
	uchar *obuf;
	int xbyte;
	int k, kk;
	int summa;

	if (allSizeX <= 0 || allSizeY <= 0)
		return -1;

	xbyte = (newX + 7) >> 3;

	if (xbyte * newY > REC_MAX_RASTER_SIZE)
		newY = REC_MAX_RASTER_SIZE / xbyte;

	allSize = allSizeX * newX * allSizeY * newY;
	tmpBufScale = realloc(tmpBufScale, allSize);

	if (!tmpBufScale)
		return -2;

	memset(tmpBufScale, 0, allSize);

	bigStep = allSizeX * newX * newY;
	oneStep = allSizeX * newX;

	obuf = inbuf;
	for (i = 0, tbuf = tmpBufScale; i < allSizeY; i++, tbuf += bigStep, obuf
			+= fullByte) {
		for (j = 0, cc = 128; j < allSizeX; j++, cc >>= 1) {
			if (!cc)
				cc = 128;
			if (obuf[j >> 3] & cc) {
				for (k = 0, cbuf = tbuf + j * newX; k < newY; k++, cbuf
						+= oneStep)
					memset(cbuf, 1, newX);
			}
		}
	}

	memset(outbuf, 0, xbyte * newY);
	obuf = outbuf;

	bigStep = allSizeX * newX * allSizeY;

	for (i = 0, tbuf = tmpBufScale; i < newY; i++, tbuf += bigStep, obuf
			+= xbyte) {
		for (j = 0, cc = 128; j < newX; j++, cc >>= 1) {
			if (cc == 0)
				cc = 128;

			cbuf = tbuf + j * allSizeX;
			summa = 0;
			for (k = 0; k < allSizeY; k++, cbuf += oneStep) {
				for (kk = 0; kk < allSizeX; kk++)
					summa += cbuf[kk];
			}

			if (summa * 2 >= allSizeX * allSizeY)
				obuf[j >> 3] |= cc;
		}
	}

	return 1;
}
//////////////////
