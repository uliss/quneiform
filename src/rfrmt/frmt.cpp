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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 10.18.98
//
// Frmt.cpp
//
// ============================================================================

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <vector>
#include <climits>

#include "resource.h"
#include "rfrmt.h"
#include "rtffragrect.h"
#include "dpuma.h"
#include "aldebug.h"
#include "rfrmt_prot.h"
#include "common/debug.h"
#include "common/tostring.h"

using namespace CIF;

uint32_t FlagMode;
uint32_t ExFlagMode;

uint32_t RtfWriteMode;
char RtfFileName[PATH_MAX];
char WriteRtfImageName[PATH_MAX];
char lpMyNameSerif[PATH_MAX];
char lpMyNameNonSerif[PATH_MAX];
char lpMyNameMono[PATH_MAX];

Bool32 FlagChangeSizeKegl;
Bool32 FlagLineTransfer;
Bool32 FlagDebugAlign;

extern Bool32 gbBold;
extern Bool32 gbItalic;
extern Bool32 gbSize;
extern uint32_t gnFormat;
extern const char* gpSerifName;
extern const char* gpSansSerifName;
extern const char* gpCourierName;
extern uint32_t CountTable;

// extern functions
void SetReturnCode_rfrmt(uint16_t rc);
uint16_t GetReturnCode_rfrmt();

#ifdef alDebug
std::vector<Rect> *pInputArray;
std::vector<Rect> *pTheGeomStep1;
std::vector<Rect> *pTheGeomStep2;
std::vector<Rect> *pTheGeomTemp;
vectorWord *pFragRectColor;
uint16_t *CountRect;
uint16_t Draw_Step;
uint16_t Draw_Cycle;
#endif

Handle hDbgWnd = NULL;
extern Handle hDebugMy;
extern Handle hDebugKegl;
extern Handle hDebugFrame;
extern Handle hDebugLineTransfer;
extern Handle hDebugAlign;

//###########################################
#define New On
Bool32 RFRMT_Formatter(const char* lpInputImageName, Handle* PtrEdTree) {
#ifdef New
    FILE *fpInternalFile = create_temp_file();
    if (fpInternalFile == NULL) {
        assert ("Could not create tmpfile\n");
    }

    LDPUMA_Skip(hDebugProfStart);

    FlagMode = 0;
    ExFlagMode = FALSE;

    RtfWriteMode = FALSE;
    strcpy((char*) WriteRtfImageName, lpInputImageName);

#ifdef alDebug
    CRtfFragRect RtfFragRect;
    RtfFragRect.m_arInputFragRect.clear();
    RtfFragRect.m_arGeomFragRectStep1.clear();
    RtfFragRect.m_arGeomFragRectStep2.clear();
    RtfFragRect.m_arGeomFragRectTemp.clear();
    RtfFragRect.m_arGeomFragRectColor.clear();
    RtfFragRect.m_GeomFragCountRect = 0;
    RtfFragRect.m_Cycle = 0;
    RtfFragRect.m_Step = 0;

#ifdef GLOBAL_DEBUG_AND_DRAW
    RtfFragRect.m_Cycle = 1;
#else
    RtfFragRect.m_Cycle = 0;
#endif

    pInputArray = &RtfFragRect.m_arInputFragRect;
    pTheGeomStep1 = &RtfFragRect.m_arGeomFragRectStep1;
    pTheGeomStep2 = &RtfFragRect.m_arGeomFragRectStep2;
    pTheGeomTemp = &RtfFragRect.m_arGeomFragRectTemp;
    pFragRectColor = &RtfFragRect.m_arGeomFragRectColor;
    CountRect = &RtfFragRect.m_GeomFragCountRect;
    Draw_Step = RtfFragRect.m_Step;
    Draw_Cycle = RtfFragRect.m_Cycle;
#endif

    SetReturnCode_rfrmt(IDS_ERR_NO);
    if (CreateInternalFileForFormatter(fpInternalFile) == FALSE) {
        fclose(fpInternalFile);
        return FALSE;
    }

    if (!gbBold)
        FlagMode |= NOBOLD;
    if (!gbItalic)
        FlagMode |= NOCURSIV;
    if (!gbSize)
        FlagMode |= NOSIZE;

    if (!LDPUMA_Skip(hDebugFrame))
        FlagMode |= USE_FRAME;
    else if (gnFormat == 1 && ExFlagMode == FALSE)
        FlagMode |= USE_FRAME_AND_COLUMN;
    else
        FlagMode |= USE_NONE;

    strcpy((char*) lpMyNameSerif, gpSerifName);
    strcpy((char*) lpMyNameNonSerif, gpSansSerifName);
    strcpy((char*) lpMyNameMono, gpCourierName);

    FlagChangeSizeKegl = TRUE;
    if (!LDPUMA_Skip(hDebugKegl))
        FlagChangeSizeKegl = FALSE;
    else
        FlagChangeSizeKegl = TRUE;

    FlagLineTransfer = FALSE;
    if (!LDPUMA_Skip(hDebugLineTransfer))
        FlagLineTransfer = TRUE;
    else
        FlagLineTransfer = FALSE;

    FlagDebugAlign = FALSE;
    if (!LDPUMA_Skip(hDebugAlign))
        FlagDebugAlign = TRUE;
    else
        FlagDebugAlign = FALSE;

    if (!FullRtf(fpInternalFile, NULL, PtrEdTree)) {
        fclose(fpInternalFile);
        return FALSE;
    }

    fclose(fpInternalFile);

#ifdef alDebug
    if (!LDPUMA_Skip(hDebugMy)) {
        for (uint i = 0; i < pInputArray->size(); i++) {
            Debug() << pInputArray->at(i) << "\n";
        }

        LDPUMA_Console("Press any key...pInputArray");
        LDPUMA_WaitUserInput(hDebugMy, hDbgWnd);

        for (uint i = 0; i < pTheGeomStep1->size(); i++) {
            Debug() << pTheGeomStep1->at(i) << "\n";
        }

        LDPUMA_Console("Press any key...pTheGeomStep1");
        LDPUMA_WaitUserInput(hDebugMy, hDbgWnd);

        for (uint i = 0; i < pTheGeomStep2->size(); i++) {
            Debug() << pTheGeomStep2->at(i) << "\n";
        }

        LDPUMA_Console("Press any key...pTheGeomStep2");
        LDPUMA_WaitUserInput(hDebugMy, hDbgWnd);
    }
#endif

    LDPUMA_Skip(hDebugProfEnd);
    return TRUE;
#else
    strcpy(WriteRtfImageName,lpInputImageName);
    return TRUE;
#endif
}

Bool32 RFRMT_SaveRtf(const char* lpOutputFileName, uint32_t code) {
    FILE *fpInternalFile = create_temp_file();
    if (fpInternalFile == NULL) {
        assert ("Could not create tmpfile\n");
    }

    LDPUMA_Skip(hDebugProfStart);

    FlagMode = 0;
    ExFlagMode = FALSE;
    RtfWriteMode = TRUE;

    strcpy((char*) RtfFileName, lpOutputFileName);

#ifdef alDebug
    CRtfFragRect RtfFragRect;
    RtfFragRect.m_arInputFragRect.clear();
    RtfFragRect.m_arGeomFragRectStep1.clear();
    RtfFragRect.m_arGeomFragRectStep2.clear();
    RtfFragRect.m_arGeomFragRectTemp.clear();
    RtfFragRect.m_arGeomFragRectColor.clear();
    RtfFragRect.m_GeomFragCountRect = 0;
    RtfFragRect.m_Cycle = 0;
    RtfFragRect.m_Step = 0;

#ifdef GLOBAL_DEBUG_AND_DRAW
    RtfFragRect.m_Cycle = 1;
#else
    RtfFragRect.m_Cycle = 0;
#endif

    pInputArray = &RtfFragRect.m_arInputFragRect;
    pTheGeomStep1 = &RtfFragRect.m_arGeomFragRectStep1;
    pTheGeomStep2 = &RtfFragRect.m_arGeomFragRectStep2;
    pTheGeomTemp = &RtfFragRect.m_arGeomFragRectTemp;
    pFragRectColor = &RtfFragRect.m_arGeomFragRectColor;
    CountRect = &RtfFragRect.m_GeomFragCountRect;
    Draw_Step = RtfFragRect.m_Step;
    Draw_Cycle = RtfFragRect.m_Cycle;
#endif

    SetReturnCode_rfrmt(IDS_ERR_NO);
    if (CreateInternalFileForFormatter(fpInternalFile) == FALSE) {
        LDPUMA_Skip(hDebugProfEnd);
        fclose(fpInternalFile);
        return FALSE;
    }

    if (!gbBold)
        FlagMode |= NOBOLD;
    if (!gbItalic)
        FlagMode |= NOCURSIV;
    if (!gbSize)
        FlagMode |= NOSIZE;

    if (!LDPUMA_Skip(hDebugFrame))
        FlagMode |= USE_FRAME;
    else if (gnFormat == 1 && ExFlagMode == FALSE)
        FlagMode |= USE_FRAME_AND_COLUMN;
    else
        FlagMode |= USE_NONE;

    strcpy((char*) lpMyNameSerif, gpSerifName);
    strcpy((char*) lpMyNameNonSerif, gpSansSerifName);
    strcpy((char*) lpMyNameMono, gpCourierName);

    FlagChangeSizeKegl = TRUE;
    if (!LDPUMA_Skip(hDebugKegl))
        FlagChangeSizeKegl = FALSE;
    else
        FlagChangeSizeKegl = TRUE;

    FlagLineTransfer = FALSE;
    if (!LDPUMA_Skip(hDebugLineTransfer))
        FlagLineTransfer = TRUE;
    else
        FlagLineTransfer = FALSE;

    FlagDebugAlign = FALSE;
    if (!LDPUMA_Skip(hDebugAlign))
        FlagDebugAlign = TRUE;
    else
        FlagDebugAlign = FALSE;

    if (!FullRtf(fpInternalFile, lpOutputFileName, NULL)) {
        LDPUMA_Skip(hDebugProfEnd);
        fclose(fpInternalFile);
        return FALSE;
    }

    fclose(fpInternalFile);

#ifdef alDebug
    if (!LDPUMA_Skip(hDebugMy)) {
        for (uint i = 0; i < pInputArray->size(); i++) {
            Debug() << pInputArray->at(i) << "\n";
        }

        LDPUMA_Console("Press any key...pInputArray");
        LDPUMA_WaitUserInput(hDebugMy, hDbgWnd);

        for (uint i = 0; i < pTheGeomStep1->size(); i++) {
            Debug() << pTheGeomStep1->at(1) << "\n";
        }

        LDPUMA_Console("Press any key...pTheGeomStep1");
        LDPUMA_WaitUserInput(hDebugMy, hDbgWnd);

        for (uint i = 0; i < pTheGeomStep2->size(); i++) {
            Debug() << pTheGeomStep2->at(1) << "\n";
        }

        LDPUMA_Console("Press any key...pTheGeomStep2");
        LDPUMA_WaitUserInput(hDebugMy, hDbgWnd);
    }
#endif

    LDPUMA_Skip(hDebugProfEnd);
    return TRUE;
}

#ifdef alDebug
void MyDrawForDebug(void) {
    Draw_Step = 3;
    Draw_Cycle = 0;
}
#endif
