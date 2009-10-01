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

////////////////////////////////////////////////////////////
// EXTRACTION OF THE COMPONENTS                           //
////////////////////////////////////////////////////////////
// A.Talalay   (for Tiger32.DLL & MAC version)            //
// V.Troyanker (Cuneiform 96, FormReader     )            //
// O.Slavin    (for Cuneifrom2000            )            //
////////////////////////////////////////////////////////////
// Creation     : 08 Jul 1998                             //
////////////////////////////////////////////////////////////
// Modification : 21 Jul 1998                             //
////////////////////////////////////////////////////////////
#ifndef __EXC_H
   #define __EXC_H

#ifdef __cplusplus
   extern "C" {
#endif

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

   #ifndef __IMGACCS_H
   #include "imgaccs.h"
   #endif

   #ifndef __EXCDEFS_H
   #include "excdefs.h"
   #endif

   #ifndef __RECDEFS_H
   #include "recdefs.h"
   #endif

   #ifndef __CCOMDEFS_H
   #include "ccomdefs.h"
   #endif

#ifdef __EXC__
  #define EXC_FUNC  FUN_EXPO
#else
  #define EXC_FUNC  FUN_IMPO
#endif

// Return codes for Extracomp

#define ExRc_OK                 0       // Compatible with Err16.h
#define ExRc_InternalError      -1      // Compatible with Err16.h
#define ExRc_MemAllocFail       -3      // Compatible with Err16.h
#define ExRc_DumpOpenFail       -6      // Compatible with Err16.h
#define ExRc_DumpWriteFail      -8      // Compatible with Err16.h
#define ExRc_ErrorInCallback    -4000
#define ExRc_IncorrectParam     -20     // Compatible with Err16.h
#define ExRc_NotEnoughMemory    4

typedef Bool  (* Tiger_ProcComp)    (void * pool,uint32_t size);
/*
#ifndef Handle
typedef void * Handle ;
#endif
*/
enum    REXCParametrs
    {
//-    REXC_FNEVNPROPERT,
    REXC_FNEXTRACOMP,
    REXC_FNEXTRA,
    REXC_FNEXTRADIB,
    REXC_FNGETCONTAINER,
    REXC_FNGETLENEREP,
    REXC_FNVERSION,
//-    REXC_FNREX_ISLANGUAGE,
	REXC_Word8_Matrix,
	REXC_Word8_Fax1x2,
    REXC_Word16_ActualResolution,
//-    REXC_FNGETORIENT,
    REXC_ProgressStart,
    REXC_ProgressStep,
    REXC_ProgressFinish,
    REXC_FNMN2CCOM,
//-    REXC_OcrPath,
    REXC_FNGETINVERTION,
//-    REXC_FNEVNALPHABET,
    REXC_FNEXTRACOMP3CB
//-    REXC_FNEXTGETRECRASTERORIENT,
//-    REXC_FNSETALLALPHAGRA
    };


   EXC_FUNC(Bool32) ExtrcompInit( void);
    // Start initialization. TRUE if OK
    // Can new call after closing the library without ExtrcompDone
   EXC_FUNC(void)   ExtrcompDone(void);
    // Closing of the library.
   EXC_FUNC(uint32_t) REXC_GetReturnCode(void);
   EXC_FUNC(char*)  REXC_GetReturnString(uint32_t dwError);
   EXC_FUNC(Bool32) REXC_Init(uint16_t wHeightCode, Handle hStorage);
   EXC_FUNC(void)   REXC_Done(void);
   EXC_FUNC(Bool32) REXC_GetExportData(uint32_t dwType, void * pData);
   EXC_FUNC(Bool32) REXC_SetImportData(uint32_t dwType, void * pData);

// 1    REXC_FNEVNPROPERT   установить опции для эвент
// setup condition for recognition all comps without Event-algoriphm
//       and alphabet (256 ANSI indicators for any letter)
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
typedef Bool32   (*FNREXC_SetEVNProperties)(ExcControl ExCW,char *spath, uchar lang);
EXC_FUNC(Bool32)  REXC_SetEVNProperties(ExcControl ExCW,char *spath, uchar lang);
-*/
// 2    REXC_FNEXTRACOMP    найти компоненты с помощью коллбэков
typedef Bool32 (*FNREXC_Excomp)(ExcControl Control,
    TImageOpen  tio,        TImageClose tic,
    TImageRead  tir,        Tiger_ProcComp tipc);
EXC_FUNC(int32_t)  Extracomp(ExcControl Control,
    TImageOpen  tio,        TImageClose tic,
    TImageRead  tir,        Tiger_ProcComp tipc);
// 3    REXC_FNEXTRA        найти компоненты в растре
typedef Bool32 (*FNREXC_Exc)(ExcControl Control,
    uchar *lpRaster, int32_t BWid, Bool32 ReverseOrder,
    int32_t Wid, int32_t Hei,
    int32_t HRes, int32_t VRes,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei);
EXC_FUNC(Bool32)  REXCExtra(ExcControl Control,
    uchar *lpRaster, int32_t BWid, Bool32 ReverseOrder,
    int32_t Wid, int32_t Hei,
    int32_t HRes, int32_t VRes,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei, Bool32 FotoMetr);
// 4    REXC_FNEXTRADIB     найти компоненты в ДИБе
typedef Bool32 (*FNREXC_ExcDIB)(ExcControl ExCW,
    uchar *lpDIB,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei);
EXC_FUNC(Bool32)  REXCExtraDIB(ExcControl ExCW,
    uchar *lpDIB,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei);
// 5    REXC_FNGETCONTAINER   Получить код контейнера с компонентами
typedef  int32_t (*FNREXC_GetContainer)(void);
EXC_FUNC(Handle)  REXCGetContainer(void);
// 6    REXC_FNGETLENEREP     Получить интервальное представление
typedef  Bool32  (*FNREXC_MakeLP)( RecRaster   *rRaster , uchar *lp, int16_t *lp_size, int16_t *numcomp);
EXC_FUNC(Bool32)     REXCMakeLP  ( RecRaster   *rRaster , uchar *lp, int16_t *lp_size, int16_t *numcomp);
// 7    REXC_FNVERSION Дать версию библиотеки
//
//
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
// 8    REXC_FNREX_ISLANGUAGE существует ли язык
typedef Bool32 (*FNREXC_IsLanguage)(uchar language);
EXC_FUNC(Bool32)  REXC_IsLanguage(uchar language);
// 9    REXC_FNGETORIENT    ориентация страницы
typedef Bool32 (*FNREXC_GetOrient)(uchar *ori);
EXC_FUNC(Bool32) REXC_GetOrient(uchar *ori);
-*/
// 10   REXC_FNMN2CCOM      из MN в CCOM_comp
typedef CCOM_comp*   (*FNREXC_MN2CCOM)(Handle hCCOM,Handle hmn);
EXC_FUNC(CCOM_comp*)   REXC_MN2CCOM(Handle hCCOM,Handle hmn);
// 11   REXC_FNGETINVERTION инверсный ли образ (белое на чёрном)
typedef Bool32 (*FNREXC_GetInvertion)(uchar *inv);
EXC_FUNC(Bool32) REXC_GetInvertion(uchar *inv);
typedef Bool32 (*FNREXC_ProgressStart )(void);
typedef Bool32 (*FNREXC_ProgressStep  )(uint32_t step);
typedef Bool32 (*FNREXC_ProgressFinish)(void);
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
// 12   REXC_FNEVNALPHABET   установка алфавита для евент
typedef Bool32 (*FNREXC_SetEVNAlphabet)(char *spath, uchar lang);
EXC_FUNC(Bool32) REXC_SetEVNAlphabet(char *spath, uchar lang);
-*/
// 13   REXC_FNEXTRACOMP3CB 3 коллбэка
typedef Bool32  (*FNREXCExtracomp3CB)(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir);
EXC_FUNC(Bool32)  REXCExtracomp3CB(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir);
/*-Andrey: moved to RRecCom (recognition) and RNorm (autorotate)
//--------------------------------------------------------------
// 14   REXC_FNEXTGETRECRASTERORIENT ориентация рабочего растра
typedef Bool32 (*FNexc_get_rec_raster_orient)(RecRaster   *rs,int32_t *res);
EXC_FUNC(Bool32) exc_get_rec_raster_orient(RecRaster   *rs,int32_t *res);
// 15   REXC_FNSETALLALPHAGRA   алфавит нейронной сети
typedef Bool32 (*FNexc_set_all_alphabet_gra)(void);
EXC_FUNC(Bool32) exc_set_all_alphabet_gra(void);
-*/

// error codes (moved 30.07.01 from "exc\src\resource.h")
#define REXC_ERR_MIN                2048
#define REXC_ERR_NO                 2048
#define REXC_ERR_NOTOPERATE         2049
#define REXC_ERR_NOMEMORY           2050
#define REXC_ERR_NOTIMPLEMENT       2051
#define REXC_ERR_INTERNAL           2052
#define REXC_ERR_DUMPOPEN           2053
#define REXC_ERR_DUMPWRITE          2054
#define REXC_ERR_CALLBACK           2055
#define REXC_ERR_PARAM              2056
#define REXC_ERR_NOCOMP             2057
#define REXC_ERR_NOINITEV           2058
#define REXC_ERR_DIFVERSIONS        2059
#define REXC_ERR_NOIMPORT           2060
#define REXC_ERR_NOEXPORT           2061
#define REXC_ERR_NOLANGUAGE         2062
#define REXC_ERR_NOSETALPHABET      2063
#define REXC_ERR_NOGRA              2064
#define REXC_ERR_USE                2065
#define REXC_ERR_NOTENOUGHTMEMORY   2066
#define REXC_ERR_GETORIENT          2067
#define REXC_ERR_IMAGEINFO          2068
#define REXC_ERR_MAX                2068


#ifdef __cplusplus
            }
#endif

#endif // __EXC_H
