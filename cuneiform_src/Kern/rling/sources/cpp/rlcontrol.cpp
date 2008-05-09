/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Ðàçðåøàåòñÿ ïîâòîðíîå ðàñïðîñòðàíåíèå è èñïîëüçîâàíèå êàê â âèäå èñõîäíîãî êîäà,
òàê è â äâîè÷íîé ôîðìå, ñ èçìåíåíèÿìè èëè áåç, ïðè ñîáëþäåíèè ñëåäóþùèõ óñëîâèé:

      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè èñõîäíîãî êîäà äîëæíû îñòàâàòüñÿ óêàçàííîå
        âûøå óâåäîìëåíèå îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è ïîñëåäóþùèé 
        îòêàç îò ãàðàíòèé. 
      * Ïðè ïîâòîðíîì ðàñïðîñòðàíåíèè äâîè÷íîãî êîäà â äîêóìåíòàöèè è/èëè â 
        äðóãèõ ìàòåðèàëàõ, ïîñòàâëÿåìûõ ïðè ðàñïðîñòðàíåíèè, äîëæíû ñîõðàíÿòüñÿ
        óêàçàííàÿ âûøå èíôîðìàöèÿ îá àâòîðñêîì ïðàâå, ýòîò ñïèñîê óñëîâèé è
        ïîñëåäóþùèé îòêàç îò ãàðàíòèé.  
      * Íè íàçâàíèå Cognitive Technologies, íè èìåíà åå ñîòðóäíèêîâ íå ìîãóò 
        áûòü èñïîëüçîâàíû â êà÷åñòâå ñðåäñòâà ïîääåðæêè è/èëè ïðîäâèæåíèÿ 
        ïðîäóêòîâ, îñíîâàííûõ íà ýòîì ÏÎ, áåç ïðåäâàðèòåëüíîãî ïèñüìåííîãî
        ðàçðåøåíèÿ. 

ÝÒÀ ÏÐÎÃÐÀÌÌÀ ÏÐÅÄÎÑÒÀÂËÅÍÀ ÂËÀÄÅËÜÖÀÌÈ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È/ÈËÈ ÄÐÓÃÈÌÈ ËÈÖÀÌÈ "ÊÀÊ
ÎÍÀ ÅÑÒÜ" ÁÅÇ ÊÀÊÎÃÎ-ËÈÁÎ ÂÈÄÀ ÃÀÐÀÍÒÈÉ, ÂÛÐÀÆÅÍÍÛÕ ßÂÍÎ ÈËÈ ÏÎÄÐÀÇÓÌÅÂÀÅÌÛÕ, 
ÂÊËÞ×Àß ÃÀÐÀÍÒÈÈ ÊÎÌÌÅÐ×ÅÑÊÎÉ ÖÅÍÍÎÑÒÈ È ÏÐÈÃÎÄÍÎÑÒÈ ÄËß ÊÎÍÊÐÅÒÍÎÉ ÖÅËÈ, ÍÎ ÍÅ
ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ. ÍÈ ÂËÀÄÅËÅÖ ÀÂÒÎÐÑÊÈÕ ÏÐÀÂ È ÍÈ ÎÄÍÎ ÄÐÓÃÎÅ ËÈÖÎ, ÊÎÒÎÐÎÅ 
ÌÎÆÅÒ ÈÇÌÅÍßÒÜ È/ÈËÈ ÏÎÂÒÎÐÍÎ ÐÀÑÏÐÎÑÒÐÀÍßÒÜ ÏÐÎÃÐÀÌÌÓ, ÍÈ Â ÊÎÅÌ ÑËÓ×ÀÅ ÍÅ 
ÍÅÑ¨Ò ÎÒÂÅÒÑÒÂÅÍÍÎÑÒÈ, ÂÊËÞ×Àß ËÞÁÛÅ ÎÁÙÈÅ, ÑËÓ×ÀÉÍÛÅ, ÑÏÅÖÈÀËÜÍÛÅ ÈËÈ 
ÏÎÑËÅÄÎÂÀÂØÈÅ ÓÁÛÒÊÈ, ÑÂßÇÀÍÍÛÅ Ñ ÈÑÏÎËÜÇÎÂÀÍÈÅÌ ÈËÈ ÏÎÍÅÑÅÍÍÛÅ ÂÑËÅÄÑÒÂÈÅ 
ÍÅÂÎÇÌÎÆÍÎÑÒÈ ÈÑÏÎËÜÇÎÂÀÍÈß ÏÐÎÃÐÀÌÌÛ (ÂÊËÞ×Àß ÏÎÒÅÐÈ ÄÀÍÍÛÕ, ÈËÈ ÄÀÍÍÛÅ, 
ÑÒÀÂØÈÅ ÍÅÃÎÄÍÛÌÈ, ÈËÈ ÓÁÛÒÊÈ È/ÈËÈ ÏÎÒÅÐÈ ÄÎÕÎÄÎÂ, ÏÎÍÅÑÅÍÍÛÅ ÈÇ-ÇÀ ÄÅÉÑÒÂÈÉ 
ÒÐÅÒÜÈÕ ËÈÖ È/ÈËÈ ÎÒÊÀÇÀ ÏÐÎÃÐÀÌÌÛ ÐÀÁÎÒÀÒÜ ÑÎÂÌÅÑÒÍÎ Ñ ÄÐÓÃÈÌÈ ÏÐÎÃÐÀÌÌÀÌÈ, 
ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÝÒÈÌÈ ÑËÓ×ÀßÌÈ), ÍÎ ÍÅ ÎÃÐÀÍÈ×ÈÂÀßÑÜ ÈÌÈ, ÄÀÆÅ ÅÑËÈ ÒÀÊÎÉ 
ÂËÀÄÅËÅÖ ÈËÈ ÄÐÓÃÎÅ ËÈÖÎ ÁÛËÈ ÈÇÂÅÙÅÍÛ Î ÂÎÇÌÎÆÍÎÑÒÈ ÒÀÊÈÕ ÓÁÛÒÊÎÂ È ÏÎÒÅÐÜ.

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
// õèäåð äëÿ âòîðîé êîïèè (òîëüêî äëÿ ïåðâîé)   
#if !defined ( RLING_SECONDARY ) //& defined (RLING_MASTER)
  #define RLING_SECONDARY
  #include "crling.h"
  #undef RLING_SECONDARY
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// äëÿ ñêèäûâàíèÿ ñ ìîçãîâ â ôàéë
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
typedef void(*pExitByCatchFunc)(Int32);
//////////////////////////////////////////////////////////////////////////////////
// defined at "Rling_Ma.c"
extern "C" 
{
	// global variables
	extern    Word8     language;
	extern    Int8      own_dir[];
	extern    PWord8    svbox_pool;
	extern    PWord8    ED_file_start;
	extern    PWord8    ED_file_end;
	extern    PWord8    ED_out_end;
	extern    Int16     CheckOpenBinType;
	extern    Int16     CheckOpenTxtType;
	extern    Int16     CheckOpenSubType;
	// C-typef functions
	void      SetErrorExit( pExitByCatchFunc );
	Int16     TE_table_op(Int16, Int16, Int16, Int16);
	Int16     TE_close(Int16);
	void      trees_load(void);
	Int16     text_findstat(PChar8);
	Int16     spelling(PWord8, Int32);
	void      load_user_dicts (PChar8, PChar8);
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
Int32  CRLControl::IsDictonaryAvailable(Word32 wLang, PChar8 pDictPath)
{
	// -1 - invalid languge code, 
	// 0  - tables not found, 
	// >0 - tables available
	Int32 iRet = -1;
	Int16 TempFile;

	if ( pDictPath != NULL )
	{
		strcpy(m_LastDictonaryPath, pDictPath);

		if ( strlen( m_LastDictonaryPath ) < 256 )
		{
			strcpy((PChar8)own_dir, m_LastDictonaryPath);
			iRet = 0;
		}
		else
		{
			SetReturnCode_rling(IDS_RLING_PATH_UNVAILABLE);
			strcpy((PChar8)own_dir, "./");
		}
	}

	if ( iRet == 0 )
	{
		language = (Word8) wLang;
		//////////////////////////////////////////////////////////////////////
		try
		{
			for ( Int16 iStream = 6; iStream < 10; iStream++ )
			{
				TempFile = TE_table_op( iStream, (Int16) wLang, (iStream == 6 ? CheckOpenBinType : CheckOpenTxtType), CheckOpenSubType);
				
				if ( TempFile == -1 )
				{
					throw (Int32)RLING_ERROR_CANT_OPEN_TABLE;	
				}

				TE_close(TempFile);
			}

			iRet = 1;
		}

		catch( Int32 PumaExceptCode )
		{
			SetCodeWhenExit(PumaExceptCode);
		}
		//////////////////////////////////////////////////////////////////////
	}

	return iRet;
}
///////////////////////////////////////////////////////////////////////////////////
//
Bool32 CRLControl::LoadDictonary(Word32 wLang, PChar8 pDictPath)
{
	Bool32 bRet = FALSE;

	if ( m_Language > 0 )
    {
        if ( wLang == (Word32)m_Language )
            return TRUE;

        UnLoadDictonary();
    }

	m_Language = wLang;
	
	if ( pDictPath != NULL )
	{
		strcpy(m_LastDictonaryPath, pDictPath);

		if ( strlen( m_LastDictonaryPath ) < 256 )
		{
			strcpy((PChar8)own_dir, m_LastDictonaryPath);
			bRet = TRUE;
		}
		else
		{
			SetReturnCode_rling(IDS_RLING_PATH_UNVAILABLE);
			strcpy((PChar8)own_dir, "./");
		}
	}

	if ( svbox_pool == NULL )
	{
		m_TablePool = RLINGAlloc(SizeTables);
		svbox_pool  = (PWord8)RLINGLock(m_TablePool); 
	}
	else
	{
		bRet = FALSE;
	}

	if ( bRet == TRUE )
	{
		language = (Word8) m_Language;
		//////////////////////////////////////////////////////////////////////
		try
		{
			trees_load();
		}

		catch( Int32 PumaExceptCode )
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
Bool32 CRLControl::LoadSecDictonary(Word32 wLang, PChar8 pDictPath)
{
	Bool32 bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_LoadDictonary(wLang, (PInt8)pDictPath);
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
Bool32 CRLControl::CheckWord(PChar8 cWord, PInt32 pOutCheck)
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
				m_LastCheck = text_findstat(m_LastWord);
			}
			/////////////////////////////////////////////////////////////////
			catch( Int32 PumaExceptCode )
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
Bool32 CRLControl::CheckED(void *pEDPool, void * pEDOutPool, Word32 wEDPoolSize, PWord32 pwEDOutPoolSize, PInt32 pOut)
{
	Bool32     bRet = FALSE;
	Word32     wHexSize = RLING_ED_BUFFER_SIZE / RLING_ED_DECREATOR;

	m_LastCheck = 0;

	if ( pEDPool == NULL || wEDPoolSize == 0 || pEDOutPool == NULL || pwEDOutPoolSize == NULL )
	{
		SetReturnCode_rling(IDS_RLING_NO_ED);
		return FALSE;
	}

	m_LastEDPool = (PWord8)pEDPool;
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
			m_LastCheck = spelling((PWord8)m_LastEDWorkPool, wHexSize/*m_LastEDWorkPoolSize*/);
			m_LastEDOutPoolSize = ED_out_end - m_LastEDOutPool;
			*pwEDOutPoolSize = m_LastEDOutPoolSize;
			memcpy(pEDOutPool, m_LastEDOutPool, m_LastEDOutPoolSize); 
		}
		/////////////////////////////////////////////////////////////////
		catch( Int32 PumaExceptCode )
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
Bool32 CRLControl::CheckSecWord(PChar8 cWord, PInt32 pOutCheck)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CheckWord((PInt8)cWord, pOutCheck);
#endif

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////
Bool32 CRLControl::CheckFile(PChar8 pFile, PInt32 pCheck)
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
void CRLControl::ExitByCatch(Int32 ExitCode)
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
				m_LastEDOutPool  = (PWord8)RLINGLock(m_hLastEDOutPool);
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
				m_LastEDWorkPool = (PWord8)RLINGLock(m_hLastEDWorkPool);
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

Bool32 CRLControl::CheckSecED(void *pEDPool, void *pEDOutPool, Word32 wEDPoolSize, PWord32 pwEDOutPoolSize, PInt32 pOut)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CheckED(pEDPool, pEDOutPool, wEDPoolSize, pwEDOutPoolSize, pOut);
#endif

	return bRet;
}

Bool32 CRLControl::LoadUserDictonary(PChar8 pUserDictonaryList, PChar8 pPoint)
{
	Bool32     bRet = FALSE;
	////////////////////////////////////////////////////////////////
	try
	{
		load_user_dicts(pUserDictonaryList, pPoint);
		bRet = TRUE;
	}
	/////////////////////////////////////////////////////////////////
	catch( Int32 PumaExceptCode )
	{
		SetCodeWhenExit(PumaExceptCode);
		bRet = FALSE;
	}
	/////////////////////////////////////////////////////////////////////
	return bRet;
}

Bool32 CRLControl::LoadSecUserDictonary(PChar8 pUserDictonaryList, PChar8 pPoint)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_LoadUserDictonary((PInt8)pUserDictonaryList, (PInt8)pPoint);
#endif

	return bRet;
}

void CRLControl::SetCodeWhenExit(Int32 Code)
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
	catch( Int32 PumaExceptCode )
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

Bool32 CRLControl::CorrectWord(CSTR_rast Beg, CSTR_rast End, PWord32 pLanguage, PChar8 CorrWord)
{
	Bool32 bRet = FALSE;
	Word32 wSizeOut;
	Int32  iOut;

	mcEderator.Init();
	
	if ( mcEderator.MakeWord(Beg, End, (PWord8)pLanguage) )
	{
		if ( CheckED(mcEderator.GetEdPool(), mcEderator.GetEdOutPool(), mcEderator.GetEdPoolSize(), &wSizeOut, &iOut) )
		{
			bRet = mcEderator.ExcludeToVers(wSizeOut, CorrWord);
		}
	}
	return bRet;
}

Bool32 CRLControl::CorrectSecWord(CSTR_rast Beg, CSTR_rast End, PWord32 pLanguage, PChar8 CorrWord)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CorrectWord(Beg, End, pLanguage, (PInt8)CorrWord);
#endif

	return bRet;
}

Bool32 CRLControl::CorrectHypWord(CSTR_rast BegF, CSTR_rast EndF, PWord32 pLanguageF, CSTR_rast BegS, CSTR_rast EndS, PWord32 pLanguageS, PChar8 CorrWord)
{
	Bool32 bRet = FALSE;
	Word32 wSizeOut;
	Int32  iOut;

	mcEderator.Init();
	
	if ( mcEderator.MakeWord(BegF, EndF, (PWord8)pLanguageF) &&
		 mcEderator.AddWord(BegS, EndS, (PWord8)pLanguageS)    )
	{
		// Ýòî óæå äåëàåòñÿ â CheckED
		//PumaMemoryToFileDumper Dmp(mcEderator.GetEdPool(), mcEderator.GetEdPoolSize(), "RLingMakeHypWord.ed");

		if ( CheckED(mcEderator.GetEdPool(), mcEderator.GetEdOutPool(), mcEderator.GetEdPoolSize(), &wSizeOut, &iOut) )
		{
			bRet = mcEderator.ExcludeToVers(wSizeOut, CorrWord);
		}
	}
	return bRet;
}

Bool32 CRLControl::CorrectSecHypWord(CSTR_rast BegF, CSTR_rast EndF, PWord32 pLanguageF, CSTR_rast BegS, CSTR_rast EndS, PWord32 pLanguageS, PChar8 CorrWord)
{
	Bool32     bRet = FALSE;

#ifndef RLING_SECONDARY
	bRet = RLINGS_CorrectHypWord(BegF, EndF, pLanguageF, BegS, EndS, pLanguageS, (PInt8)CorrWord);
#endif

	return bRet;
}

Rect16 CRLControl::GetCorrectedRectElement(Word32 i)
{
	return mcEderator.GetRectElement(i);
}

Rect16 CRLControl::GetSecCorrectedRectElement(Word32 i)
{
#ifndef RLING_SECONDARY
	return RLINGS_GetCorrectedRectElement(i);
#endif

	Rect16 Empt = {0,0,0,0};
	return Empt;
}

RecVersions CRLControl::GetCorrectedVersElemet(Word32 i, PWord32 pNVers)
{
	return mcEderator.GetVersElement(i, pNVers);
}

RecVersions CRLControl::GetSecCorrectedVersElement(Word32 i, PWord32 pNVers)
{
#ifndef RLING_SECONDARY
	return RLINGS_GetCorrectedVersElement(i, pNVers);
#endif

	RecVersions Empt = {0,0,0};
	*pNVers = 0;
	return Empt;
}
