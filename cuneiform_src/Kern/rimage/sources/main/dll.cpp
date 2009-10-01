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
// Written by Peter Khlebutin
// This file creation date: 20.07.98
//
// dll_cpage.cpp :
// ============================================================================

#include "resource.h"
#include "cridefines.h"
#include "crimemory.h"
#include "cricontrol.h"
#include "criimage.h"
//#include "dpuma.h"
#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t            gwHeightRC = 0;
static uint16_t            gwLowRC = 0;
static Handle            ghStorage = NULL;
static Handle            ghInst =  NULL;
CRIControl *      Control_cri = NULL;
static int32_t             InitCount =  0;

Bool32 InitCFIOInterface(Bool32 Status);
/////////////////////////////////////////
Bool APIENTRY DllMain( HINSTANCE hModule,
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
RIMAGE_FUNC(Bool32) RIMAGE_Init(uint16_t wHeightCode,Handle hStorage)
{

	if ( !Control_cri )
	{
		if ( InitCFIOInterface(TRUE) )
		{
			Control_cri = new CRIControl;
			gwHeightRC = wHeightCode;
		}
		else
			return FALSE;
	}

	if ( Control_cri )
	{
		InitCount++;
		return TRUE;
	}

	SetReturnCode_rimage(IDS_RIMAGE_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32)RIMAGE_Done()
{

	if ( Control_cri )
	{
		if (--InitCount == 0)
		{
			delete Control_cri;
			Control_cri = NULL;
			InitCFIOInterface(FALSE);
		}
		return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32)RIMAGE_Reset()
{

	if ( Control_cri )
	{
		if (InitCount == 1)
		{
			delete Control_cri;
			Control_cri = new CRIControl;
			return TRUE;
		}
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(uint32_t) RIMAGE_GetReturnCode()
{
	if ( !gwLowRC )
		return 0;

	return (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_RIMAGE_ERR_NO);
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(char *) RIMAGE_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF) + IDS_RIMAGE_ERR_NO;
	static char szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;

	if( rc >= IDS_RIMAGE_ERR_NO )
		LoadString((HINSTANCE)ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//
#define CASE_FUNCTION(a)	case RIMAGE_FN_##a:	*(FNRIMAGE##a *)pData = RIMAGE_##a; break
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;


	switch(dwType)
	{
	CASE_FUNCTION(SetMargins);
	CASE_FUNCTION(Binarise);
	CASE_FUNCTION(Rotate);
	CASE_FUNCTION(Turn);
	CASE_FUNCTION(Inverse);

	default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
RIMAGE_FUNC(Bool32) RIMAGE_SetImportData(uint32_t dwType, void * pData)
{
	Bool rc = FALSE;
	gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;

	switch(dwType)
	{
	case RIMAGE_FN_SetProgressStart:
		Control_cri->SetProgressCallBacks((PRIMAGECBPRogressStart)pData, NULL, NULL);
		rc = TRUE;
		break;
	case RIMAGE_FN_SetProgressStep:
		Control_cri->SetProgressCallBacks(NULL, (PRIMAGECBPRogressStep)pData, NULL);
		rc = TRUE;
		break;
	case RIMAGE_FN_SetProgressFinish:
		Control_cri->SetProgressCallBacks(NULL, NULL, (PRIMAGECBPRogressFinish)pData);
		rc = TRUE;
		break;
	default:
		gwLowRC = IDS_RIMAGE_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
void SetReturnCode_rimage(uint16_t rc)
{
	if ( rc == IDS_RIMAGE_ERR_NO || gwLowRC == IDS_RIMAGE_ERR_NO )
		gwLowRC = rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint16_t GetReturnCode_rimage()
{
	return gwLowRC;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
