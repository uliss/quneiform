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

#ifndef __CPAGE_H
#define __CPAGE_H

#include "globus.h"
#include "cttypes.h"
#include "cpagetyps.h"
#include "cpagedefs.h"
#include "common/point.h"

#ifdef __CPAGE__
#define CPAGE_FUNC  FUN_EXPO__
#else
#define CPAGE_FUNC  FUN_IMPO__
#endif

CPAGE_FUNC Bool32 CPAGE_Init(uint16_t wHeightCode, Handle hStorage);
CPAGE_FUNC bool CPAGE_Done();
CPAGE_FUNC uint32_t CPAGE_GetReturnCode();
CPAGE_FUNC Bool32 CPAGE_GetExportData(uint32_t dwType, void * pData);

enum CPAGE_EXPORT_ENTRIES {
    CPAGE_FNCPAGE_CreatePage = 1,
    CPAGE_FNCPAGE_DeletePage,
    CPAGE_FNCPAGE_SavePage,
    CPAGE_FNCPAGE_RestorePage,
    CPAGE_FNCPAGE_GetHandlePage,
    CPAGE_FNCPAGE_GetPageType,
    CPAGE_FNCPAGE_SetPageData,
    CPAGE_FNCPAGE_GetPageData,
    CPAGE_FNCPAGE_BackUp,
    CPAGE_FNCPAGE_Undo,
    CPAGE_FNCPAGE_Redo,
    CPAGE_FNCPAGE_GetCountPage,
    CPAGE_FNCPAGE_GetCountBlock,
    CPAGE_FNCPAGE_CreateBlock,
    CPAGE_FNCPAGE_GetHandleBlock,
    CPAGE_FNCPAGE_GetBlockType,
    CPAGE_FNCPAGE_GetBlockUserNum,
    CPAGE_FNCPAGE_SetBlockUserNum,
    CPAGE_FNCPAGE_GetBlockFlags,
    CPAGE_FNCPAGE_SetBlockFlags,
    CPAGE_FNCPAGE_SetBlockData,
    CPAGE_FNCPAGE_GetBlockData,
    CPAGE_FNCPAGE_DeleteBlock,
    CPAGE_FNCPAGE_SetConvertorPages,
    CPAGE_FNCPAGE_SetConvertorBlocks,
    CPAGE_FNCPAGE_GetUserPageType,
    CPAGE_FNCPAGE_GetUserBlockType,
    CPAGE_FNCPAGE_GetBuckUpCount,
    CPAGE_FNCPAGE_GetBuckUpHandle,
    CPAGE_FNCPAGE_GetPageFirst,
    CPAGE_FNCPAGE_GetPageNext,
    CPAGE_FNCPAGE_GetBlockFirst,
    CPAGE_FNCPAGE_GetBlockNext,
    CPAGE_FNCPAGE_DeleteAll,
    CPAGE_FNCPAGE_GetCurrentPage,
    CPAGE_FNCPAGE_SetCurrentPage,
    CPAGE_FNCPAGE_GetNumberPage,
    CPAGE_FNCPAGE_TableCreate,
    CPAGE_FNCPAGE_TableGetFirst,
    CPAGE_FNCPAGE_TableGetNext,
    CPAGE_FNCPAGE_GeTableGetNumberCells,
    CPAGE_FNCPAGE_PhTableGetNumberCells,
    CPAGE_FNCPAGE_TableGetNumberRow,
    CPAGE_FNCPAGE_TableGetNumberColumn,
    CPAGE_FNCPAGE_GeTableGetSizeCell,
    CPAGE_FNCPAGE_PhTableGetSizeCell,
    CPAGE_FNCPAGE_TableGetNumberBlock,
    CPAGE_FNCPAGE_GeTableGetPhysical,
    CPAGE_FNCPAGE_PhTableGetNumberGeometry,
    CPAGE_FNCPAGE_PhTableGetGeometry,
    CPAGE_FNCPAGE_PhTableSetNumberBlock,
    CPAGE_FNCPAGE_UpdateBlocks,
    CPAGE_FNCPAGE_TableIsPhysicCell,
    CPAGE_FNCPAGE_TableSize,
    CPAGE_FNCPAGE_HL_TableExtract,
    CPAGE_FNCPAGE_TableGetSkew,
    CPAGE_FNCPAGE_PictureGetFirst,
    CPAGE_FNCPAGE_PictureGetNext,
    CPAGE_FNCPAGE_PictureGetPlace,
    CPAGE_FNCPAGE_PictureGetMask,
    CPAGE_FNCPAGE_GetBlockInterNum,
    CPAGE_FNCPAGE_SetBlockInterNum,
    CPAGE_FNCPAGE_GetBlockDataPtr,
    CPAGE_FNCPAGE_GetBuckUpCurPos,
    CPAGE_FNCPAGE_GetInternalType,
    CPAGE_FNCPAGE_GetNameInternalType,

    CPAGE_FNCPAGE_ExTableCreate,
    CPAGE_FNCPAGE_ExTableDelete,
    CPAGE_FNCPAGE_ExTableGetFirst,
    CPAGE_FNCPAGE_ExTableGetNext,
    CPAGE_FNCPAGE_ExGeTableGetNumberCells,
    CPAGE_FNCPAGE_ExPhTableGetNumberCells,
    CPAGE_FNCPAGE_ExTableGetNumberRow,
    CPAGE_FNCPAGE_ExTableGetNumberColumn,
    CPAGE_FNCPAGE_ExGeTableGetSizeCell,
    CPAGE_FNCPAGE_ExPhTableGetSizeCell,
    CPAGE_FNCPAGE_ExTableGetNumberBlock,
    CPAGE_FNCPAGE_ExGeTableGetPhysical,
    CPAGE_FNCPAGE_ExPhTableGetNumberGeometry,
    CPAGE_FNCPAGE_ExPhTableGetGeometry,
    CPAGE_FNCPAGE_ExPhTableSetNumberBlock,
    CPAGE_FNCPAGE_ExTableIsPhysicCell,
    CPAGE_FNCPAGE_ExTableSize,
    CPAGE_FNCPAGE_ExTableGetSkew,
    CPAGE_FNCPAGE_GetTableFlag,
    CPAGE_FNCPAGE_GetTableCellFlag,
    CPAGE_FNCPAGE_SetTableFlag,
    CPAGE_FNCPAGE_SetTableCellFlag,
    CPAGE_FNCPAGE_ClearBackUp, //Paul 19-01-2001
    CPAGE_FNCPAGE_PhTableGetRect,
    CPAGE_FNCPAGE_PhTableGetBoundType
};

#define DEC_FUN(a,b,c) typedef a (*FN##b)c; CPAGE_FUNC a b c;

CPAGE_FUNC CPageHandle CPAGE_CreatePage(CDataType type, const void * data, uint32_t size);
CPAGE_FUNC void CPAGE_DeletePage(CPageHandle hPage);
CPAGE_FUNC size_t CPAGE_GetPageCount();
CPAGE_FUNC CPageHandle CPAGE_GetHandlePage(uint32_t number);
CPAGE_FUNC uint32_t CPAGE_GetCurrentPageNumber();
CPAGE_FUNC bool CPAGE_SetCurrentPage(uint32_t number);
CPAGE_FUNC uint32_t CPAGE_GetPageNumber(CPageHandle page);
CPAGE_FUNC bool CPAGE_SavePage(CPageHandle page, const char * fname);
CPAGE_FUNC CPageHandle CPAGE_RestorePage(bool remove, const char * fname);
CPAGE_FUNC bool CPAGE_SetPageData(CPageHandle page, CDataType type, const void * data, uint32_t size);
CPAGE_FUNC uint32_t CPAGE_GetPageData(CPageHandle page, CDataType type, void * lpData, uint32_t Size);
CPAGE_FUNC bool CPAGE_GetPageInfo(CPageHandle page, PAGEINFO * info);
CPAGE_FUNC bool CPAGE_SetPageInfo(CPageHandle page, const PAGEINFO& info);
CPAGE_FUNC CBlockHandle CPAGE_CreateBlock(CPageHandle page, CDataType Type, uint32_t UserNum , uint32_t Flags, void * lpData, uint32_t Size);
CPAGE_FUNC void CPAGE_DeleteBlock(CPageHandle page, CBlockHandle block);
CPAGE_FUNC uint32_t CPAGE_GetCountBlock(CPageHandle page);
CPAGE_FUNC CDataType CPAGE_GetBlockType(CBlockHandle block);
CPAGE_FUNC uint32_t CPAGE_GetBlockUserNum(CBlockHandle block);
CPAGE_FUNC void CPAGE_SetBlockUserNum(CBlockHandle block, uint32_t number);
CPAGE_FUNC uint32_t CPAGE_GetBlockFlags(CBlockHandle block);
CPAGE_FUNC void CPAGE_SetBlockFlags(CBlockHandle block, uint32_t flags);
CPAGE_FUNC bool CPAGE_SetBlockData(CBlockHandle block, CDataType type, const void * data, uint32_t size);
typedef uint32_t (*FNCPAGE_GetBlockData)(CBlockHandle block, CDataType type, void * data, uint32_t size);
CPAGE_FUNC uint32_t CPAGE_GetBlockData(CBlockHandle block, CDataType type, void * data, uint32_t size);
typedef CBlockHandle (*FNCPAGE_GetBlockFirst)(CPageHandle page, CDataType type);
CPAGE_FUNC CBlockHandle CPAGE_GetBlockFirst(CPageHandle page, CDataType type);
typedef CBlockHandle (*FNCPAGE_GetBlockNext)(CPageHandle page, CBlockHandle block, CDataType type);
CPAGE_FUNC CBlockHandle CPAGE_GetBlockNext(CPageHandle page, CBlockHandle block, CDataType type);
CPAGE_FUNC CDataType CPAGE_GetUserPageType();
CPAGE_FUNC CDataType CPAGE_GetUserBlockType();
CPAGE_FUNC CPageHandle CPAGE_GetPageFirst(CDataType type);
CPAGE_FUNC CPageHandle CPAGE_GetPageNext(CPageHandle page, CDataType type);
CPAGE_FUNC bool CPAGE_DeleteAll();
CPAGE_FUNC Bool32 CPAGE_UpdateBlocks(CPageHandle hPage, CDataType type);
CPAGE_FUNC CBlockHandle CPAGE_PictureGetFirst(CPageHandle hPage);
CPAGE_FUNC CBlockHandle CPAGE_PictureGetNext(CPageHandle hPage, CBlockHandle hPicture);
CPAGE_FUNC bool CPAGE_PictureGetPlace(CBlockHandle picture, int skew2048, cf::Point * pos, cf::Size * size);
CPAGE_FUNC bool CPAGE_PictureGetMask(CBlockHandle hPicture, char * data, uint32_t * size);
CPAGE_FUNC uint32_t CPAGE_GetBlockInterNum(CBlockHandle block);
CPAGE_FUNC void CPAGE_SetBlockInterNum(CBlockHandle block, uint32_t inter);
CPAGE_FUNC bool CPAGE_GetBlockDataPtr(CBlockHandle block, CDataType Type, void ** lpData);
CPAGE_FUNC CDataType CPAGE_GetInternalType(const char * name);
CPAGE_FUNC const char * CPAGE_GetNameInternalType(CDataType type);

// Виртуальные функции:
// Они доступны только через непосредственный вызов функций
// CPAGE_GetExportData и CPAGE_SetImportData
// HL означает ручной фрагментатор
#define VRT_FUN(a,b,c) typedef a (*FN##b)c;

// Битовые флаги.
enum {
    CPAGE_TABLE_DEFAULT = 0x0, CPAGE_TABLE_LINE = 0x1, CPAGE_TABLE_TEXT = 0x2
};
VRT_FUN(Bool32, CPAGE_HL_TableExtract, (CPageHandle hPAGE, uint32_t type, Rect32 rect ))

#undef DEC_FUN

#endif
