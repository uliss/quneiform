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

#include "stdafx.h"

# include <fcntl.h>
# include <io.h>
# include <string.h>

//# include "tgcall.h"
# include "tgcallbk.h"
# include "tigerdef.h"
# include "dpuma.h"

HANDLE ToMono(HANDLE hDIB);

typedef struct
	{
	uint32_t dwBeg;   // Начало текущего диапозона
	uint32_t dwEnd;	// Конец

	uint32_t dwStep;  // текущий номер шага
	const char * name;	// текущее название шага
	} PRGTIME;


PRGTIME g_PrgTime;

static HANDLE s_hTempDIB = NULL;
static char * s_pData = NULL;
static char * s_pDIB = NULL;
static TIGER_IMAGEINFO s_ImageInfo;
static int s_line = 0;
static int s_WidthBytesInDIB = 0;
////////////////////////////////////
extern FNPROGRESS_START fnProgressStart;
extern FNPROGRESS_STOP  fnProgressStop;
extern FNPROGRESS_STEP  fnProgressStep;

//////////////////////////////////////////////////////
static uint32_t bInitPrgTime = 0;
Bool32  DonePRGTIME()
{
	Bool32 rc = FALSE;
	if(	bInitPrgTime )
		bInitPrgTime--;

	if(!bInitPrgTime)
	{
		fnProgressStop();
		g_pRecognition->Fire_ProgressFinish();
		rc = TRUE;
	}

return rc;
}

Bool32  InitPRGTIME()
{
	Bool32 rc = FALSE;
	if(!bInitPrgTime)
	{
		g_PrgTime.dwBeg  = 0;
		g_PrgTime.dwEnd  = 100;
		g_PrgTime.dwStep = 0;
		g_PrgTime.name   = "";
		rc = TRUE;

		fnProgressStart();
		g_pRecognition->Fire_ProgressStart();
	}
	bInitPrgTime++;
	return rc;
}
PRGTIME	 StorePRGTIME(uint32_t beg, uint32_t end)
{
	PRGTIME rc = g_PrgTime;

	uint32_t newBeg = g_PrgTime.dwBeg + (g_PrgTime.dwEnd - g_PrgTime.dwBeg)*beg/100;
	uint32_t newEnd = g_PrgTime.dwBeg + (g_PrgTime.dwEnd - g_PrgTime.dwBeg)*end/100;

	g_PrgTime.dwBeg = newBeg;
	g_PrgTime.dwEnd = newEnd;

return rc;
}

void RestorePRGTIME(PRGTIME	prev)
{
	g_PrgTime  = prev;
}
////////////////////////////////////////////
FNBOOL16 Tiger_Callback_ImageOpen (LPTIGER_IMAGEINFO lpImageInfo)
{

 s_pDIB = NULL;

 LPBITMAPINFOHEADER pDIB = (LPBITMAPINFOHEADER)g_pDIB;

 if(!pDIB)
	 return FALSE;


 if(pDIB->biBitCount==1)
	s_pDIB = g_pDIB;

 if(s_pDIB==NULL)
 {
	s_hTempDIB = ToMono(g_pDIB);
	if(s_hTempDIB)
		s_pDIB = (char *)GlobalLock(s_hTempDIB);
	LDPUMA_CreateWindow(NULL,s_pDIB);
 }

 if(s_pDIB == NULL)
	return FALSE;


 pDIB = (LPBITMAPINFOHEADER)s_pDIB;



lpImageInfo->wImageWidth = (uint16_t)pDIB->biWidth;           // Height of the image in lines
lpImageInfo->wImageHeight = (uint16_t)pDIB->biHeight;            // Width of the image in pixels
s_WidthBytesInDIB = ((((lpImageInfo->wImageWidth+7)/8 + 3)/4) * 4);
lpImageInfo->wImageByteWidth = (lpImageInfo->wImageWidth+7)/8;        // Size of the image line in bytes
lpImageInfo->wImageDisplacement = 0;     // Displacement of the first pixel of
							  //   image line in pixels, usually 0
lpImageInfo->wResolutionX = ((uint16_t)((double)pDIB->biXPelsPerMeter/100.0*25.4)+5)/10;           // Resolution by X-axe

if(lpImageInfo->wResolutionX<200)
	lpImageInfo->wResolutionX = 200;

if(lpImageInfo->wResolutionX>400)
	lpImageInfo->wResolutionX = 400;

lpImageInfo->wResolutionY = lpImageInfo->wResolutionX;           // Resolution by Y-axe
lpImageInfo->bFotoMetrics = 1;           // White pixel
lpImageInfo->bUnused = 0;                // not used; for alignment purpose only

lpImageInfo->wAddX = 0;
lpImageInfo->wAddY = 0;

s_ImageInfo = *lpImageInfo;
s_line = 0;
s_pData = (char *)pDIB +
	 sizeof(BITMAPINFOHEADER) + 2*sizeof(RGBQUAD) +
	 s_WidthBytesInDIB*s_ImageInfo.wImageHeight;

return TRUE;
}
////////////////////////////////////////////

FNWORD Tiger_Callback_ImageRead (char* lpImage, uint16_t wMaxSize)
{
	 if(wMaxSize<s_ImageInfo.wImageByteWidth)
		return 0;

	 if(s_line < s_ImageInfo.wImageHeight)
		memcpy(lpImage,s_pData - (s_line+1)*s_WidthBytesInDIB,s_ImageInfo.wImageByteWidth);
	 else
		 return 0;
	s_line++;
return s_ImageInfo.wImageByteWidth;
}
/////////////////////////////////////////
FNBOOL16 Tiger_Callback_ImageClose (void)
{
	if(s_hTempDIB)
	{
		GlobalFree(s_hTempDIB);
		s_hTempDIB = NULL;
	}
    return (TRUE);
}

FNVOID Tiger_Callback_ReportError (uint16_t wStatus, char* lpMessage)
{
    wStatus = wStatus;
//	g_pRecognition->SetError(lpMessage);
}

static uint16_t swPercent = 0;
static uint16_t swStageNumber = 0;
static char StageName[256];

//static int  countProgressStart = 0;

FNBOOL16 Tiger_Callback_ProgressStart (void)
{
	BOOL16 rc = TRUE;
	swPercent = 0;
	swStageNumber = 0;
	StageName[0]='\0';
//	countProgressStart - позволяет отследить вложенный вызов прогресс индикатора
//	if(!countProgressStart)
//		rc = fnProgressStart() & (BOOL16)g_pRecognition->Fire_ProgressStart();

//	countProgressStart++;

	return rc;
}

FNBOOL16 Tiger_Callback_ProgressFinish (void)
{
	BOOL16 rc = TRUE;
//	if(countProgressStart)
//		countProgressStart--;

//	if(!countProgressStart)
//		rc = fnProgressStop() & (BOOL16)g_pRecognition->Fire_ProgressFinish();
return rc;
}


FNBOOL16 Tiger_Callback_ProgressSetPerc (uint16_t wPercent)
{
	USES_CONVERSION;

	uint32_t perc = g_PrgTime.dwBeg + wPercent*(g_PrgTime.dwEnd - g_PrgTime.dwBeg)/100;
	swPercent = (uint16_t)perc;

    return fnProgressStep(swStageNumber,StageName,perc) &
		(BOOL16)g_pRecognition->Fire_ProgressStep((long)swStageNumber,T2OLE(StageName),(long)perc);
}

FNBOOL16 Tiger_Callback_ProgressSetStage
(
    uint16_t wStageNumber,
    char* lpStageName,
    uint16_t wVolume
)
{
	USES_CONVERSION;
	swStageNumber = wStageNumber;
	strcpy(StageName,lpStageName);

	g_PrgTime.dwStep = wStageNumber;
	g_PrgTime.name = lpStageName;


    return fnProgressStep(swStageNumber,StageName,swPercent) &
		(BOOL16)g_pRecognition->Fire_ProgressStep((long)swStageNumber,T2OLE(StageName),(long)swPercent);
}

FNBOOL16 Tiger_Callback_ImageOpen_xy (LPTIGER_IMAGEINFO lpImageInfo,uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
lpImageInfo=lpImageInfo;
x=x;
y=y;
w=w;
h=h;
return TRUE;
}

FNuint32_t Tiger_Callback_ProcessPictures(char* buf, uint32_t lth)
{
buf=buf;
lth=lth;
return TRUE;
}

FNBOOL Tiger_Callback_Get_One_Word (char* Buffer )
{
	return TRUE;
}
