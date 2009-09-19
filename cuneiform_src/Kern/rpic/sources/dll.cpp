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
//
// ============================================================================

/*# include <windows.h>*/
#include "compat_defs.h"

# include "mainpic.h"

///////////////////////////////////GLOBAL VARIABLES///////////////////////////////
static uint16_t            gwHeightRC = 0;
static uint16_t            gwLowRC = 0;
static HANDLE            ghStorage = NULL;
static HINSTANCE         ghInst =  NULL;
//////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(Bool32) RPIC_Init(uint16_t wHeightCode,HANDLE hStorage)
{
	gwHeightRC = wHeightCode;
	LDPUMA_Init(0,NULL);
	InitDebug();

	ClearLogRes();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(Bool32) RPIC_Done()
{

	CloseLogRes();
	LDPUMA_Done();
	DoneDebug();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(uint32_t) RPIC_GetReturnCode()
{
	uint32_t rc = 0;
	if(gwLowRC && (gwLowRC - IDS_ERR_NO)> 0 )
		rc = (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_ERR_NO);
	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(char *) RPIC_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF) + IDS_ERR_NO;
	static char szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
		gwLowRC = IDS_ERR_NOTIMPLEMENT;

	if( rc >= IDS_ERR_NO )
		LoadString(ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
		return NULL;

	return szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(Bool32) RPIC_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;

#define CASE_FUNCTION(a)	case RPIC_FN##a:	*(FN##a *)pData = a; break
#define CASE_DATA(a,b,c)	case a: *(b *)pData = c; break

	switch(dwType)
	{
	CASE_FUNCTION(RPIC_SearchPictures);
	/* reserv
	CASE_FUNCTION();
	CASE_FUNCTION();
	CASE_FUNCTION();
	*/
	 default:
		*(char **)pData = NULL;
		gwLowRC = IDS_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

#undef CASE_DATA
#undef CASE_FUNCTION
return rc;
}
//////////////////////////////////////////////////////////////////////////////////
RPIC_FUNC(Bool32) RPIC_SetImportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;

#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break
///	switch(dwType)
///	{
//	CASE_DATA(RLINE_Bool32_NOFILLGAP3,Bool32,gbNOFILLGAP3);
///	default:
		rc = FALSE;
		gwLowRC = IDS_ERR_NOTIMPLEMENT;
///	}
#undef CASE_DATA

return rc;
}
//////////////////////////////////////////////////////////////////////////////////

void SetReturnCode_rpic(uint32_t rc)
{
uint16_t low = (uint16_t)(rc &  0xFFFF);
uint16_t hei = (uint16_t)(rc >> 16);

	if(hei)
		gwLowRC = (uint16_t)rc;
	else
	{
		if(low - IDS_ERR_NO)
			gwLowRC = (uint16_t)((uint32_t)(gwHeightRC<<16)|(low - IDS_ERR_NO));
		else
			gwLowRC = 0;
	}
}

uint32_t GetReturnCode_rpic()
{
uint32_t rc = gwLowRC;
uint16_t low = (uint16_t)(gwLowRC &  0xFFFF);
uint16_t hei = (uint16_t)(gwLowRC >> 16);

	if(hei == gwHeightRC || hei == 0)
		rc = low + IDS_ERR_NO;

return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
//////////////////////////////////////////////////////////////////////////////////
