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

#ifndef __EXC_H
#define __EXC_H

#include "globus.h"
#include "imgaccs.h"
#include "excdefs.h"
#include "recdefs.h"
#include "ccom/ccomdefs.h"
#include "componentextractor.h"

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

typedef Bool (* Tiger_ProcComp)(void * pool, uint32_t size);

enum REXCParametrs
{
    REXC_Word8_Matrix,
    REXC_Word8_Fax1x2,
    REXC_Word16_ActualResolution
};

Bool32 ExtrcompInit(void);
// Start initialization. TRUE if OK
// Can new call after closing the library without ExtrcompDone
void ExtrcompDone(void);
// Closing of the library.
// REXC_FNEXTRACOMP    найти компоненты с помощью коллбэков
int32_t Extracomp(ExcControl Control, TImageOpen tio, TImageClose tic, TImageRead tir,
        Tiger_ProcComp tipc);
EXC_FUNC(uint32_t) REXC_GetReturnCode(void);
Bool32 REXC_Init(uint16_t wHeightCode, Handle hStorage);
void REXC_Done(void);
EXC_FUNC(Bool32) REXC_SetImportData(uint32_t dwType, void * pData);
// REXC_FNEXTRA        найти компоненты в растре
EXC_FUNC(Bool32) REXCExtra(ExcControl Control, uchar *lpRaster, int32_t BWid, Bool32 ReverseOrder,
        int32_t Wid, int32_t Hei, int32_t HRes, int32_t VRes, int32_t TemplCol, int32_t TemplRow,
        int32_t TemplWid, int32_t TemplHei, Bool32 FotoMetr);
// REXC_FNEXTRADIB     найти компоненты в ДИБе
EXC_FUNC(Bool32) REXCExtraDIB(ExcControl ExCW, uchar *lpDIB, int32_t TemplCol, int32_t TemplRow,
        int32_t TemplWid, int32_t TemplHei);
// REXC_FNGETCONTAINER   Получить код контейнера с компонентами
EXC_FUNC(Handle) REXCGetContainer(void);
// REXC_FNGETLENEREP     Получить интервальное представление
EXC_FUNC(Bool32) REXCMakeLP(RecRaster *rRaster, uchar *lp, int16_t *lp_size, int16_t *numcomp);
// REXC_FNMN2CCOM      из MN в CCOM_comp
EXC_FUNC(CCOM_comp*) REXC_MN2CCOM(Handle hCCOM, Handle hmn);
// REXC_FNGETINVERTION инверсный ли образ (белое на чёрном)
EXC_FUNC(Bool32) REXC_GetInvertion(uchar *inv);
// REXC_FNEXTRACOMP3CB 3 коллбэка
EXC_FUNC(Bool32) REXCExtracomp3CB(ExcControl ExCW, TImageOpen tio, TImageClose tic, TImageRead tir);

enum
{
    REXC_ERR_MIN = 2048,
    REXC_ERR_NO = 2048,
    REXC_ERR_NOTOPERATE = 2049,
    REXC_ERR_NOMEMORY = 2050,
    REXC_ERR_NOTIMPLEMENT = 2051,
    REXC_ERR_INTERNAL = 2052,
    REXC_ERR_DUMPOPEN = 2053,
    REXC_ERR_DUMPWRITE = 2054,
    REXC_ERR_CALLBACK = 2055,
    REXC_ERR_PARAM = 2056,
    REXC_ERR_NOCOMP = 2057,
    REXC_ERR_NOINITEV = 2058,
    REXC_ERR_DIFVERSIONS = 2059,
    REXC_ERR_NOIMPORT = 2060,
    REXC_ERR_NOEXPORT = 2061,
    REXC_ERR_NOLANGUAGE = 2062,
    REXC_ERR_NOSETALPHABET = 2063,
    REXC_ERR_NOGRA = 2064,
    REXC_ERR_USE = 2065,
    REXC_ERR_NOTENOUGHTMEMORY = 2066,
    REXC_ERR_GETORIENT = 2067,
    REXC_ERR_IMAGEINFO = 2068,
    REXC_ERR_MAX = 2068
};

#endif // __EXC_H
