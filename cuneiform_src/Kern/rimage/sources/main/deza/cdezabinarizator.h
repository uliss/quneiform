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

// CDezaBinarizator.h: interface for the CDezaBinarizator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CDEZA_BINARIZATOR_H_)
#define _CDEZA_BINARIZATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdlib.h"
#include "globus.h"
#include "crimemory.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef	struct tagTigerImageInfo
	{
		unsigned short int wImageHeight;       /* Height of the image in lines */
		unsigned short int wImageWidth;        /* Width of the image in pixels */
		unsigned short int wImageByteWidth; /* Size of the image line in bytes */
		unsigned short int wImageDisplacement;
		/* Displacement of the first pixel of image line in pixels, usually 0 */
		unsigned short int wResolutionX;                /* Resolution by X-axe */
		unsigned short int wResolutionY;                /* Resolution by Y-axe */
		unsigned char bFotoMetrics;                             /* White pixel */
		unsigned char bUnused;         /* not used; for alignment purpose only */
		unsigned short int wAddX;
		unsigned short int wAddY;
	} CTBINTigerImageInfo, *PCTBINTigerImageInfo;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
#define   DB_MALLOC(a)        RIMAGEAlloc(a)
#define   DB_FREE(a)          RIMAGEFree(a)
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CRIBinarizator;
///////////////////////////////////////////////////////////////////////////////////////////////////
//
class CDezaBinarizator
{
	#define BUFS 3*1000
    #define SB 256
    #define LONGBUF 15L*4096L

public:
	int32_t CloseTrackBin(void);
	int32_t GetBinarized(puchar ptbl, uint32_t lenbl);
	uint32_t OpenTrackBin(PCTBINTigerImageInfo Info, CRIBinarizator * pCaller, uint32_t wdezaFlag);
	CDezaBinarizator();
	virtual ~CDezaBinarizator();

private:
	int32_t Get1(void);
	int32_t Read_por_first(void);
	int32_t Elem_st(void);
	int32_t Grey_black(void);
	int32_t Xygo(int32_t, int32_t);
	int32_t Our1(int32_t);
	puchar Black_file(puchar, int32_t);

private:
	void Ras1_pred(int32_t);
	void Raspred(int32_t);
	void Stek(void);
	void Left_str(void);
	void Right_str(void);
	int32_t bWhitePixel;
	int32_t colall;
	int32_t chet_color[SB];
	uchar lg0;
	puchar ptbtek;
	puchar ptbosn;
	puchar ptb;
	puchar ptgrey;
	uint32_t indend;
	uint32_t indbeg;
	int32_t nstrb;
	int32_t nstr;
	int32_t sy1;
	int32_t sdvig;
	int32_t lg0i;
	int32_t spx2;
	int32_t spx1;
	int32_t spy;
	int32_t spx;
	int32_t bufmark[4];
	int32_t sheds;
	int32_t shed;
	int32_t st[BUFS];
	int32_t ypor;
	int32_t tip;
	int32_t indbl;
	int32_t lgn;
	int32_t lg;
	int32_t xx;
	int32_t y;
	int32_t x;
	int32_t urov[2];
	CRIBinarizator * mpBinarizatorControl;
	//////////////////////////////////////////////////////////////////////
	#define  DB_GREYREAD(a,b,c)    mpBinarizatorControl->KronrodImageRead(a,b,c)
};
/////////////////////////////////////////////////////
//
typedef CDezaBinarizator *PCDezaBinarizator;
///////////////////////////////////////////////////
//
#endif // !defined(_CDEZA_BINARIZATOR_H_)
