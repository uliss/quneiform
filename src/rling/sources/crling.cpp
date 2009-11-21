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

#include "resource.h"
#include "rlcontrol.h"
#include "crling.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// exteren globals
extern CRLControl * Control_crl;
// extern functions
void SetReturnCode_rling(uint16_t rc);
uint16_t GetReturnCode_rling();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(int32_t) RLING_IsDictonaryAvailable(uint32_t wLang, pchar pPath)
#else
RLINGS_FUNC(int32_t) RLINGS_IsDictonaryAvailable(uint32_t wLang, pchar pPath)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->IsDictonaryAvailable( wLang, (char*) pPath);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_LoadDictonary(uint32_t wLang, pchar pPath)
#else
RLINGS_FUNC(Bool32) RLINGS_LoadDictonary(uint32_t wLang, pchar pPath)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->LoadDictonary( wLang, (char*) pPath);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_LoadUserDictonary(pchar pUserDictList, pchar pPath)
#else
RLINGS_FUNC(Bool32) RLINGS_LoadUserDictonary(pchar pUserDictList, pchar pPath)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->LoadUserDictonary( (char*)pUserDictList, (char*)pPath);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_LoadSecDictonary(uint32_t wLang, pchar pPath)
#else
RLINGS_FUNC(Bool32) RLINGS_LoadSecDictonary(uint32_t wLang, pchar pPath)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->LoadSecDictonary( wLang, (char*) pPath);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_LoadSecUserDictonary(pchar pUserDictList, pchar pPath)
#else
RLINGS_FUNC(Bool32) RLINGS_LoadSecUserDictonary(pchar pUserDictList, pchar pPath)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->LoadSecUserDictonary( (char*)pUserDictList, (char*)pPath);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_UnloadDictonary(void)
#else
RLINGS_FUNC(Bool32) RLINGS_UnloadDictonary(void)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->UnLoadDictonary();

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_UnloadUserDictonary(void)
#else
RLINGS_FUNC(Bool32) RLINGS_UnloadUserDictonary(void)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->UnLoadUserDictonary();

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_UnloadSecDictonary(void)
#else
RLINGS_FUNC(Bool32) RLINGS_UnloadSecDictonary(void)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->UnLoadSecDictonary();

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_UnloadSecUserDictonary(void)
#else
RLINGS_FUNC(Bool32) RLINGS_UnloadSecUserDictonary(void)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->UnLoadSecUserDictonary();

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CheckWord(pchar pWord, int32_t * pOutCheck)
#else
RLINGS_FUNC(Bool32) RLINGS_CheckWord(pchar pWord, int32_t * pOutCheck)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CheckWord( (char*)pWord, pOutCheck);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CheckSecWord(pchar pWord, int32_t * pOutCheck)
#else
RLINGS_FUNC(Bool32) RLINGS_CheckSecWord(pchar pWord, int32_t * pOutCheck)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CheckSecWord( (char*)pWord, pOutCheck);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/*
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CheckFile(pchar pFile, int32_t * pOutCheck)
#else
RLINGS_FUNC(Bool32) RLINGS_CheckFile(pchar pFile, int32_t * pOutCheck)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CheckFile( (char*)pFile, pOutCheck);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CheckED(void * pEDPool, void * pEDOutPool, uint32_t wEDPoolSize, uint32_t * pwEDOutPoolSize, int32_t * pOutCheck)
#else
RLINGS_FUNC(Bool32) RLINGS_CheckED(void * pEDPool, void * pEDOutPool, uint32_t wEDPoolSize, uint32_t * pwEDOutPoolSize, int32_t * pOutCheck)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CheckED( pEDPool, pEDOutPool, wEDPoolSize, pwEDOutPoolSize, pOutCheck);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CheckSecED(void * pEDPool, void * pEDOutPool, uint32_t wEDPoolSize, uint32_t * pwEDOutPoolSize, int32_t * pOutCheck)
#else
RLINGS_FUNC(Bool32) RLINGS_CheckSecED(void * pEDPool, void * pEDOutPool, uint32_t wEDPoolSize, uint32_t * pwEDOutPoolSize, int32_t * pOutCheck)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CheckSecED( pEDPool, pEDOutPool, wEDPoolSize, pwEDOutPoolSize, pOutCheck);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CorrectWord(void * Begin, void * End, uint32_t * pLanguage, pchar pCorrWord)
#else
RLINGS_FUNC(Bool32) RLINGS_CorrectWord(void * Begin, void * End, uint32_t * pLanguage, pchar pCorrWord)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CorrectWord( (CSTR_rast)Begin, (CSTR_rast)End, pLanguage, (char*)pCorrWord);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CorrectSecWord(void * Begin, void * End, uint32_t * pLanguage, pchar pCorrWord)
#else
RLINGS_FUNC(Bool32) RLINGS_CorrectSecWord(void * Begin, void * End, uint32_t * pLanguage, pchar pCorrWord)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CorrectSecWord( (CSTR_rast)Begin, (CSTR_rast)End, pLanguage, (char*)pCorrWord);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CorrectHypWord(void * BeginF, void * EndF, uint32_t * pLanguageF, void * BeginS, void * EndS, uint32_t * pLanguageS, pchar pCorrWord)
#else
RLINGS_FUNC(Bool32) RLINGS_CorrectHypWord(void * BeginF, void * EndF, uint32_t * pLanguageF, void * BeginS, void * EndS, uint32_t * pLanguageS, pchar pCorrWord)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CorrectHypWord( (CSTR_rast)BeginF, (CSTR_rast)EndF, pLanguageF, (CSTR_rast)BeginS, (CSTR_rast)EndS, pLanguageS, (char*)pCorrWord);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Bool32) RLING_CorrectSecHypWord(void * BeginF, void * EndF, uint32_t * pLanguageF, void * BeginS, void * EndS, uint32_t * pLanguageS, pchar pCorrWord)
#else
RLINGS_FUNC(Bool32) RLINGS_CorrectSecHypWord(void * BeginF, void * EndF, uint32_t * pLanguageF, void * BeginS, void * EndS, uint32_t * pLanguageS, pchar pCorrWord)
#endif
{
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->CorrectSecHypWord( (CSTR_rast)BeginF, (CSTR_rast)EndF, pLanguageF, (CSTR_rast)BeginS, (CSTR_rast)EndS, pLanguageS, (char*)pCorrWord);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Rect16) RLING_GetCorrectedRectElement(uint32_t i)
#else
RLINGS_FUNC(Rect16) RLINGS_GetCorrectedRectElement(uint32_t i)
#endif
{
	Rect16 Empt = {0,0,0,0};
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->GetCorrectedRectElement(i);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return Empt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(Rect16) RLING_GetSecCorrectedRectElement(uint32_t i)
#else
RLINGS_FUNC(Rect16) RLINGS_GetSecCorrectedRectElement(uint32_t i)
#endif
{
	Rect16 Empt = {0,0,0,0};
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->GetSecCorrectedRectElement(i);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return Empt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(RecVersions) RLING_GetCorrectedVersElement(uint32_t i, uint32_t * pNVers)
#else
RLINGS_FUNC(RecVersions) RLINGS_GetCorrectedVersElement(uint32_t i, uint32_t * pNVers)
#endif
{
	RecVersions Empt = {0,0,0};
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->GetCorrectedVersElemet(i, pNVers);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return Empt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined( __RLING__ )
RLING_FUNC(RecVersions) RLING_GetSecCorrectedVersElement(uint32_t i, uint32_t * pNVers)
#else
RLINGS_FUNC(RecVersions) RLINGS_GetSecCorrectedVersElement(uint32_t i, uint32_t * pNVers)
#endif
{
	RecVersions Empt = {0,0,0};
	SetReturnCode_rling(IDS_RLING_ERR_NO);

	if ( Control_crl )
		return Control_crl->GetSecCorrectedVersElement(i, pNVers);

	SetReturnCode_rling(IDS_RLING_DLL_NOT_INITIALISING);
	return Empt;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
