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

#ifndef __CTI_CONTROL_H_
#define __CTI_CONTROL_H_

#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "ctimemory.h"
#include "ctdib.h"
#include "ctimask.h"
#include "ctiimagelist.h"
#include "ctiimageheader.h"

#include "minmax.h"

//#define CIMAGE_CBR_ONE_LINE

class CTIControl {
protected:
	CTIImageList mlImages;
	Handle hCBImage;
	void * pCBImage;
	char * pCBBuffer;
	uint32_t wCBBufferSize;
	uint32_t wCBLine;
	uint32_t wCBWidth;
	uint32_t wCBLines;
	uint32_t wCBStep;
	char mCBName[CIMAGE_MAX_IMAGE_NAME];
	char mCBWName[CIMAGE_MAX_IMAGE_NAME];
	Handle mhBitFildFromImage;
	puchar mpBitFildFromImage;
	PCTDIB mpDIBFromImage;

public:
	CTIControl();
	~CTIControl();

private:
	Bool32 ApplayMaskToDIBLine(PCTDIB pcDIB, PCTIMaskLineSegment pSegm,
			uint32_t wLine, uint32_t wAtX, uint32_t wAtY);
	Bool32 ApplayMaskToDIB(PCTDIB pDIB, PCTIMask pMask, uint32_t wAtX = 0,
			uint32_t wAtY = 0);
	Bool32 RemoveRectsFromMask(const char *lpName, uint32_t wNumber,
			CIMAGE_Rect * pFirstRect, const char*pcType);
	Bool32 AddRectsToMask(const char *lpName, uint32_t wNumber,
			CIMAGE_Rect * pFirstRect, const char *pcType);
	Bool32 OpenDIBFromList(const char *lpName, PCTDIB pDIB);
	Bool32 SetMaskToList(const char* pName, PCTIMask pMask, const char* pcType);
	Bool32 OpenMaskFromList(const char *lpName, PPCTIMask ppMask,
			PBool32 pEnMask, const char *pcType);
	Bool32 OpenDIBFromList(const char* lpName, Handle* phImage);
	Bool32 WriteDIBtoBMP(const char *cName, PCTDIB pDIB);
	Bool32 SetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB,
			CIMAGE_InfoDataInReplace * pIn);
	Bool32 ApplayBitMaskToFrame(CIMAGE_InfoDataInGet* pIn,
			CIMAGE_InfoDataOutGet * pOut);
	Bool32 ApplayBitMaskToDIB(puchar pMask, PCTDIB pDIB);
	Bool32 CopyFromFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB,
			CIMAGE_InfoDataInReplace * pFrameIn);
	Bool32 CopyToFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB,
			CIMAGE_InfoDataInGet* pFrameInfo, puchar pMask);
	Bool32 GetFrame(PCTDIB pSrcDIB, PCTDIB pDscDIB, CIMAGE_InfoDataInGet* pIn,
			puchar pMask);
	Bool32 CopyDIB(Handle hDIB, Handle* hCopyedDib);
	Bool32 DumpToFile(const char* FileName, puchar pData, uint32_t Size);
	Bool32 CheckInData(PCTDIB pDIB, CIMAGE_InfoDataInGet* lpIn,
			CIMAGE_InfoDataInGet* lpNewIn = NULL);

public:
	Bool32 EnableMask(const char* pcName, const char* pcType, Bool32 bEnable);
	Bool32 RemoveReadRectangles(const char* lpName, uint32_t wNumber,
			CIMAGE_Rect * pFirst);
	Bool32 AddReadRectangles(const char* lpName, uint32_t wNumber,
			CIMAGE_Rect * pFirst);
	Bool32 RemoveWriteRectangles(const char* lpName, uint32_t wNumber,
			CIMAGE_Rect * pFirst);
	Bool32 AddWriteRectangles(const char* lpName, uint32_t wNumber,
			CIMAGE_Rect * pFirst);
	Bool32 FreeBuffers(void);
	Bool32 FreeAlloced(Handle hDIB);
	Bool32 GetDIBFromImage(const char* lpName, CIMAGE_InfoDataInGet* lpIn,
			pchar *pDIB);
	Bool32 RemoveImage(const char* lpName);
	Bool32 GetImageInfo(const char* lpImage, CIMAGEBITMAPINFOHEADER * lpBIH);
	Bool32 ReplaceImage(const char* lpName, CIMAGE_InfoDataInReplace * lpIn);
	Bool32 GetImage(const char* lpName, CIMAGE_InfoDataInGet * lpIn,
			CIMAGE_InfoDataOutGet * lplpOut);
	Bool32 GetDIB(const char* lpName, Handle* phDIB, uint32_t wFlag = 0);
	Bool32 SetDIB(const char* lpName, Handle hDIB, uint32_t wFlag = 0);
	Bool32 GetCBImage(const char* lpName, CIMAGEIMAGECALLBACK * pCbk);
	Bool32 WriteCBImage(const char* lpName, CIMAGEIMAGECALLBACK Cbk);
	Bool32 CBImageOpen(CIMAGE_ImageInfo * lpImageInfo);
	Bool32 CBImageClose(void);
	uint32_t CBImageRead(char * buffer, uint32_t wMaxSize);
	Bool32 CloseDIBFromList(PCTDIB pDIB);

protected:
	Handle mhCopyedDIB;
	Bool32 mbWriteFlag;
	uint32_t mwMemoryErrors;
	uchar mwLAWhiteRightMask[8];
	uchar mwLAWhiteLeftMask[8];
	uchar mwLABlackRightMask[8];
	uchar mwLABlackLeftMask[8];
	uchar mwIndexMask[8];
	Bool32 mbSourceDIBCopy;
	PCTIMask mpcSrcDIBReadMask;
	PCTIMask mpcSrcDIBWriteMask;
	Bool32 mbEnableDIBReadMask;
	Bool32 mbEnableDIBWriteMask;
	PCTDIB mCBDestianationDIB;
	PCTDIB mCBSourceDIB;
	PCTDIB mCBWDestianationDIB;
	PCTDIB mCBWSourceDIB;
	Bool32 mCBWInProcess;

};

#endif

