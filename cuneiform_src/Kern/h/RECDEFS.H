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

#ifndef __RECDEFS_H
#define __RECDEFS_H


#include "globus.h"

#pragma pack(1)

#define  RD_UNRECOG_CHAR   '~'
/////////////////////////////////////////////////////////
// RecRaster - rectangular peace of black/white image
// Black pixel is 1
// Alignment is 8 bytes ( 64 bits )

#define  REC_MAX_RASTER_SIZE   4*1024  // 256*128
#define  REC_MAX_RASTER_SIZE_EX  256*128
#define  REC_MAX_VERS          16

#define  REC_GW_WORD8(a)  ( ( (a+63)/64 )*8)
#define  REC_GW_WORD16(a) ( ( (a+63)/64 )*4)
#define  REC_GW_WORD32(a) ( ( (a+63)/64 )*2)
#define  REC_GW_WORD64(a) ( ( (a+63)/64 )*1)

#define  RRBWr( recRasterRef ) ((( (recRasterRef) .lnPixWidth + 63) >> 6) << 3)
#define  RRBWp( recRasterPtr ) ((( (recRasterPtr)->lnPixWidth + 63) >> 6) << 3)


typedef struct tagRecRaster
   {
      int32_t    lnPixWidth;
      int32_t    lnPixHeight;
      int32_t    lnRasterBufSize;				// maximum of raster length
      uchar    Raster[REC_MAX_RASTER_SIZE];	// image
   }  RecRaster;

typedef struct tagRecRasterEx
   {
      int32_t    lnPixWidth;
      int32_t    lnPixHeight;
      int32_t    lnRasterBufSize;				// maximum of raster length
      int32_t    lnLevelGray;
      uchar    Raster[REC_MAX_RASTER_SIZE_EX];	// image
   }  RecRasterEx;
/////////////////////////////////////////////////////////
// RecBmp - .............................................
// Black pixel is 1

typedef struct tagRecBmp
   {
      Rect16         Region;
      uchar  *   pBits;
      int32_t          nOffsetX;   //this is offset of Region.left-bit
                                 //relatively 0-bit 0-byte of pBits
      int32_t          nByteWidth;
      int32_t          nResolutionX;
      int32_t          nResolutionY;
      char           reserved[16];  // zero filled reserve
   }  RecBmp;


typedef struct tagRecAlt
   {
      uchar    Code;
      uchar    CodeExt;        // for unicode
      uchar    Method;         // recognize method
      uchar    Prob;
      uint16_t   Info;
   }  RecAlt;
typedef struct tagUniAlt
   {
      uchar    Code[4];
      uchar    Liga;        // for unicode
      uchar    Method;         // recognize method
      uchar    Prob;
      uchar    Charset;
      uint16_t   Info;
   }  UniAlt;

typedef struct tagClustAlt
   {
      int16_t    nClust;         // cluster index; zero - structure is not initiated
      uchar    nDist;          // distance
      uchar    nReserved;
   }  ClustAlt;

typedef struct tagRecVersions
	{
	int32_t  lnAltCnt;           // count of alternates
	int32_t  lnAltMax;		   // maximum of alternates
	RecAlt Alt[REC_MAX_VERS];  // alternates array
	} RecVersions;
typedef struct tagUniVersions
	{
	int32_t  lnAltCnt;           // count of alternates
	int32_t  lnAltMax;		   // maximum of alternates
	UniAlt Alt[REC_MAX_VERS];  // alternates array
	} UniVersions;

typedef struct tagRecVector {
Point16 beg,end;
int32_t incline;   // 2048 * tangens
#define INCLINE_UNKNOWN	(-0x41414141)	  ///	      _______
int32_t  len;  // -1 - It isn't vector;  metric Eucleede ы xэ + yэ
int32_t Mx,My;
int32_t Cx,Cy;  // debug purposes
int32_t area;
uchar Mn; // normalized moment
uchar reserve[3];
#define MAX_VECT	64
#define NORMA_FACTOR	12
}  RecVector;

typedef struct tagRecData
	{
	uint32_t    lwStatus;

	RecRaster recRaster;       // basic raster. Can be not changed
	uint16_t    v3x5[16];        // normalizeted image 3x5
	uint32_t    lwCompCnt;       // number of components, 0 - not counted
	Rect16    rect;
  RecVector    vSticks[MAX_VECT];
  int16_t     lwSticksCnt;
	} RecData;

typedef struct tagRecObject
	{
	RecData     recData;
	RecVersions recResults;
	uint32_t      lwStatusMethods; // indicator of used methods


	} RecObject;

// Styles of images
#define LS_HAND      1   // handprinted letters
#define LS_INDEX     2   // post-indexes
#define LS_PRINT     4   // printed lettres

#pragma pack()

#endif   // __RECDEFS_H



