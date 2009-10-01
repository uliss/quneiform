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

// RLControl.cpp: implementation of the CRLControl class.
//
//////////////////////////////////////////////////////////////////////

#include "resource.h"
#include "rlcontrol.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// хидер для второй копии (только для первой)
#if !defined ( RLING_SECONDARY ) //& defined (RLING_MASTER)
  #define RLING_SECONDARY
  #include "crling.h"
  #undef RLING_SECONDARY
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// для скидывания с мозгов в файл
//#define RLING_DUMP
#ifdef RLING_DUMP
  #include "PumaMemoryToFileDumper.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// exit by catch
///////////////////////////////////////////////////////////////////////////////////
typedef void(*pExitByCatchFunc)(int32_t);
//////////////////////////////////////////////////////////////////////////////////
// defined at "Rling_Ma.c"
extern "C"
{
	// global variables
	extern    uchar     language;
	extern    char      own_dir[];
	extern    puchar    svbox_pool;
	extern    puchar    ED_file_start;
	extern    puchar    ED_file_end;
	extern    puchar    ED_out_end;
	extern    int16_t     CheckOpenBinType;
	extern    int16_t     CheckOpenTxtType;
	extern    int16_t     CheckOpenSubType;
	// C-typef functions
	void      SetErrorExit( pExitByCatchFunc );
	int16_t     TE_table_op(int16_t, int16_t, int16_t, int16_t);
	int16_t     TE_close(int16_t);
	void      trees_load_rling(void);
	int16_t     text_findstat_rling(char*);
	int16_t     spelling(puchar, int32_t);
	void      load_user_dicts (char*, char*);
	void      unload_user_dicts(void);
}
///////////////////////////////////////////////////////////////////////////////////
//
CRLControl::CRLControl()
{
	m_Language          = -1;
	m_SecLanguage       = -1;
	m_TablePool         = NULL;
	m_SecTablePool      = NULL;
	m_LastEDPool        = NULL;
	m_LastEDPoolSize    = 0;
	m_hLastEDOutPool    = NULL;
	m_LastEDOutPool     = NULL;
	m_LastEDOutPoolSize = 0;
	m_hLastEDWorkPool   = NULL;
	m_LastEDWorkPool    = NULL;

	AllocEDBuffer();

	SetErrorExit(ExitByCatch);
}
///////////////////////////////////////////////////////////////////////////////////
//
CRLControl::~CRLControl()
{
	UnLoadDictonary();
	FreeEDBuffer();
	FreeEDBuffer();
}
///////////////////////////////////////////////////////////////////////////////////
//
int32_t  CRLControl::IsDictonaryAvailable(uint32_t wLang, char* pDictPath)
{
	// -1 - invalid languge code,
	// 0  - tables not found,
	// >0 - tables available
	int32_t iRet = -1;
	int16_t TempFile;

	if ( pDictPath != NULL )
	{
		strcpy(m_LastDictonaryPath, pDictPath);

		if ( strlen( m_LastDictonaryPath ) < 256 )
		{
			strcpy((char*)own_dir, m_LastDictonaryPath);
			iRet = 0;
		}
		else
		{
			SetReturnCode_rling(IDS_RLING_PATH_UNVAILABLE);
			strcpy((char*)own_dir, "./");
		}
	}

	if ( iRet == 0 )
	{
		language = (uchar) wLang;
		//////////////////////////////////////////////////////////////////////
		try
		{
			for ( int16_t iStream = 6; iStream < 10; iStream++ )
			{
				TempFile = TE_table_op( iStream, (int16_t) wLang, (iStream == 6 ? CheckOpenBinType : CheckOpenTxtType), CheckOpenSubType);

				if ( TempFile == -1 )
				{
					throw (int32_t)RLING_ERROR_CANT_OPEN_TABLE;
				}

				TE_close(TempFile);
			}

			iRet = 1;
		}

		catch( int32_t PumaExceptCode )
		{
			SetCodeWhenExit(PumaExceptCode);
		}
		//////////////////////////////////////////////////////////////////////
	}

	return iRet;
}
///////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRLControl::LoadDictonary(uint32_t wLang, char* pDictPath)
{
	Bool32 bRet = FALSE;

	if ( m_Language > 0 )
    {
        if ( wLang == (uint32_t)m_Language )
            return TRUE;

        UnLoadDictonary();
    }

	m_Language = wLang;

	if ( pDictPath != NULL )
	{
		strcpy(m_LastDictonaryPath, pDictPath);

		if ( strlen( m_LastDictonaryPath ) < 256 )
		{
			strcpy((char*)own_dir, m_LastDictonaryPath);
			bRet = TRUE;
		}
		else
		{
			SetReturnCode_rling(IDS_RLING_PATH_UNVAILABLE);
			strcpy((char*)own_dir, "./");
		}
	}

	if ( svbox_pool == NULL )
	{
		m_TablePool = RLINGAlloc(SizeTables);
		svbox_pool  = (puchar)RLINGLock(m_TablePool);
	}
	else
	{
		bRet = FALSE;
	}

	if ( bRet == TRUE )
	{
		language = (uchar) m_Language;
		//////////////////////////////////////////////////////////////////////
		try
		{
			trees_load_rling();
		}

		catch( int32_t PumaExceptCode )
		{
			SetCodeWhenExit(PumaExceptCode);
			UnLoadDictonary();
			bRet = FALSE;
		}
		//////////////////////////////////////////////////////////////////////
	}

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRLControl::LoadSecDictonary(uint32_t wLang, char* pDictPath)
{
	Bool32 bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_LoadDictonary(wLang, (pchar)pDictPath);
#endif

	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRLControl::UnLoadDictonary()
{
	m_Language = -1;

	if ( m_SecLanguage != -1 )
		UnLoadSecDictonary();

	if ( m_TablePool )
	{
		RLINGUnlock(m_TablePool);
		RLINGFree(m_TablePool);
	}

	m_TablePool = NULL;
	svbox_pool = NULL;

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRLControl::UnLoadSecDictonary()
{
	Bool32 bRet = FALSE;

#ifndef RLING_SECONDARY
	RLINGS_UnloadDictonary();
	bRet = TRUE;
#endif

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////
Bool32 CRLControl::CheckWord(char* cWord, int32_t * pOutCheck)
{
	Bool32     bRet = FALSE;

	m_LastCheck = 0;

	if ( strlen( cWord ) > RLING_MAX_WORD_LENGHT )
	{
		SetReturnCode_rling(IDS_RLING_ERROR_TOO_LONG_WORD);
	}
	else
	{
		strcpy(m_LastWord, cWord);

		if ( m_Language < 0 )
		{
			SetReturnCode_rling(IDS_RLING_DICT_NOT_LOAD);
		}
		else
		{
			bRet        = TRUE;
			////////////////////////////////////////////////////////////////
			try
			{
				m_LastCheck = text_findstat_rling(m_LastWord);
			}
			/////////////////////////////////////////////////////////////////
			catch( int32_t PumaExceptCode )
			{
				SetCodeWhenExit(PumaExceptCode);
				bRet = FALSE;
			}
			/////////////////////////////////////////////////////////////////////
		}
	}

	*pOutCheck = m_LastCheck;

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////
Bool32 CRLControl::CheckED(void *pEDPool, void * pEDOutPool, uint32_t wEDPoolSize, uint32_t * pwEDOutPoolSize, int32_t * pOut)
{
	Bool32     bRet = FALSE;
	uint32_t     wHexSize = RLING_ED_BUFFER_SIZE / RLING_ED_DECREATOR;

	m_LastCheck = 0;

	if ( pEDPool == NULL || wEDPoolSize == 0 || pEDOutPool == NULL || pwEDOutPoolSize == NULL )
	{
		SetReturnCode_rling(IDS_RLING_NO_ED);
		return FALSE;
	}

	m_LastEDPool = (puchar)pEDPool;
	m_LastEDPoolSize = wEDPoolSize;

	if ( !AllocEDBuffer() )
	{
		SetReturnCode_rling(IDS_RLING_ERR_NO_MEMORY);
		return FALSE;
	}

 	if ( m_Language < 0 )
	{
		SetReturnCode_rling(IDS_RLING_DICT_NOT_LOAD);
	}
	else
	{
		bRet        = TRUE;
		////////////////////////////////////////////////////////////////
		ED_file_start = m_LastEDPool;
		ED_file_end   = m_LastEDPool + m_LastEDPoolSize;
		ED_out_end    = m_LastEDOutPool;
		////////////////////////////////////////////////////////////////
#ifdef RLING_DUMP

#ifndef RLING_SECONDARY
		PumaMemoryToFileDumper    EDDump("rling.ed.dmp");
#else
		PumaMemoryToFileDumper    EDDump("rlings.ed.dmp");
#endif

		EDDump.AddDump(m_LastEDPool, m_LastEDPoolSize);
		EDDump.EndDump();
#endif
		////////////////////////////////////////////////////////////////
		try
		{
			*pwEDOutPoolSize = 0;
			m_LastCheck = spelling((puchar)m_LastEDWorkPool, wHexSize/*m_LastEDWorkPoolSize*/);
			m_LastEDOutPoolSize = ED_out_end - m_LastEDOutPool;
			*pwEDOutPoolSize = m_LastEDOutPoolSize;
			memcpy(pEDOutPool, m_LastEDOutPool, m_LastEDOutPoolSize);
		}
		/////////////////////////////////////////////////////////////////
		catch( int32_t PumaExceptCode )
		{
			SetCodeWhenExit(PumaExceptCode);
			bRet = FALSE;
		}
		/////////////////////////////////////////////////////////////////////
	}
	*pOut = m_LastCheck;
	FreeEDBuffer();
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////
Bool32 CRLControl::CheckSecWord(char* cWord, int32_t * pOutCheck)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CheckWord((pchar)cWord, pOutCheck);
#endif

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////
Bool32 CRLControl::CheckFile(char* pFile, int32_t * pCheck)
{
	Bool32     bRet = FALSE;
	m_LastCheck = 0;

	if ( m_Language < 0 )
	{
		SetReturnCode_rling(IDS_RLING_DICT_NOT_LOAD);
	}
	else
	{
		bRet        = TRUE;
	}

	*pCheck = m_LastCheck;
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////
void CRLControl::ExitByCatch(int32_t ExitCode)
{
	throw ExitCode;
}

Bool32 CRLControl::AllocEDBuffer()
{
	Bool32 bRet = TRUE;

	if ( m_LastEDPoolSize != 0 )
	{
		if ( m_hLastEDWorkPool != NULL )
		{
			if ( m_hLastEDOutPool != NULL )
				FreeEDBuffer();

			m_hLastEDOutPool = RLINGAlloc(m_LastEDPoolSize * 4);

			if ( m_hLastEDOutPool == NULL )
			{
				bRet =  FALSE;
			}
			else
			{
				m_LastEDOutPool  = (puchar)RLINGLock(m_hLastEDOutPool);
			}
		}
		else
			bRet = FALSE;
	}
	else
	{
		if ( m_hLastEDOutPool == NULL )
		{
			m_hLastEDWorkPool = RLINGAlloc(m_LastEDWorkPoolSize = RLING_ED_BUFFER_SIZE);

			if ( m_hLastEDWorkPool == NULL )
			{
				FreeEDBuffer();
				bRet = FALSE;
			}
			else
			{
				m_LastEDWorkPool = (puchar)RLINGLock(m_hLastEDWorkPool);
			}
		}
		else
			bRet = FALSE;
	}

	return bRet;
}

void CRLControl::FreeEDBuffer()
{
	if ( m_hLastEDOutPool != NULL )
	{
		RLINGFree(m_hLastEDOutPool);
		m_hLastEDOutPool = m_LastEDOutPool = NULL;
		m_LastEDOutPoolSize = 0;
	}
	else
	{
		if ( m_hLastEDWorkPool != NULL)
		{
			RLINGFree(m_hLastEDWorkPool);
			m_hLastEDWorkPool = m_LastEDWorkPool = NULL;
		}
	}
}

Bool32 CRLControl::CheckSecED(void *pEDPool, void *pEDOutPool, uint32_t wEDPoolSize, uint32_t * pwEDOutPoolSize, int32_t * pOut)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CheckED(pEDPool, pEDOutPool, wEDPoolSize, pwEDOutPoolSize, pOut);
#endif

	return bRet;
}

Bool32 CRLControl::LoadUserDictonary(char* pUserDictonaryList, char* pPoint)
{
	Bool32     bRet = FALSE;
	////////////////////////////////////////////////////////////////
	try
	{
		load_user_dicts(pUserDictonaryList, pPoint);
		bRet = TRUE;
	}
	/////////////////////////////////////////////////////////////////
	catch( int32_t PumaExceptCode )
	{
		SetCodeWhenExit(PumaExceptCode);
		bRet = FALSE;
	}
	/////////////////////////////////////////////////////////////////////
	return bRet;
}

Bool32 CRLControl::LoadSecUserDictonary(char* pUserDictonaryList, char* pPoint)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_LoadUserDictonary((pchar)pUserDictonaryList, (pchar)pPoint);
#endif

	return bRet;
}

void CRLControl::SetCodeWhenExit(int32_t Code)
{
	switch (Code)
	{
	case RLING_ERROR_CANT_OPEN_TABLE:
		SetReturnCode_rling(IDS_RLING_ERR_CANT_OPEN_DICTONARY);
		break;
	case RLING_ERROR_MEMORY_ERROR:
		SetReturnCode_rling(IDS_RLING_ERR_MEMORY_ERROR);
		break;
	case RLING_ERROR_ED_ALREADY_CHEKED:
		SetReturnCode_rling(IDS_RLING_ERROR_ED_ALREADY_CHEKED);
		break;
	case RLING_ERROR_WRONG_ED_FILE:
		SetReturnCode_rling(IDS_RLING_ERR_ED_ERROR);
		break;
	case RLING_ERROR_MEMORY_FAULT:
		SetReturnCode_rling(IDS_RLING_ERR_MEMORY_FAULT);
		break;
	case RLING_ERROR_CANT_OPEN_USER_DICTONARY:
		SetReturnCode_rling(IDS_RLING_ERROR_CANT_OPEN_USER_DICTONARY);
		break;
	case RLING_ERROR_TOO_MANY_USER_DICTONARY:
		SetReturnCode_rling(IDS_RLING_ERROR_TOO_MANY_USER_DICTONARY);
		break;
	default:
		SetReturnCode_rling(IDS_RLING_ERR_NOTIMPLEMENT);
	}
}

Bool32 CRLControl::UnLoadUserDictonary()
{
	Bool32     bRet = FALSE;
	////////////////////////////////////////////////////////////////
	try
	{
		unload_user_dicts();
		bRet = TRUE;
	}
	/////////////////////////////////////////////////////////////////
	catch( int32_t PumaExceptCode )
	{
		SetCodeWhenExit(PumaExceptCode);
		bRet = FALSE;
	}
	/////////////////////////////////////////////////////////////////////
	return bRet;
}

Bool32 CRLControl::UnLoadSecUserDictonary()
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_UnloadUserDictonary();
#endif

	return bRet;
}

Bool32 CRLControl::CorrectWord(CSTR_rast Beg, CSTR_rast End, uint32_t * pLanguage, char* CorrWord)
{
	Bool32 bRet = FALSE;
	uint32_t wSizeOut;
	int32_t  iOut;

	mcEderator.Init();

	if ( mcEderator.MakeWord(Beg, End, (puchar)pLanguage) )
	{
		if ( CheckED(mcEderator.GetEdPool(), mcEderator.GetEdOutPool(), mcEderator.GetEdPoolSize(), &wSizeOut, &iOut) )
		{
			bRet = mcEderator.ExcludeToVers(wSizeOut, CorrWord);
		}
	}
	return bRet;
}

Bool32 CRLControl::CorrectSecWord(CSTR_rast Beg, CSTR_rast End, uint32_t * pLanguage, char* CorrWord)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CorrectWord(Beg, End, pLanguage, (pchar)CorrWord);
#endif

	return bRet;
}

Bool32 CRLControl::CorrectHypWord(CSTR_rast BegF, CSTR_rast EndF, uint32_t * pLanguageF, CSTR_rast BegS, CSTR_rast EndS, uint32_t * pLanguageS, char* CorrWord)
{
	Bool32 bRet = FALSE;
	uint32_t wSizeOut;
	int32_t  iOut;

	mcEderator.Init();

	if ( mcEderator.MakeWord(BegF, EndF, (puchar)pLanguageF) &&
		 mcEderator.AddWord(BegS, EndS, (puchar)pLanguageS)    )
	{
		// Это уже делается в CheckED
		//PumaMemoryToFileDumper Dmp(mcEderator.GetEdPool(), mcEderator.GetEdPoolSize(), "RLingMakeHypWord.ed");

		if ( CheckED(mcEderator.GetEdPool(), mcEderator.GetEdOutPool(), mcEderator.GetEdPoolSize(), &wSizeOut, &iOut) )
		{
			bRet = mcEderator.ExcludeToVers(wSizeOut, CorrWord);
		}
	}
	return bRet;
}

Bool32 CRLControl::CorrectSecHypWord(CSTR_rast BegF, CSTR_rast EndF, uint32_t * pLanguageF, CSTR_rast BegS, CSTR_rast EndS, uint32_t * pLanguageS, char* CorrWord)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CorrectHypWord(BegF, EndF, pLanguageF, BegS, EndS, pLanguageS, (pchar)CorrWord);
#endif

	return bRet;
}

Rect16 CRLControl::GetCorrectedRectElement(uint32_t i)
{
	return mcEderator.GetRectElement(i);
}

Rect16 CRLControl::GetSecCorrectedRectElement(uint32_t i)
{
#ifndef RLING_SECONDARY
	return RLINGS_GetCorrectedRectElement(i);
#endif

	Rect16 Empt = {0,0,0,0};
	return Empt;
}

RecVersions CRLControl::GetCorrectedVersElemet(uint32_t i, uint32_t * pNVers)
{
	return mcEderator.GetVersElement(i, pNVers);
}

RecVersions CRLControl::GetSecCorrectedVersElement(uint32_t i,
		uint32_t * pNVers)
{
#ifndef RLING_SECONDARY
	return RLINGS_GetCorrectedVersElement(i, pNVers);
#endif

	RecVersions Empt = {0,0,0};
	*pNVers = 0;
	return Empt;
}
