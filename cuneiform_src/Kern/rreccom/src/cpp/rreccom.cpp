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
#include <string.h>
#include "stdafx.h"
#include "rreccom.h"
#include "resource.h"
#include "compat_defs.h"
/*********************************************************************************************/
static uint16_t gwHeightRC = 0;
uint16_t gwLowRC_rrec = RRECCOM_ERR_NO; /* Not static since it is accessed in recog.cpp. */
uchar*  lnOcrPath = NULL;
/*********************************************************************************************/
/*********************************************************************************************/
Bool APIENTRY DllMain( HANDLE hModule,
uint32_t  ul_reason_for_call,
                       pvoid lpReserved
					 )
{
    return TRUE;
}
/*********************************************************************************************/
// JussiP: This function is never called anywhere.
#if 0
RRECCOM_FUNC(Bool32) RRECCOM_GetExportData(uint32_t dwType, void * pData)
{
#define CASE_DATA(a,b,c)        case a: *(b *)pData = c; break

	gwLowRC_rrec = RRECCOM_ERR_NO;

	switch(dwType)
    {
		CASE_DATA(RRECCOM_FNRECOG, Bool32, (uint32_t)RRECCOM_Recog);
		CASE_DATA(RRECCOM_FNREX_ISLANGUAGE, Bool32, (uint32_t)RRECCOM_IsLanguage);
//		CASE_DATA(RRECCOM_FNRECOGCOMP, Bool32, (uint32_t)RRECCOM_RecogComp);

/*        CASE_DATA(RRECCOM_Word8_Matrix                     ,uchar,matrix);
        CASE_DATA(RRECCOM_Word8_Fax1x2                     ,uchar,fax1x2);
        CASE_DATA(RRECCOM_Word16_ActualResolution  ,uint16_t,actual_resolution);
        case RRECCOM_FNEVNPROPERT:
            *(uint32_t*)pData =          (uint32_t)RRECCOM_SetEVNProperties;
            break;
        case RRECCOM_FNEXTRACOMP :
            *(uint32_t*)pData =          (uint32_t)RRECCOMExtracomp;
            break;
        case RRECCOM_FNEXTRA :
            *(uint32_t*)pData =          (uint32_t)RRECCOMExtra;
            break;
        case RRECCOM_FNEXTRADIB:
            *(uint32_t*)pData =          (uint32_t)RRECCOMExtraDIB;
            break;

        case    RRECCOM_FNGETCONTAINER:
            *(uint32_t*)pData =          (uint32_t)RRECCOMGetContainer;
            break;

        case    RRECCOM_FNGETLENEREP:
            *(uint32_t*)pData =          (uint32_t)RRECCOMMakeLP;
            break;
        case    RRECCOM_FNVERSION:
            *(uint32_t*)pData =          (uint32_t)RRECCOM_VERSION_CODE;
            break;
        case    RRECCOM_FNREX_ISLANGUAGE:
            *(uint32_t*)pData =          (uint32_t)RRECCOM_IsLanguage;
            break;
        case    RRECCOM_FNGETORIENT:
            *(uint32_t*)pData =          (uint32_t)RRECCOM_GetOrient;
            break;
        case    RRECCOM_FNMN2CCOM:
            *(uint32_t*)pData =          (uint32_t)RRECCOM_MN2CCOM;
            break;
        case    RRECCOM_FNGETINVERTION:
            *(uint32_t*)pData =          (uint32_t)RRECCOM_GetInvertion;
            break;
        case    RRECCOM_FNEVNALPHABET:     // установка алфавита для евент
            *(uint32_t*)pData =          (uint32_t)RRECCOM_SetEVNAlphabet;
            break;
        case    RRECCOM_FNEXTRACOMP3CB:    // 3 коллбэка
            *(uint32_t*)pData =          (uint32_t)RRECCOMExtracomp3CB;
            break;
        case    RRECCOM_FNEXTGETRECRASTERORIENT: // ориентация рабочего растра
            *(uint32_t*)pData =          (uint32_t)exc_get_rec_raster_orient;
            break;
        case    RRECCOM_FNSETALLALPHAGRA: // алфавит нейронной сети
            *(uint32_t*)pData =          (uint32_t)exc_set_all_alphabet_gra;
            break;*/
		default:
                gwLowRC_rrec = RRECCOM_ERR_NOTIMPLEMENT;
                return FALSE;
	}

#undef CASE_DATA

	return TRUE;
}
#endif
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_SetImportData(uint32_t dwType, const void * pData)
{
//#define CASE_DATA(a,b,c)        case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)       case a: c = (b)pData;    break

	gwLowRC_rrec = RRECCOM_ERR_NO;

	switch(dwType)
    {
/*        CASE_DATA(RRECCOM_Word8_Matrix             ,uchar,matrix);
        CASE_DATA(RRECCOM_Word8_Fax1x2             ,uchar,fax1x2);
    CASE_DATA(RRECCOM_Word16_ActualResolution,uint16_t,actual_resolution);
    CASE_PDATA(RRECCOM_ProgressStart,      FNRRECCOM_ProgressStart, fnProgressStart);
        CASE_PDATA(RRECCOM_ProgressStep,   FNRRECCOM_ProgressStep,  fnProgressStep);
        CASE_PDATA(RRECCOM_ProgressFinish, FNRRECCOM_ProgressFinish,fnProgressFinish);
*/
    CASE_PDATA(RRECCOM_OcrPath,    uchar*, lnOcrPath);
		default:
				gwLowRC_rrec = RRECCOM_ERR_NOTIMPLEMENT;
				return FALSE;
    }

//#undef CASE_DATA
#undef CASE_PDATA

	return TRUE;

}
/*********************************************************************************************/
RRECCOM_FUNC(uint32_t)   RRECCOM_GetReturnCode(void)
{
	if(gwLowRC_rrec == RRECCOM_ERR_NO) return 0;

	return (gwHeightRC<<16)|(gwLowRC_rrec-RRECCOM_ERR_MIN);
}
/*********************************************************************************************/
RRECCOM_FUNC(char*)   RRECCOM_GetReturnString(uint32_t dwError)
{
	uint16_t rc = (uint16_t)((dwError & 0xFFFF) );
    static char szBuffer[512];

    if (dwError >> 16 != gwHeightRC) gwLowRC_rrec = RRECCOM_ERR_NOTIMPLEMENT;

    if (rc > 0 && rc <= RRECCOM_ERR_MAX-RRECCOM_ERR_MIN)
		strcpy((char *)szBuffer, RRECCOM_error_name [rc]);
	else return NULL;

	return szBuffer;
}
/*********************************************************************************************/
RRECCOM_FUNC(Bool32) RRECCOM_Init(uint16_t wHeightCode, Handle hStorage)
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
