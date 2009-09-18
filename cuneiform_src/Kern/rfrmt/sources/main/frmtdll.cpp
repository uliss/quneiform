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
// frmtdll.cpp :
// ============================================================================

/*#include <windows.h>*/
#include "resource.h"
#define __RFRMT_EXTERN__
#include "rfrmt.h"
#include "dpuma.h"
#include "pumadef.h"
#include "rfrmt_prot.h"
#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t     gwHeightRC = 0;
static uint16_t     gwLowRC    = 0;
static HANDLE     ghStorage  = NULL;
static HINSTANCE  ghInst     = NULL;
Bool32     gbBold     = TRUE;
Bool32     gbItalic   = TRUE;
Bool32     gbSize     = TRUE;
uint32_t     gnFormat	  = 1;    // 0 - не форматировать
							  // 1 - форматировать с фреймами и  колонками
							  // 2 - только фреймы
const char * gpSerifName	  = "MS Serif Cyr";
const char * gpSansSerifName= "MS Sans Serif Cyr";
const char * gpCourierName  = "Courier Cyr";
char   UnRecogSymbol  = '~';
uint32_t gnLanguage = LANG_RUSENG;

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

///////////////////////////////////////////////////////////////
//Handle hUseCLine;

RFRMT_FUNC(Bool32) RFRMT_Init(uint16_t wHeightCode,HANDLE hStorage)
{
 LDPUMA_Init(0,NULL);
 LDPUMA_Registry(&hDebugRoot,SNAP_ROOT_CONVERTERS,NULL);
 LDPUMA_Registry(&hDebugMy,"Отладка форматирования",hDebugRoot);

 LDPUMA_Registry(&hDebugKegl,"Кегль не изменять",hDebugMy);
 LDPUMA_RegistryHelp(hDebugKegl,"Эта опция предназначена для корректировки размера кегля при форматировании",FALSE);

 LDPUMA_Registry(&hDebugFrame,"Все фрагменты фреймы",hDebugMy);
 LDPUMA_RegistryHelp(hDebugFrame,"Эта опция предназначена для форматирования фреймами",FALSE);

 LDPUMA_Registry(&hDebugAlign,"Отладка выравнивания параграфа",hDebugMy);
 LDPUMA_RegistryHelp(hDebugAlign,"Отладка выравнивания параграфа",FALSE);

 LDPUMA_Registry(&hDebugLineTransfer,"Строки не переносить",hDebugMy);
 LDPUMA_RegistryHelp(hDebugLineTransfer,"Строки не переносить",FALSE);

 LDPUMA_Registry(&hDebugProfile,"Оценка времени работы.",hDebugMy);
 LDPUMA_RegistryHelp(hDebugProfile,"Оценка времени работы.",FALSE);
 LDPUMA_Registry(&hDebugProfStart,"Начало работы",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfStart,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfTable,"Таблица",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfTable,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfCell,"Ячейки",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfCell,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfTmp,"Tmp",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfTmp,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfEndTable,"Конец таблицы",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfEndTable,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfWrite,"Запись таблицы",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfWrite,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfWriteRow,"Запись строки таблицы",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfWriteRow,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfWriteCell,"Запись ячейки таблицы",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfWriteCell,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfWriteChar,"Запись символа",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfWriteChar,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hDebugProfEnd,"Конец работы",hDebugProfile);
 LDPUMA_RegistryHelp(hDebugProfEnd,"Оценка времени работы.",FALSE);

 LDPUMA_Registry(&hTest,"Тест памяти записи картинок",hDebugMy);
 LDPUMA_Registry(&hTestDIBData,"Получить исходную картинку",hTest);

 LDPUMA_Registry(&hTestTurn,"Поворота на 90\180",hTest);
 LDPUMA_Registry(&hTestRotate,"Доворота на малый угол",hTest);

 LDPUMA_Registry(&hTestWriteMetafile,"Запись в метафайл",hTest);
 LDPUMA_Registry(&hTestGetMaskDIB,"Получения маскированного DIB",hTest);

 LDPUMA_Registry(&hTestWriteED,"Запись в ED",hTest);
 LDPUMA_Registry(&hTestEnd,"В конце функции",hTest);
 LDPUMA_Registry(&hTestDeleteImage,"Удаление временных изображений",hTest);

// LDPUMA_Registry (&hUseCLine,"Работа с контейнером линий", NULL);


 gwHeightRC = wHeightCode;

 return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RFRMT_FUNC(Bool32) RFRMT_Done()
{
 LDPUMA_Done();
 return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RFRMT_FUNC(uint32_t) RFRMT_GetReturnCode()
{
 uint32_t rc = 0;

 if((gwLowRC - IDS_ERR_NO)>0)
	rc = (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_ERR_NO);

 return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
RFRMT_FUNC(char *) RFRMT_GetReturnString(uint32_t dwError)
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
RFRMT_FUNC(Bool32) RFRMT_GetExportData(uint32_t dwType, void * pData)
{
 Bool32 rc = TRUE;

 gwLowRC = 0;

#define CASE_FUNCTION(a)	case RFRMT_FN##a:	*(FN##a *)pData = a; break
#define CASE_DATA(a,b,c)	case a: *(b *)pData = c; break

 switch(dwType)
 {
    CASE_FUNCTION(RFRMT_Formatter);
	CASE_FUNCTION(RFRMT_SaveRtf);
	CASE_DATA(RFRMT_Bool32_Bold,Bool32,gbBold);
	CASE_DATA(RFRMT_Bool32_Italic,Bool32,gbItalic);
	CASE_DATA(RFRMT_Bool32_Size,Bool32,gbSize);
	CASE_DATA(RFRMT_Word8_UnRecogSymbol,uchar,UnRecogSymbol);

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
RFRMT_FUNC(Bool32) RFRMT_SetImportData(uint32_t dwType, const void * pData)
{
 Bool32 rc = TRUE;

 gwLowRC = 0;

#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)	case a: c = (b)pData; break
 switch(dwType)
 {
	CASE_DATA(RFRMT_Bool32_Bold,Bool32,gbBold);
	CASE_DATA(RFRMT_Bool32_Italic,Bool32,gbItalic);
	CASE_DATA(RFRMT_Bool32_Size,Bool32,gbSize);
	CASE_DATA(RFRMT_Word32_Format,uint32_t,gnFormat);
	CASE_PDATA(RFRMT_char_SerifName,const char *,gpSerifName);
	CASE_PDATA(RFRMT_char_SansSerifName,const char *,gpSansSerifName);
	CASE_PDATA(RFRMT_char_CourierName,const char *,gpCourierName);
	CASE_DATA(RFRMT_Word8_UnRecogSymbol,uchar,UnRecogSymbol);
	CASE_DATA(RFRMT_Word32_Language,uint32_t,gnLanguage);// !!!Art - язык распознавания понадобился для умолчания в редактор
	 default:
		gwLowRC = IDS_ERR_NOTIMPLEMENT;
		rc = FALSE;
 }
#undef CASE_DATA
#undef CASE_PDATA

return rc;
}

void SetReturnCode_rfrmt(uint16_t rc)
{
 gwLowRC = rc;
}

uint16_t GetReturnCode_rfrmt()
{
 return gwLowRC;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
