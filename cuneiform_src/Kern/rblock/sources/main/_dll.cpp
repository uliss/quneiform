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
#include <setjmp.h>

# include "resource.h"
# include "dpuma.h"
# include "rblock.h"
# include "newfunc.h"
# include "kernel.h"

#include "compat_defs.h"


//#include "status.h"
//#include "newfunc.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t            gwHeightRC = 0;
static uint32_t            gwRC = 0;
static HANDLE            ghStorage = NULL;
static HINSTANCE         ghInst =  NULL;
/////////////////////////////////////////
extern "C" {
extern jmp_buf fatal_error_exit;	// For error handling
extern unsigned short int run_options;
}

/////////////////////////////////////////
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
RBLOCK_FUNC(Bool32) RBLOCK_Init(uint16_t wHeightCode,HANDLE hStorage)
{
	gwHeightRC = wHeightCode;
	LDPUMA_Init(0,NULL);
	InitDebug();

	//Open_Res_Log();

 return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RBLOCK_FUNC(Bool32) RBLOCK_Done()
{

	Close_Res_Log();

	DoneDebug();
	LDPUMA_Done();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RBLOCK_FUNC(uint32_t) RBLOCK_GetReturnCode()
{
	return gwRC;
}
//////////////////////////////////////////////////////////////////////////////////
//
RBLOCK_FUNC(char *) RBLOCK_GetReturnString(uint32_t dwError)
{
	static char szBuffer[512];
	uint16_t low = (uint16_t)(dwError &  0xFFFF);
	uint16_t hei = (uint16_t)(dwError >> 16);

	if(hei == gwHeightRC)
	{
		if(!LoadString(ghInst,low + IDS_ERR_NO,(char *)szBuffer,sizeof(szBuffer)))
			LoadString(ghInst,IDS_ERR_NOTIMPLEMENT,(char *)szBuffer,sizeof(szBuffer));
	}
	else
	{
		LoadString(ghInst,IDS_ERR_NOTIMPLEMENT,(char *)szBuffer,sizeof(szBuffer));
	}

	return (char*)szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//
RBLOCK_FUNC(Bool32) RBLOCK_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

#define CASE_FUNCTION(a)	case RBLOCK_FN##a:	*(FN##a *)pData = a; break

	switch(dwType)
	{

	CASE_FUNCTION(RBLOCK_ExtractTextBlocks);
	CASE_FUNCTION(RBLOCK_ExtractTextStrings);
	CASE_FUNCTION(RBLOCK_GetAnglePage);
	case RBLOCK_Bool32_OneColumn:
		*(Bool32*)pData = run_options & FORCE_ONE_COLUMN ? TRUE: FALSE;
		break;
	default:
		*(Handle *)pData = NULL;
		SetReturnCode_rblock(IDS_ERR_NOTIMPLEMENT);
		rc = FALSE;
	}
return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
RBLOCK_FUNC(Bool32) RBLOCK_SetImportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwRC = 0;

#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)	case a: c = (b)pData; break

	switch(dwType)
	{
	CASE_PDATA(RBLOCK_FNRBLOCK_ProgressStart,	FNRBLOCK_ProgressStart ,fnProgressStart_rblock);
	CASE_PDATA(RBLOCK_FNRBLOCK_ProgressStep,	FNRBLOCK_ProgressStep,	fnProgressStep_rblock);
	CASE_PDATA(RBLOCK_FNRBLOCK_ProgressFinish,	FNRBLOCK_ProgressFinish,fnProgressFinish_rblock);
    CASE_DATA(RBLOCK_Bool32_SearchPicture,Bool32,bSearchPicture);
	case RBLOCK_Bool32_OneColumn:

		if(*(Bool32*)pData)
			run_options |= FORCE_ONE_COLUMN ;
		else
			run_options &= ~FORCE_ONE_COLUMN ;

		break;
	 default:
		SetReturnCode_rblock(IDS_ERR_NOTIMPLEMENT);
		rc = FALSE;
	}

#undef CASE_DATA
#undef CASE_PDATA

return rc;
}

void SetReturnCode_rblock(uint32_t rc)
{
uint16_t low = (uint16_t)(rc &  0xFFFF);
uint16_t hei = (uint16_t)(rc >> 16);

	if(hei)
		gwRC = rc;
	else
	{
		if(low - IDS_ERR_NO)
			gwRC = (uint32_t)(gwHeightRC<<16)|(low - IDS_ERR_NO);
		else
			gwRC = 0;
	}
}

uint32_t GetReturnCode_rblock()
{
uint32_t rc = gwRC;
uint16_t low = (uint16_t)(gwRC &  0xFFFF);
uint16_t hei = (uint16_t)(gwRC >> 16);

	if(hei == gwHeightRC || hei == 0)
		rc = low + IDS_ERR_NO;

return rc;
}
//end of file
