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

// RCUTP.cpp : Defines the entry point for the DLL application.
//
/*#include "StdAfx.h"*/
#include <stdlib.h>
#include "resource.h"
#include "rcutp.h"
#include "rcutp_prot.h"
#define __RCUTP_EXTERN__

#include "dpuma.h"
#include "dsnap.h"

#include "compat_defs.h"

//--------------------------GLOBAL VARIABLES------------------------
int16_t      minrow,bbs1,bbs2,bbs3,bbs4,Nb1,Nb2,Nb3;
uchar      language;

HINSTANCE  ghInst = NULL;
uint16_t     gwHeightRC = 0;
uint16_t     gwLowRC = 0;
Handle     ghStorage = NULL;
Handle     hDebugRoot, hDebugMy,hDebugCutP;

///////////////////////////////////////////////////////////////
RCUTP_FUNC(Bool32) RCUTP_Init(uint16_t wHeightCode,Handle hStorage)
{
	LDPUMA_Init(0,NULL);
	LDPUMA_Registry(&hDebugRoot,SNAP_ROOT_CONVERTERS,NULL);
	LDPUMA_Registry(&hDebugMy,"Отладка точек разрезания",hDebugRoot);

	LDPUMA_Registry(&hDebugCutP,"Просмотр точек разрезания",hDebugMy);
	LDPUMA_RegistryHelp(hDebugCutP,"Эта опция предназначена для вывода точек разрезания",FALSE);

	gwHeightRC = wHeightCode;

 return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Bool32) RCUTP_Done()
{
	LDPUMA_Done();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(uint32_t) RCUTP_GetReturnCode()
{
uint32_t rc = 0;
	if((gwLowRC - IDS_ERR_NO)>0)
		rc = (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_ERR_NO);

return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(char *) RCUTP_GetReturnString(uint32_t dwError)
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
//
RCUTP_FUNC(Bool32) RCUTP_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;

#define CASE_FUNCTION(a)	case RCUTP_FN##a:	*(FN##a *)pData = a; break
#define CASE_DATA(a,b,c)	case a: *(b *)pData = c; break

	switch(dwType)
 {
		CASE_FUNCTION(RCUTP_CutPoints);
		CASE_FUNCTION(RCUTP_SetBL_for_CutPoints);

  default:
        		*(Handle *)pData = NULL;
		        gwLowRC = IDS_ERR_NOTIMPLEMENT;
		        rc = FALSE;
	}

#undef CASE_DATA
#undef CASE_FUNCTION

return rc;
}

//////////////////////////////////////////////////////////////////////////////////
//
RCUTP_FUNC(Bool32) RCUTP_SetImportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;
return rc;
}

void SetReturnCode_rcutp(uint16_t rc)
{
	gwLowRC = rc;
}

uint16_t GetReturnCode_rcutp()
{
	return gwLowRC;
}
