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

// RLControl.h: interface for the CRLControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_RLING_CONTROL_H_)
#define _RLING_CONTROL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globus.h"
#include "cttypext.h"
#include "crlmemory.h"
#include "crled.h"	// Added by ClassView

class CRLControl
{
#define     RLING_ED_BUFFER_SIZE                0x00020000
#define     RLING_ED_DECREATOR                  16

public:
	RecVersions GetSecCorrectedVersElement(Word32 i, PWord32 pNVers);
	RecVersions GetCorrectedVersElemet(Word32 i, PWord32 pNVers);
	Rect16 GetSecCorrectedRectElement(Word32 i);
	Rect16 GetCorrectedRectElement(Word32 i);
	Bool32 CorrectSecHypWord(CSTR_rast BegF, CSTR_rast EndF, PWord32 pLanguageF, CSTR_rast BegS, CSTR_rast EndS, PWord32 pLanguageS, PChar8 CorrWord);
	Bool32 CorrectHypWord(CSTR_rast BegF, CSTR_rast EndF, PWord32 pLanguageF, CSTR_rast BegS, CSTR_rast EndS, PWord32 pLanguageS, PChar8 CorrWord);
	Bool32 CorrectSecWord(CSTR_rast Beg, CSTR_rast End, PWord32 pLanguage, PChar8 CorrWord);
	Bool32 CorrectWord(CSTR_rast Beg, CSTR_rast End, PWord32 pLanguage, PChar8 CorrWord);
	Bool32 UnLoadSecUserDictonary(void);
	Bool32 UnLoadUserDictonary(void);
	Bool32 LoadSecUserDictonary( PChar8 pUserDictonaryList, PChar8 pPoint);
	Bool32 LoadUserDictonary(PChar8 pUserDictonaryList, PChar8 pPoint);
	Bool32 CheckSecED(void *pEDPool, void * pEDOutPool, Word32 wEDPoolSize, PWord32 pwEDOutPoolSize, PInt32 pOut);
	Bool32 CheckED(void * pEDPool, void * pEDOutPool, Word32 wEDPoolsize, PWord32 pwEDOutPoolSize, PInt32 pOut);
	Bool32 CheckSecWord(PChar8 cWord, PInt32 pOutCheck);
	Bool32 UnLoadSecDictonary(void);
	Bool32 LoadSecDictonary(Word32 wLang, PChar8 pDictPath);
	Bool32 CheckFile(PChar8 pFile, PInt32 pCheck);
	static void ExitByCatch( Int32 ExitCode);
	Bool32 CheckWord(PChar8 cWord, PInt32 pOutCheck);
	Bool32 UnLoadDictonary(void);
	Bool32 LoadDictonary(Word32 wLang, PChar8 pDictPath);
	Int32  IsDictonaryAvailable(Word32 wLang, PChar8 pDictPath);

	CRLControl();
	virtual ~CRLControl();

protected:
	Bool32 AllocEDBuffer(void);
	Bool32 CheckEDFile(PChar8 pEDFile);
	void   FreeEDBuffer(void);
	void   SetCodeWhenExit( Int32 Code );

protected:
	CRLEd   mcEderator;
	Handle  m_hLastEDOutPool;
	Handle  m_hLastEDWorkPool;
	PWord8  m_LastEDOutPool;
	PWord8  m_LastEDWorkPool;
	Word32  m_LastEDPoolSize;
	Word32  m_LastEDOutPoolSize;
	Word32  m_LastEDWorkPoolSize;
	PWord8  m_LastEDPool;
	//Char8   m_SecLastWord[RLING_MAX_WORD_LENGHT + 4];
	//Int32   m_SecLastCheck;
	//Char8   m_SecLastDictonaryPath[512];
	Handle  m_SecTablePool;
	Int32   m_SecLanguage;
	Handle  m_TablePool;
	Char8   m_LastDictonaryPath[512];
	Int32   m_Language;
	Int32   m_LastCheck;
	Char8   m_LastWord[RLING_MAX_WORD_LENGHT + 4];
};

#endif // !defined(_RLING_CONTROL_H_)
