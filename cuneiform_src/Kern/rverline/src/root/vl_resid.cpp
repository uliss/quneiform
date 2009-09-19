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

/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  06.09.00                                                    */
/*  Файл       :  'VL_Resid.CPP'                                              */
/*  Содержание :  Общие функции библиотеки "RVERLINE"                         */
/*  Назначение :  Стандартный диалог с другими библиотеками                   */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
/*#include <windows.h>*/
/*  interface our  */
#include "rverline.h"
/*  interface my   */
#include "am_err.h"
#include "am_comm.h"
#include "vl_rule.h"
#include "compat_defs.h"
/*------------own objects-----------------------------------------------------*/
/* These two would be static, but they are accessed in vl_kern.cpp. */
uint16_t      gwHeightRC_rver = 0;      // Уникальный номер библиотеки в одном сеансе
uint16_t      gwLowRC_rver    = ER_ROUGH_NONE;      // Ошибки в работе библиотеки
static HANDLE      ghStorage  = NULL;   // Указатель на хранилище
static HANDLE      ghInst     = NULL;   // Указатель на свое окно
static char szBuffer[512];               // Для докладов наверх
Handle RltVertix_VL; // корневая вершина отладки для верификации линий
/*------------extern functions------------------------------------------------*/
Bool MyInit_CPage ();
/*------------own functions---------------------------------------------------*/
void   SetReturnCode_rverline (uint16_t rc);
uint16_t GetReturnCode_rverline ();
Bool WasInitRVERLINE ();
/*----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(Bool32) RVERLINE_Init (uint16_t wHeightCode, HANDLE hStorage)
{
	uchar err8;
	Bool ret;
	if (gwHeightRC_rver!=0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_YET_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	if (wHeightCode==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_BAD_UNICAL_NUMBER;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	ret = AM_InitComm (wHeightCode);
	if (!ret)
	{
		err8 = (uchar)ER_ROUGH_NORMAL;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_FUNC_DPUMA;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	/*  регистрация корневых вершин отладки  */
	AM_RegiVert (&RltVertix_VL, "Верификация линий...", NULL);
	/*  построение деревьев корневых вершин отладки  */
	AM_MakeTreeRules_VerifLines (RltVertix_VL);
	/*  отметить важнейшие переменные  */
	ret = MyInit_CPage ();
	if (!ret)
		return FALSE;
	gwHeightRC_rver = wHeightCode;
	gwLowRC_rver = ER_ROUGH_NONE;
	ghStorage  = hStorage;
	return TRUE;
}
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(Bool32) RVERLINE_Done()
{
	uchar err8;
	if (gwHeightRC_rver==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	gwHeightRC_rver = 0;
	gwLowRC_rver = ER_ROUGH_NONE;
	ghStorage  = NULL;
	CloseAllRes ();
	AM_DoneComm ();
	return TRUE;
}
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(uint32_t) RVERLINE_GetReturnCode()
{
	uchar  err8;
	uint32_t err32;
	if (gwHeightRC_rver==0)
	{
		err32 = (uint32_t)(0);
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		err32 |= (uint32_t)gwLowRC_rver;
		return err32;
	}
	if (gwLowRC_rver==ER_ROUGH_NONE)
		return (uint32_t)(0);
	return (uint32_t)(gwHeightRC_rver<<16)|(gwLowRC_rver);
}
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(char *) RVERLINE_GetReturnString(uint32_t dwError)
{
	uchar  err8, err8_1;
	uint16_t err16;
	char  Work[256];
	if (gwHeightRC_rver==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		return NULL;
	}
	if (dwError >> 16 != gwHeightRC_rver)
	{
		err8 = (uchar)ER_ROUGH_OTHER_LIBRARY;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_NO_COMMENT;
		gwLowRC_rver |= (uint16_t)err8;
		return NULL;
	}
	err16  = (uint16_t)(dwError & 0xFFFF);
	err8   = (uchar)((err16/256) & 255);
	err8_1 = (uchar)(err16 & 255);
	LoadString ((HINSTANCE)ghInst, err16, (char *)szBuffer, sizeof (szBuffer));
	switch (err8)
	{
		case ER_ROUGH_NONE :
			sprintf ((char *)szBuffer, "RVERLINE : Ошибок нет.");
			break;
		case ER_ROUGH_OTHER_LIBRARY :
			sprintf ((char *)szBuffer, "RVERLINE : Ошибка другой библиотеки.");
			break;
		case ER_ROUGH_NOT_SUCH_ERROR_CODE :
			sprintf ((char *)szBuffer, "RVERLINE : Нет такого кода ошибки.");
			break;
		case ER_ROUGH_CALL_REFUSED :
			sprintf ((char *)szBuffer, "RVERLINE : Игнорирование вызова.");
			break;
		case ER_ROUGH_NORMAL :
			sprintf ((char *)szBuffer, "RVERLINE : Ошибка.");
			break;
		default :
			err8 = (uchar)ER_ROUGH_NOT_SUCH_ERROR_CODE;
			gwLowRC_rver = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_NO_COMMENT;
			gwLowRC_rver |= (uint16_t)err8;
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
/*----------------------------------------------------------------------------*/
RVERLINE_FUNC(Bool32) RVERLINE_GetExportData(uint32_t dwType, void * pData)
{
	uchar err8;
	if (gwHeightRC_rver==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	gwLowRC_rver = ER_ROUGH_NONE;

#define CASE_FUNCTION(a)	case RVERLINE_FN##a:	*(FN##a *)pData = a; break

	switch(dwType)
	{
		CASE_FUNCTION(RVERLINE_MarkLines);
		default:
			*(Handle *)pData = NULL;
			err8 = (uchar)ER_ROUGH_CALL_REFUSED;
			gwLowRC_rver = (uint16_t)(err8<<8);
			err8 = (uchar)ER_DETAIL_NOT_MADE_SUCH_DATA;
			gwLowRC_rver |= (uint16_t)err8;
			return FALSE;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
void SetReturnCode_rverline(uint16_t rc)
{
	gwLowRC_rver = rc;
}
/*----------------------------------------------------------------------------*/
uint16_t GetReturnCode_rverline()
{
	return gwLowRC_rver;
}
/*----------------------------------------------------------------------------*/
Bool WasInitRVERLINE ()
{
	uchar err8;
	if (gwHeightRC_rver==0)
	{
		err8 = (uchar)ER_ROUGH_CALL_REFUSED;
		gwLowRC_rver = (uint16_t)(err8<<8);
		err8 = (uchar)ER_DETAIL_WAS_NOT_INIT;
		gwLowRC_rver |= (uint16_t)err8;
		return FALSE;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*/
