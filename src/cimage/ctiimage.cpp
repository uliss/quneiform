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

#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"
#include "cticontrol.h"

using namespace CIF;

Bool32 CIMAGE_WriteCallbackImage(const char * Name, CIMAGEIMAGECALLBACK Cbk) {
	return CImage::instance().WriteCBImage(Name, Cbk);
}

Bool32 CIMAGE_GetCallbackImage(const char * Name, CIMAGEIMAGECALLBACK * pCbk) {
	return CImage::instance().GetCBImage(Name, pCbk);
}

Bool32 CIMAGE_WriteDIB(const char * Name, Handle lpDIB, uint32_t wFlag) {
	return CImage::instance().SetDIB(Name, lpDIB, wFlag);
}

Bool32 CIMAGE_ReadDIB(const char * Name, Handle* lplpDIB, uint32_t wFlag) {
	return CImage::instance().GetDIB(Name, lplpDIB, wFlag);
}

Bool32 CIMAGE_GetData(const char *Name, CIMAGE_InfoDataInGet * lpIn,
		CIMAGE_InfoDataOutGet * lpOut) {
	return CImage::instance().GetImage(Name, lpIn, lpOut);
}

Bool32 CIMAGE_GetDIBData(const char * Name, CIMAGE_InfoDataInGet * lpIn,
		void ** lpDIB) {
	return CImage::instance().GetDIBFromImage(Name, lpIn, lpDIB);
}

Bool32 CIMAGE_ReplaceData(const char * Name, CIMAGE_InfoDataInReplace * lpIn) {
	return CImage::instance().ReplaceImage(Name, lpIn);
}

Bool32 CIMAGE_GetImageInfo(const char * Name, BitmapInfoHeader * lpBIH) {
	return CImage::instance().GetImageInfo(Name, lpBIH);
}

Bool32 CIMAGE_DeleteImage(const char * Name) {
	return CImage::instance().RemoveImage(Name);
}

Bool32 CIMAGE_FreeCopedDIB(Handle hDIB) {
	return CImage::instance().FreeAlloced(hDIB);
}

Bool32 CIMAGE_FreeBuffers() {
	CImage::instance().FreeBuffers();
	return TRUE;
}

void CIMAGE_Reset(void) {
	CImage::instance().Reset();
}

Bool32 CIMAGE_AddReadCloseRects(const char * Name, uint32_t wCount,
		CIMAGE_Rect * pFirst) {
	return CImage::instance().AddReadRectangles(Name, wCount, pFirst);
}

Bool32 CIMAGE_RemoveReadCloseRects(const char * Name, uint32_t wCount,
		CIMAGE_Rect * pFirst) {
	return CImage::instance().RemoveReadRectangles(Name, wCount, pFirst);
}

Bool32 CIMAGE_AddWriteCloseRects(const char *Name, uint32_t wCount,
		CIMAGE_Rect * pFirst) {
	return CImage::instance().AddWriteRectangles(Name, wCount, pFirst);
}

Bool32 CIMAGE_RemoveWriteCloseRects(const char * Name, uint32_t wCount,
		CIMAGE_Rect * pFirst) {
	return CImage::instance().RemoveWriteRectangles(Name, wCount, pFirst);
}

Bool32 CIMAGE_EnableMask(const char * Name, const char * lpType,
		Bool32 bEnabler) {
	return CImage::instance().EnableMask(Name, lpType, bEnabler);
}

// For GetCBImage
Bool16 CIMAGE_Callback_ImageOpen(CIMAGE_ImageInfo * lpImageInfo) {
	return CImage::instance().CBImageOpen(lpImageInfo);
}

uint16_t CIMAGE_Callback_ImageRead(char * lpImage, uint16_t wMaxSize) {
	return CImage::instance().CBImageRead(lpImage, wMaxSize);
}

Bool16 CIMAGE_Callback_ImageClose(void) {
	return CImage::instance().CBImageClose();
}
