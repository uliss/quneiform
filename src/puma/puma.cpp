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

#define __PUMA_CPP__

#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include "puma.h"

#include "mpuma.h"
#include "compat_defs.h"
//////////////////////////////////////////////////////////////////
// 07.07,2000 Allex
// Вынес Бинаризацию наверх из PreProcessImage
//////////////////////////////////////////////////////////Allex
//BitmapInfoHeader info;

Bool32 PreProcessImage() {
	Bool32 rc = TRUE;
	uint32_t Angle = 0;

	if (InitPRGTIME())
		ProgressStart();

	if (!ProgressStep(1, GetResourceString(IDS_PRG_OPEN), 5))
		rc = FALSE;

	//
	// Выделим компоненты
	//
	if (!ProgressStep(2, GetResourceString(IDS_PRG_OPEN), 65))
		rc = FALSE;

	if (rc) {
		if (LDPUMA_Skip(hDebugCancelComponent)) {
			PRGTIME prev = StorePRGTIME(65, 85);
			rc = ExtractComponents(gbAutoRotate, NULL, glpRecogName);
			RestorePRGTIME(prev);

			if (!ProgressStep(2, NULL, 100))
				rc = FALSE;

		} else
			LDPUMA_Console("Пропущен этап выделения компонент.\n");
	}
	//
	// Проинициализируем контейнер CPAGE
	//
	if (rc) {
		PAGEINFO PInfo;// = { 0 };
		GetPageInfo(hCPAGE, &PInfo);
		strcpy((char*) PInfo.szImageName, glpRecogName);
		PInfo.BitPerPixel = info.biBitCount;
		PInfo.DPIX = info.biXPelsPerMeter * 254L / 10000;
		PInfo.DPIX = PInfo.DPIX < 200 ? 200 : PInfo.DPIX;
		PInfo.DPIY = info.biYPelsPerMeter * 254L / 10000;
		PInfo.DPIY = PInfo.DPIY < 200 ? 200 : PInfo.DPIY;
		PInfo.Height = info.biHeight;
		PInfo.Width = info.biWidth;
		//		PInfo.X = 0; Уже установлено
		//		PInfo.Y = 0;
		PInfo.Incline2048 = 0;
		PInfo.Page = 1;
		PInfo.Angle = Angle;

		SetPageInfo(hCPAGE, PInfo);
	}

	if (DonePRGTIME())
		ProgressFinish();

	return rc;
}

int32_t PUMA_EnumLanguages(int32_t nPrev) {
	return _EnumLanguage(nPrev);
}

int32_t PUMA_EnumFormats(int32_t nPrev) {
	return _EnumFormats(nPrev);
}

int32_t PUMA_EnumCodes(int32_t format, int32_t nPrev) {
	return _EnumCodes(format, nPrev);
}

int32_t PUMA_EnumFormatMode(int32_t nPrev) {
	return _EnumFormatMode(nPrev);
}

int32_t PUMA_EnumTable(int32_t nPrev) {
	return _EnumTable(nPrev);
}

int32_t PUMA_EnumPicture(int32_t nPrev) {
	return _EnumPicture(nPrev);
}

bool PUMA_XGetTemplate(Rect32 *pRect) {
	*pRect = gRectTemplate;
	return true;
}

bool PUMA_XSetTemplate(Rect32 rect) {
	Rect32 old_rect = gRectTemplate;
	bool rc = false;
	BitmapInfoHeader info;

	if (CIMAGE_GetImageInfo(PUMA_IMAGE_USER, &info)) {
		CIMAGE_Rect full = { 0, 0, info.biWidth, info.biHeight };
		PAGEINFO PInfo;

		GetPageInfo(hCPAGE, &PInfo);
		//		PInfo.status &= ~(PINFO_USERTEMPLATE | PINFO_AUTOTEMPLATE);
		PInfo.status &= ~3;
		if (rect.left < 0 && rect.right < 0 && rect.bottom < 0 && rect.top < 0) {
			rect.bottom = full.dwHeight;
			rect.left = full.dwX;
			rect.right = full.dwWidth;
			rect.top = full.dwY;
		}

		if (old_rect.bottom == rect.bottom && old_rect.left == rect.left
				&& old_rect.right == rect.right && old_rect.top == rect.top) {
			PInfo.X = rect.left;
			PInfo.Y = rect.top;
			SetPageInfo(hCPAGE, PInfo);
			return true;
		}
		if (CIMAGE_AddReadCloseRects(PUMA_IMAGE_USER, 1, &full)) {
			if (rect.left >= 0 && rect.top >= 0 && (rect.right - rect.left)
					<= info.biWidth && (rect.bottom - rect.top)
					<= info.biHeight)

			{
				CIMAGE_Rect r = { rect.left, rect.top, rect.right - rect.left,
						rect.bottom - rect.top };
				rc = CIMAGE_RemoveReadCloseRects(PUMA_IMAGE_USER, 1, &r);
				PInfo.X = rect.left;
				PInfo.Y = rect.top;
			} else {
				CIMAGE_Rect r = { 0, 0, info.biWidth - 1, info.biHeight - 1 };
				rc = CIMAGE_RemoveReadCloseRects(PUMA_IMAGE_USER, 1, &r);
				PInfo.X = 0;
				PInfo.Y = 0;
			}
			SetPageInfo(hCPAGE, PInfo);
			SetUpdate(FLG_UPDATE, FLG_UPDATE_NO);
			gRectTemplate = rect;
		}
	}
	return rc;
}

//OLEG fot Consistent
void PUMA_GetSpecialBuffer(char * szResult, int32_t *nResultLength) {
	extern char global_buf[];
	extern int32_t global_buf_len;

	if (nResultLength)
		*nResultLength = global_buf_len;
	if (szResult) {
		if (global_buf_len)
			memcpy(szResult, global_buf, global_buf_len);
		else
			szResult[0] = '\0';
	}
}

bool PUMA_SetSpecialProject(uchar nSpecialProject) {
	gnSpecialProject = nSpecialProject;
	RSTUFF_RSSetSpecPrj(nSpecialProject);
	RSTR_SetSpecPrj(nSpecialProject);

	return true;
}
