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

// RLControl.h: interface for the CRLControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_RLING_CONTROL_H_)
#define _RLING_CONTROL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "globus.h"
#include "crlmemory.h"
#include "crled.h"	// Added by ClassView
class CRLControl {
#define     RLING_ED_BUFFER_SIZE                0x00020000
#define     RLING_ED_DECREATOR                  16

public:
	RecVersions GetSecCorrectedVersElement(uint32_t i, uint32_t * pNVers);
	RecVersions GetCorrectedVersElemet(uint32_t i, uint32_t * pNVers);
	Rect16 GetSecCorrectedRectElement(uint32_t i);
	Rect16 GetCorrectedRectElement(uint32_t i);
	Bool32 CorrectSecHypWord(CSTR_rast BegF, CSTR_rast EndF,
			uint32_t * pLanguageF, CSTR_rast BegS, CSTR_rast EndS,
			uint32_t * pLanguageS, char* CorrWord);
	Bool32 CorrectHypWord(CSTR_rast BegF, CSTR_rast EndF,
			uint32_t * pLanguageF, CSTR_rast BegS, CSTR_rast EndS,
			uint32_t * pLanguageS, char* CorrWord);
	Bool32 CorrectSecWord(CSTR_rast Beg, CSTR_rast End, uint32_t * pLanguage,
			char* CorrWord);
	Bool32 CorrectWord(CSTR_rast Beg, CSTR_rast End, uint32_t * pLanguage,
			char* CorrWord);
	Bool32 UnLoadSecUserDictonary(void);
	Bool32 UnLoadUserDictonary(void);
	Bool32 LoadSecUserDictonary(char* pUserDictonaryList, char* pPoint);
	Bool32 LoadUserDictonary(char* pUserDictonaryList, char* pPoint);
	Bool32 CheckSecED(void *pEDPool, void * pEDOutPool, uint32_t wEDPoolSize,
			uint32_t * pwEDOutPoolSize, int32_t * pOut);
	Bool32 CheckED(void * pEDPool, void * pEDOutPool, uint32_t wEDPoolsize,
			uint32_t * pwEDOutPoolSize, int32_t * pOut);
	Bool32 CheckSecWord(char* cWord, int32_t * pOutCheck);
	Bool32 UnLoadSecDictonary(void);
	Bool32 LoadSecDictonary(uint32_t wLang, char* pDictPath);
	Bool32 CheckFile(char* pFile, int32_t * pCheck);
	static void ExitByCatch(int32_t ExitCode);
	Bool32 CheckWord(char* cWord, int32_t * pOutCheck);
	Bool32 UnLoadDictonary(void);
	Bool32 LoadDictonary(uint32_t wLang, char* pDictPath);
	int32_t IsDictonaryAvailable(uint32_t wLang, char* pDictPath);

	CRLControl();
	virtual ~CRLControl();

protected:
	Bool32 AllocEDBuffer(void);
	Bool32 CheckEDFile(char* pEDFile);
	void FreeEDBuffer(void);
	void SetCodeWhenExit(int32_t Code);

protected:
	CRLEd mcEderator;
	Handle m_hLastEDOutPool;
	Handle m_hLastEDWorkPool;
	puchar m_LastEDOutPool;
	puchar m_LastEDWorkPool;
	uint32_t m_LastEDPoolSize;
	uint32_t m_LastEDOutPoolSize;
	uint32_t m_LastEDWorkPoolSize;
	puchar m_LastEDPool;
	//char   m_SecLastWord[RLING_MAX_WORD_LENGHT + 4];
	//int32_t   m_SecLastCheck;
	//char   m_SecLastDictonaryPath[512];
	Handle m_SecTablePool;
	int32_t m_SecLanguage;
	Handle m_TablePool;
	char m_LastDictonaryPath[512];
	int32_t m_Language;
	int32_t m_LastCheck;
	char m_LastWord[RLING_MAX_WORD_LENGHT + 4];
};

#endif // !defined(_RLING_CONTROL_H_)
