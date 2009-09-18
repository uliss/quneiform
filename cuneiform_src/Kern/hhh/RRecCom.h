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

#ifndef __RRECCOM_H
   #define __RRECCOM_H

#ifdef __cplusplus
   extern "C" {
#endif

   #ifndef __GLOBUS_H
   #include "globus.h"
   #endif

   #ifndef __RECDEFS_H
   #include "recdefs.h"
   #endif
/*
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
*/
#ifdef __RRECCOM__
  #define RRECCOM_FUNC  FUN_EXPO
#else
  #define RRECCOM_FUNC  FUN_IMPO
#endif

// Return codes for Extracomp
/*
#define ExRc_OK                 0       // Compatible with Err16.h
#define ExRc_InternalError      -1      // Compatible with Err16.h
#define ExRc_MemAllocFail       -3      // Compatible with Err16.h
#define ExRc_DumpOpenFail       -6      // Compatible with Err16.h
#define ExRc_DumpWriteFail      -8      // Compatible with Err16.h
#define ExRc_ErrorInCallback    -4000
#define ExRc_IncorrectParam     -20     // Compatible with Err16.h
#define ExRc_NotEnoughMemory    4

typedef Bool  (* Tiger_ProcComp)    (void * pool,uint32_t size);
*/
#ifndef Handle
typedef void * Handle ;
#endif

enum    RRECCOMParametrs
    {
    RRECCOM_FNREX_ISLANGUAGE,
	RRECCOM_FNRECOG,
    RRECCOM_OcrPath,
	RRECCOM_FNRECOGCOMP
/*    RRECCOM_FNEVNPROPERT,
    RRECCOM_FNEXTRACOMP,
    RRECCOM_FNEXTRA,
    RRECCOM_FNEXTRADIB,
    RRECCOM_FNGETCONTAINER,
    RRECCOM_FNGETLENEREP,
    RRECCOM_FNVERSION,
    RRECCOM_FNREX_ISLANGUAGE,
	RRECCOM_Word8_Matrix,
	RRECCOM_Word8_Fax1x2,
    RRECCOM_Word16_ActualResolution,
    RRECCOM_FNGETORIENT,
    RRECCOM_ProgressStart,
    RRECCOM_ProgressStep,
    RRECCOM_ProgressFinish,
    RRECCOM_FNMN2CCOM,
    RRECCOM_OcrPath,
    RRECCOM_FNGETINVERTION,
    RRECCOM_FNEVNALPHABET,
    RRECCOM_FNEXTRACOMP3CB,
    RRECCOM_FNEXTGETRECRASTERORIENT,
    RRECCOM_FNSETALLALPHAGRA*/
    };

typedef struct tagRRecComControl
{
        uint32_t flags;
#define RECOG_EVN 1 //опознание с помошью метода "Event"
#define RECOG_GRA 2 //опознание с помошью нейросети

   uint16_t MaxCompWid;  // if comp width > MaxCompWid => ignored; 0 - not init
   uint16_t MaxCompHei;  // =""= by height
   uint16_t MinCompWid;  // if comp width <= MinCompWid => ignored; 0 - not init
   uint16_t MinCompHei;  // =""= by width
   uint16_t MaxScale;    // if scaling > => use long intervals
} RRecComControl;

//   RRECCOM_FUNC(Bool32) ExtrcompInit( void);
    // Start initialization. TRUE if OK
    // Can new call after closing the library without ExtrcompDone
//   RRECCOM_FUNC(void)   ExtrcompDone(void);
    // Closing of the library.
   RRECCOM_FUNC(uint32_t) RRECCOM_GetReturnCode(void);
   RRECCOM_FUNC(char*)  RRECCOM_GetReturnString(uint32_t dwError);
   RRECCOM_FUNC(Bool32) RRECCOM_Init(uint16_t wHeightCode, Handle hStorage);
   RRECCOM_FUNC(Bool32) RRECCOM_Done(void);
   RRECCOM_FUNC(Bool32) RRECCOM_GetExportData(uint32_t dwType, void * pData);
   RRECCOM_FUNC(Bool32) RRECCOM_SetImportData(uint32_t dwType, const void * pData);

// 1    RRECCOM_FNRECOG   опознать компоненты
typedef Bool32   (*FNRRECCOM_Recog)(Handle hCCOM, RRecComControl Control, char *spath, uchar lang);
RRECCOM_FUNC(Bool32)  RRECCOM_Recog(Handle hCCOM, RRecComControl Control, char *spath, uchar lang);
//2		RRECCOM_FNRECOGCOMP опознать одну компоненту
//typedef Bool32   (*FNRRECCOM_RecogComp)(RecRaster* comp_raster, RecVersions* comp_vers, uchar lang, Bool32 need_align8 = false);
//RRECCOM_FUNC(Bool32)  RRECCOM_RecogComp(RecRaster* comp_raster, RecVersions* comp_vers, uchar lang, Bool32 need_align8 = false);

/*
// 1    RRECCOM_FNEVNPROPERT   установить опции для эвент
// setup condition for recognition all comps without Event-algoriphm
//       and alphabet (256 ANSI indicators for any letter)
typedef Bool32   (*FNRRECCOM_SetEVNProperties)(ExcControl ExCW,char *spath, uchar lang);
RRECCOM_FUNC(Bool32)  RRECCOM_SetEVNProperties(ExcControl ExCW,char *spath, uchar lang);
// 2    RRECCOM_FNEXTRACOMP    найти компоненты с помощью коллбэков
typedef Bool32 (*FNRRECCOM_Excomp)(ExcControl Control,
    TImageOpen  tio,        TImageClose tic,
    TImageRead  tir,        Tiger_ProcComp tipc);
RRECCOM_FUNC(int32_t)  Extracomp(ExcControl Control,
    TImageOpen  tio,        TImageClose tic,
    TImageRead  tir,        Tiger_ProcComp tipc);
// 3    RRECCOM_FNEXTRA        найти компоненты в растре
typedef Bool32 (*FNRRECCOM_Exc)(ExcControl Control,
    uchar *lpRaster, int32_t BWid, Bool32 ReverseOrder,
    int32_t Wid, int32_t Hei,
    int32_t HRes, int32_t VRes,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei);
RRECCOM_FUNC(Bool32)  RRECCOMExtra(ExcControl Control,
    uchar *lpRaster, int32_t BWid, Bool32 ReverseOrder,
    int32_t Wid, int32_t Hei,
    int32_t HRes, int32_t VRes,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei, Bool32 FotoMetr);
// 4    RRECCOM_FNEXTRADIB     найти компоненты в ДИБе
typedef Bool32 (*FNRRECCOM_ExcDIB)(ExcControl ExCW,
    uchar *lpDIB,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei);
RRECCOM_FUNC(Bool32)  RRECCOMExtraDIB(ExcControl ExCW,
    uchar *lpDIB,
    int32_t TemplCol, int32_t TemplRow, int32_t TemplWid, int32_t TemplHei);
// 5    RRECCOM_FNGETCONTAINER   Получить код контейнера с компонентами
typedef  int32_t (*FNRRECCOM_GetContainer)(void);
RRECCOM_FUNC(int32_t)  RRECCOMGetContainer(void);
// 6    RRECCOM_FNGETLENEREP     Получить интервальное представление
typedef  Bool32  (*FNRRECCOM_MakeLP)( RecRaster   *rRaster , uchar *lp, int16_t *lp_size, int16_t *numcomp);
RRECCOM_FUNC(Bool32)     RRECCOMMakeLP  ( RecRaster   *rRaster , uchar *lp, int16_t *lp_size, int16_t *numcomp);
// 7    RRECCOM_FNVERSION Дать версию библиотеки
//
//
*/
// 8    RRECCOM_FNREX_ISLANGUAGE существует ли язык
typedef Bool32 (*FNRRECCOM_IsLanguage)(uchar language);
RRECCOM_FUNC(Bool32)  RRECCOM_IsLanguage(uchar language);
/*
// 9    RRECCOM_FNGETORIENT    ориентация страницы
//typedef Bool32 (*FNRRECCOM_GetOrient)(uchar *ori);
   RRECCOM_FUNC(Bool32) RRECCOM_GetOrient(uchar *ori, Handle hCCOM);

// 10   RRECCOM_FNMN2CCOM      из MN в CCOM_comp
typedef CCOM_comp*   (*FNRRECCOM_MN2CCOM)(Handle hCCOM,Handle hmn);
RRECCOM_FUNC(CCOM_comp*)   RRECCOM_MN2CCOM(Handle hCCOM,Handle hmn);
// 11   RRECCOM_FNGETINVERTION инверсный ли образ (белое на чёрном)
typedef Bool32 (*FNRRECCOM_GetInvertion)(uchar *inv);
RRECCOM_FUNC(Bool32) RRECCOM_GetInvertion(uchar *inv);
typedef Bool32 (*FNRRECCOM_ProgressStart )(void);
typedef Bool32 (*FNRRECCOM_ProgressStep  )(uint32_t step);
typedef Bool32 (*FNRRECCOM_ProgressFinish)(void);
// 12   RRECCOM_FNEVNALPHABET   установка алфавита для евент
typedef Bool32 (*FNRRECCOM_SetEVNAlphabet)(char *spath, uchar lang);
RRECCOM_FUNC(Bool32) RRECCOM_SetEVNAlphabet(char *spath, uchar lang);
// 13   RRECCOM_FNEXTRACOMP3CB 3 коллбэка
typedef Bool32  (*FNRRECCOMExtracomp3CB)(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir);
RRECCOM_FUNC(Bool32)  RRECCOMExtracomp3CB(ExcControl ExCW,TImageOpen tio,
                                TImageClose tic, TImageRead tir);
// 14   RRECCOM_FNEXTGETRECRASTERORIENT ориентация рабочего растра
typedef Bool32 (*FNexc_get_rec_raster_orient)(RecRaster   *rs,int32_t *res);
RRECCOM_FUNC(Bool32) exc_get_rec_raster_orient(RecRaster   *rs,int32_t *res);
// 15   RRECCOM_FNSETALLALPHAGRA   алфавит нейронной сети
typedef Bool32 (*FNexc_set_all_alphabet_gra)(void);
RRECCOM_FUNC(Bool32) exc_set_all_alphabet_gra(void);
*/

// error codes (moved 30.07.01 from "exc\src\resource.h")
#define RRECCOM_ERR_MIN                2048
#define RRECCOM_ERR_NO                 2048
//#define RRECCOM_ERR_NOTOPERATE         2049
//#define RRECCOM_ERR_NOMEMORY           2050
#define RRECCOM_ERR_NOTIMPLEMENT       2051
//#define RRECCOM_ERR_INTERNAL           2052
//#define RRECCOM_ERR_DUMPOPEN           2053
//#define RRECCOM_ERR_DUMPWRITE          2054
//#define RRECCOM_ERR_CALLBACK           2055
//#define RRECCOM_ERR_PARAM              2056
//#define RRECCOM_ERR_NOCOMP             2057
#define RRECCOM_ERR_NOINITEV           2058
//#define RRECCOM_ERR_DIFVERSIONS        2059
//#define RRECCOM_ERR_NOIMPORT           2060
//#define RRECCOM_ERR_NOEXPORT           2061
#define RRECCOM_ERR_NOLANGUAGE         2062
#define RRECCOM_ERR_NOSETALPHABET      2063
#define RRECCOM_ERR_NOGRA              2064
//#define RRECCOM_ERR_USE                2065
//#define RRECCOM_ERR_NOTENOUGHTMEMORY   2066
//#define RRECCOM_ERR_GETORIENT          2067
//#define RRECCOM_ERR_IMAGEINFO          2068
#define RRECCOM_ERR_MAX                2068


#ifdef __cplusplus
            }
#endif

#endif // __RRECCOM_H
