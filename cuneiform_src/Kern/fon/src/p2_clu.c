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

// проба разбить некоторые кластеры
#define _TEST_DIVIDE_

//   for use in cl_bcwin   #define _WINEDIT_
//   for non-flat model    #define _NONFLAT_


#define _UNION_ONEONE_    // union 1-1 clusters
#define _NEW_POROG_
//#define _TEST_FAT_
#define _POROG_FAT_ 70  // 70 - good for cten42, very bad cten100
// 80 - better for cten100, bad cten42

// add symbols to clusters (if exist)
// or create new clusters always
#define _ADDCLU_
// try union clusters with 1 elements with other
// and non-solid with solid
#define _ADDONE_
// try union clusters with 1 elements with other  names
// and non-solid with solid
#define _RENAME_
///////////////////////
#include "fon.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*#include <io.h>*/
#include <fcntl.h>
#include <sys/stat.h>
#ifdef _GETTIME_
#include <time.h>
#endif

#include "sfont.h"
#include "ctb.h"

#include "compat_defs.h"

//#include "frombas.h"
int OpenBase(char *);
void CloseBase(void);

static SINT ReadAllFromBase(CHAR *name, SINT *nClu, CHAR *movxy, SINT AllCount);

#define memmove memmove
#define MAXINCLUS 127

// if numpoint > level_grey*POROG_IDEAL  - remove this grey point
#define POROG_IDEAL 10
// for Signal()   1 - for accept
#define SIGNAL_START    9
#define SIGNAL_CLUSTER  50
#define SIGNAL_SAVE     40
void Signal(void);

typedef LONG (* MKFAM)(raster_header * rh, WORD nclu);

// now - common number of symbols <= MAXSYM
// number in one weighted raster <= MAXINCLUS (127)
// must be MAXINCLUS*3 < MAXSYM ! (used in SaveCluster)
// must be sizeof(welet) >= MAXSYM*sizeof(SINT)
//  -use dist_wel as mysteck
//   (now 128*64+... > 8192)
//
// working buffer
static CHAR mybuffer[MAX(2*MAXSYM * sizeof(SINT), max(2*sizeof (welet ),sizeof(access_tab)))];
welet *welBuf = (welet *) mybuffer;
welet *dist_wel = (welet *) (mybuffer + MAX(MAXSYM * sizeof(SINT),
		sizeof(welet))); // use as

// union twins with solid? - tiger
//             remove week? - CTB
static BYTE p2_active = 4;
static SINT porogCluster = 2;
static SINT IsCTBBase = 1; // load from b/w CTB
static SINT OutCTBBase = 1; // save as grey CTB
static BYTE metkaGoodStat[MAXWEICLUS * 2];
static BYTE saveOnlyBest = 0; // make one font ?
static uint32_t allFields[4][NFIELDDWORD];
#define MAXKEGL 127
static SINT keglBuffer[MAXKEGL + 1];
////////////////
// ctb-functions
int StartCTB(char *outname, CTB_handle *ccc, Int16 countFont, uint32_t *allFil);
void EndCTB(CTB_handle *ccc);
int SaveWeletAsCTB(welet *wel, CTB_handle *ccc);
///////////////////
int FindBestClusters(int numSymbol, int numCluster, Nraster_header *rh,
		SINT *nClus, BYTE *metka, BYTE *metkaValid, int maxOutFonts,
		uint32_t *ffFields);
int MultiFindBestClusters(int numSymbol, int numCluster, Nraster_header *rh,
		SINT *nClus, BYTE *metka, BYTE *metkaValid);
int GetProbValid(int numSymbol, int numCluster, Nraster_header *rh,
		SINT *nClus, BYTE *metkaGood, BYTE *metkaValid);
SINT AddClusterHausdorf(CHAR *NameWr, CHAR *szOutName, SINT porog, SINT porog2,
		MKFAM accept, PBYTE extern_buf, LONG size_extern, clu_info *cin);
SINT SetAccessTab(SINT fl, void *buf);
SINT CheckAccessTab(SINT fh, void *buf);
static SINT ReOrderClusters(SINT NumClus, SINT NumAll, clu_info *cin);
static SINT TestUnionSolid(SINT porog, SINT NumAll, SINT Clus2, SINT NumClus);
SINT MakeMoved(BYTE *etalon, SINT xbyte, SINT yrow, BYTE *tmpbuf);
static SINT UnionOneAll(SINT fir, SINT las, BYTE *buf, BYTE *bufr, SINT xbyte,
		SINT yrow, WORD CurName, SINT porog, SINT *NumIn);
static SINT TestUnionOne(SINT porog, SINT NumAll, SINT NumClus);
SINT FindDistanceWr(welet *wel, welet *outwel);
SINT CheckCenterSymbol(BYTE *b1, SINT xbyte, SINT yrow, BYTE *buf2, BYTE *tbuf,
		SINT xbit2, SINT yrow2, SINT *sdvigx, SINT *sdvigy, SINT sum);
void init11(void);
SINT SaveCluster(SINT fh, CTB_handle *cc, SINT fhh, CTB_handle *ccc, SINT clus,
		SINT NumAll, BYTE *m1, BYTE *m2);

SINT NumHauBit = 0; // number of bitmap buffers
static BYTE *BitHau[MAXHAU]; // big buffers
static LONG LastBit = 0;
Nraster_header *rh = NULL;
SINT nClus[MAXSYM];
Word8 language = 0, langCyrilRoman = 0;
static SINT clusBuffer[MAXSYM];
static SINT *mysteck = NULL; // == dist_wel

static uint32_t MaxSizeBuf = 0; // space in buffer for bitmaps
static BYTE IsRhHauBuf = 0; // 0 - use extern buffer for rh, BitHau[0]
// 1 - is only rh buf
// 2 - exist rh,BitHau[0]

//  number of 1-s in all bytes
const int Num11[256] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2,
		2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3,
		3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2,
		2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
		4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4,
		4, 5, 4, 5, 5, 6, 4, 5, 5,
		6,
		5,
		6,
		6,
		7, // 0 - 127
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7,
		5, 6, 6, 7, 6, 7, 7, 8 };
static BYTE mas00[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

static void (*PutPercent)(uint32_t pp) = NULL;
//////////
static uint32_t my_percent = 0;
//////
void Signal(void) {
	if (PutPercent)
		PutPercent(++my_percent);
	// progress_set_percent(++my_percent);
}
/**************************/

void EndHausdorfDLL(void) {
	SINT i;

	for (i = (IsRhHauBuf >= 2 ? 0 : 1); i < NumHauBit; i++) {
		if (BitHau[i] != NULL)
			free(BitHau[i]);
		BitHau[i] = NULL;
	}
	BitHau[0] = NULL;

	if (IsRhHauBuf != 0 && rh != NULL)
		free(rh);
	rh = NULL;
	NumHauBit = 0;

	EndNumMemory();
}
/***********************/
// was parameter num - number of input symbols ( 0 if unknown)
// use ExternBuf - if not NULL && SizeExternBuf > 0
//  return > 0 - how many memory alloccated
//
LONG StartHausdorfDLL(int num, void *ExternBuf, uint32_t SizeExternBuf) {
	LONG size = 0;

#ifdef _NONFLAT_
	IsRhHauBuf=2;
	rh=(Nraster_header *)malloc((uint32_t)MAXSYM*sizeof(Nraster_header));
	if(rh==NULL) return -1;

	NumHauBit=0;
	// init bitmap for buffers
	BitHau[0]=(BYTE *)malloc(SIZEBUF); // take one segment
	if(BitHau[0]==NULL) return -1;
	MaxSizeBuf=SIZEBUF;
	size=(uint32_t)MAXSYM*sizeof(Nraster_header)+SIZEBUF;
#else
	if (num <= 0)
		num = MAXSYM;
	if (ExternBuf == NULL) {
		rh = (Nraster_header *) malloc((uint32_t) MAXSYM
				* sizeof(Nraster_header) + (uint32_t) SIZEBUF);
		if (rh == NULL)
			return -1;
		BitHau[0] = (BYTE *) rh;
		BitHau[0] += (uint32_t) num * sizeof(Nraster_header);
		IsRhHauBuf = 1;
		MaxSizeBuf = (uint32_t) MAXSYM * sizeof(Nraster_header)
				+ (uint32_t) SIZEBUF - (uint32_t) num * sizeof(Nraster_header);
		size = (uint32_t) MAXSYM * sizeof(Nraster_header) + (uint32_t) SIZEBUF;
	} else if (SizeExternBuf >= (uint32_t) num * sizeof(Nraster_header)) {
		rh = (Nraster_header *) ExternBuf;
		BitHau[0] = (BYTE *) rh;
		BitHau[0] += (uint32_t) num * sizeof(Nraster_header);
		MaxSizeBuf = SizeExternBuf - (uint32_t) num * sizeof(Nraster_header);
		IsRhHauBuf = 0;
		size = 0;
	} else // use extern buffer for BitHau[0]
	{
		rh = (Nraster_header *) malloc((uint32_t) MAXSYM
				* sizeof(Nraster_header));
		if (rh == NULL)
			return -1;
		BitHau[0] = (BYTE *) ExternBuf;
		MaxSizeBuf = SizeExternBuf;
		IsRhHauBuf = 1;
		size = (uint32_t) MAXSYM * sizeof(Nraster_header);
	}
#endif

	NumHauBit = 1; // number of bitmap buffers
	LastBit = 0; // first empty byte in BitHau

	mysteck = (SINT *) dist_wel;

	return size;
}

/***********************/
BYTE *AddBuffer(LONG sizebitmap) {
	BYTE *bubu;

	if ((uint32_t) (LastBit + sizebitmap) > MaxSizeBuf) {
		// get new buffer
		if (NumHauBit >= MAXHAU)
			return NULL;
		BitHau[NumHauBit] = malloc(SIZEBUF);
		if (BitHau[NumHauBit] == NULL)
			return NULL;
		NumHauBit++;
		LastBit = 0;
		MaxSizeBuf = SIZEBUF;
	}

	bubu = BitHau[NumHauBit - 1] + LastBit;
	LastBit += sizebitmap;

	return bubu;
}
/*********************/
// Hausdorf distance from b1 to b2
// in b1 black poSINT == 1 !
// b2 in reverse - black==0
SINT DistanceHausDLL(BYTE *b1, SINT xbyte1, SINT yrow1, BYTE *b2, SINT xbyte2,
		SINT yrow2, SINT porog) {
	SINT i, j;
	SINT xbyte = MIN(xbyte1, xbyte2);
	SINT yrow = MIN(yrow1, yrow2);
	SINT dist;

	for (i = 0, dist = 0; i < yrow; i++, b1 += xbyte1, b2 += xbyte2) {
		for (j = 0; j < xbyte; j++) {
			dist += Num11[b1[j] & b2[j]]; // was ~b2[j]
			if (dist > porog)
				return dist;
		}
		for (j = xbyte; j < xbyte1; j++) {
			dist += Num11[b1[j]];
			if (dist > porog)
				return dist;
		}
	}

	// if yrow2 < yrow1
	for (i = yrow; i < yrow1; i++, b1 += xbyte1) {
		for (j = 0; j < xbyte1; j++) {
			dist += Num11[b1[j]];
			if (dist > porog)
				return dist;
		}
	}

	return dist;
}
/**************************************/
// make next bitmaps
// if nClu != NULL && nClu[num] > 0 ( already set cluster)
// make only good picture
static SINT MakeBitmapsDLL(Nraster_header *rhh, BYTE *pp, SINT num, SINT *nClu,
		CHAR *movxy) {
	SINT j, i;
	SINT sx = rhh->w, sy = rhh->h, sxbyte;
	LONG sizebitmap;
	BYTE *pic;

	// make good and razmaz pictures
	if (nClu == NULL || num >= 0 && nClu[num] <= 0) {
		rhh->xbyte = sxbyte = (sx >> 3) + 1;
		sizebitmap = sxbyte * sy;
		if ((rhh->pHau = AddBuffer(2* sizebitmap + sxbyte)) == NULL)
			return -1;

		pic = rhh->pHau;
		j = (sx + 7) >> 3; // real bytes
		if (sxbyte == j) // not add empty bytes
			memcpy(pic, pp, sizebitmap);
		else {
			for (i = 0; i < sy; i++, pic += sxbyte, pp += j)
				memcpy(pic, pp, j);
		}

		rhh->pHaur = rhh->pHau + sxbyte * sy;
		rhh->fat = (BYTE) Razmaz(rhh->pHau, rhh->pHaur, sxbyte, sx, sy,
				rhh->bold);
	}

	else // make only good picture - use movxy as buffer for saved
	// best movx, movy
	{
		sxbyte = (sx + 7) >> 3;
		sizebitmap = sxbyte * sy;
		if ((rhh->pHau = AddBuffer(sizebitmap)) == NULL)
			return -1;
		pic = rhh->pHau;
		memcpy(pic, pp, sizebitmap);

		//  save - how many need move
		rhh->sr_col = *(movxy + 2* num );
		rhh->sr_row = *(movxy + 2* num + 1);
	}

	return 0;
}
/////////////////////
// how many symbols ?
// use nClus as buffer
SINT GetNumSym(CHAR *NameWr) {
	SINT fh;
	SINT i;
	SINT num;
	BYTE *buf = (BYTE *) nClus;
	SINT size = sizeof(nClus), csize;
	raster_header *rh;
	int j;

	if (IsCTBBase) {
		i = OpenBase(NameWr);
		CloseBase();
		return i;
	}

	fh = open(NameWr, O_RDWR | O_BINARY);
	if (fh < 0)
		return (-2);

	csize = read(fh, buf, size);
	if (csize <= 0) {
		close(fh);
		return -3;
	}

	for (i = 0, num = 0;;) {
		while ((SINT) (num + sizeof(raster_header)) <= csize) {
			i++;
			rh = (raster_header *) (buf + num);
			num += ((rh->w + 7) >> 3) * rh->h + sizeof(raster_header);
		}

		// stay part of raster header?
		if (num < csize) {
			memcpy(buf, buf + num, csize - num);
			j = read(fh, buf + csize - num, size - csize + num);
			if (j <= 0)
				break;
			csize = csize - num + j;
			num = 0;
		} else {
			num -= csize;
			csize = read(fh, buf, size);
			if (csize <= 0)
				break;
		}
	}

	close(fh);
	return i;
}
/***********************/
// use buf as buffer for reading
// must :size of picture + sizeof(raster_header) <= size) !
SINT ReadAllFromWr(CHAR *name, BYTE *buf, SINT size, SINT *nClu, CHAR *movxy,
		SINT NumAll, SINT AllCount) {
	SINT allnum;
	SINT fh, i;
	SINT csize;
	SINT num;
	raster_header *rhh;
	SINT CurCount = 0;

	if (IsCTBBase)
		return ReadAllFromBase(name, nClu, movxy, AllCount);

	if ((fh = open(name, O_RDWR | O_BINARY)) == -1)
		return -2;

	for (allnum = 0, num = 0; allnum < NumAll;) {
		if ((csize = read(fh, buf + num, size - num)) <= 0)
			break;
		csize += num; // all bytes in buffer
		num = 0;

		while ((SINT) (num + sizeof(raster_header)) < csize) {
			rhh = (raster_header *) (buf + num);
			if (num + sizeof(raster_header) + ((rhh->w + 7) >> 3) * rhh->h
					> (unsigned) csize)
				break;

			// to memory - only need parameters
			memset(rh + allnum, 0, sizeof(Nraster_header));
			rh[allnum].w = rhh->w;
			rh[allnum].h = rhh->h;
			rh[allnum].let = rhh->let;
			rh[allnum].sr_col = rhh->sr_col;
			rh[allnum].sr_row = rhh->sr_row;
			rh[allnum].solid = (BYTE) rhh->solid;

			if ((i = MakeBitmapsDLL(rh + allnum, (BYTE *) (rhh + 1), allnum,
					nClu, movxy)) < 0) { // error
				close(fh);
				return i;
			}
			allnum++;
			if (allnum >= MAXSYM) {
				close(fh);
				return allnum;
			}

			// to next raster
			num += sizeof(raster_header) + ((rhh->w + 7) >> 3) * rhh->h;

			if (++CurCount == AllCount) {
				CurCount = 0;
				Signal();
			}
		}

		memcpy(buf, buf + num, csize - num);
		num = csize - num;
	}

	close(fh);
	return allnum;
}
/////////////////////
// save symbols & call function
// use mysteck as buffer
SINT SaveSym(CHAR *NameWr, SINT NumAll, BYTE *buf, SINT size, MKFAM accept) {
	SINT fh;
	SINT i;
	SINT num;
	SINT csize;
	raster_header *rh;
	LONG position = 0;

	if (IsCTBBase)
		return 0;

	fh = open(NameWr, O_RDWR | O_BINARY);
	if (fh < 0)
		return (-10);

	for (i = 0, num = 0; i < NumAll;) {
		csize = read(fh, buf, size);
		if (csize <= 0)
			break;
		num = 0;

		while ((SINT) (num + sizeof(raster_header)) <= csize) {
			rh = (raster_header *) (buf + num);
			rh->num = nClus[i];
			if (accept)
				accept(rh, (SINT) (nClus[i] - 1));
			num += ((rh->w + 7) >> 3) * rh->h + sizeof(raster_header);
			i++;
			if (i >= NumAll)
				break;
		}

		// stay part of raster header?
		if (num < csize) {
			// save neew rh->num
			lseek(fh, position, SEEK_SET);
			write(fh, buf, num);
			position += num;
		} else {
			lseek(fh, position, SEEK_SET);
			write(fh, buf, csize);
			position += num;
			lseek(fh, num - csize, SEEK_CUR);
		}
	}

	close(fh);
	return 0;
}
/***********************/
//////////////////////////
// make clusters with porog - (quick clusters)
// start from symbol fir
// pHau - massiv of NumAll*2 bitmaps
//    real bitmap -( xbyte*yrow),next razmaz - (xbyte*(yrow+1))
// use pHau[], rh,
SINT MakeClusters(SINT fir, SINT NumAll, SINT CurClus, SINT porog,
		SINT AllCount) {

	SINT i, j;
	SINT IsSame, IsNew;
	SINT NumSame;
	WORD CurName;
	BYTE *buf, *bufr;
	SINT xbyte, yrow; // size of current
	SINT dist;
	SINT CurCount = 0;
	SINT sporog = porog;

	if (fir >= NumAll)
		return CurClus - 1;
	i = fir;
	nClus[i] = CurClus;
	CurName = rh[i].let;
	IsSame = NumSame = 0;

	// main circle
	for (j = i + 1;;) {
		buf = rh[i].pHau;
		xbyte = rh[i].xbyte;
		yrow = rh[i].h;
		bufr = rh[i].pHaur;

		for (IsNew = -1; j < NumAll; j++) {
			if (nClus[j])
				continue; // already tested or for second pass
			if (rh[j].let != CurName) // different name
			{
				if (IsNew == -1)
					IsNew = j;
				continue;
			}

			if (rh[i].fat || rh[j].fat)
				porog = 0;
			else
				porog = sporog;

			// newTest - for big letters!
			//	 if( xbyte >= 4 && yrow > 30 ) porog<<=1;

			dist = DistanceHausDLL(buf, xbyte, yrow, rh[j].pHaur, rh[j].xbyte,
					(SINT) (rh[j].h + 1), porog);

			if (dist <= porog)
				dist = DistanceHausDLL(rh[j].pHau, rh[j].xbyte, rh[j].h, bufr,
						xbyte, (SINT) (yrow + 1), porog);

			if (dist <= porog) {
				nClus[j] = CurClus;
				mysteck[NumSame++] = j;
				if (++CurCount == AllCount) {
					CurCount = 0;
					Signal();
				}
			} else if (IsNew == -1)
				IsNew = j;

		} // end j

		if (IsNew == -1)
			break; /* study all */
		if (IsSame >= NumSame) // is from the same cluster ? - no
		{
			CurClus++;
			i = IsNew;
			nClus[i] = CurClus;
			j = IsNew + 1;
			CurName = rh[IsNew].let;
			if (++CurCount == AllCount) {
				CurCount = 0;
				Signal();
			}
		} else {
			i = mysteck[IsSame++]; // continue add to the same cluster
			j = IsNew;
		}

	} // end main circle

	return CurClus;
}
///////////////
// load symbols, make clusters with threshold
// call external function
// save clusters
// NumAll - number of symbols
// results - in nClus ,
static SINT ClusterHausdorfDLL(CHAR *NameWr, SINT porog, CHAR *szOutName,
		MKFAM accept, PBYTE extern_buf, LONG size_extern, clu_info *cin) {
	SINT i;
	SINT CurClus = 0;
	SINT NumAll;
	SINT fh;
	SINT ret = 0;
	LONG position;
	SINT Clus2 = 0; // start non-solid clusters
	SINT Pass2 = 0;
	SINT AllCount = 0; // how many times Signal
	SINT CurCount, j; // need signal now ?
#ifdef _GETTIME_
	clock_t cl1,cl2,cl3,cl4,cl5;
	cl1=clock();
#endif
	BYTE *metkaGood = NULL;
	BYTE *metkaValid;
	SINT fhSnap = -1; // 30.10.98
	CTB_handle CTBfile, CTBsnap, *CTBpointer = NULL;
	Int16 countFont = 0;

	if (NameWr == NULL) {
		if ((NumAll = (SINT) GetNumMemory()) <= 0)
			return NumAll;
	}

	// если растры не лежат в памяти - берем из файла
	else {
		// if don't no how many symbols - find how many (in order to find start of
		//    pictures (BitHau) in common for rh,BitHau buffer
		NumAll = GetNumSym(NameWr);
		if (NumAll <= 0)
			return NumAll;
		if (NumAll > MAXSYM)
			NumAll = MAXSYM;

		// how many memory allocated ?
		position = StartHausdorfDLL(NumAll, extern_buf, size_extern);
		if (position < 0) //  no memory
		{
			EndHausdorfDLL();
			return -1;
		}

		cin->memused = position;

		// make bitmaps
		NumAll = ReadAllFromWr(NameWr, (BYTE *) mysteck, sizeof(welet), NULL,
				NULL, NumAll, (SINT) (NumAll / SIGNAL_START));
		if (NumAll <= 0) // read invalid
		{
			EndHausdorfDLL();
			return NumAll;
		}

#ifdef _GETTIME_
		cl2=clock();
#endif
	}

	memset(nClus, 0, MAXSYM * sizeof(SINT));

	AllCount = NumAll / SIGNAL_CLUSTER; // number of symbols for one Signal!

	for (i = 0, j = 0; i < NumAll; i++)
		if (rh[i].solid == 0)
			nClus[i] = -1; // for second pass

	for (Pass2 = 0, CurClus = 1; Pass2 < 2; Pass2++) {
		// restore for second pass
		if (Pass2 == 1) {
			for (i = 0; i < NumAll; i++) {
				if (nClus[i] == -1)
					nClus[i] = 0;
			}
			CurClus++;
			Clus2 = CurClus; // remember start non-solid clusters
		}
		// look first
		for (i = 0; i < NumAll; i++)
			if (nClus[i] == 0)
				break;
		if (i >= NumAll) {
			if (Pass2 == 1)
				CurClus--;
			continue;
		}

		CurClus = MakeClusters(i, NumAll, CurClus, porog, AllCount);

#ifdef _ADDONE_
		if (Pass2 == 0)
			CurClus = TestUnionOne(porog, NumAll, CurClus);
#endif
	} // end pass2

	// try union not-solid & solid
	CurClus = TestUnionSolid(porog, NumAll, Clus2, CurClus);

#ifdef _GETTIME_
	cl3=clock();
#endif
	// reorder clusters - most valid first
	// and fill clu_info
	ReOrderClusters(CurClus, NumAll, cin);

	// проба разбить некоторые кластеры
#ifdef _TEST_DIVIDE_
	{
		int TryDivide(int numSymbol, Nraster_header *rh, SINT *nClus,
				int numCluster);
		CurClus = TryDivide(NumAll, rh, nClus, CurClus);
	}
#endif

	// fill cluster results - some when ReOrderCluster
	// not more clusters - for Vladimir
	if (CurClus > MAXWEICLUS && IsCTBBase == 0)
		CurClus = MAXWEICLUS;

	cin->totclu = CurClus;
	cin->nsymbols = NumAll;
	cin->memused += (uint32_t) (NumHauBit - 1) * SIZEBUF;

	//ret=CurClus;  // if return - good, return as in prev.version

	// если чтение было из файла .r - записать туда номера кластеров
	if (NameWr != NULL) {
		// call external function && write numbers to file .r
		i = SaveSym(NameWr, NumAll, (BYTE *) mysteck, MAXSYM * sizeof(SINT),
				accept);
		if (i < 0)
			ret = i;
	}

	Signal();

#ifdef _GETTIME_
	cl4=clock();
#endif

	// study clusters
	if (IsCTBBase) {
		if (CurClus > MAXWEICLUS) {
			metkaGood = malloc(CurClus * 2);
			if (!metkaGood) {
				metkaGood = metkaGoodStat;
				CurClus = MAXWEICLUS;
			}
		} else
			metkaGood = metkaGoodStat;

		metkaValid = metkaGood + CurClus;

		// stay only good clusters from fonts?
		if (saveOnlyBest)
			countFont = FindBestClusters(NumAll, CurClus, rh, nClus, metkaGood,
					metkaValid, 4, &allFields[0][0]);
		else {
			if (p2_active == 4) // only remove week clusters ?
				MultiFindBestClusters(NumAll, CurClus, rh, nClus, metkaGood,
						metkaValid);
			else
				// only fill metki
				GetProbValid(NumAll, CurClus, rh, nClus, metkaGood, metkaValid);
		}
	}

	// save common rasters
	AllCount = CurClus / SIGNAL_SAVE;
	j = AllCount * (SIGNAL_SAVE - (CurClus % SIGNAL_SAVE));

	if (szOutName == NULL) {
		StartAddMemCluster(metkaValid, CurClus, countFont, &allFields[0][0]);
		OutCTBBase = 1;
		CTBpointer = NULL;
		fh = 1;
	} else {
		CTBpointer = &CTBfile;
		if (OutCTBBase == 0)
			fh = open(szOutName, O_RDWR | O_BINARY | O_CREAT | O_TRUNC, S_IREAD
					| S_IWRITE);
		else
			fh = StartCTB(szOutName, &CTBfile, countFont, &allFields[0][0]);
	}

	// need save full version ? 30.10.98
	fhSnap = open("c:\\clu.txt", O_RDONLY);
	if (fhSnap > 0) {
		close(fhSnap);
		fhSnap = StartCTB(".\\tmp\\page1.ctb", &CTBsnap, countFont,
				&allFields[0][0]);
	}

	if (fh < 0)
		ret = -10;
	else {
		for (i = 1, ret = 0, CurCount = 0; i <= CurClus; i++) {
			if (IsCTBBase == 0 || (metkaValid[i - 1] & METKA_VALID) != 0
					|| fhSnap > 0) {
				if (SaveCluster(fh, CTBpointer, fhSnap, &CTBsnap, i, NumAll,
						metkaGood, metkaValid) < 0) {
					ret = -11;
					break;
				}

				if (IsCTBBase == 0 || (metkaValid[i - 1] & METKA_VALID) != 0)
					ret++; // how many good clusters saved
			}

			if (++CurCount == AllCount) {
				CurCount = 0;
				Signal();
			}
			if (i == j)
				AllCount++;
		}

		if (OutCTBBase == 0) {
			if ((i = SetAccessTab(fh, welBuf)) < 0)
				ret = i;
			close(fh);
		} else if (szOutName)
			EndCTB(&CTBfile);
	}

	if (fhSnap > 0)
		EndCTB(&CTBsnap);
#ifdef _GETTIME_
	cl5=clock();
#endif

	// fill clusBuffer  > 0 для кластеров,
	// вошедших в отобранные
	// < 0 для остальных
	for (i = 0; i < NumAll; i++) {
		if (rh[i].nInCTB > 0 && rh[i].nInCTB <= MAXSYM && nClus[i] > 0
				&& nClus[i] <= CurClus) {
			if (IsCTBBase == 0 || (metkaValid[nClus[i] - 1] & METKA_VALID) != 0)
				clusBuffer[rh[i].nInCTB - 1] = nClus[i];
			else
				clusBuffer[rh[i].nInCTB - 1] = -nClus[i];
		}
	}

	if (metkaGood != metkaGoodStat)
		free(metkaGood);

	EndHausdorfDLL();

#ifdef _GETTIME_
	printf("\nstart=%f cluster=%f accept=%f save=%f \n",
			(float)(cl2-cl1)/CLK_TCK, (float)(cl3-cl2)/CLK_TCK,
			(float)(cl4-cl3)/CLK_TCK, (float)(cl5-cl4)/CLK_TCK);
#endif
	return (ret);
}
//////////////
//
static BYTE tabl[8* 256 ];
static BYTE WasInit11 = 0;
/////
// пометить, где в байтах стоят 1
void init11(void) {
	SINT i, j;
	BYTE k;
	BYTE *curtab;
	if (WasInit11)
		return;

	//  not put first bytes
	for (i = 1, curtab = tabl + 8; i < 255; i++, curtab += 8) {
		for (k = 0, j = 0; k < 8; k++)
			if (i & mas00[k])
				curtab[j++] = k;
	}
	WasInit11 = 1;

	return;
}
/////////////
// rbyte =8*...  !!!
// return number of black points
WORD PutSymbolRaster(BYTE *pHau, BYTE *rast, SINT rbyte, SINT xbits,
		SINT xbyte, SINT yrow) {
	SINT i, j;
	SINT xb = (xbits + 7) >> 3; // actual bytes in row
	BYTE *rr;
	BYTE *tb;
	BYTE num;
	WORD summa = 0;

	for (i = 0; i < yrow; i++, rast += rbyte, pHau += xbyte) {
		for (j = 0, rr = rast; j < xb; j++, rr += 8) {
			if ((num = pHau[j]) == 255) {
				for (num = 0; num < 8; num++)
					rr[num]++;
				summa += 8;
			} else {
				if (num == 0)
					continue;
				tb = tabl + (((WORD) num) << 3);
				num = Num11[num];
				summa += num;
				for (; num; num--, tb++)
					rr[*tb]++;
				/*
				 summa+=Num11[num];
				 if(num&128) *rr++;   rr++;
				 if(num&64) *rr++;    rr++;
				 if(num&32) *rr++;    rr++;
				 if(num&16) *rr++;    rr++;
				 if(num&8) *rr++;     rr++;
				 if(num&4) *rr++;     rr++;
				 if(num&2) *rr++;     rr++;
				 if(num&1) *rr++;     rr++;
				 */
			}
		}
	}
	return summa;
}
//////////////////
// make, save weighted raster to file
// use mysteck as buffer
SINT SaveCluster(SINT fh, CTB_handle *CTBfile, SINT fhSnap,
		CTB_handle *CTBsnap, SINT clus, SINT NumAll, BYTE *metkaGood,
		BYTE *metkaValid) {
	SINT i, j;
	BYTE *rast;
	LONG summax;
	LONG summay;
	SINT maxx, maxy;
	SINT fir, fat;
	SINT *NextInClus;
	SINT *movex;
	SINT *movey;
	SINT startx, starty;

	BYTE *etalon;
	SINT xbyte; // size of etalon
	SINT yrow;
	SINT sdvigx; // need to move bitmap?
	SINT sdvigy; //   ------""------
	SINT fx, fy, distXY;
	SINT sumcol1;
	BYTE *tmpbuf;
	uint32_t fields[NFIELDDWORD];
	int32_t nItalic, nBold, nSerif, nGelv, nArrow;
	uint32_t tablColumn = 0;
	BYTE odin = (BYTE) 1;

	movex = (SINT *) mysteck;
	movey = movex + MAXINCLUS;
	NextInClus = movey + MAXINCLUS;

	// looking for first symbol
	for (i = 0; i < NumAll; i++)
		if (nClus[i] == clus)
			break;
	if (i >= NumAll)
		return 0;

	fir = i;
	summax = maxx = rh[i].w;
	summay = maxy = rh[i].h;

	// first symbol - as etalon
	etalon = rh[i].pHau;
	xbyte = rh[i].xbyte;
	yrow = rh[i].h;

	MakeDWORDField(rh[i].nField, fields);

	movex[0] = 0; // sdvigx
	movey[0] = 0; // sdvigy
	fx = fy = 0;
	tmpbuf = (BYTE *) welBuf;

	// study maximal sizes and move position
	for (++i, j = 1; i < NumAll; i++) {
		if (nClus[i] != clus)
			continue;

		// need to move bitmap ?
		// make moved on +-1 on x bitmaps  (xbyte*yrow*2 <=WR_MAX_WIDTH*WR_MAX_HEIGHT)
		if (j == 1)
			sumcol1 = MakeMoved(etalon, xbyte, yrow, tmpbuf);
		distXY = CheckCenterSymbol(etalon, xbyte, yrow, rh[i].pHau, tmpbuf,
				rh[i].w, rh[i].h, &sdvigx, &sdvigy, sumcol1);

		if (sdvigx < 0)
			fx = -1;
		if (sdvigy < 0)
			fy = -1;
		movex[j] = sdvigx;
		movey[j] = sdvigy;
		maxx = MAX(maxx, (SINT) rh[i].w + sdvigx);
		maxy = MAX(maxy, (SINT) rh[i].h + sdvigy);

		NextInClus[j - 1] = i; // pointer from previous to current
		summax += rh[i].w;
		summay += rh[i].h;
		j++;
		if (j >= MAXINCLUS)
			break;
	}

	NextInClus[j - 1] = 0;

	if (fx < 0)
		maxx++; //  moved left
	if (fy < 0)
		maxy++; // moved up

	if (maxx > WR_MAX_WIDTH)
		maxx = WR_MAX_WIDTH;
	if (maxy > WR_MAX_HEIGHT)
		maxy = WR_MAX_HEIGHT;

	// collect all symbols to raster
	memset(welBuf, 0, sizeof(welet));
	welBuf->weight = (CHAR) j;
	welBuf->w = maxx;
	welBuf->h = maxy;

	rast = (BYTE *) welBuf->raster;
	starty = (WR_MAX_HEIGHT - maxy) / 2;
	startx = (WR_MAX_WIDTH - maxx) / 2;
	rast += starty * WR_MAX_WIDTH + startx;

	memset(keglBuffer, 0, sizeof(keglBuffer));
	for (i = fir, welBuf->summa = 0, j = 0, nArrow = nItalic = nBold = nSerif
			= nGelv = 0;; j++) {
		sdvigy = (movey[j] - fy);
		sdvigx = (movex[j] - fx);
		// where put next raster ?
		etalon = rast + sdvigy * WR_MAX_WIDTH + sdvigx;

		fat = PutSymbolRaster(rh[i].pHau, etalon, WR_MAX_WIDTH, (SINT) MIN(
				rh[i].w, WR_MAX_WIDTH - startx - sdvigx),
				(SINT) ((rh[i].w >> 3) + 1), (SINT) MIN(rh[i].h, WR_MAX_HEIGHT
						- starty - sdvigy));
		welBuf->summa += fat;

		AddDWORDField(rh[i].nField, fields);
		if (rh[i].italic)
			nItalic++;
		if (rh[i].bold)
			nBold++;
		if (rh[i].serif)
			nSerif++;
		if (rh[i].gelv)
			nGelv++;
		if (rh[i].narrow)
			nArrow++;
		keglBuffer[MIN(rh[i].kegl, MAXKEGL)]++;
		if (rh[i].tablColumn && rh[i].tablColumn <= 32)
			tablColumn |= (odin << (rh[i].tablColumn - 1));

		i = NextInClus[j];
		if (i == 0)
			break;
	}

	j = welBuf->weight;

	if ((nBold << 1) >= j) {
		welBuf->attr |= FON_CLU_BOLD;
		if ((nItalic << 2) >= j) // 22.02.00
			welBuf->attr |= FON_CLU_ITALIC;
	} else if ((nItalic << 1) >= j)
		welBuf->attr |= FON_CLU_ITALIC;

	if ((nSerif << 1) >= j)
		welBuf->attr |= FON_CLU_SERIF;
	if ((nGelv << 1) >= j)
		welBuf->attr |= FON_CLU_GELV;
	if ((nArrow << 1) >= j)
		welBuf->attr |= FON_CLU_NARROW;

	// save twin/invalid stamps
	// if rh[fir].num >0 - number of symbol (with which union) +1
	if (rh[fir].num) // exist twin or cluster invalid
	{
		// get from weighted ? (addclu)
		if (rh[fir].num > NumAll)
			welBuf->invalid = rh[fir].num - NumAll;
		else // main
		{
			if (rh[fir].solid == 0 && rh[rh[fir].num - 1].let == rh[fir].let)
				welBuf->attr |= FON_CLU_TWIN;
			else
				welBuf->invalid = nClus[rh[fir].num - 1];
		}
	}

	welBuf->let = (BYTE) rh[fir].let;
	SetFields( welBuf->fields ,fields);
	welBuf->tablColumn = tablColumn;

	if (IsCTBBase) {
		if ((metkaValid[clus - 1] & METKA_VALID) != 0)
			welBuf->attr |= FON_CLU_SOLID;
		welBuf->nInCTB = rh[fir].nInCTB; // accord first symbol in CTB
	} else {
		if (rh[fir].solid)
			welBuf->attr |= FON_CLU_SOLID;
	}

	// coordinates
	welBuf->sr_col = rh[fir].sr_col;
	welBuf->sr_row = rh[fir].sr_row;

	if (IsCTBBase && metkaGood) {
		Word16 meme = METKA_VALID;
		welBuf->prob = metkaGood[clus - 1];
		welBuf->valid = metkaValid[clus - 1] & (~meme);
	}

	// middle width,height
	welBuf->mw = (BYTE)((summax + ((WORD) welBuf->weight / 2))
			/ (WORD) welBuf->weight);
	welBuf->mh = (BYTE)((summay + ((WORD) welBuf->weight / 2))
			/ (WORD) welBuf->weight);

	j = WR_MAX_WIDTH * WR_MAX_HEIGHT;

#ifdef _NEW_POROG_
	if (welBuf->weight < 5)
		welBuf->porog = 0;
	else if (welBuf->weight <= 10)
		welBuf->porog = 1;
	else
		welBuf->porog = ((int) welBuf->weight * (int) 3 + 10) / 20; // 15%
#else
	welBuf->porog=welBuf->weight/POROG_IDEAL;
#endif

	//#ifdef _IDEAL_
	// if(wel->weight >= POROG_IDEAL) MakeIdeal(wel,wel->weight/POROG_IDEAL);
	// wel->fill=FindDistanceWr(wel,dist_wel);
	//#else

	for (i = 0; i < j; i++)
		if (welBuf->raster[i])
			welBuf->fill++;
	for (j = 0, i = 1; i <= MAXKEGL; i++)
		if (keglBuffer[i] > keglBuffer[j])
			j = i;
	welBuf->kegl = (BYTE) j;

	//#endif

	welBuf->num = clus;

	// Oleg portit welBuf !?
	if (fhSnap > 0)
		memcpy(dist_wel, welBuf, sizeof(welet));

	if (OutCTBBase == 0) {
		if (write(fh, welBuf, sizeof(welet)) != sizeof(welet))
			return -11;
	}
	// save main ctb-base
	else if ((metkaValid[clus - 1] & METKA_VALID) != 0) {
		// поместить в память ?
		if (CTBfile == NULL)
			AddClusterMemFont(welBuf);
		else if (SaveWeletAsCTB(welBuf, CTBfile) == 0)
			return -11;
	}

	// save for snap
	if (fhSnap > 0)
		SaveWeletAsCTB(dist_wel, CTBsnap);

	return 0;
}
////////////////////
/*************************/
void MakRas(CHAR *inp, CHAR *ras, SINT point) {
	register SINT i;
	SINT j = MIN(3, strlen(ras));
	SINT lens = strlen(inp);

	for (i = lens - 1; i >= 0; i--) {
		if (inp[i] == '.') {
			if (point)
				return;
			lens = i;
			break;
		}
		if (inp[i] == '\\' || inp[i] == ':')
			break;
	}
	inp[lens] = '.';
	for (i = 1; i <= j; i++)
		inp[lens + i] = ras[i - 1];
	inp[lens + j + 1] = 0;
	return;
}
////////////////////////
#ifdef _ADDONE_
SINT UnionOne(SINT fir, SINT las, BYTE *buf, BYTE *bufr, SINT xbyte, SINT xbit,
		SINT yrow, SINT CurClus, SINT porog) {
	SINT j;
	SINT dist;

	for (j = fir; j < las; j++) {
		if (nClus[j] != CurClus)
			continue;
		if (xbit > rh[j].w + 1)
			continue;
		//add 17.04.98 - nick (убрать вообще _ADDONE_ для Font?)
		if (yrow > rh[j].h + 2)
			continue;

		dist = DistanceHausDLL(buf, xbyte, yrow, rh[j].pHaur, rh[j].xbyte,
				(SINT) (rh[j].h + 1), porog);
		if (dist > porog)
			continue;
		dist = DistanceHausDLL(rh[j].pHau, rh[j].xbyte, rh[j].h, bufr, xbyte,
				(SINT) (yrow + 1), (SINT) porog);
		if (dist <= porog)
			return 1;
	}

	return 0;
}
/////////////////
#ifdef _UNION_ONEONE_
//
// try union symbols - move centers
//
static int UnionSingles(int let, int porog, int NumClus, int NumAll,
		SINT *LasIn, SINT *NumIn) {
	int TestMoveRaster(int start, Nraster_header *rh, int NumAll, int NumClus,
			SINT *nClus, SINT *LasIn, SINT *NumIn, int porog);
	SINT i, j;

	for (i = 0; i < NumAll; i++) {
		if (rh[i].let != let)
			continue;

		j = TestMoveRaster(i, rh, NumAll, NumClus, nClus, LasIn, NumIn, porog);
		NumClus -= j;
	}
	return NumClus;
}

///////////////////////
// test - can union uniqal symbols with other clusters
static SINT maxClusName[256];
#endif
static SINT TestUnionOne(SINT porog, SINT NumAll, SINT NumClus) {
	SINT i, j, k;
	SINT *LasIn;
	SINT *NumIn;
	WORD CurName;
	SINT best, numbest;

	NumClus++; // start from 1
	if (NumClus <= 1 || NumClus * 2 > MAXSYM)
		return NumClus;
	NumIn = mysteck;
	LasIn = mysteck + NumClus;

	memset(mysteck, 0, 2* NumClus * sizeof(SINT));
	// how many symbols in clusters and get names
	for (i = 0; i < NumAll; i++) {
		if (nClus[i] <= 0)
			continue;

		NumIn[(j = nClus[i])]++;
		LasIn[j] = i;
	}

#ifdef _UNION_ONEONE_
	memset(maxClusName, 0, 256* sizeof (SINT));
	for(i=1;i< NumClus;i++)
	{
		CurName=rh[LasIn[i]].let;
		if( CurName < 0 || CurName >=256 )
		continue;

		// tested name already ?
			if( maxClusName[CurName] != 0 ) continue;

			maxClusName[CurName]=NumIn[i];
			if( maxClusName[CurName] > 1)
			continue;

			for(j=i+1;j<NumClus;j++)
			{
				if(rh[LasIn[j]].let != CurName) continue;
				if( NumIn[j] > 1 )
				{	maxClusName[CurName]=NumIn[j];
					break;
				}
			}
			if( maxClusName[CurName] > 1 ) continue;

			k=NumClus;
			NumClus= UnionSingles( CurName, porog,
					NumClus, NumAll,
					LasIn, NumIn );
			if(NumClus < k) // was unions
			{	i=MAX(0,i-(k-NumClus));
				maxClusName[CurName]++;
			}
		}

#endif

			// main circle
			for(i=1;i< NumClus;i++)
			{
				if( NumIn[i] > 1) continue;

				k=LasIn[i]; // current number uniqal - same as last
				CurName=rh[k].let;

#ifdef _UNION_ONEONE_
			if(CurName >= 0 && CurName < 256 &&
					maxClusName[CurName] <= 1 )
			continue; // goto rename; ????
#endif

			for(j=1,best=-1,numbest=1;j<NumClus;j++)
			{
				if(rh[LasIn[j]].let != CurName) continue;
				if(NumIn[j] > numbest) {best=j;numbest=NumIn[j];}
			}

			// if only one with the name - stay ( not go to _RENAME_ ) ?
			if(best == -1)
			continue; // ???

			// can union ?
			if( UnionOne(0,(SINT)(LasIn[best]+1),rh[k].pHau,rh[k].pHaur,rh[k].xbyte,
				 rh[k].w,rh[k].h,
				 best,
				 (SINT)(rh[k].h<20?porog*2:rh[k].h<30?porog*3:porog*4)) )
	 {
	   /*printf(" union %d with %d %c \n",i,best,(char)rh[k].let);*/
	  nClus[k]=best;
	  NumIn[best]++;
	  for(j=0;j<NumAll;j++)  if(nClus[j] > i ) nClus[j]--;
	  NumClus--;
	  memcpy(LasIn+i,LasIn+i+1,(NumClus-i)*sizeof(SINT));
	  memcpy(NumIn+i,NumIn+i+1,(NumClus-i)*sizeof(SINT));
	  i--;
	  continue;
	 }

	// try to union with clusters with other name
//rename:
 #ifdef _RENAME_
  if( (j=UnionOneAll(0,NumAll,rh[k].pHau,rh[k].pHaur,rh[k].xbyte,
				 rh[k].h, CurName,(SINT)( porog>>2),NumIn) ) != -1 )
	 {
	  rh[k].num=j+1;  // symbol number+1 - to mark ! ( > 0 - was union)
	  /*printf(" union N=%d %c with %c (N=%d)\n",i,(char)rh[k].let,(char)rh[j].let,j);*/
	 }
  #endif
 }

 NumClus--;   // was ++ - at beginning
 return NumClus;
}
	//////////////////////
#endif

#ifdef _RENAME_
SINT UnionOneAll(SINT fir, SINT las, BYTE *buf, BYTE *bufr, SINT xbyte,
		SINT yrow, WORD CurName, SINT porog, SINT *NumIn) {
	SINT j;
	SINT dist;

	for (j = fir; j < las; j++) {
		if (rh[j].let == CurName)
			continue; // already try...
		if (nClus[j] <= 0)
			continue;

		// not union uniqal with uniqal
		if (NumIn[nClus[j]] <= 1)
			continue;

		dist = DistanceHausDLL(buf, xbyte, yrow, rh[j].pHaur, rh[j].xbyte,
				(SINT) (rh[j].h + 1), porog);
		if (dist > porog)
			continue;
		dist = DistanceHausDLL(rh[j].pHau, rh[j].xbyte, rh[j].h, bufr, xbyte,
				(SINT) (yrow + 1), porog);
		if (dist <= porog)
			return j;
	}

	return -1;
}
///////////////////////
#endif
////////////
// try union non-solid clusters with solid clusters
// (really don't make union - only mark Or union - by p2_active)
// return number of clusters
// Clus2 - start non-solid clusters
SINT TestUnionSolid(SINT porog, SINT NumAll, SINT Clus2, SINT NumClus) {
	SINT j, k;
	SINT *IsTwin; // buffer for number union
	SINT *FirIn; // first from every non-solid cluster
	WORD CurName;
	SINT CurClus;
	BYTE *buf;
	BYTE *bufr;
	SINT xbyte, yrow;
	SINT dist;
	SINT porog1;

	if (Clus2 >= NumClus)
		return NumClus;
	NumClus++; // cluster 0 don't exist, but need space for it
	if (NumClus * 2 > MAXSYM)
		return NumClus - 1;
	// mysteck use as buffers for study
	IsTwin = (SINT *) mysteck;
	FirIn = mysteck + NumClus;
	for (j = Clus2; j < NumClus; j++)
		IsTwin[j] = FirIn[j] = -1;

	for (k = 0; k < NumAll; k++) {
		if ((CurClus = nClus[k]) < Clus2)
			continue; // solid - not study
		CurName = rh[k].let; // name
		// found twin already ?
		if (IsTwin[CurClus] >= 0 && rh[IsTwin[CurClus]].let == CurName)
			continue;
		if (FirIn[CurClus] == -1)
			FirIn[CurClus] = k;

		// try union with other cluster
		buf = rh[k].pHau;
		bufr = rh[k].pHaur;
		xbyte = rh[k].xbyte;
		yrow = rh[k].h;

		for (j = 0; j < NumAll; j++) {
			if (nClus[j] >= Clus2)
				continue; // don't union with non-solid

			// if already found some alternative - try only find twin
			if (IsTwin[CurClus] >= 0 && rh[j].let != CurName)
				continue;

			if (rh[k].fat || rh[j].fat)
				porog1 = 0;
			else if (rh[j].let != CurName)
				porog1 = (yrow <= 18 ? porog >> 2 : porog >> 1);
			else
				porog1 = porog;

			dist = DistanceHausDLL(buf, xbyte, yrow, rh[j].pHaur, rh[j].xbyte,
					(SINT) (rh[j].h + 1), porog1);
			if (dist > porog1)
				continue;
			dist = DistanceHausDLL(rh[j].pHau, rh[j].xbyte, rh[j].h, bufr,
					xbyte, (SINT) (yrow + 1), porog);
			if (dist <= porog1) {
				IsTwin[CurClus] = j;
				break;
			}
		} // end j

	} // end k


	if (p2_active >= 4) {
		// really union with solid
		for (j = 0; j < NumAll; j++) {
			if ((CurClus = nClus[j]) < Clus2)
				continue;
			if ((k = IsTwin[CurClus]) == -1)
				continue; // not twin
			if (rh[k].let != rh[j].let) // invalid !
				rh[FirIn[CurClus]].num = k + 1; // +1 - to mark that was union
			else {
				nClus[j] = nClus[k]; // new cluster
				rh[j].solid = 1;
			}
		}

		// restruct clusters numbers
		// use FirIn as new numbers
		// dist - new last cluster
		for (CurClus = Clus2, dist = Clus2; CurClus < NumClus; CurClus++) {
			if ((k = IsTwin[CurClus]) == -1 || // no twin
					rh[FirIn[CurClus]].let != rh[k].let) // invalid
			{
				FirIn[CurClus] = dist;
				dist++;
			}
		}

		// set new numbers for not-twins
		for (j = 0; j < NumAll; j++) {
			if ((CurClus = nClus[j]) >= Clus2)
				nClus[j] = FirIn[CurClus];
		}
		return dist - 1;
	}

	// set only marks
	// results - to first from cluster
	for (k = Clus2, j = 0; k < NumClus; k++) {
		if (IsTwin[k] == -1)
			continue;
		/*
		 if(rh[IsTwin[k]].let != rh[FirIn[k]].let )
		 printf(" Union %c with solid %c \n",(BYTE)rh[FirIn[k]].let,(BYTE)rh[IsTwin[k]].let);
		 */
		rh[FirIn[k]].num = IsTwin[k] + 1; // +1 - to mark that was union
		j++;
	}
	return NumClus - 1;
}
//////////////////////
// in input - nClus,rh
// use mysteck
// order : 1 - solid; 2- non-solid,twin; 3- non-solid,not twin;
//         4- solid,invalid; 5 - non-solid,invalid
// NumClus - last Number of cluster (exist 1 - NumClus)
//
SINT ReOrderClusters(SINT NumClus, SINT NumAll, clu_info *cin) {
	SINT i, j, k;
	SINT *newclus;
	SINT *order;

	cin->nsolid = 0;
	cin->ninvalid = 0;
	cin->ntwins = 0;

	NumClus++;
	if (NumClus * 2 > MAXSYM)
		return -1; // too many clusters

	newclus = mysteck;
	order = mysteck + NumClus;
	memset(order, 0, sizeof(SINT) * NumClus);
	for (i = 1; i < NumClus; i++)
		newclus[i] = i;

	// set parameters
	for (i = 0; i < NumAll; i++) {
		j = nClus[i];
		if (order[j])
			continue; // already study cluster j
		if (rh[i].solid) {
			order[j] = 1;
			cin->nsolid++;
		} else
			order[j] = 3;
		if (rh[i].num) // exist twin or cluster invalid
		{
			// twin ? (only for not solid )
			if (rh[i].solid == 0 && rh[rh[i].num - 1].let == rh[i].let) {
				order[j] = 2;
				cin->ntwins++;
			} else {
				order[j] += 4;
				cin->ninvalid++;
			} // invalid - to end
		}
	}

	// now - find new cluster numbers
	for (i = 1; i < NumClus; i++) {
		if ((k = order[i]) == 1)
			continue; // most clusters - solid !?
		for (j = i + 1; j < NumClus; j++) {
			if (k > order[j]) {
				newclus[i]++;
				newclus[j]--;
			}
		}
	}

	// and now - set new numbers
	for (i = 0; i < NumAll; i++)
		nClus[i] = newclus[nClus[i]];
	return 0;
}
/////////////////////
// write acces_tab
SINT CheckAccessTab(SINT fh, void *buf) {
	access_tab *act = buf;

	if (read(fh, act, sizeof(access_tab)) != sizeof(access_tab))
		return -20;

	if (act->wlsize != sizeof(welet))
		return -22;
	if (strcmp(act->sign, SIGNATURE))
		return -21;

	return 0;
}
///////////////////
// sizeof(buf) must be >= sizeof(access_tab) !!!
SINT SetAccessTab(SINT fh, void *buf) {
	access_tab *act = (access_tab *) buf;

	memset(act, 0, sizeof(access_tab));
	act->wlsize = sizeof(welet);
	STRCPY(act->sign, SIGNATURE);
	if (write(fh, act, sizeof(access_tab)) != sizeof(access_tab))
		return -11;
	return 0;
}
////////////////

// return (clu_info.rc) < 0 - error
//        -1  - no memory
//        -2  - error open input file
//        -3  - error read input file
//        -6  - internal buffer - small
//        -10  - error open output file
//        -11  - error write output file
//
//        -20,-21,-22 - invalid  cluster file for AddClusters!
//
//       >=0 - number of clusters
//
clu_info make_font(PCHAR rname, MKFAM accept, PBYTE extern_buf, LONG size) {
	CHAR szOutName[144];
	SINT ret;
	clu_info cin;

	memset(&cin, 0, sizeof(clu_info));
	STRCPY(szOutName, rname);
	MakRas(szOutName, "clu", 0);
	my_percent = 0;
	// wel=(welet *)mybuffer;
	// dist_wel=wel+1;

#ifdef _ADDCLU_
	// if exist .clu - add to cluster
	if ((ret = open(szOutName, O_RDONLY)) > 0) {
		close(ret);
		ret = AddClusterHausdorf(rname, szOutName, 4, 1, accept, extern_buf,
				size, &cin);
	}

	else
		// standard clustering
#endif
		ret = ClusterHausdorfDLL(rname, porogCluster, szOutName, accept,
				extern_buf, size, &cin);

	STRCPY(rname, szOutName);
	cin.rc = ret;
	return cin;
}
/////////////////

//
// Reading from CTBBase

/////////////////////
FON_FUNC(int32_t) FONGetNumCluster(int32_t nInCTB)
{
	if(nInCTB <= 0 || nInCTB > MAXSYM)
	return 0;
	return clusBuffer[nInCTB-1];
}
////////////////
// fill static Nraster_header rh !!!
static SINT ReadAllFromBase(CHAR *name, SINT *nClu, CHAR *movxy, SINT AllCount) {
	int GetSymbolFromBase(int i, Nraster_header *rh, BYTE **pBuf);

	int allnum; // really read
	SINT CurCount = 0;
	BYTE *pp;
	int NumAll;
	int i;

	NumAll = OpenBase(name);
	if (NumAll > MAXSYM)
		NumAll = MAXSYM;

	for (i = allnum = 0; i < NumAll; i++) {
		// bad symbol ?

		if (GetSymbolFromBase(i, rh + allnum, &pp) == 0)
			continue;

		if (MakeBitmapsDLL(rh + allnum, pp, (SINT) allnum, nClu, movxy) < 0)
			break;

		allnum++;
		if (++CurCount == AllCount) {
			CurCount = 0;
			Signal();
		}
	}
	CloseBase();
	if (allnum <= 0)
		return 0;

	return allnum;
}
/////////////////////
FON_FUNC(int32_t) FONFontClusters(char *rname,char *cluname,void *accept,Word8 *extern_buf,int32_t size,
		uint32_t param,void *ShowProgress,Word8 lang)
{
	clu_info cin;
	char szOutName[144];
	int ret;
	void SetFillAll(int val);
	BYTE SetHand(BYTE val);

	language=lang; // язык символов
	langCyrilRoman=0;

	// fill massiv
	init11();

	memset(&cin,0,sizeof(clu_info));
	my_percent=0;
	PutPercent=ShowProgress;

	if( (param & FONCLU_ClusterHand) != 0 ) SetHand(1);
	else SetHand(0);

	if( (param & FONCLU_MultiFontRow) != 0 ) SetFillAll(0);
	else SetFillAll(1);

	if( (param & FONCLU_SaveAsClu) != 0 ) OutCTBBase=0;
	else OutCTBBase=1;

	if( (param & FONCLU_NotFonts) != 0 ) saveOnlyBest=0;
	else saveOnlyBest=1;

	if( (param & FONCLU_SaveWeak) != 0 ) p2_active=2;
	else p2_active=4;

	if( cluname==NULL && OutCTBBase == 0)
	{
		STRCPY(szOutName,rname);
		MakRas(szOutName,"clu",0);
		cluname=szOutName;
	}

	if( (param & FONCLU_InputR)!=0 ) IsCTBBase=0;
	else IsCTBBase=1;

	porogCluster=(SINT)(param&FONCLU_Threshold);
	if(porogCluster<=0) porogCluster=2;
	else porogCluster--;

	// make font only from CTB !
	if(IsCTBBase==0)
	saveOnlyBest=0;

	memset(clusBuffer,0,sizeof(clusBuffer));
	memset(allFields,0,sizeof(allFields));

	if( (param & FONCLU_AddClu) !=0 )
	ret= AddClusterHausdorf(rname,cluname,
			(SINT)(2*porogCluster),1,accept, extern_buf, size,&cin);
	else // standard clustering
	ret=ClusterHausdorfDLL(rname,porogCluster,cluname,
			accept,extern_buf,size,&cin);

	return ret;
}
//////////////////////
