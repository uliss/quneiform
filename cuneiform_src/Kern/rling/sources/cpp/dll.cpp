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
#include "rlcontrol.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// хидер для второй копии (только для первой)
#if !defined ( RLING_SECONDARY ) //& defined (RLING_MASTER)
#define RLING_SECONDARY
#include "crling.h"
#undef RLING_SECONDARY
#endif
#include "crling.h"

#include "compat_defs.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t gwHeightRC = 0;
static uint16_t gwLowRC = 0;
static Handle ghInst = NULL;
CRLControl * Control_crl = NULL;
///////////////////////////////////////////////////////////////////////////////////
void SetReturnCode_rling(uint16_t rc);
uint16_t GetReturnCode_rling();
///////////////////////////////////////////////////////////////////////////////////

// FIXME: temp hack
#ifndef _MSC_VERS
#undef APIENTRY
#define APIENTRY
#endif

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
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_Init(uint16_t wHeightCode,Handle hStorage)
#else
RLINGS_FUNC(Bool32) RLINGS_Init(uint16_t wHeightCode,Handle hStorage)
#endif
{
	gwHeightRC = wHeightCode;

	Control_crl = new CRLControl;

	if ( Control_crl )
#if defined ( __RLING__ )
	return RLINGS_Init(wHeightCode, hStorage);
#else
	return TRUE;
#endif

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32)RLING_Done()
#else
RLINGS_FUNC(Bool32)RLINGS_Done()
#endif
{
	delete Control_crl;
	Control_crl = NULL;

#if defined ( __RLING__ )
	return RLINGS_Done();
#else
	return TRUE;
#endif
}
//////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(uint32_t) RLING_GetReturnCode()
#else
RLINGS_FUNC(uint32_t) RLINGS_GetReturnCode()
#endif
{
	if ( !gwLowRC )
	return 0;

#if defined ( __RLING__ )
	if ( gwLowRC == IDS_RLING_ERR_NO )
	return RLINGS_GetReturnCode();
#endif

	return (uint32_t)(gwHeightRC<<16)|(gwLowRC - IDS_RLING_ERR_NO);
}
//////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(char *) RLING_GetReturnString(uint32_t dwError)
#else
RLINGS_FUNC(char *) RLINGS_GetReturnString(uint32_t dwError)
#endif
{
	uint16_t rc = (uint16_t)(dwError & 0xFFFF) + IDS_RLING_ERR_NO;
	static char szBuffer[512];

	if( dwError >> 16 != gwHeightRC)
	gwLowRC = IDS_RLING_ERR_NOTIMPLEMENT;

	if( rc >= IDS_RLING_ERR_NO )
	LoadString((HINSTANCE)ghInst,rc,(char *)szBuffer,sizeof(szBuffer));
	else
	return NULL;

	return szBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
#define CASE_FUNCTION(a)	case RLING_FN_##a:	*(FNRLING##a *)pData = RLING_##a; break
#else
#define CASE_FUNCTION(a)	case RLINGS_FN_##a:	*(FNRLINGS##a *)pData = RLINGS_##a; break
#endif
//////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_GetExportData(uint32_t dwType, void * pData)
#else
RLINGS_FUNC(Bool32) RLINGS_GetExportData(uint32_t dwType, void * pData)
#endif
{
	Bool32 rc = TRUE;

	gwLowRC = 0;

	switch(dwType)
	{
		CASE_FUNCTION(IsDictonaryAvailable);
		CASE_FUNCTION(LoadDictonary);
		CASE_FUNCTION(LoadSecDictonary);
		CASE_FUNCTION(UnloadDictonary);
		CASE_FUNCTION(UnloadSecDictonary);
		CASE_FUNCTION(LoadUserDictonary);
		CASE_FUNCTION(LoadSecUserDictonary);
		CASE_FUNCTION(UnloadUserDictonary);
		CASE_FUNCTION(UnloadSecUserDictonary);
		CASE_FUNCTION(CheckWord);
		CASE_FUNCTION(CheckSecWord);
		CASE_FUNCTION(CheckED);
		CASE_FUNCTION(CheckSecED);
		/*
		 CASE_FUNCTION(CorrectWord);
		 CASE_FUNCTION(CorrectSecWord);
		 CASE_FUNCTION(CorrectHypWord);
		 CASE_FUNCTION(CorrectSecHypWord);
		 CASE_FUNCTION(GetCorrectedRectElement);
		 CASE_FUNCTION(GetSecCorrectedRectElement);
		 CASE_FUNCTION(GetCorrectedVersElement);
		 CASE_FUNCTION(GetSecCorrectedVersElement);
		 */
		default:
		*(Handle *)pData = NULL;
		gwLowRC = IDS_RLING_ERR_NOTIMPLEMENT;
		rc = FALSE;
	}

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_SetImportData(uint32_t dwType, void * pData)
#else
RLINGS_FUNC(Bool32) RLINGS_SetImportData(uint32_t dwType, void * pData)
#endif
{
	Bool rc = FALSE;
	gwLowRC = IDS_RLING_ERR_NOTIMPLEMENT;

	return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
void SetReturnCode_rling(uint16_t rc) {
	if (rc == IDS_RLING_ERR_NO || gwLowRC == IDS_RLING_ERR_NO)
		gwLowRC = rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
uint16_t GetReturnCode_rling() {
	return gwLowRC;
}
//////////////////////////////////////////////////////////////////////////////////
//end of file
