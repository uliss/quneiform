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

// ============================================================================
// Copyright (c) 1998 Cognitive Technology Ltd.
// Written by Peter Khlebutin
// This file creation date: 20.07.98
//
// dll_cpage.cpp :
// ============================================================================

#include <cstring>
#include <cstdio>
#include <ctime>
#include <climits>

#include "puma.h"

#include "resource.h"
#include "mpuma.h"
#include "pumadef.h"
#include "compat_defs.h"

//GLOBAL VARIABLES
static uint16_t gwHeightRC = 0;
static uint32_t gwRC = 0;
static HINSTANCE ghInst = NULL;
static char szPath[PATH_MAX] = ".";
static char szTempPath[PATH_MAX] = ".";

int PUMA_GetReturnCode() {
	return gwRC;
}

char * PUMA_GetReturnString(int dwError) {
	static char szBuffer[512];
	uint16_t low = (uint16_t) (dwError & 0xFFFF);
	uint16_t hei = (uint16_t) (dwError >> 16);

	if (hei == gwHeightRC) {
		if (!LoadString(ghInst, low + IDS_ERR_NO, (char *) szBuffer,
				sizeof(szBuffer)))
			LoadString(ghInst, IDS_ERR_NOTIMPLEMENT, (char *) szBuffer,
					sizeof(szBuffer));
	} else {
		char * p = GetModulesString(dwError);
		if (p)
			return p;
		LoadString(ghInst, IDS_ERR_NOTIMPLEMENT, (char *) szBuffer,
				sizeof(szBuffer));
	}

	return szBuffer;

}

bool PUMA_SetImportData(uint32_t dwType, void * pData) {
	bool rc = true;

	gwRC = 0;

#define CASE_DATA(a,b,c)		case a: c = *(b *)pData; break;
#define CASE_DATAUP(a,b,c,d)	case a: if(c != *(b *)pData){c = *(b *)pData; SetUpdate(d,FLG_UPDATE_NO);}; break;
#define CASE_PDATA(a,b,c)		case a: c = (b)pData; break;

	switch (dwType) {
	CASE_DATAUP(PUMA_Word32_Language,uint32_t,gnLanguage,FLG_UPDATE_CCOM)
	CASE_DATA(PUMA_Bool32_Speller,Bool32,gbSpeller)
	CASE_DATAUP(PUMA_Bool32_OneColumn,Bool32,gbOneColumn,FLG_UPDATE_CPAGE)
	CASE_DATAUP(PUMA_Bool32_Fax100,Bool32,gbFax100,FLG_UPDATE_CCOM)
	CASE_DATAUP(PUMA_Bool32_DotMatrix,Bool32,gbDotMatrix,FLG_UPDATE_CCOM)
	CASE_DATA(PUMA_Bool32_Bold,Bool32,gbBold)
	CASE_DATA(PUMA_Bool32_Italic,Bool32,gbItalic)
	CASE_DATA(PUMA_Bool32_Size,Bool32,gbSize)
	CASE_DATA(PUMA_Bool32_Format,Bool32,gbFormat)
	CASE_PDATA(PUMA_pchar_UserDictName,char *,gpUserDictName)
	CASE_PDATA(PUMA_pchar_SerifName,char *,gpSerifName)
	CASE_PDATA(PUMA_pchar_SansSerifName,char *,gpSansSerifName)
	CASE_PDATA(PUMA_pchar_CourierName,char *,gpCourierName)
	CASE_DATAUP(PUMA_Word32_Pictures,uint32_t,gnPictures,FLG_UPDATE_CPAGE)
	CASE_DATAUP(PUMA_Word32_Tables,uint32_t,gnTables,FLG_UPDATE_CPAGE)
	CASE_DATA(PUMA_Word32_Format,Bool32,gnFormat)
	CASE_DATA(PUMA_Word8_Format,uchar,gnUnrecogChar)
	CASE_PDATA(PUMA_FNPUMA_ProgressStart, FNPUMA_ProgressStart ,fnProgressStart)
	CASE_PDATA(PUMA_FNPUMA_ProgressFinish,FNPUMA_ProgressFinish,fnProgressFinish)
	CASE_PDATA(PUMA_FNPUMA_ProgressStep, FNPUMA_ProgressStep ,fnProgressStep)
	CASE_DATAUP(PUMA_Bool32_AutoRotate,Bool32,gbAutoRotate,FLG_UPDATE)
	CASE_DATA(PUMA_Handle_CurrentEdPage,Handle,ghEdPage)
	default:
		SetReturnCode_puma(IDS_ERR_NOTIMPLEMENT);
		rc = FALSE;
	}
	// Связь с предыдущими версиями
	switch (dwType) {
	case PUMA_Bool32_Format:
		gnFormat = gbFormat ? gnFormat : 64;
		break;
	case PUMA_Word32_Format:
		gbFormat = gnFormat == 64 ? 0 : 1;
		break;
	}

#undef CASE_DATA
#undef CASE_PDATA

	return rc;
}

void SetReturnCode_puma(uint32_t rc) {
	uint16_t low = (uint16_t) (rc & 0xFFFF);
	uint16_t hei = (uint16_t) (rc >> 16);

	if (hei)
		gwRC = rc;
	else {
		if (low >= IDS_ERR_NO)
			gwRC = (uint32_t) (gwHeightRC << 16) | (low - IDS_ERR_NO);
		else
			gwRC = low;
	}

	if (low > 0 && low != IDS_ERR_NO)
		LDPUMA_Console("%s\n", GetModulesString(gwRC));

	if (gwRC == nDebugReturnCode && nDebugReturnCode)
		LDPUMA_Stop();
}

uint32_t GetReturnCode_puma() {
	uint32_t rc = gwRC;
	uint16_t low = (uint16_t) (gwRC & 0xFFFF);
	uint16_t hei = (uint16_t) (gwRC >> 16);

	if (hei == gwHeightRC || hei == 0)
		rc = low + IDS_ERR_NO;

	return rc;
}

char * GetModulePath() {
	return szPath;
}

char * GetModuleTempPath() {
	return szTempPath;
}

char * GetResourceString(uint32_t id) {
	static char szBuffer[1024] = "";
	LoadString(ghInst, id, szBuffer, sizeof(szBuffer));
	return szBuffer;
}
