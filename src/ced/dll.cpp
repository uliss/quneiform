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
// Copyright (c) 1998 Cognitive Technology Ltd.
// Written by Peter Khlebutin
// This file creation date: 20.07.98
//
// dll_cpage.cpp :
// ============================================================================

#include "resource.h"
#include "ced_struct.h"
#include "cedint.h"
#include "puma/pumadef.h"
#include "cfio/cfio.h"

//GLOBAL VARIABLES
static uint16_t gwHeightRC = 0;
static uint32_t gwRC = 0;
static HINSTANCE ghInst = NULL;

Bool32 APIENTRY DllMain(HINSTANCE hModule, uint32_t ul_reason_for_call, pvoid /*lpReserved*/) {
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

Bool32 CED_Init(uint16_t wHeightCode, Handle /*hStorage*/) {
    gwHeightRC = wHeightCode;
    //define stub functions
#define DEC_FUN(a,b,c)  b=My##b

    DEC_FUN(void, CED_BitmapRef,(const bit_map_ref* pt));
    DEC_FUN(void, CED_TextRef,(const text_ref* pt));
    DEC_FUN(void, CED_FontKegl,(const font_kegl *pt));
    DEC_FUN(void, CED_Kegl,(const kegl* pt));
    DEC_FUN(void, CED_Shift,(const shift* pt));
    DEC_FUN(void, CED_RetrieveLevel,(const retrieve_level* pt));
    DEC_FUN(void, CED_Underline,(const underline* pt));
    DEC_FUN(void, CED_DensPrint,(const dens_print* pt));
    DEC_FUN(void, CED_Tabul,(const tabul* pt));
    DEC_FUN(void, CED_TablTabul,(const tabl_tabul* pt));
    DEC_FUN(void, CED_SheetDiskDescr,(const sheet_disk_descr* pt));
    DEC_FUN(void, CED_FragmDiskDescr,(const fragm_disk_descr* pt));
    DEC_FUN(void, CED_FragmDisk,(const fragm_disk* pt));
    DEC_FUN(void, CED_StepBack,(const step_back* pt));
    DEC_FUN(void, CED_LineBeg,(const line_beg* pt));
    DEC_FUN(void, CED_Position,(const position* pt));
    DEC_FUN(void, CED_EdTagLanguage,(const EdTagLanguage* pt));
    DEC_FUN(void, CED_TableConformSizes,(const table_conform_sizes* pt));
    DEC_FUN(void, CED_GroupWords,(const group_words* pt));
    DEC_FUN(void, CED_GroupSymbols,(const group_symbols* pt));
    DEC_FUN(void, CED_Border,(const border* pt));
    DEC_FUN(void, CED_TableHeader,(const table_header* pt));
    DEC_FUN(void, CED_ListOfFragments,(const list_of_fragments* pt));
    DEC_FUN(void, CED_Extention,(const edExtention* pt,const void* ptExt));
    DEC_FUN(void, CED_ExtentionNew,(const edExtentionNew* pt,const void* ptExt));
    DEC_FUN(void, CED_Aksant,(const aksant* pt));
    DEC_FUN(void, CED_Letter,(const letter* pt,const uint32_t alternatives));

#undef DEC_FUN

    logName[0] = 0;
    logStream = 0;

    return GetReturnCode_ced() == 0 ? 1 : GetReturnCode_ced();
}

Bool32 CED_Done() {
    return 0;
}

uint32_t CED_GetReturnCode() {
    return gwRC;
}

char * CED_GetReturnString(uint32_t /*dwError*/) {
    return 0;
}

Bool32 CED_SetImportData(uint32_t dwType, void * pData) {
#define CASE_FUNCTION(a)	case CED_FN##a:	a=(FN##a)pData; break;
    Bool32 rc = TRUE;
    gwRC = 0;
    switch (dwType) {
    CASE_FUNCTION(CED_BitmapRef)
    CASE_FUNCTION(CED_TextRef)
    CASE_FUNCTION(CED_FontKegl)
    CASE_FUNCTION(CED_Kegl)
    CASE_FUNCTION(CED_Shift)
    CASE_FUNCTION(CED_RetrieveLevel)
    CASE_FUNCTION(CED_Underline)
    CASE_FUNCTION(CED_DensPrint)
    CASE_FUNCTION(CED_Tabul)
    CASE_FUNCTION(CED_TablTabul)
    CASE_FUNCTION(CED_SheetDiskDescr)
    CASE_FUNCTION(CED_FragmDiskDescr)
    CASE_FUNCTION(CED_FragmDisk)
    CASE_FUNCTION(CED_StepBack)
    CASE_FUNCTION(CED_LineBeg)
    CASE_FUNCTION(CED_Position)
    CASE_FUNCTION(CED_EdTagLanguage)
    CASE_FUNCTION(CED_TableConformSizes)
    CASE_FUNCTION(CED_GroupWords)
    CASE_FUNCTION(CED_GroupSymbols)
    CASE_FUNCTION(CED_Border)
    CASE_FUNCTION(CED_TableHeader)
    CASE_FUNCTION(CED_ListOfFragments)
    CASE_FUNCTION(CED_Extention)
    CASE_FUNCTION(CED_ExtentionNew)
    CASE_FUNCTION(CED_Aksant)
    CASE_FUNCTION(CED_Letter)
    default:
        SetReturnCode_ced(IDS_ERR_NOTIMPLEMENT);
        rc = FALSE;
    }
#undef CASE_FUNCTION
    return rc;
}

void SetReturnCode_ced(uint32_t rc) {
    uint16_t low = (uint16_t) (rc & 0xFFFF);
    uint16_t hei = (uint16_t) (rc >> 16);

    if (hei)
        gwRC = rc;
    else {
        if (low >= IDS_ERR_NO)
            gwRC = (uint32_t) (gwHeightRC << 16) | (low - IDS_ERR_NO);
        else
            gwRC = low;
    }
}

uint32_t GetReturnCode_ced() {
    uint32_t rc = gwRC;
    uint16_t low = (uint16_t) (gwRC & 0xFFFF);
    uint16_t hei = (uint16_t) (gwRC >> 16);

    if (hei == gwHeightRC || hei == 0)
        rc = low + IDS_ERR_NO;

    return rc;
}

char * GetModulesString(uint32_t dwError) {
    uint16_t hei = (uint16_t) (dwError >> 16);
    static char szString[512];
    sprintf(szString, "Unknown code error 0x%X", dwError);

    switch (hei) {
    case PUMA_MODULE_CFIO:
        sprintf(szString, "%s", CIF::CFIO::CFIO_GetReturnString(dwError));
        return szString;
    }
    return szString;
}
