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

#if ( !defined ( __CRLING_HEADER_ ) & !defined ( RLING_SECONDARY ) ) | ( !defined (__CRLINGS_HEADER_) & defined(RLING_SECONDARY) )

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

# ifdef RLING_SECONDARY
  #define __RLINGS__
  #undef  __RLING__
  #define __CRLINGS_HEADER_
#else
  #define __RLING__
  #undef  __RLINGS__
  #define __CRLING_HEADER_
#endif

# if defined (_DEBUG)
    # if defined __RLING__
         # pragma message( __FILE__": master checker")
    #else
         # pragma message( __FILE__": slave checker - for RLING and RLINGS project only!")
    #endif
# endif // (_DEBUG)

#ifndef __GLOBUS_H
 #include "globus.h"
#endif
 #include "cttypext.h"
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
#undef  RLING_FUNC
#ifdef __RLING__
  #define RLING_FUNC  FUN_EXPO
#else
  #define RLING_FUNC  FUN_IMPO
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#undef  RLINGS_FUNC
#ifdef __RLINGS__
  #define RLINGS_FUNC  FUN_EXPO
#else
  #define RLINGS_FUNC  FUN_IMPO
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	extern "C" {
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack (push,8)
///////////////////////////////////////////////////////////////////////////////////////////////
# define	RLING_MAXNAME 260
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
# define    RLINGBOOL16          Bool16
# define    RLINGWORD            Word16
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLING__
RLING_FUNC(Bool32) RLING_Init(Word16 wHeightCode,Handle hStorage);
RLING_FUNC(Bool32) RLING_Done();
RLING_FUNC(Word32) RLING_GetReturnCode();
RLING_FUNC(Int8 *) RLING_GetReturnString(Word32 dwError);
RLING_FUNC(Bool32) RLING_GetExportData(Word32 dwType, void * pData);
RLING_FUNC(Bool32) RLING_SetImportData(Word32 dwType, void * pData);
#endif
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLINGS__
RLINGS_FUNC(Bool32) RLINGS_Init(Word16 wHeightCode,Handle hStorage);
RLINGS_FUNC(Bool32) RLINGS_Done();
RLINGS_FUNC(Word32) RLINGS_GetReturnCode();
RLINGS_FUNC(Int8 *) RLINGS_GetReturnString(Word32 dwError);
RLINGS_FUNC(Bool32) RLINGS_GetExportData(Word32 dwType, void * pData);
RLINGS_FUNC(Bool32) RLINGS_SetImportData(Word32 dwType, void * pData);
#endif
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLING__
typedef enum
{
		RLING_FN_IsDictonaryAvailable = 1,
		RLING_FN_LoadDictonary,
		RLING_FN_LoadSecDictonary,
		RLING_FN_LoadUserDictonary,
		RLING_FN_LoadSecUserDictonary,
		RLING_FN_UnloadDictonary,
		RLING_FN_UnloadSecDictonary,
		RLING_FN_UnloadUserDictonary,
		RLING_FN_UnloadSecUserDictonary,
		RLING_FN_CheckWord,
		RLING_FN_CheckSecWord,
		RLING_FN_CheckED,
		RLING_FN_CheckSecED,
		RLING_FN_CorrectWord,
		RLING_FN_CorrectSecWord,
		RLING_FN_CorrectHypWord,
		RLING_FN_CorrectSecHypWord,
		RLING_FN_GetCorrectedRectElement,
		RLING_FN_GetSecCorrectedRectElement,
		RLING_FN_GetCorrectedVersElement,
		RLING_FN_GetSecCorrectedVersElement
} RLING_EXPORT_ENTRIES;
#endif
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLINGS__
typedef enum
{
		RLINGS_FN_IsDictonaryAvailable = 1,
		RLINGS_FN_LoadDictonary,
		RLINGS_FN_LoadSecDictonary,
		RLINGS_FN_LoadUserDictonary,
		RLINGS_FN_LoadSecUserDictonary,
		RLINGS_FN_UnloadDictonary,
		RLINGS_FN_UnloadSecDictonary,
		RLINGS_FN_UnloadUserDictonary,
		RLINGS_FN_UnloadSecUserDictonary,
		RLINGS_FN_CheckWord,
		RLINGS_FN_CheckSecWord,
		RLINGS_FN_CheckED,
		RLINGS_FN_CheckSecED,
		RLINGS_FN_CorrectWord,
		RLINGS_FN_CorrectSecWord,
		RLINGS_FN_CorrectHypWord,
		RLINGS_FN_CorrectSecHypWord,
		RLINGS_FN_GetCorrectedRectElement,
		RLINGS_FN_GetSecCorrectedRectElement,
		RLINGS_FN_GetCorrectedVersElement,
		RLINGS_FN_GetSecCorrectedVersElement
} RLINGS_EXPORT_ENTRIES;
#endif
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLING__
  #define DEC_FUN(a,b,c) typedef a (*FNRLING##b)c; RLING_FUNC(a) RLING_##b c
#endif
/////////////////////////////////////////////////////////////////////////////////////////
#if defined( __RLINGS__ ) & !defined (__RLING__)
  #define DEC_FUN(a,b,c) typedef a (*FNRLINGS##b)c; RLINGS_FUNC(a) RLINGS_##b c
#endif
//////////////////////////////////////////////////////////////////////////////////////////
//
DEC_FUN(Bool32,  IsDictonaryAvailable,     (Word32, PInt8));
DEC_FUN(Bool32,  LoadDictonary,            (Word32, PInt8));
DEC_FUN(Bool32,  LoadSecDictonary,         (Word32, PInt8));
DEC_FUN(Bool32,  LoadUserDictonary,        (PInt8, PInt8));
DEC_FUN(Bool32,  LoadSecUserDictonary,     (PInt8, PInt8));
DEC_FUN(Bool32,  UnloadDictonary,          (void));
DEC_FUN(Bool32,  UnloadSecDictonary,       (void));
DEC_FUN(Bool32,  UnloadUserDictonary,      (void));
DEC_FUN(Bool32,  UnloadSecUserDictonary,   (void));
DEC_FUN(Bool32,  CheckWord,                (PInt8 , PInt32));
DEC_FUN(Bool32,  CheckSecWord,             (PInt8 , PInt32));
DEC_FUN(Bool32,  CheckED,                  (void *, void *, Word32, PWord32, PInt32));
DEC_FUN(Bool32,  CheckSecED,               (void *, void *, Word32, PWord32, PInt32));
DEC_FUN(Bool32,  CorrectWord,              (void *, void *, PWord32, PInt8));
DEC_FUN(Bool32,  CorrectSecWord,           (void *, void *, PWord32, PInt8));
DEC_FUN(Bool32,  CorrectHypWord,           (void *, void *, PWord32, void *, void *, PWord32, PInt8));
DEC_FUN(Bool32,  CorrectSecHypWord,        (void *, void *, PWord32, void *, void *, PWord32, PInt8));
DEC_FUN(Rect16,  GetCorrectedRectElement,   (Word32));
DEC_FUN(Rect16,  GetSecCorrectedRectElement,(Word32));
DEC_FUN(RecVersions,  GetCorrectedVersElement,   (Word32, PWord32));
DEC_FUN(RecVersions,  GetSecCorrectedVersElement,(Word32, PWord32));
//////////////////////////////////////////////////////////////////////////////////////////
//
#undef DEC_FUN
//////////////////////////////////////////////////////////////////////////////////////////
//
#pragma pack (pop)

#ifdef __cplusplus
            }
#endif
//////////////////////////////////////////////////////////////////////////////////////////
//
#endif //__CRLING_HEADER_
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of file
