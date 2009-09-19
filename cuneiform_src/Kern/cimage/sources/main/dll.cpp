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

#include "resource.h"
#include "ctidefines.h"
#include "ctiimage.h"

#include "cticontrol.h"

#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t            gwHeightRC = 0;
static uint16_t            gwLowRC =    0;
static HINSTANCE         ghInst =     NULL;
CTIControl *      Control_cti =    NULL;
static int32_t             InitCount =  0;
/////////////////////////////////////////
#undef APIENTRY
#define APIENTRY

Bool APIENTRY DllMain( HINSTANCE  hModule,
                        uint32_t ul_reason_for_call,
                        pvoid lpReserved )
{
    switch( ul_reason_for_call )
	{
    case DLL_PROCESS_ATTACH:
		ghInst = hModule;
		break;
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		break;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Init(uint16_t wHeightCode,Handle hStorage)
{
	gwHeightRC = wHeightCode;

	if ( !Control_cti )
	{
		if ( !InitCFIOInterface(TRUE) )
		{
			SetReturnCode_cimage(IDS_CIMAGE_OTHER_DLL_NOT_INITIALIZED);
			return FALSE;
		}

		Control_cti = new CTIControl;
	}

	if ( Control_cti )
	{
		InitCount++;
		return TRUE;
	}

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_Done()
{
	SetReturnCode_cimage(IDS_CIMAGE_ERR_NO);

	if ( Control_cti )
	{
		if (--InitCount == 0)
		{
			delete Control_cti;
			Control_cti = NULL;
			InitCFIOInterface(FALSE);
		}
		return TRUE;
	}

	SetReturnCode_cimage(IDS_CIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(uint32_t) CIMAGE_GetReturnCode()
{
	if ( !gwLowRC )
		return 0;

	return (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_CIMAGE_ERR_NO);
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(pchar) CIMAGE_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF) + IDS_CIMAGE_ERR_NO;
	static char szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_CIMAGE_ERR_NOTIMPLEMENT;

	if( rc >= IDS_CIMAGE_ERR_NO )
		LoadString(ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return (pchar)szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//
#define CASE_FUNCTION(a)	case CIMAGE_FN_##a:	*(FNCIMAGE##a *)pData = CIMAGE_##a; break
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;


	switch(dwType)
	{
	CASE_FUNCTION(WriteCallbackImage);
	CASE_FUNCTION(GetCallbackImage);
	CASE_FUNCTION(WriteDIB);
	CASE_FUNCTION(ReadDIB);
	CASE_FUNCTION(GetData);
	CASE_FUNCTION(GetDIBData);
	CASE_FUNCTION(ReplaceData);
	CASE_FUNCTION(GetImageInfo);
	CASE_FUNCTION(DeleteImage);
	CASE_FUNCTION(FreeCopedDIB);
	CASE_FUNCTION(FreeBuffers);
	CASE_FUNCTION(Reset);
	CASE_FUNCTION(AddReadCloseRects);
	CASE_FUNCTION(RemoveReadCloseRects);
	CASE_FUNCTION(AddWriteCloseRects);
	CASE_FUNCTION(RemoveWriteCloseRects);

	default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_CIMAGE_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
CIMAGE_FUNC(Bool32) CIMAGE_SetImportData(uint32_t dwType, void * pData)
{
	Bool rc = FALSE;
	gwLowRC = IDS_CIMAGE_ERR_NOTIMPLEMENT;

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
void SetReturnCode_cimage(uint16_t rc)
{
	if ( rc == IDS_CIMAGE_ERR_NO || gwLowRC == IDS_CIMAGE_ERR_NO )
		gwLowRC = rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint16_t GetReturnCode_cimage()
{
	return gwLowRC;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
