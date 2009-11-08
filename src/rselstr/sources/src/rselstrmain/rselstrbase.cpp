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

#include <setjmp.h>

# include "dpuma.h"
# include "rselstr.h"
# include "resource.h"
# include "newfunc.h"
# include "kernel.h"
# include "puma/pumadef.h"
# include "lhstr.h"

#include "compat_defs.h"
#include "dsnap.h"

//////////////////////////////////////////////////////////////////GLOBAL VARIABLES
static uint16_t gwHeightRC = 0;
static uint32_t gwRC = 0;
static Handle ghStorage = NULL;
static HINSTANCE ghInst = NULL;
Handle Root;
Handle VertCells = NULL;
Handle NegaCells = NULL;
Handle hShowCells = NULL;
Handle ShowNegaCells = NULL;
Handle ShowVertCells = NULL;
Handle hShowString;
Handle hDebugCancelPicturesAndTables = NULL;
Handle CutStrPrintFILE = NULL;
Handle hCutStr = NULL;
Handle hDebugCutStr = NULL;
Handle CutStrD = NULL;
Handle DifferenCutToHistory = NULL;
Handle hCutComp = NULL;
Handle CutCompPrintFILE = NULL;
Handle CutCompD = NULL;
Handle DifferenCutCompToHistory = NULL;
Handle MainDebug = NULL;
//Handle hVertCellsWork=NULL;
//Handle hNegaCellsWork=NULL;
Handle hTime = NULL;
//Handle hUseCLine=NULL;
Handle hStrPointedD;
//Handle hStrPointed;
Handle RselstrTime = NULL;
Handle hExit = NULL;
Handle MainTime = NULL;
Handle hDrawComp;

CLhstr* list = NULL;
CLPrepHstr* prelist = NULL;
Handle Root_CHSTR = NULL;
Handle PutToCHSTR;
Handle PutToCSTR;
Handle StrD;
Handle MainWindowD;
Handle CutStrV;
Handle StrPix;
Handle CutStrDel;
Handle SkewFromRLTABLE;
double cut_white = .90;
double cut_str = .85;
double inf_let_black = .2;
int inf_str_h = 15;
int inf_str_w = 8;

/////////////////////////////////////////


/////////////////////////////////////////
Bool APIENTRY DllMain(HINSTANCE hModule, uint32_t ul_reason_for_call, pvoid lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        ghInst = hModule;
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_Init(uint16_t wHeightCode, Handle hStorage) {
    gwHeightRC = wHeightCode;
    list = NULL;
    prelist = NULL;
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_Done() {
    delete list;
    delete prelist;
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(uint32_t) RSELSTR_GetReturnCode() {
    return gwRC;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(char *) RSELSTR_GetReturnString(uint32_t dwError) {
    return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_GetExportData(uint32_t dwType, void * pData) {
    Bool32 rc = TRUE;

#define CASE_FUNCTION(a)	case RSELSTR_FN##a:	*(FN##a *)pData = a; break

    switch (dwType) {
    CASE_FUNCTION(RSELSTR_ExtractTextStrings)
        ;
    CASE_FUNCTION(RSELSTR_PutObjects)
        ;
    CASE_FUNCTION(RSELSTR_CutStr)
        ;
    CASE_FUNCTION(RSELSTR_RotateRaster)
        ;
    CASE_FUNCTION(RSELSTR_UnRotateRect)
        ;
    CASE_FUNCTION(RSELSTR_CleanStr)
        ;
    CASE_FUNCTION(RSELSTR_TestForVertical)
        ;
    CASE_FUNCTION(RSELSTR_CutCompInTableZones)
        ;

    default:
        *(Handle *) pData = NULL;
        rc = FALSE;
    }
    return rc;
}
//////////////////////////////////////////////////////////////////////////////////
//
RSELSTR_FUNC(Bool32) RSELSTR_SetImportData(uint32_t dwType, void * pData) {
    Bool32 rc = TRUE;

    gwRC = 0;

#define CASE_DATA(a,b,c)	case a: c = *(b *)pData; break
#define CASE_PDATA(a,b,c)	case a: c = (b)pData; break

    switch (dwType) {
    CASE_PDATA(RSELSTR_FNRSELSTR_ProgressStart, FNRSELSTR_ProgressStart ,fnProgressStart_rsel)
        ;
    CASE_PDATA(RSELSTR_FNRSELSTR_ProgressStep, FNRSELSTR_ProgressStep, fnProgressStep_rsel)
        ;
    CASE_PDATA(RSELSTR_FNRSELSTR_ProgressFinish, FNRSELSTR_ProgressFinish,fnProgressFinish_rsel)
        ;
    default:
        SetReturnCode_rselstr(IDS_ERR_NOTIMPLEMENT);
        rc = FALSE;
    }

#undef CASE_DATA
#undef CASE_PDATA

    return rc;

}

RSELSTR_FUNC(uint32_t) RSELSTR_SetReturnCode(uint32_t rc) {
    return (uint32_t) (0);
}

void SetReturnCode_rselstr(uint32_t rc) {
    uint16_t low = (uint16_t) (rc & 0xFFFF);
    uint16_t hei = (uint16_t) (rc >> 16);

    if (hei)
        gwRC = rc;
    else {
        if (low - IDS_ERR_NO)
            gwRC = (uint32_t) (gwHeightRC << 16) | (low - IDS_ERR_NO);
        else
            gwRC = 0;
    }
}

uint32_t GetReturnCode_rselstr() {
    uint32_t rc = gwRC;
    uint16_t low = (uint16_t) (gwRC & 0xFFFF);
    uint16_t hei = (uint16_t) (gwRC >> 16);

    if (hei == gwHeightRC || hei == 0)
        rc = low + IDS_ERR_NO;

    return rc;
}

void EnableDebug() {

# ifdef LT_DEBUG
    switch (layout)
    {
        case 0: LT_DebugGraphicsLevel = 0; break;
        case 1: LT_DebugGraphicsLevel = 1; break;
        case 2: LT_DebugGraphicsLevel = 2; break;
        case 3: LT_DebugGraphicsLevel = 3; break;
        case 4: LT_DebugGraphicsLevel = 4; break;
        case 5: LT_DebugGraphicsLevel = 1; break;
        case 6: LT_DebugGraphicsLevel = 1; break;
        case 7: LT_DebugGraphicsLevel = 1; break;
        case 8: LT_DebugGraphicsLevel = 1; break;
        case 9: LT_DebugGraphicsLevel = 0; break;
        case 10: LT_DebugGraphicsLevel = 0; break;
    }
# endif
# ifdef SE_DEBUG
    switch (layout)
    {
        case 0: SE_DebugGraphicsLevel = 0; break;
        case 1: SE_DebugGraphicsLevel = 0; break;
        case 2: SE_DebugGraphicsLevel = 0; break;
        case 3: SE_DebugGraphicsLevel = 0; break;
        case 4: SE_DebugGraphicsLevel = 0; break;
        case 5: SE_DebugGraphicsLevel = 1; break;
        case 6: SE_DebugGraphicsLevel = 2; break;
        case 7: SE_DebugGraphicsLevel = 3; break;
        case 8: SE_DebugGraphicsLevel = 4; break;
        case 9: SE_DebugGraphicsLevel = 0; break;
        case 10: SE_DebugGraphicsLevel = 0; break;
    }
# endif
# ifdef MA_DEBUG
    switch (layout)
    {
        case 0: MA_DebugLevel = 0; break;
        case 1: MA_DebugLevel = 0; break;
        case 2: MA_DebugLevel = 0; break;
        case 3: MA_DebugLevel = 0; break;
        case 4: MA_DebugLevel = 0; break;
        case 5: MA_DebugLevel = 0; break;
        case 6: MA_DebugLevel = 0; break;
        case 7: MA_DebugLevel = 0; break;
        case 8: MA_DebugLevel = 0; break;
        case 9: MA_DebugLevel = 1; break;
        case 10: MA_DebugLevel = 2; break;
    }
#endif
}

//end of file
