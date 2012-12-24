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

#include "resource.h"
#include "mymem.h"
#include "cpage.h"
#include "backup.h"
#include "tableclass.h"
#include "compat_defs.h"
#include "internal.h"

using namespace cf;

Handle CPAGE_ExTableCreate(CPageHandle hPage, int32_t Skew2048, uint32_t nVer,
                           int32_t * lpVCor, uint32_t nHor, int32_t * lpHCor)
{
    SetReturnCode_cpage(IDS_ERR_NO);
    CBlockHandle block = NULL;
    Handle rc = NULL;
    Bool32 res = FALSE;
    TableClass tc;

    if (tc.Create(Skew2048, nVer, lpVCor, nHor, lpHCor))
        block = tc.Store(hPage);

    rc = block ? TableClass::Attach(hPage, block) : NULL;
    return rc;
}

void CPAGE_ExTableDelete(Handle hTable)
{
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc)
        tc->Remove();
}

Handle CPAGE_ExTableGetFirst(CPageHandle hPage)
{
    Handle rc = NULL;
    CBlockHandle hBlock = NULL;
    SetReturnCode_cpage(IDS_ERR_NO);
    Handle Type = CPAGE_GetInternalType("TableClass");

    if (!CPAGE_UpdateBlocks(hPage, Type)) {
        rc = NULL;
        goto lOut;
    }

    hBlock = CPAGE_GetBlockFirst(hPage, Type);
    rc = hBlock ? TableClass::Attach(hPage, hBlock) : NULL;
lOut:
    return rc;
}

Handle CPAGE_ExTableGetNext(Handle hTable)
{
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc)
        tc = tc->GetNext();

    return tc;
}

Bool32 CPAGE_ExGeTableGetNumberCells(Handle hTable, int32_t * lpNumber)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpNumber = tc->GetNumberGeCell();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExPhTableGetNumberCells(Handle hTable, int32_t * lpNumber)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpNumber = tc->GetNumberPhCell();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExTableGetNumberRow(Handle hTable, int32_t * lpNumber)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpNumber = tc->GetNumberRow();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExTableGetNumberColumn(Handle hTable, int32_t * lpNumber)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpNumber = tc->GetNumberColumn();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExGeTableGetSizeCell(Handle hTable, Point point, Rect32 * lpRect)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    Rect32 rect = { 0 };
    assert(lpRect);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        rect.left = tc->GetVLine(point.x());
        rect.right = tc->GetVLine(point.x() + 1);
        rect.top = tc->GetVLine(point.y());
        rect.bottom = tc->GetVLine(point.y() + 1);
        *lpRect = rect;
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExPhTableGetSizeCell(Handle hTable, Point point, Rect32 * lpRect)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        // НАДО СДЕЛАТЬ !!!!
        // TODO         !!!!
    }

    return rc;
}

Bool32 CPAGE_ExTableGetNumberBlock(Handle hTable, Point point,
                                   int32_t * lpNumber)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpNumber);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        Point phPoint = tc->GetCell(point).Point();
        *lpNumber = tc->GetCell(phPoint);
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExGeTableGetPhysical(Handle hTable, Point point,
                                  Point * lpPoint)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpPoint);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpPoint = tc->GetCell(point).Point();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExPhTableGetNumberGeometry(Handle hTable, Point point,
                                        int32_t * lpNumber)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpNumber);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        Point PhCoord = tc->GetCell(point).Point();
        *lpNumber = tc->GetCell(PhCoord).GetGeCount();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExPhTableGetGeometry(Handle hTable, Point point, int32_t count,
                                  Point * lpPoint)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpPoint);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        // НАДО СДЕЛАТЬ !
        // TODO         !
    }

    return rc;
}

Bool32 CPAGE_ExPhTableSetNumberBlock(Handle hTable, Point point,
                                     int32_t number)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        Point PhCoord = tc->GetCell(point).Point();
        tc->GetCell(PhCoord) = number;
    }

    return rc;
}

Bool32 CPAGE_ExTableIsPhysicCell(Handle hTable, Point point,
                                 Bool32 * lpIsPhysic)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpIsPhysic);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpIsPhysic = tc->GetCell(point).IsPhysic();
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExTableSize(Handle hTable, Rect32 * lpRect)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpRect);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        lpRect->left = tc->GetVLine(0);
        lpRect->right = tc->GetVLine(tc->GetNumberColumn());
        lpRect->top = tc->GetHLine(0);
        lpRect->bottom = tc->GetHLine(tc->GetNumberRow());
        rc = TRUE;
    }

    return rc;
}

Bool32 CPAGE_ExTableGetSkew(Handle hTable, int32_t * lpNumerator,
                            int32_t * lpDenominator)
{
    Bool32 rc = FALSE;
    SetReturnCode_cpage(IDS_ERR_NO);
    assert(lpNumerator);
    assert(lpDenominator);
    TableClass * tc = (TableClass *) hTable;

    if (tc) {
        *lpNumerator = tc->GetSkew2048();
        *lpDenominator = 2048;
        rc = TRUE;
    }

    return rc;
}
