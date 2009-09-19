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

 #include "globus.h"
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
# define    RLINGWORD            uint16_t
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLING__
RLING_FUNC(Bool32) RLING_Init(uint16_t wHeightCode,Handle hStorage);
RLING_FUNC(Bool32) RLING_Done();
RLING_FUNC(uint32_t) RLING_GetReturnCode();
RLING_FUNC(char *) RLING_GetReturnString(uint32_t dwError);
RLING_FUNC(Bool32) RLING_GetExportData(uint32_t dwType, void * pData);
RLING_FUNC(Bool32) RLING_SetImportData(uint32_t dwType, void * pData);
#endif
/////////////////////////////////////////////////////////////////////////////////////////
#ifdef __RLINGS__
RLINGS_FUNC(Bool32) RLINGS_Init(uint16_t wHeightCode,Handle hStorage);
RLINGS_FUNC(Bool32) RLINGS_Done();
RLINGS_FUNC(uint32_t) RLINGS_GetReturnCode();
RLINGS_FUNC(char *) RLINGS_GetReturnString(uint32_t dwError);
RLINGS_FUNC(Bool32) RLINGS_GetExportData(uint32_t dwType, void * pData);
RLINGS_FUNC(Bool32) RLINGS_SetImportData(uint32_t dwType, void * pData);
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
DEC_FUN(Bool32,  IsDictonaryAvailable,     (uint32_t, pchar));
DEC_FUN(Bool32,  LoadDictonary,            (uint32_t, pchar));
DEC_FUN(Bool32,  LoadSecDictonary,         (uint32_t, pchar));
DEC_FUN(Bool32,  LoadUserDictonary,        (pchar, pchar));
DEC_FUN(Bool32,  LoadSecUserDictonary,     (pchar, pchar));
DEC_FUN(Bool32,  UnloadDictonary,          (void));
DEC_FUN(Bool32,  UnloadSecDictonary,       (void));
DEC_FUN(Bool32,  UnloadUserDictonary,      (void));
DEC_FUN(Bool32,  UnloadSecUserDictonary,   (void));
DEC_FUN(Bool32,  CheckWord,                (pchar , int32_t *));
DEC_FUN(Bool32,  CheckSecWord,             (pchar , int32_t *));
DEC_FUN(Bool32,  CheckED,                  (void *, void *, uint32_t, uint32_t *, int32_t *));
DEC_FUN(Bool32,  CheckSecED,               (void *, void *, uint32_t, uint32_t *, int32_t *));
DEC_FUN(Bool32,  CorrectWord,              (void *, void *, uint32_t *, pchar));
DEC_FUN(Bool32,  CorrectSecWord,           (void *, void *, uint32_t *, pchar));
DEC_FUN(Bool32,  CorrectHypWord,           (void *, void *, uint32_t *, void *, void *, uint32_t *, pchar));
DEC_FUN(Bool32,  CorrectSecHypWord,        (void *, void *, uint32_t *, void *, void *, uint32_t *, pchar));
DEC_FUN(Rect16,  GetCorrectedRectElement,   (uint32_t));
DEC_FUN(Rect16,  GetSecCorrectedRectElement,(uint32_t));
DEC_FUN(RecVersions,  GetCorrectedVersElement,   (uint32_t, uint32_t *));
DEC_FUN(RecVersions,  GetSecCorrectedVersElement,(uint32_t, uint32_t *));
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
