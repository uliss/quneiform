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

// RCorrKegl.cpp : Defines the entry point for the DLL application.
//

#include <string.h>
#include "stdafx.h"
#include "rcorrkegl.h"
#include "resource.h"
#include "dpuma.h"
#include "compat_defs.h"

Bool32 CorrectKegl(int32_t version);
void kegl_snap_init();

static uint16_t gwHeightRC = 0;
static uint16_t gwLowRC = 0;
Bool32 snap_enable = TRUE;
Bool32 exit_enable = FALSE;
Bool32 gbFax100 = FALSE;
uchar language = 3;

Bool APIENTRY DllMain(HANDLE hModule, uint32_t ul_reason_for_call,
		pvoid lpReserved) {
	return TRUE;
}

RCK_FUNC(Bool32) RCORRKEGL_Init(uint16_t wHeightCode,HANDLE hStorage)
{
	gwHeightRC = wHeightCode;
	snap_enable = TRUE;
	exit_enable = FALSE;

	kegl_snap_init();
	LDPUMA_Init(0,NULL);

	return TRUE;
}

RCK_FUNC(Bool32) RCORRKEGL_Done()
{
	LDPUMA_Done();

	return TRUE;
}

RCK_FUNC(uint32_t) RCORRKEGL_GetReturnCode()
{
	if(gwLowRC == RCORRKEGL_ERR_NO)
	return 0;

	return (gwHeightRC<<16)|(gwLowRC-RCORRKEGL_ERR_MIN);
}

RCK_FUNC(char*) RCORRKEGL_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)((dwError & 0xFFFF) );
	static char szBuffer[512];

	if (dwError >> 16 != gwHeightRC) gwLowRC = RCORRKEGL_ERR_NOTIMPLEMENT;

	if (rc > 0 && rc <= RCORRKEGL_ERR_MAX - RCORRKEGL_ERR_MIN)
	strcpy((char*)szBuffer, RCORRKEGL_error_name [rc]);
	else
	return NULL;

	return szBuffer;
}

RCK_FUNC(Bool32) RCORRKEGL_SetImportData(uint32_t dwType, void * pData)
{

	gwLowRC = RCORRKEGL_ERR_NO;
#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)	case a: c = (b)pData; break
	switch(dwType)
	{
		CASE_DATA(RCORRKEGL_Bool32_Fax100,Bool32,gbFax100);
		// 12.06.2002 E.P.
		CASE_DATA(RCORRKEGL_FNIMP_LANGUAGE,uchar,language);

		default:
		gwLowRC = RCORRKEGL_ERR_NOTIMPLEMENT;
		return FALSE;
	}
#undef CASE_DATA
#undef CASE_PDATA

	return TRUE;
}

RCK_FUNC(Bool32) RCORRKEGL_CorrectKegl(int32_t version)
{
	return CorrectKegl(version);
}
