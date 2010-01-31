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

#define _ADDCLU_

#ifdef _ADDCLU_

// mov on +- 1 on x & y
#define _MOVE_
// find center of symbols & use for compare
// as find center ? correct current ?
//#define _CENTER_

// use or not on distance 1 ?
//#define _DIST1_
// use all weighted points or only > wei
//#define _NOWEI_

//  calculate distances
#define _L1DIST_

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

#ifdef _GETTIME_
#include <time.h>
#endif

#include "compat/filefunc.h"
#include "fon.h"
#include "sfont.h"
#include "minmax.h"

#include "cfcompat.h"

int16_t SetAccessTab(int16_t fl, void *buf);
int16_t CheckAccessTab(int16_t fh, void *buf);
typedef int32_t (* MKFAM)(raster_header * rh, uint16_t nclu);
void init11(void);
int32_t StartHausdorfDLL(int num, void *ExternBuf, uint32_t SizeExternBuf);
void EndHausdorfDLL(void);
int16_t MakeClusters(int16_t fir, int16_t NumAll, int16_t CurClus,
                     int16_t porog, int16_t AllCount);
int16_t SaveSym(char *NameWr, int16_t NumAll, uchar *buf, int16_t size,
                MKFAM accept);
int16_t ReadAllFromWr(char *name, uchar *buf, int16_t size, int16_t *nClu,
                      char *movxy, int16_t NumAll, int16_t AllCount);
void MakRas(char *inp, char *ras, int16_t point);
int16_t GetNumSym(char *NameWr);

#define memmove memmove

#define MAXWELHAU  16
#define MAXWELSYM  2048    // maximal number old clusters
#define SIGNAL_START    12
#define SIGNAL_CLUSTER  50
#define SIGNAL_ADD       5
// 3- for st.cluster, reorder, accept
#define SIGNAL_SAVE     30
void Signal(void);

// if numpoint > level_grey*POROG_IDEAL  - remove this grey point
#define POROG_IDEAL 10
#define POROG_HALF  25
#ifdef _L1DIST_
#define MAXINCLUS 127
#else
#define MAXINCLUS 255
#endif
typedef struct tagSWEL {
    uint16_t let;
    int16_t w;
    int16_t h;
    uint16_t invalid; // invalid ?
    pchar raster;
    char wei;
    char dist1;
    char xcen;
    char ycen;
} SWEL;

extern int16_t NewFx, NewFy;
extern int16_t NumHauBit; // number of bitmap buffers
extern Nraster_header *rh;
extern int16_t nClus[MAXSYM];
extern welet *welBuf, *dist_wel; // use as xbytes,myst

static int16_t NumWelHau = 0; // number of bitmap buffers
static uchar *WelHau[MAXWELHAU]; // big buffers for welet
static int16_t IsStart[MAXWELSYM]; // first added symbol
static int16_t IsAdd[MAXWELSYM]; // last added symbol+1
static int16_t FirLet[256];
static uint32_t LastWel = 0; // last size
static uint32_t MaxSizeBuf2 = 0; // space in buffer for bitmaps
static int16_t IsRhHauBuf2 = 0;
static SWEL *swel = NULL;

static int32_t StartHausdorfDLL2(int num, void *ExternBuf,
                                 uint32_t SizeExternBuf);
static void EndHausdorfDLL2(void);
static int16_t FindBestClusterMemory(int16_t let, int16_t w, int16_t h,
                                     uchar *buf, uchar *bufrazmaz, int16_t NumClus, int16_t porog,
                                     int16_t xcen, int16_t ycen, char *movxy);

typedef struct tagPOSXY {
    int16_t sr_col, sr_row;
} // source row,col
POSXY;
static int16_t NumClusRecog = 0;
static POSXY *posXY = NULL;

int16_t
FindCenterRaster(uchar *raster, int16_t xbyte, int16_t yrow, int16_t *y);
int16_t FindCenterWel(welet *wl, int16_t *y);
// Ґбвм «Ё в®зЄЁ ў r  б а ббв®п­ЁҐ¬ > dist1
int16_t DistBitRas(puchar r, int16_t w, int16_t h, // b/w bitmap
                   pchar wr, int16_t ww, int16_t wh, char dist1, // grey raster
                   int16_t xo, int16_t yo, int16_t bdist);
// Ґбвм «Ё в®зЄЁ ў wr  б ўҐб®¬ > wei ­Ґ ўе®¤пйЁҐ ў r
int16_t DistRasBit(puchar r, int16_t w, int16_t h, // b/w bitmap
                   pchar wr, int16_t ww, int16_t wh, char wei, // grey raster
                   int16_t xo, int16_t yo, int16_t bdist);

#define POROGWH 8     // we add two rows & columns !
/***********************/
// porog - for same letters
// porog2 - for other names
// return - number of all symbols
int16_t AddToClusters(char *rname, int16_t NumClus, int16_t porog,
                      int16_t porog2, int16_t *nClus, char *movxy, uchar *tmpbuf,
                      int16_t AllCount)
{
    int16_t allnum;
    int16_t fh;
    int16_t i;
    int16_t NotClus = 0;
    int16_t NumGood = 0;
    int16_t NumBad = 0;
    int16_t xcen = 0, ycen = 0; // middle of symbol
    raster_header rh;
    uchar *bufrh = tmpbuf;
    uchar *bufrazmaz = tmpbuf + (WR_MAX_WIDTH >> 1) * WR_MAX_HEIGHT;
    int16_t CurCount = 0;

    if ((fh = open(rname, O_RDWR | O_BINARY)) == -1) {
        return -2;
    }

    memset(IsAdd, 0, NumClus * sizeof(int16_t));

    for (allnum = 0; allnum < MAXSYM; allnum++, movxy += 2) {
        if (read(fh, &rh, sizeof(raster_header)) != sizeof(raster_header))
            break;

        if (rh.w > WR_MAX_WIDTH - 2 || rh.h > WR_MAX_HEIGHT - 2) {
            lseek(fh, ((rh.w + 7) >> 3) * rh.h, SEEK_CUR);
            continue;
        }

        if (read(fh, bufrh, ((rh.w + 7) >> 3) * rh.h) != ((rh.w + 7) >> 3)
                * rh.h) {
            close(fh);
            return -3;
        }

        Razmaz2(bufrh, bufrazmaz, rh.w, rh.h, 0, 0);
#ifdef _CENTER_
        xcen = FindCenterRaster(bufrh, (rh.w + 7) >> 3, rh.h, &ycen);
#endif
        // find from same name
        i = FindBestClusterMemory(rh.let, rh.w, rh.h, bufrh, bufrazmaz,
                                  NumClus, porog, xcen, ycen, movxy);

        if (i <= 0 && porog2 >= 0)
            i = FindBestClusterMemory(0, rh.w, rh.h, bufrh, bufrazmaz, NumClus,
                                      porog2, xcen, ycen, movxy);

        if (i <= 0)
            NotClus++;

        else {
            nClus[allnum] = i;

            if (swel[i - 1].let != rh.let) {
                nClus[allnum] = -i; // mark invalid
                NumBad++;
                /*
                 printf(" Invalid - %c with %c (%d)!\n",(uchar)rh.let,(uchar)swel[i-1].let,i);
                 */
            }

            else {
                NumGood++;

                if (IsAdd[i - 1] <= 0)
                    IsStart[i - 1] = allnum;

                IsAdd[i - 1] = allnum + 1;
                *movxy += swel[i - 1].xcen; // start in full weighted cluster
                *(movxy + 1) += swel[i - 1].ycen; // added to cut off
            } // end else
        } // end else

        if (++CurCount == AllCount) {
            CurCount = 0;
            Signal();
        }
    } // end for i

    close(fh);
    /*
     for(i=0,fh=0;i< NumClus;i++) if(IsAdd[i]) fh++;
     printf( " Bad symbol=%d ,Good symbol=%d Not-add %d\n",  NumBad,NumGood,NotClus);
     printf("allnum=%d  init cluster=%d  updated %d\n",allnum,NumClus,fh);
     */
    return allnum;
}
//////////////////////////
void EndHausdorfDLL2(void)
{
    int16_t i;

    for (i = (IsRhHauBuf2 >= 2 ? 0 : 1); i < NumWelHau; i++) {
        if (WelHau[i] != NULL)
            free(WelHau[i]);

        WelHau[i] = NULL;
    }

    WelHau[0] = NULL;
    NumWelHau = 0;

    if (IsRhHauBuf2 != 0 && swel != NULL)
        free(swel);

    swel = NULL;
}
/***********************/
// was parameter num - number of input symbols ( 0 if unknown)
// use ExternBuf - if not NULL && SizeExternBuf > 0
//  return > 0 - how many memory alloccated
//
int32_t StartHausdorfDLL2(int num, void *ExternBuf, uint32_t SizeExternBuf)
{
    int32_t size = 0;
#ifdef _NONFLAT_
    IsRhHauBuf2 = 2;
    swel = (SWEL *)malloc(( uint32_t)num * sizeof(SWEL));

    if (swel == NULL) return -1;

    NumWelHau = 0;
    // init bitmap for buffers
    WelHau[0] = (uchar *)malloc(SIZEBUF); // take one segment

    if (WelHau[0] == NULL) return -1;

    MaxSizeBuf2 = SIZEBUF;
    size = ( uint32_t)num * sizeof(SWEL) + SIZEBUF;
#else

    if (num <= 0)
        num = MAXWELSYM;

    if (ExternBuf == NULL) {
        swel = (SWEL *) malloc((uint32_t) MAXWELSYM * sizeof(SWEL)
                               + (uint32_t) SIZEBUF);

        if (swel == NULL)
            return -1;

        WelHau[0] = (uchar *) swel;
        WelHau[0] += (uint32_t) num * sizeof(SWEL);
        IsRhHauBuf2 = 1;
        MaxSizeBuf2 = (uint32_t) MAXWELSYM * sizeof(SWEL) + (uint32_t) SIZEBUF
                      - (uint32_t) num * sizeof(SWEL);
        size = (uint32_t) MAXWELSYM * sizeof(SWEL) + (uint32_t) SIZEBUF;
    }

    else if (SizeExternBuf >= (uint32_t) num * sizeof(SWEL)) {
        swel = (SWEL *) ExternBuf;
        WelHau[0] = (uchar *) swel;
        WelHau[0] += (uint32_t) num * sizeof(SWEL);
        MaxSizeBuf2 = SizeExternBuf - (uint32_t) num * sizeof(SWEL);
        IsRhHauBuf2 = 0;
        size = 0;
    }

    else { // use extern buffer for WelHau[0]
        swel = (SWEL *) malloc((uint32_t) MAXWELSYM * sizeof(SWEL));

        if (swel == NULL)
            return -1;

        WelHau[0] = (uchar *) ExternBuf;
        MaxSizeBuf2 = SizeExternBuf;
        IsRhHauBuf2 = 1;
        size = (uint32_t) MAXWELSYM * sizeof(SWEL);
    }

#endif
    NumWelHau = 1; // number of bitmap buffers
    LastWel = 0; // first empty byte in WelHau
    return size;
}
/***********************/
int16_t AddBuffer2(int32_t sizebitmap)
{
    if (LastWel + sizebitmap > MaxSizeBuf2) {
        // get new buffer
        if (NumWelHau >= MAXWELHAU)
            return -6;

        WelHau[NumWelHau] = static_cast<uchar*> (malloc(SIZEBUF));

        if (WelHau[NumWelHau] == NULL)
            return -1;

        NumWelHau++;
        LastWel = 0;
        MaxSizeBuf2 = SIZEBUF;
    }

    return 0;
}
/*********************/
static int16_t AddRaster(SWEL *sw, welet *wl)
{
    int16_t i;
    int16_t w, h;
    pchar ras, rr;
    int16_t stx, sty;
    w = MIN(WR_MAX_WIDTH, wl->w + 2); // add two rows !!!
    h = MIN(WR_MAX_HEIGHT, wl->h + 2);

    if ((i = AddBuffer2(w * h)) < 0)
        return i;

    stx = (WR_MAX_WIDTH - w) / 2;
    sty = (WR_MAX_HEIGHT - h) / 2;
    ras = wl->raster + (int32_t) sty * WR_MAX_WIDTH + stx;
    sw->w = w; // new width,height - with frame!!
    sw->h = h;
    sw->wei = wl->weight / 2;
    rr = ras + WR_MAX_WIDTH + 1; // from second row ! - first row empty
    sw->dist1 = 0;

    for (i = 0; i < wl->w; i++)
        if (rr[i] >= 0 && rr[i + 1] < 0) {
            sw->dist1 = rr[i + 1];
            break;
        }

    rr = sw->raster = (char*) WelHau[NumWelHau - 1] + LastWel;

    for (i = 0; i < h; i++, ras += WR_MAX_WIDTH, rr += w)
        memcpy(rr, ras, w);

    LastWel += (int32_t) w * h;
    return 0;
}

int16_t FindBestClusterMemory(int16_t let, int16_t w, int16_t h, uchar *buf,
                              uchar *bufrazmaz, int16_t NumClus, int16_t porog, int16_t xcen,
                              int16_t ycen, char *bxy)
{
    int16_t i;
    int16_t dist;
    int16_t bestdist;
    int16_t best = -1; // best cluster
    int16_t x, y;
    xcen = xcen;
    ycen = ycen;
    best = -1;
    bestdist = porog + 1;

    if (bxy) {
        *bxy = *(bxy + 1) = 0;
    }

    if (let > 0 && let < 256) {
        if (FirLet[let] < 0)
            return -1;

        i = FirLet[let];
    }

    else
        i = 0;

    for (; i < NumClus; i++) {
        if (swel[i].invalid)
            continue; // not union with invalid!

        if (let && swel[i].let != let || abs(swel[i].w - w) > POROGWH || abs(
                    swel[i].h - h) > POROGWH)
            continue;

        //  return (i+1);
#ifdef _MOVE_

        for (x = 0; x < 2; x = (x == 0 ? -1 : x + 2)) {
            for (y = 0; y < 2; y = (y == 0 ? -1 : y + 2)) {
#else
#ifdef _CENTER_
        x = (WR_MAX_WIDTH - swel[i].w) / 2 + swel[i].xcen -
            (WR_MAX_WIDTH - rh->w) / 2 - xcen;
        y = (WR_MAX_HEIGHT - swel[i].h) / 2 + swel[i].ycen -
            (WR_MAX_HEIGHT - rh->h) / 2 - ycen;
#else
                x = y = 0;
#endif
#endif
                dist = DistBitRas(buf, w, h, // b/w bitmap
                                  swel[i].raster, swel[i].w, swel[i].h, swel[i].dist1, // grey raster
                                  (int16_t) x, (int16_t) y, bestdist);

                if (dist >= bestdist)
                    continue;

                dist += DistRasBit(bufrazmaz, (int16_t) (w + 2), (int16_t) (h
                                                                            + 2), swel[i].raster, swel[i].w, swel[i].h,
                                   swel[i].wei, (int16_t) x, (int16_t) y,
                                   (int16_t) (bestdist - dist));

                if (dist < bestdist) {
                    bestdist = dist;
                    best = i;

                    if (bxy) {
                        *bxy = (char) x;
                        *(bxy + 1) = (char) y;
                    }

                    if (let != 0 || dist == 0)
                        return (best + 1);
                }

#ifdef _MOVE_
            } // endy
        } // endx

#endif
    }

    return (best + 1);
}
///////////////////////
static int16_t GetNumSwelWeight(int16_t fclu, int16_t nclus, uchar weight)
{
    int16_t i, j;
    lseek(fclu, 0, SEEK_SET);

    for (i = 0, j = 0; i < nclus; i++) {
        if (read(fclu, welBuf, sizeof(welet)) != sizeof(welet)) {
            close(fclu);
            return -3;
        }

        if (welBuf->invalid)
            continue;

        // check - solid,twin,... !!! - future
        if (welBuf-> weight <= weight)
            j++;
    }

    return j;
}
////////////////

static int16_t GetSwel(pchar cluname, int16_t nclus, SWEL *sw, clu_info *cin,
                       int16_t AllCount, uchar size)
{
    int16_t i;
    int16_t fclu;
    int16_t ret = 0;
    int16_t CurCount = 0;
    int16_t x, y;
    cin->nsolid = 0;
    cin->ntwins = 0;
    cin->ninvalid = 0;

    for (i = 0; i < 256; i++)
        FirLet[i] = -1;

    if ((fclu = open(cluname, O_RDWR | O_BINARY)) == -1)
        return -2;

    for (i = 0; i < nclus;) {
        if (++CurCount == AllCount) {
            CurCount = 0;
            Signal();
        }

        if (read(fclu, welBuf, sizeof(welet)) != sizeof(welet)) {
            close(fclu);
            return -3;
        }

        x = welBuf->w; // full size
        y = welBuf->h;

        if (welBuf->invalid) {
            if (size > 0)
                continue;

            cin->ninvalid++;
            sw[i].invalid = welBuf->invalid;
            sw[i].let = welBuf->let;
            sw[i].w = welBuf->w;
            sw[i].h = welBuf->h;
            sw[i].raster = NULL;
            i++;
            continue; // not need raster
        }

        if (size > 0 && welBuf->weight > size)
            continue;

        if (size <= 0)
            sw[i].invalid = 0;

        else
            sw[i].invalid = welBuf->num; // remember real number!

        // make ideal picture,set distances
        // get NewFx, NewFy
        FONCutOffClusters(welBuf, 1);
        sw[i].let = welBuf->let;

        if (welBuf->let < 256 && FirLet[welBuf->let] < 0)
            FirLet[welBuf->let] = i;

        sw[i].w = welBuf->w;
        sw[i].h = welBuf->h;
        sw[i].raster = NULL;

        if ((ret = AddRaster(sw + i, welBuf)) < 0)
            break;

        if (welBuf->attr & FON_CLU_SOLID)
            cin->nsolid++;

        else if (welBuf->attr & FON_CLU_TWIN)
            cin->ntwins++;

#ifdef _CENTER_
        x = FindCenterWel(welBuf, &y);
        sw[i].xcen = (char)x;
        sw[i].ycen = (char)y;
#else
        // get distance from real center to cutted off) !
        x = (WR_MAX_WIDTH - x) / 2 + NewFx - (WR_MAX_WIDTH - welBuf->w) / 2;
        y = (WR_MAX_HEIGHT - y) / 2 + NewFy - (WR_MAX_HEIGHT - welBuf->h) / 2;
        sw[i].xcen = (char) x;
        sw[i].ycen = (char) y;
#endif

        if (size > 0) {
            posXY[i].sr_col = welBuf->sr_col;
            posXY[i].sr_row = welBuf->sr_row;
        }

        i++;
    }

    close(fclu);
    return ret;
}
////////////////
// move picture - if need recenter
// from startx,starty to newstartx,newstarty
// size - sx,sy
void MoveWeighted(welet *wel, int16_t newstartx, int16_t newstarty, int16_t sx,
                  int16_t sy, int16_t startx, int16_t starty)
{
    int16_t i;
    char *rr;
    char *rr1;
    int16_t sdvigx = startx - newstartx;
    int16_t sdvigy = starty - newstarty;
#ifndef _L1DIST_
    int16_t j;
#endif

    if (sdvigx == 0 && sdvigy == 0)
        return;

    // mov up / left
    if (sdvigy > 0 || sdvigy == 0 && sdvigx > 0) {
        rr = wel->raster + newstarty * WR_MAX_WIDTH + newstartx;
        rr1 = rr + sdvigy * WR_MAX_WIDTH + sdvigx;

        for (i = 0; i < sy; i++, rr += WR_MAX_WIDTH, rr1 += WR_MAX_WIDTH)
            memcpy(rr, rr1, sx);
    }

    else if (sdvigy == 0 && sdvigx < 0) { // move right
        rr = wel->raster + newstarty * WR_MAX_WIDTH + newstartx;

        for (i = 0; i < sy; i++, rr += WR_MAX_WIDTH)
            memmove(rr, rr + sdvigx, sx);
    }

    else { // move down - sdvigy < 0
        rr = wel->raster + (newstarty + sy - 1) * WR_MAX_WIDTH + newstartx;
        rr1 = rr + sdvigy * WR_MAX_WIDTH + sdvigx;

        for (i = 0; i < sy; i++, rr -= WR_MAX_WIDTH, rr1 -= WR_MAX_WIDTH)
            memcpy(rr, rr1, sx);
    }

    // if set distance - not need set 0-s !
#ifndef _L1DIST_
    // set 0 - on moved space
    // upper lines
    rr = wel->raster + starty * WR_MAX_WIDTH + startx;

    for (i = starty; i < newstarty; i++, rr += WR_MAX_WIDTH) memset(rr, 0, sx);

    // down lines
    rr = wel->raster + (newstarty + sy) * WR_MAX_WIDTH + startx;

    for (i = newstarty; i < starty; i++, rr += WR_MAX_WIDTH)
        memset(rr, 0, sx);

    // left columns
    if ( (j = newstartx - startx) > 0) {
        rr = wel->raster + newstarty * WR_MAX_WIDTH + startx;

        for (i = 0; i < sy; i++, rr += WR_MAX_WIDTH) memset(rr, 0, j);
    }

    // right columns
    if ( (j = startx - newstartx) > 0) {
        rr = wel->raster + newstarty * WR_MAX_WIDTH + newstartx + sx;

        for (i = 0; i < sy; i++, rr += WR_MAX_WIDTH) memset(rr, 0, j);
    }

#endif
}
////////////////////////////////////
// add, save raster to weighted raster
// use mysteck as buffer
int16_t SaveAddCluster(int16_t fh, int16_t clus, int16_t firCl, int16_t lastCl,
                       welet *wel, welet *dist_wel, Nraster_header *rh, int16_t *nClu)
{
    int16_t i, j;
    char *rast;
    int16_t lastx, lasty;
    int16_t startx, starty;
    int16_t sdvigx, sdvigy;
    int16_t wei = wel->weight;
    int32_t summax = wel->mw;
    int32_t summay = wel->mh;
    wel->attr &= ~FON_CLU_FIXED;

    if (wei >= MAXINCLUS) {
        j = 0; /*wel->attr &= ~FON_CLU_UPDATE;*/
        goto lsave;
    }

    startx = (WR_MAX_WIDTH - wel->w) / 2;
    starty = (WR_MAX_HEIGHT - wel->h) / 2;
    lastx = startx + wel->w;
    lasty = starty + wel->h;
    summax *= wei;
    summay *= wei;

    // set 0 -z on distances!
    for (i = 0, rast = wel->raster; i < WR_MAX_WIDTH * WR_MAX_HEIGHT; i++)
        if (rast[i] < 0)
            rast[i] = 0;

    for (i = firCl, j = 0; i < lastCl; i++) {
        if (nClu[i] != clus)
            continue;

        sdvigx = MAX(0, (WR_MAX_WIDTH - (int16_t) rh[i].w) / 2 + rh[i].sr_col);
        sdvigy = MAX(0, (WR_MAX_HEIGHT - (int16_t) rh[i].h) / 2 + rh[i].sr_row);
        lastx = MAX(lastx, sdvigx + rh[i].w);
        lasty = MAX(lasty, sdvigy + rh[i].h);
        startx = MIN(startx, sdvigx);
        starty = MIN(starty, sdvigy);
        // where put next raster ?
        rast = wel->raster + sdvigy * WR_MAX_WIDTH + sdvigx;
        wel->summa += PutSymbolRaster(rh[i].pHau, (uchar*) rast,
                                      (int16_t) WR_MAX_WIDTH, (int16_t) MIN(rh[i].w, WR_MAX_WIDTH
                                                                            - sdvigx), (int16_t) ((rh[i].w + 7) >> 3),
                                      (int16_t) MIN(rh[i].h, WR_MAX_HEIGHT - sdvigy));
        summax += rh[i].w;
        summay += rh[i].h;
        wei++;
        j++;

        if (wei >= MAXINCLUS)
            break;
    }

    if (j == 0) {/*wel->attr &= ~FON_CLU_UPDATE;*/
        goto lsave;
    } // nothing add

    // middle width,height
    wel->mw = (uchar)((summax + ((uint16_t) wei / 2)) / (uint16_t) wei);
    wel->mh = (uchar)((summay + ((uint16_t) wei / 2)) / (uint16_t) wei);
    // check new center of weighted raster
    // get new sizes:
    lastx = MIN(lastx, WR_MAX_WIDTH) - startx;
    lasty = MIN(lasty, WR_MAX_HEIGHT) - starty;
    MoveWeighted(wel, (int16_t) ((WR_MAX_WIDTH - lastx) / 2),
                 (int16_t) ((WR_MAX_HEIGHT - lasty) / 2), lastx, lasty, startx,
                 starty);
    wel->w = lastx;
    wel->h = lasty;
    wel->weight = (uchar) wei;
    wel->porog = wel->weight / POROG_IDEAL;

    //#ifdef _IDEAL_
    // if(wel->weight >= POROG_IDEAL) MakeIdeal(wel,wel->weight/POROG_IDEAL);
    // wel->fill=FindDistanceWr(wel,dist_wel);
    //#else
    for (i = 0, wel->fill = 0; i < WR_MAX_WIDTH * WR_MAX_HEIGHT; i++)
        if (wel->raster[i])
            wel->fill++;

    //#endif
    // wel->attr |= FON_CLU_UPDATE;
lsave:
    wel->num = clus;

    if (write(fh, wel, sizeof(welet)) != sizeof(welet))
        return -11;

    return j;
}
////////////////////
// get clusters from nameClu, get memory,
// put clusters in special form
//
static int16_t StartWeightedClusters(char *nameClu, puchar extern_buf,
                                     int32_t size_extern, clu_info *cin, int sizeCluster)
{
    int32_t size;
    int16_t fh, i;
    int16_t ret;
    int16_t NumClus; // how many clusters
    // how many symbols=clusters ?
    if ((fh = open(nameClu, O_RDWR | O_BINARY)) == -1)
        return -2;

    size = filelength(fh);

    if (size < sizeof(access_tab)) {
        close(fh);
        return -20;
    }

    size -= sizeof(access_tab);
    lseek(fh, size, SEEK_SET);

    if ((ret = CheckAccessTab(fh, welBuf)) < 0) {
        close(fh);
        return ret;
    }

    NumClus = size / sizeof(welet);

    if (sizeCluster > 0)
        NumClus = GetNumSwelWeight(fh, NumClus, (uchar) sizeCluster);

    close(fh);

    if (NumClus <= 0)
        return -4;

    NumClus = MIN(NumClus, MAXWELSYM);
    size = StartHausdorfDLL2(NumClus, extern_buf, size_extern);

    if (size < 0)
        return -1;

    cin->memused = size;

    if (size > 0) {
        posXY = static_cast<POSXY*> (malloc(NumClus * sizeof(POSXY)));

        if (posXY == NULL)
            return -1;

        cin->memused += NumClus * sizeof(POSXY);
    }

    // put weighted clusters to memory, get some statistics
    if ((i = GetSwel(nameClu, NumClus, swel, cin, (int16_t) (NumClus
                                                             / SIGNAL_START), (uchar)(sizeCluster <= 0 ? 0 : sizeCluster))) < 0) {
        EndHausdorfDLL2();
        return i;
    }

    // was new memory allocated?
    cin->memused += (uint32_t)(NumWelHau - 1) * SIZEBUF;
    return NumClus;
}
/////////////
// load symbols, make clusters with threshold
// call external function
// save clusters
// NumAll - number of symbols
// results - in nClus ,
int16_t AddClusterHausdorf(char *NameWr, char *szOutName, int16_t porog,
                           int16_t porog2, MKFAM accept, puchar extern_buf, int32_t size_extern,
                           clu_info *cin)
{
    int16_t i, j, k;
    int16_t CurClus = 0;
    int16_t NumAll;
    int16_t NumClus;
    int16_t fh, fold;
    int16_t ret = 0;
    int32_t size;
    char *movxy;
#ifdef _GETTIME_
    clock_t cl1 = clock(), cl2, cl3, cl4, cl5, cl22;
#endif
    int16_t NewSolid = 0; // new solid clusters
    int16_t NewNotInvalid = 0; // total new good clusters
    int16_t NewInvalid = 0; // new invalid clusters
    int16_t *newclus;
    char tmpname[144];
    int16_t CurCount = 0, AllCount;

    // how many input symbols?
    if ((NumAll = GetNumSym(NameWr)) <= 0)
        return NumAll;

    // how many symbols=clusters ?
    NumClus = StartWeightedClusters(szOutName, extern_buf, size_extern, cin, 0);

    if (NumClus < 0)
        return NumClus;

    memset(nClus, 0, MAXSYM * sizeof(int16_t));
    movxy = (char *) welBuf;
#ifdef _GETTIME_
    cl2 = clock();
#endif
    // add to clusters, remember - how need move symbols
    NumAll = AddToClusters(NameWr, NumClus, porog, porog2, nClus, movxy,
                           (uchar *) dist_wel, (int16_t) (NumAll / SIGNAL_CLUSTER));
    EndHausdorfDLL2();

    if (NumAll < 0)
        return NumAll;

#ifdef _GETTIME_
    cl22 = clock();
#endif
    // start standard clusterization for not-added symbols
    init11();
    // how many memory allocated ?
    size = StartHausdorfDLL(NumAll, extern_buf, size_extern);

    if (size < 0) { //  no memory
        EndHausdorfDLL();
        return -1;
    }

    // make bitmaps, razmaz for not-added
    // set values from movxy to Nraster_header for added
    i = ReadAllFromWr(NameWr, (uchar *) dist_wel, sizeof(welet), nClus, movxy,
                      NumAll, (int16_t) (NumAll / SIGNAL_ADD));

    if (i <= 0) { // read invalid
        EndHausdorfDLL();
        return i;
    }

    // full memory for b/w rasters
    size += (uint32_t)(NumHauBit - 1) * SIZEBUF;
    // compare memory for weighted rasters and b/w rasters
    cin->memused = MAX(size, cin->memused);
#ifdef _GETTIME_
    cl3 = clock();
#endif

    // look first not-added to old clusters
    for (i = 0; i < NumAll; i++)
        if (nClus[i] == 0)
            break;

    // set clusters numbers (from NumClus+1) for not-added
    CurClus = MakeClusters(i, NumAll, (int16_t) (NumClus + 1), 2, 0);
    Signal();
    newclus = (int16_t *) dist_wel;
    NewNotInvalid = CurClus - NumClus;
    NewSolid = 0;

    // how many new solid clusters?
    // set new numbers for new clusters
    // i - stay the same!
    for (j = NumClus + 1; i < NumAll; i++) {
        if (nClus[i] < j)
            continue;

        if (rh[i].solid) {
            NewSolid++;
            newclus[j] = cin->nsolid + NewSolid;
        }

        else
            newclus[j] = 0; // mark - before invalid!

        j++; // cluster j tested !
    }

    // set new numbers for not-solid
    for (j = NumClus + 1, k = NumClus - cin->ninvalid + NewSolid + 1; j
            <= CurClus; j++)
        if (newclus[j] == 0)
            newclus[j] = k++;

    // add new invalid clusters
    for (i = 0, NewInvalid = 0; i < NumAll; i++) {
        if ((k = nClus[i]) < 0) {
            j = -k;

            if (j > cin->nsolid)
                j += NewSolid; // new cluster number!

            rh[i].num = j + NumAll; // to mark - not symbol, but weighted cluster
            CurClus++;
            nClus[i] = CurClus;
            NewInvalid++;
        }
    }

    // reorder clusters
    for (i = 0; i < NumAll; i++) {
        if ((k = nClus[i]) <= NumClus) { // old cluster
            if (k > NumClus - cin->ninvalid)
                nClus[i] += NewNotInvalid;

            else if (k > cin->nsolid)
                nClus[i] += NewSolid;
        }

        else if (k <= NumClus + NewNotInvalid) // new not-invalid cluster
            nClus[i] = newclus[nClus[i]];
    }

    Signal();
    // fill cluster results
    cin->totclu = CurClus;
    cin->nsymbols = NumAll;
    ret = CurClus; // if return - good, return as in prev.version
    // call external function && write numbers to file .r
    // dist_wel - as temporary buffer
    i = SaveSym(NameWr, NumAll, (uchar *) dist_wel, MAXSYM * sizeof(int16_t),
                accept);

    if (i < 0)
        ret = i;

    Signal();
#ifdef _GETTIME_
    cl4 = clock();
#endif
    // save common rasters
    strcpy(tmpname, szOutName);
    MakRas(tmpname, "$$$", 0);
    unlink(tmpname); // if exist ...

    if (rename(szOutName, tmpname) == -1) {
        EndHausdorfDLL();
        return -10;
    }

    if ((fold = open(tmpname, O_RDWR | O_BINARY)) <= 0) {
        EndHausdorfDLL();
        return -10;
    }

    /*
     printf("Clus=%d NewSolid=%d NewGood=%d NewInvalid=%d\n",
     CurClus,NewSolid,NewNotInvalid,NewInvalid);
     */
    AllCount = CurClus / SIGNAL_SAVE;
    k = AllCount * (SIGNAL_SAVE - CurClus % SIGNAL_SAVE);
    fh = open(szOutName, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);

    if (fh < 0)
        ret = -10;

    else {
        for (i = 1, j = 0; i <= CurClus; i++) { // j - count old clusters
            if (i <= cin->nsolid || i > cin->nsolid + NewSolid && i <= NumClus
                                           + NewSolid - cin->ninvalid || i > CurClus - cin->ninvalid
                                                                                          - NewInvalid && i <= CurClus - NewInvalid) {
                // save old clusters  - add new symbols
                j++;

                if (read(fold, welBuf, sizeof(welet)) != sizeof(welet)) {
                    ret = -11;
                    break;
                }

                if (welBuf->invalid > cin->nsolid)
                    welBuf->invalid += NewSolid;

                // nothing added
                if (IsAdd[j - 1] <= 0) {
                    //    welBuf->attr &= ~FON_CLU_UPDATE;
                    welBuf->num = i;

                    if (write(fh, welBuf, sizeof(welet)) != sizeof(welet)) {
                        ret = -11;
                        break;
                    }
                }

                else {
                    if (SaveAddCluster(fh, i, IsStart[j - 1], IsAdd[j - 1],
                                       welBuf, dist_wel, rh, nClus) < 0) {
                        ret = -11;
                        break;
                    }
                }
            }

            else { // save new cluster
                if (SaveCluster(fh, NULL, i, NULL, NULL, NumAll, NULL, NULL) < 0) {
                    ret = -11;
                    break;
                }
            }

            if (++CurCount == AllCount) {
                CurCount = 0;
                Signal();
            }

            if (i == k)
                AllCount++;
        }

        if ((i = SetAccessTab(fh, welBuf)) < 0)
            ret = i;

        close(fh);
    }

    close(fold);
    unlink(tmpname); // delete temporary file
    cin->nsolid += NewSolid;
    cin->ninvalid += NewInvalid;
    // no new twins (change ???)
#ifdef _GETTIME_
    cl5 = clock();
#endif
    EndHausdorfDLL();
#ifdef _GETTIME_
    printf("start=%f add=%f cluster=%f accept=%f save=%f \n",
           (float)(cl2 - cl1) / CLK_TCK, (float)(cl22 - cl2) / CLK_TCK,
           (float)(cl3 - cl22) / CLK_TCK,
           (float)(cl4 - cl3) / CLK_TCK, (float)(cl5 - cl4) / CLK_TCK);
#endif
    return (ret);
}
//////////////
#endif
