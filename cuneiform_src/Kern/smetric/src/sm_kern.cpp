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

/**********  Title  **********/
/*  Support    :  Alexander Mikhailov                                        */
/*  Last Edit  :  04.02.01                                                   */
/*  Source     :  'FT_Resid.CPP'                                             */
/*  Means      :  Common Functions of "SMetric"                              */
/*  Aim        :  Standard Dialogue with Other Libraries                     */
/*---------------------------------------------------------------------------*/
#define __SMetric__

#include <stdio.h>
#include <string.h>
/*#include <windows.h>*/
/*  interface our  */
#include "smetric.h"
/*  interface my   */
#include "un_err.h"
#include "skewtrip.h"
#include "compat_defs.h"
/*------------own objects----------------------------------------------------*/
static uint16_t      gwHeightRC = 0;      // Уникальный номер библиотеки в одном сеансе
static uint16_t      gwLowRC    = ER_ROUGH_NONE;      // Ошибки в работе библиотеки
static HANDLE      ghStorage  = NULL;   // Указатель на хранилище
static HANDLE      ghInst     = NULL;   // Указатель на свое окно
static char szBuffer[512];               // Для докладов наверх
/*------------own functions--------------------------------------------------*/
void   SetReturnCode_smetric (uint16_t rc);
uint16_t GetReturnCode_smetric ();
Bool WasInitSMetric ();
/*------------extern functions------------------------------------------------*/
Bool AM_InitComm (uint16_t wRltOwnCode);
void AM_DoneComm ();
/*---------------------------------------------------------------------------*/
Bool APIENTRY DllMain( HANDLE hModule,
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
/*---------------------------------------------------------------------------*/
SMetric_FUNC(Bool32) SMetric_Init (uint16_t wHeightCode, HANDLE hStorage)
{
	uchar err8;
	Bool ret;
	if (gwHeightRC!=0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_YET_INIT;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	if (wHeightCode==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_BAD_UNICAL_NUMBER;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	ret = AM_InitComm (wHeightCode);
	if (!ret)
	{
		err8 = (uchar)ER_ROUGH_NORMAL;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_FUNC_DPUMA;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	gwHeightRC = wHeightCode;
	gwLowRC = ER_ROUGH_NONE;
	ghStorage  = hStorage;
	return TRUE;
}
/*---------------------------------------------------------------------------*/
SMetric_FUNC(Bool32) SMetric_Done()
{
	uchar err8;
	if (gwHeightRC==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	gwHeightRC = 0;
	gwLowRC = ER_ROUGH_NONE;
	ghStorage  = NULL;
	AM_DoneComm ();
	return TRUE;
}
/*---------------------------------------------------------------------------*/
SMetric_FUNC(uint32_t) SMetric_GetReturnCode()
{
	uchar  err8;
	uint32_t err32;
	if (gwHeightRC==0)
	{
		err32 = (uint32_t)(0);
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC |= (uint16_t)err8;
		err32 |= (uint32_t)gwLowRC;
		return err32;
	}
	if (gwLowRC==ER_ROUGH_NONE)
		return (uint32_t)(0);
	return (uint32_t)(gwHeightRC<<16)|(gwLowRC);
}
/*---------------------------------------------------------------------------*/
SMetric_FUNC(char *) SMetric_GetReturnString(uint32_t dwError)
{
	uchar  err8, err8_1;
	uint16_t err16;
	char  Work[256];
	if (gwHeightRC==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC |= (uint16_t)err8;
		return NULL;
	}
	if (dwError >> 16 != gwHeightRC)
	{
		err8 = (uchar)ER_ROUGH_OTHER_LIBRARY;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_NO_COMMENT;
		gwLowRC |= (uint16_t)err8;
		return NULL;
	}
	err16  = (uint16_t)(dwError & 0xFFFF);
	err8   = (uchar)((err16/256) & 255);
	err8_1 = (uchar)(err16 & 255);
	LoadString ((HINSTANCE)ghInst, err16, (char *)szBuffer, sizeof (szBuffer));
	switch (err8)
	{
		case ER_ROUGH_NONE :
			sprintf ((char *)szBuffer, "SMetric : Ошибок нет.");
			break;
		case ER_ROUGH_OTHER_LIBRARY :
			sprintf ((char *)szBuffer, "SMetric : Ошибка другой библиотеки.");
			break;
		case ER_ROUGH_NOT_SUCH_ERROR_CODE :
			sprintf ((char *)szBuffer, "SMetric : Нет такого кода ошибки.");
			break;
		case ER_ROUGH_CALL_REFUSED :
			sprintf ((char *)szBuffer, "SMetric : Игнорирование вызова.");
			break;
		case ER_ROUGH_NORMAL :
			sprintf ((char *)szBuffer, "SMetric : Ошибка.");
			break;
		default :
			err8 = (uchar)ER_ROUGH_NOT_SUCH_ERROR_CODE;
			gwLowRC = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_NO_COMMENT;
			gwLowRC |= (uint16_t)err8;
			return NULL;
	}
	switch (err8)
	{
		case ER_ROUGH_CALL_REFUSED :
		case ER_ROUGH_NORMAL :
			Work[0] = '\0';
			switch (err8_1)
			{
				case ER_DETAIL_WAS_YET_INIT :
					sprintf (Work, " Инициализация уже была.");
					break;
				case ER_DETAIL_WAS_NOT_INIT :
					sprintf (Work, " Инициализации еще не было.");
					break;
				case ER_DETAIL_BAD_UNICAL_NUMBER :
					sprintf (Work, " Плохой уникальный номер.");
					break;
				case ER_DETAIL_TOO_MUCH_CALLS :
					sprintf (Work, " Слишком много вызовов.");
					break;
				case ER_DETAIL_NOT_MADE_SUCH_DATA :
					sprintf (Work, " Не изготовляю такие данные.");
					break;
				case ER_DETAIL_EMPTY_FUNC :
					sprintf (Work, " Содержательная часть функции отсутствует.");
					break;
				case ER_DETAIL_MAKET_FUNC :
					sprintf (Work, " Функция-макет (выдуманные входные данные).");
					break;
				case ER_DETAIL_BAD_PARAMETRS :
					sprintf (Work, " Плохие параметры.");
					break;
				case ER_DETAIL_NO_MEMORY :
					sprintf (Work, " Нет памяти.");
					break;
				case ER_DETAIL_FUNC_CPAGE :
					sprintf (Work, " Ошибка вызванной функции из 'CPAGE'.");
					break;
				case ER_DETAIL_FUNC_DPUMA :
					sprintf (Work, " Ошибка вызванной функции из 'DPUMA'.");
					break;
				case ER_DETAIL_NONE :
				case ER_DETAIL_NO_COMMENT :
				default :
					break;
			}
			strcat ((char *)szBuffer, Work);
			break;
		default :
			break;
	}
	return szBuffer;
}
/*---------------------------------------------------------------------------*/
SMetric_FUNC(Bool32) SMetric_GetExportData(uint32_t dwType, void * pData)
{
	uchar err8;
	if (gwHeightRC==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	gwLowRC = ER_ROUGH_NONE;

#define CASE_FUNCTION(a)	case SMetric_FN##a:	*(FN##a *)pData = a; break

	switch(dwType)
	{
		CASE_FUNCTION(SMetric_FindMainSkew);
		case SMetric_ResolX:
			*((int32_t *)pData) = SMetric_GetResolX();
			break;
		case SMetric_ResolY:
			*((int32_t *)pData) = SMetric_GetResolY();
			break;
		default:
			*(Handle *)pData = NULL;
			err8 = (uchar)ER_ROUGH_CALL_REFUSED;
			gwLowRC = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_NOT_MADE_SUCH_DATA;
			gwLowRC |= (uint16_t)err8;
			return FALSE;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
SMetric_FUNC(Bool32) SMetric_SetImportData(uint32_t dwType, void *pData)
{
	uchar err8;
	if (gwHeightRC==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	switch (dwType)
	{
/**		case SMetric_DTSMetric_RegimeOfVerifyLines :
			if (pData!=NULL)
				memcpy (&MainRegime, pData, sizeof (Regime_VerifyLines));
			else
				memset (&MainRegime, 0, sizeof (Regime_VerifyLines));
			break;**/
		case SMetric_ResolX:
			SMetric_SetResolX(reinterpret_cast<intptr_t>(pData));
			break;
		case SMetric_ResolY:
			SMetric_SetResolY(reinterpret_cast<intptr_t>(pData));
			break;
		case 1 :
		default :
			err8 = (uchar)ER_ROUGH_CALL_REFUSED;
			gwLowRC = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_BAD_PARAMETRS;
			gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
void SetReturnCode_smetric(uint16_t rc)
{
	gwLowRC = rc;
}
/*---------------------------------------------------------------------------*/
uint16_t GetReturnCode_smetric()
{
	return gwLowRC;
}
/*---------------------------------------------------------------------------*/
Bool WasInitSMetric ()
{
	uchar err8;
	if (gwHeightRC==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC |= (uint16_t)err8;
		return FALSE;
	}
	return TRUE;
}
/*---------------------------------------------------------------------------*/
