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

/*********************************************************************************************/
#define __RRECCOM__
/*********************************************************************************************/
#include <string.h>
#include "stdafx.h"
#include "rreccom.h"
#include "resource.h"
#include "compat_defs.h"
/*********************************************************************************************/
static Word16 gwHeightRC = 0;
static Word16 gwLowRC = RRECCOM_ERR_NO;
Word8*  lnOcrPath = NULL;
/*********************************************************************************************/
/*********************************************************************************************/
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_GetExportData(Word32 dwType, void * pData)
{
#define CASE_DATA(a,b,c)        case a: *(b *)pData = c; break

	gwLowRC = RRECCOM_ERR_NO;

	switch(dwType)
    {
		CASE_DATA(RRECCOM_FNRECOG, Bool32, (Word32)RRECCOM_Recog);
		CASE_DATA(RRECCOM_FNREX_ISLANGUAGE, Bool32, (Word32)RRECCOM_IsLanguage);
//		CASE_DATA(RRECCOM_FNRECOGCOMP, Bool32, (Word32)RRECCOM_RecogComp);

/*        CASE_DATA(RRECCOM_Word8_Matrix                     ,Word8,matrix);
        CASE_DATA(RRECCOM_Word8_Fax1x2                     ,Word8,fax1x2);
        CASE_DATA(RRECCOM_Word16_ActualResolution  ,Word16,actual_resolution);
        case RRECCOM_FNEVNPROPERT:
            *(Word32*)pData =          (Word32)RRECCOM_SetEVNProperties;
            break;
        case RRECCOM_FNEXTRACOMP :
            *(Word32*)pData =          (Word32)RRECCOMExtracomp;
            break;
        case RRECCOM_FNEXTRA :
            *(Word32*)pData =          (Word32)RRECCOMExtra;
            break;
        case RRECCOM_FNEXTRADIB:
            *(Word32*)pData =          (Word32)RRECCOMExtraDIB;
            break;

        case    RRECCOM_FNGETCONTAINER:
            *(Word32*)pData =          (Word32)RRECCOMGetContainer;
            break;

        case    RRECCOM_FNGETLENEREP:
            *(Word32*)pData =          (Word32)RRECCOMMakeLP;
            break;
        case    RRECCOM_FNVERSION:
            *(Word32*)pData =          (Word32)RRECCOM_VERSION_CODE;
            break;
        case    RRECCOM_FNREX_ISLANGUAGE:
            *(Word32*)pData =          (Word32)RRECCOM_IsLanguage;
            break;
        case    RRECCOM_FNGETORIENT:
            *(Word32*)pData =          (Word32)RRECCOM_GetOrient;
            break;
        case    RRECCOM_FNMN2CCOM:
            *(Word32*)pData =          (Word32)RRECCOM_MN2CCOM;
            break;
        case    RRECCOM_FNGETINVERTION:
            *(Word32*)pData =          (Word32)RRECCOM_GetInvertion;
            break;
        case    RRECCOM_FNEVNALPHABET:     // установка алфавита для евент
            *(Word32*)pData =          (Word32)RRECCOM_SetEVNAlphabet;
            break;
        case    RRECCOM_FNEXTRACOMP3CB:    // 3 коллбэка
            *(Word32*)pData =          (Word32)RRECCOMExtracomp3CB;
            break;
        case    RRECCOM_FNEXTGETRECRASTERORIENT: // ориентация рабочего растра 
            *(Word32*)pData =          (Word32)exc_get_rec_raster_orient;
            break;
        case    RRECCOM_FNSETALLALPHAGRA: // алфавит нейронной сети
            *(Word32*)pData =          (Word32)exc_set_all_alphabet_gra;
            break;*/
		default:
                gwLowRC = RRECCOM_ERR_NOTIMPLEMENT;
                return FALSE;
	}

#undef CASE_DATA

	return TRUE;
}
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_SetImportData(Word32 dwType, void * pData)
{
//#define CASE_DATA(a,b,c)        case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)       case a: c = (b)pData;    break

	gwLowRC = RRECCOM_ERR_NO;
	
	switch(dwType)
    {
/*        CASE_DATA(RRECCOM_Word8_Matrix             ,Word8,matrix);
        CASE_DATA(RRECCOM_Word8_Fax1x2             ,Word8,fax1x2);
    CASE_DATA(RRECCOM_Word16_ActualResolution,Word16,actual_resolution);
    CASE_PDATA(RRECCOM_ProgressStart,      FNRRECCOM_ProgressStart, fnProgressStart);
        CASE_PDATA(RRECCOM_ProgressStep,   FNRRECCOM_ProgressStep,  fnProgressStep);
        CASE_PDATA(RRECCOM_ProgressFinish, FNRRECCOM_ProgressFinish,fnProgressFinish);        
*/
    CASE_PDATA(RRECCOM_OcrPath,    Word8*, lnOcrPath);     
		default:
				gwLowRC = RRECCOM_ERR_NOTIMPLEMENT;
				return FALSE;
    }

//#undef CASE_DATA
#undef CASE_PDATA

	return TRUE;

}
/*********************************************************************************************/
RRECCOM_FUNC(Word32)   RRECCOM_GetReturnCode(void)
{
	if(gwLowRC == RRECCOM_ERR_NO) return 0;

	return (gwHeightRC<<16)|(gwLowRC-RRECCOM_ERR_MIN);
}
/*********************************************************************************************/
RRECCOM_FUNC(char*)   RRECCOM_GetReturnString(Word32 dwError)
{
	Word16 rc = (Word16)((dwError & 0xFFFF) );
    static char szBuffer[512];

    if (dwError >> 16 != gwHeightRC) gwLowRC = RRECCOM_ERR_NOTIMPLEMENT;

    if (rc > 0 && rc <= RRECCOM_ERR_MAX-RRECCOM_ERR_MIN)
		strcpy((char *)szBuffer, RRECCOM_error_name [rc]);
	else return NULL;
		
	return szBuffer;
}
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_Init(Word16 wHeightCode, Handle hStorage)
{
/*
if(Q.boxstart)
    {
    //wLowRC = RRECCOM_ERR_USE;
    //return FALSE;
    ExtrcompDone();
    }
*/

	gwHeightRC = wHeightCode;

    if (!gwHeightRC) return FALSE;

/*	norm_ori_init();

NumContainer =0;
return ExtrcompInit();
*/
	return TRUE;
}
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_Done(void)
{
/*ExtrcompDone();
language=-1;
NumContainer =0;
*/
	return TRUE;
}
/*********************************************************************************************/
