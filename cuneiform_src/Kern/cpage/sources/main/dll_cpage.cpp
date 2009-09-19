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
#include "cpage.h"
#include "backup.h"
#include "dpuma.h"
#include <time.h>
//
// Виртуальные функции
//
FNCPAGE_HL_TableExtract CPAGE_HL_TableExtract = NULL;

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t gwHeightRC = 0;
static uint16_t gwLowRC = 0;
static HANDLE ghStorage = NULL;
static HINSTANCE ghInst = NULL;

#ifdef DPUMA_ON
Handle hSnapTimerBeg = NULL;
Handle hSnapTimerEnd = NULL;
#endif

/////////////////////////////////////////
Bool APIENTRY DllMain(HINSTANCE hModule, uint32_t ul_reason_for_call,
		pvoid lpReserved) {
	switch (ul_reason_for_call) {
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
CPAGE_FUNC(Bool32) CPAGE_Init(uint16_t wHeightCode,HANDLE hStorage)
{
	gwHeightRC = wHeightCode;
#ifdef DPUMA_ON
	Handle hSnap = NULL;
	LDPUMA_Init(0,NULL);
	LDPUMA_Registry(&hSnap,"Контейнер CPAGE", NULL);
	LDPUMA_RegistryHelp(hSnap,
			"<Ответственный:П.Хлебутин> Эта вершина предназначена для тестирования работы с контейнером CPAGE."
			,FALSE);
	LDPUMA_Registry(&hSnapTimerBeg,"Время работы CPAGE", hSnap);
	LDPUMA_RegistryHelp(hSnapTimerBeg,
			"Эта вершина предназначена для определения времени работы контейнера."
			,FALSE);
	/*
	 LDPUMA_Registry(&hSnapTimerEnd,"Время работы остальных модулей.", hSnap);
	 LDPUMA_RegistryHelp(hSnapTimerEnd,
	 "Эта вершина предназначена для совместной работы с вершиной 'Время работы остальных модулей'."
	 ,FALSE);
	 */
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
CPAGE_FUNC(Bool32) CPAGE_Done()
{
	Bool32 rc = CPAGE_DeleteAll();
#ifdef DPUMA_ON
	LDPUMA_Done();
#endif
	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
CPAGE_FUNC(uint32_t) CPAGE_GetReturnCode()
{
	uint32_t rc = 0;
	if((gwLowRC - IDS_ERR_NO)>0)
	rc = (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_ERR_NO);

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
CPAGE_FUNC(char *) CPAGE_GetReturnString(uint32_t dwError)
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
CPAGE_FUNC(Bool32) CPAGE_GetExportData(uint32_t dwType, void * pData)
{
	Bool32 rc = TRUE;

	gwLowRC = 0;

#define CASE_FUNCTION(a)	case CPAGE_FN##a:	*(FN##a *)pData = a; break

	switch(dwType)
	{
		CASE_FUNCTION(CPAGE_CreatePage);
		CASE_FUNCTION(CPAGE_DeletePage);
		CASE_FUNCTION(CPAGE_SavePage);
		CASE_FUNCTION(CPAGE_RestorePage);
		CASE_FUNCTION(CPAGE_GetPageType);
		CASE_FUNCTION(CPAGE_SetPageData);
		CASE_FUNCTION(CPAGE_GetPageData);
		CASE_FUNCTION(CPAGE_ClearBackUp);
		CASE_FUNCTION(CPAGE_BackUp);
		CASE_FUNCTION(CPAGE_Undo);
		CASE_FUNCTION(CPAGE_Redo);
		CASE_FUNCTION(CPAGE_GetCountPage);
		CASE_FUNCTION(CPAGE_GetCountBlock);
		CASE_FUNCTION(CPAGE_CreateBlock);
		CASE_FUNCTION(CPAGE_GetBlockType);
		CASE_FUNCTION(CPAGE_GetBlockUserNum);
		CASE_FUNCTION(CPAGE_SetBlockUserNum);
		CASE_FUNCTION(CPAGE_GetBlockFlags);
		CASE_FUNCTION(CPAGE_SetBlockFlags);
		CASE_FUNCTION(CPAGE_SetBlockData);
		CASE_FUNCTION(CPAGE_GetBlockData);
		CASE_FUNCTION(CPAGE_DeleteBlock);
		CASE_FUNCTION(CPAGE_GetHandlePage);
		CASE_FUNCTION(CPAGE_GetHandleBlock);
		CASE_FUNCTION(CPAGE_SetConvertorPages);
		CASE_FUNCTION(CPAGE_SetConvertorBlocks);
		CASE_FUNCTION(CPAGE_GetUserPageType);
		CASE_FUNCTION(CPAGE_GetUserBlockType);
		CASE_FUNCTION(CPAGE_GetBuckUpCount);
		CASE_FUNCTION(CPAGE_GetBuckUpHandle);
		CASE_FUNCTION(CPAGE_GetPageFirst);
		CASE_FUNCTION(CPAGE_GetPageNext);
		CASE_FUNCTION(CPAGE_GetBlockFirst);
		CASE_FUNCTION(CPAGE_GetBlockNext);
		CASE_FUNCTION(CPAGE_DeleteAll);
		CASE_FUNCTION(CPAGE_GetCurrentPage);
		CASE_FUNCTION(CPAGE_SetCurrentPage);
		CASE_FUNCTION(CPAGE_GetNumberPage);
		CASE_FUNCTION(CPAGE_UpdateBlocks);
		CASE_FUNCTION(CPAGE_HL_TableExtract);
		CASE_FUNCTION(CPAGE_PictureGetFirst);
		CASE_FUNCTION(CPAGE_PictureGetNext);
		CASE_FUNCTION(CPAGE_PictureGetPlace);
		CASE_FUNCTION(CPAGE_PictureGetMask);
		CASE_FUNCTION(CPAGE_GetBlockInterNum);
		CASE_FUNCTION(CPAGE_SetBlockInterNum);
		CASE_FUNCTION(CPAGE_GetBlockDataPtr);
		CASE_FUNCTION(CPAGE_GetInternalType);
		CASE_FUNCTION(CPAGE_GetNameInternalType);

		/* reserv
		 CASE_FUNCTION();
		 CASE_FUNCTION();
		 CASE_FUNCTION();
		 */
		default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}
#undef CASE_FUNCTION
	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
CPAGE_FUNC(Bool32) CPAGE_SetImportData(uint32_t dwType, void * pData)
{
	Bool rc = FALSE;
	gwLowRC = IDS_ERR_NOTIMPLEMENT;

#define CASE_FUNCTION(a)	case CPAGE_FN##a:	a = (FN##a)pData; break

	switch(dwType)
	{
		CASE_FUNCTION(CPAGE_HL_TableExtract);
		default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}
#undef CASE_FUNCTION

	return rc;
}

void SetReturnCode_cpage(uint16_t rc) {
	gwLowRC = rc;
}

uint16_t GetReturnCode_cpage() {
	return gwLowRC;
}

#ifdef DPUMA_ON

//#define TIMECONTROL 1

static int s_prolog = 0;
#ifdef TIMECONTROL
static clock_t s_tbeg = 0;
#endif

Handle ProfileProlog()
{
	Handle rc = NULL;
	if(!s_prolog)
	{
#ifdef TIMECONTROL
		s_tbeg = clock();
#endif
		rc = LDPUMA_GetPrevSkipOwner();
		LDPUMA_Skip(hSnapTimerBeg);
		// Проверим - используется ли эта вершина на самом деле.
		if(LDPUMA_GetPrevSkipOwner() == rc)
		rc = NULL;// Нет. Иначе, она равнялась бы hSnapTimerBeg
	}
	s_prolog++;
	return rc;
}

void ProfileEpilog(Handle prev)
{
	if(s_prolog>0)
	{
		s_prolog--;
		if(!s_prolog && prev)
		{
#ifdef TIMECONTROL
			uint32_t c = clock() - s_tbeg;
			_ASSERT(c < 99);
#endif
			LDPUMA_Skip(prev);
		}
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////////
//end of file
