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
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "new_c.h"
#include "minmax.h"
#include "internal.h"

using namespace cf;

extern Handle hNotUseAntonCross;

CCOM_handle hCcom;
uint32_t NumberOfLettersInArea(Rect32 rect, int Number)
{
    uint32_t Result = 0;
    ROOT *pRoot;

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        if ((pRoot->xColumn + pRoot->nWidth - 1 <= rect.right) && (pRoot->yRow
                                                                   + pRoot->nHeight - 1 <= rect.bottom) && (pRoot->xColumn
                                                                                                            >= rect.left) && (pRoot->yRow >= rect.top) && (pRoot->nBlock
                                                                                                                                                           == Number))
            Result++;
    }

    return Result;
}

Bool32 InsertVertex(PolyBlock * poly, int position, Point point)
{
    poly->insertVertex(position, point);
    return TRUE;
}

Bool32 InsertBottom(const PolyBlock * rectangle, PolyBlock * poly)
{
    poly->insertBottom(rectangle->rect());
    return TRUE;
}

Bool32 InsertTop(const PolyBlock * rectangle, PolyBlock * poly)
{
    poly->insertTop(rectangle->rect());
    return TRUE;
}

Bool32 InsertLeft(const PolyBlock * rectangle, PolyBlock * poly)
{
    poly->insertLeft(rectangle->rect());
    return TRUE;
}

Bool32 InsertRight(const PolyBlock * rectangle, PolyBlock * poly)
{
    poly->insertRight(rectangle->rect());
    return TRUE;
}

Bool32 InsertRectangleInPoly(const PolyBlock * rectangle, PolyBlock * poly)
{
    if (IsInPoly(rectangle->vertexAt(0), poly)) {
        if (IsInPoly(rectangle->vertexAt(1), poly))
            InsertTop(rectangle, poly);
        else
            InsertLeft(rectangle, poly);
    }
    else {
        if (IsInPoly(rectangle->vertexAt(1), poly))
            InsertRight(rectangle, poly);
        else
            InsertBottom(rectangle, poly);
    }

    return TRUE;
}

Bool32 CrossedBy0(PolyBlock * poly, const PolyBlock * rectangle)
{
    poly->crossBy0(rectangle->rect());
    return TRUE;
}

Bool32 CrossedBy1(PolyBlock * poly, const PolyBlock * rectangle)
{
    poly->crossBy1(rectangle->rect());
    return TRUE;
}

Bool32 CrossedBy2(PolyBlock * poly, const PolyBlock * rectangle)
{
    poly->crossBy2(rectangle->rect());
    return TRUE;
}

Bool32 CrossedBy3(PolyBlock * poly, const PolyBlock * rectangle)
{
    poly->crossBy3(rectangle->rect());
    return TRUE;
}

//STEPA_AM
int GetPOLYHeight(const PolyBlock* poly)
{
    return poly->calcHeight();
}

Bool32 PolysIsCrossed(PolyBlock * poly1, const PolyBlock * poly2, Bool32 pic)
{
    //STEPA_AM
    //Пока так определяем кто врезаемый, а кто обтекаемый,
    //Но правильность этого сомнительна
    if (!pic) {
        if (GetPOLYHeight(poly1) < GetPOLYHeight(poly2))
            return TRUE;
    }

    //  if(NumberOfLettersInArea(rect, poly1) < NumberOfLettersInArea(rect, poly2))
    //      return TRUE;

    if (IsInPoly(poly2->vertexAt(0), poly1)) {
        CrossedBy0(poly1, poly2);
        return TRUE;
    }

    if (IsInPoly(poly2->vertexAt(1), poly1)) {
        CrossedBy1(poly1, poly2);
        return TRUE;
    }

    if (IsInPoly(poly2->vertexAt(2), poly1)) {
        CrossedBy2(poly1, poly2);
        return TRUE;
    }

    if (IsInPoly(poly2->vertexAt(3), poly1)) {
        CrossedBy3(poly1, poly2);
        return TRUE;
    }

    return TRUE;
}

Bool32 PageRoatateBlocks(CPageHandle hPage)
{
    CBlockHandle hBlock;
    PolyBlock block;
    int i, j;
    div_t d1;
    int defect;
    // Теперь мы знаем угол, и можем повернуть картинки
    j = 0;
    hBlock = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE);

    while (hBlock != NULL) {
        j++;
        CPAGE_GetBlockData(hBlock, TYPE_IMAGE, &block, sizeof(block));

        if (block.vertexCount() == 4) {
            if (nIncline >= 0) {
                defect = ((block.vertexX(1) - block.vertexX(0)) * nIncline) / INCLINE_FACTOR;
                block.moveVertexY(1, defect);
                block.moveVertexY(3, -defect);
                defect = ((block.vertexY(3) - block.vertexY(0)) * nIncline) / INCLINE_FACTOR;
                block.moveVertexX(0, defect);
                block.moveVertexX(2, -defect);
            }
            else {
                defect = ((block.vertexX(1) - block.vertexX(0)) * nIncline) / INCLINE_FACTOR;
                block.moveVertexY(0, -defect);
                block.moveVertexY(2, defect);
                defect = ((block.vertexY(3) - block.vertexY(0)) * nIncline) / INCLINE_FACTOR;
                block.moveVertexX(1, defect);
                block.moveVertexX(3, -defect);
            }
        }

        block.rotateVertexesToIdeal(nIncline);

        /*********************/

        for (i = 0; i < block.vertexCount() - 1; i++) {
            d1 = div(i, 2);

            if (d1.rem == 0) {
                if (block.vertexY(i) != block.vertexY(i + 1))
                    block.setVertexY(i + 1, block.vertexY(i));
            }
            else {
                if (block.vertexX(i) != block.vertexX(i + 1)) {
                    // NOTE: is right argument order?
                    block.setVertexX(i, block.vertexX(i + 1));
                }
            }
        }

        if (block.vertexX(0) != block.vertexX(block.vertexCount() - 1))
            block.setVertexX(block.vertexCount() - 1, block.vertexX(0));

        /*********************/
        sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
                block.vertexX(0), block.vertexY(0),
                block.vertexX(1), block.vertexY(1),
                block.vertexY(2) - block.vertexY(1));
        LDPUMA_FPuts(resFile_pict, tmp_str);
        /*********************/
        CPAGE_SetBlockData(hBlock, TYPE_IMAGE, &block, sizeof(block));
        hBlock = CPAGE_GetBlockNext(hPage, hBlock, TYPE_IMAGE);
    }

    sprintf(tmp_str, "  <4 Р %d %d %d \n", j, 0, 0);
    LDPUMA_FPuts(resFile_pict, tmp_str);
    return TRUE;
}

void PageLayoutBlocks(CCOM_handle hCCOM)
{
    CPageHandle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPageNumber());
    file_name = LDPUMA_GetFileName(NULL);
    hCcom = hCCOM;

    if (ReadRoots(hCCOM, TRUE)) {
        LayoutPart1();
        LDPUMA_FPuts(resFile_pict, "  <4 Н После поворота \n");
        PageRoatateBlocks(hPage);
        LDPUMA_FPuts(resFile_pict, "  <4 К После поворота \n");
        LDPUMA_FPuts(resFile_pict, "  <3 К Картинки \n");
        LDPUMA_FPuts(resFile_pict, "  <2 К Страница \n");
        // ************************************************
        free(CellsPage); // *** Rom 03-03-99
        CellsPage = NULL; // *** Rom 03-03-99
    }
}

Bool32 DeletePoly(PolyBlock * all_polys, int max, int i)
{
    int j;

    //!!! Redart 14.01.08 - Уменьшил с верхнюю границу цикла на 1
    for (j = i; j < max - 1; j++) {
        all_polys[j] = all_polys[j + 1];
    }

    return TRUE;
}
/**/

/**/
Bool32 BlockAnalyse()
{
    BLOCK *p;
    //BLOCK *q;
    //BLOCK *r;
    uint32_t nAverageHeight;
    ROOT *pRoot;
    RecRaster rec;
    RecVersions vs;
    int16_t Language;
    uint32_t i = 0;
    int nRealLetters = 0;
    nAverageHeight = 0;

    if (nRoots == 0)
        return TRUE;

    for (i = 0; i < nRoots; i++) {
        nAverageHeight += pRoots[i].nHeight;
    }

    nAverageHeight /= nRoots;
    //r = malloc(sizeof(BLOCK));
    //r->pNext = pBlocksList;
AGAIN_PQ:

    //for (p = r; p->pNext != NULL; p = p -> pNext)
    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        //if(p->pNext->Rect.yBottom - p->pNext->Rect.yTop < nAverageHeight*0.7)
        if (p->Rect.yBottom - p->Rect.yTop < nAverageHeight * 0.7) {
            /*
             q = p -> pNext;
             p->pNext = p->pNext->pNext;
             free(q);
             */
            BlocksRemoveDescriptor(p);
            goto AGAIN_PQ;
        }

        if ((uint32_t)(p->Rect.xRight - p->Rect.xLeft) < nAverageHeight) {
            BlocksRemoveDescriptor(p);
            goto AGAIN_PQ;
        }

        if ((p->nLetters < 2) && ((p->nAverageHeight < nAverageHeight / 0.7)
                                  || (p->nAverageHeight > nAverageHeight * 0.7))) {
            nRealLetters = 0;

            if (p->nRoots < 20) {
                pRoot = p->pRoots;

                while (pRoot != NULL) {
                    if (!CCOM_GetRaster(static_cast<CCOM_comp*> (pRoot->pComp),
                                        &rec))
                        continue;

                    if (!CCOM_GetLanguage(hCcom, &Language))
                        continue;

                    if (!RSTR_RecogOneLetter(&rec, Language, &vs))
                        continue;

                    if ((vs.lnAltCnt > 0) && (vs.Alt[0].Prob > 240)
                            && (vs.Alt[0].Code != 'i') && (vs.Alt[0].Code
                                                           != 'I') && (vs.Alt[0].Code != 'l')) {
                        nRealLetters++;
                    }

                    pRoot = pRoot->u1.pNext;
                }
            }

            if ((nRealLetters > 1) || (nRealLetters == 1 && p->nRoots < 4)) {
                continue;
            }

            BlocksRemoveDescriptor(p);
            goto AGAIN_PQ;
        }
    }

    return TRUE;
}

Bool32 OutputFragments(CPageHandle hPage)
{
    BLOCK * p = NULL;
    Bool32 rc = TRUE;
    int count;
    uint32_t BlockNumber;
    int32_t Min;
    int32_t PrevMin;
    CBlockHandle hBlock = NULL; // ***** Rom 01-03-99
    CBlockHandle h;
    CBlockHandle h_next;
    int i;
    int j;
    int max;
    int nPics;
    PolyBlock * pPics;
    Bool32 Same;
    PolyBlock block;
    PolyBlock * all_polys;
    LDPUMA_FPuts(resFile_blocks, "  <2 Н Страница =");
    LDPUMA_FPuts(resFile_blocks, file_name);
    LDPUMA_FPuts(resFile_blocks, " \n");
    LDPUMA_FPuts(resFile_blocks, "  <3 Н Фрагменты \n");
    //LDPUMA_FPuts(resFile_blocks, "  <4 Н Перед обработкой пересечений \n");
    LDPUMA_FPuts(resFile_blocks,
                 "  <4 Н Результат работы старого фрагментатора \n");
    BlocksExtract(); // inserted by Rom to remove NULL-sized blocks
    BlockAnalyse();
    all_polys = static_cast<PolyBlock*> (malloc(COMPS_QUANTUM * sizeof(PolyBlock)));
    pPics = static_cast<PolyBlock*> (malloc(PICS_QUANTUM * sizeof(PolyBlock)));
    BlocksBuildLeftAndRightLists();
    BlocksBuildTopAndBottomLists();
    //*************************************************
    count = 0;
    BlockNumber = 0;
    i = 0;

    for (p = pTopBlocksList; p != NULL; p = p -> pDown) {
        //// uliss NOTE: very strange magic number
        if (p->Type == ROM_TYPE_TABLE) {
            //// NOTE: very strange cast
            CPAGE_SetBlockUserNum((CBlockHandle) p->pHystogram, ++BlockNumber);
            continue;
        }

        if (i % COMPS_QUANTUM == 0) {
            all_polys = static_cast<PolyBlock*> (realloc(all_polys, (size_t) ((i
                                                                           / COMPS_QUANTUM + 1) * COMPS_QUANTUM * sizeof(PolyBlock))));
        }

        all_polys[i].setType(TYPE_TEXT);//Текст, Картинка, Таблица;
        //all_polys[i].com.number = ++count;//порядковый номер
        all_polys[i].setNumber(p->nNumber);//порядковый номер
        Rect prect(Point(p->Rect.xLeft, p->Rect.yTop), Point(p->Rect.xRight, p->Rect.yBottom));
        all_polys[i].setRect(prect);
        all_polys[i].setAlphabet(0);
        sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
                all_polys[i].vertexX(0), all_polys[i].vertexY(0),
                all_polys[i].vertexX(1), all_polys[i].vertexY(1),
                all_polys[i].vertexY(2) - all_polys[i].vertexY(1));
        LDPUMA_FPuts(resFile_blocks, tmp_str);
        i++;
    }

    max = i;
    sprintf(tmp_str, "  <4 Р %d %d %d \n", i, 0, 0);
    LDPUMA_FPuts(resFile_blocks, tmp_str);
    LDPUMA_FPuts(resFile_blocks,
                 "  <4 К Результат работы старого фрагментатора \n");
    i = 0;

    for (h = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE); h != NULL; h
            = CPAGE_GetBlockNext(hPage, h, TYPE_IMAGE)) {
        if (i % COMPS_QUANTUM == 0) {
            pPics = static_cast<PolyBlock*> (realloc(pPics, (size_t) ((i
                                                                   / COMPS_QUANTUM + 1) * COMPS_QUANTUM * sizeof(PolyBlock))));
        }

        CPAGE_GetBlockData(h, TYPE_IMAGE, &pPics[i++], sizeof(PolyBlock));
    }

    nPics = i;

    // **********************************************
    for (i = 0; i < max; i++) {
        for (j = 0; j < nPics; j++) {
            if (pPics[j].vertexCount() != 4)
                continue;

            block = pPics[j];
            Rect radj = block.rect().adjusted(-BOUND, -BOUND, BOUND, BOUND);
            block.setRect(radj);
            count = 0;

            if (IsInPoly(block.vertexAt(0), &all_polys[i]))
                count++;

            if (IsInPoly(block.vertexAt(1), &all_polys[i]))
                count++;

            if (IsInPoly(block.vertexAt(2), &all_polys[i]))
                count++;

            if (IsInPoly(block.vertexAt(3), &all_polys[i]))
                count++;

            switch (count) {
                case 1:
                    PolysIsCrossed(&all_polys[i], &block, TRUE);
                    break;
                case 2:
                    InsertRectangleInPoly(&block, &all_polys[i]);
                    break;
                case 3:
                    break;
                case 4:
                    break;
                default:
                    break;
            }
        }
    }

    // ******** Processing crossed bloks ************
AGAIN:

    for (i = 0; i < max; i++) {
        for (j = 0; j < max; j++) {
            if (all_polys[j].vertexCount() != 4)
                continue;

            if (i == j)
                continue;

            block = all_polys[j];

            /*
             Check if fragments are not crossed without "bounding"
             */
            if (!(IsInPoly(block.vertexAt(0), &all_polys[i]))
                    && !(IsInPoly(block.vertexAt(1), &all_polys[i]))
                    && !(IsInPoly(block.vertexAt(2), &all_polys[i]))
                    && !(IsInPoly(block.vertexAt(3), &all_polys[i]))) {
                continue;
            }

            /**/
            if ((IsInPoly(block.vertexAt(0), &all_polys[i]))
                    && (IsInPoly(block.vertexAt(1), &all_polys[i]))
                    && (IsInPoly(block.vertexAt(2), &all_polys[i]))
                    && (IsInPoly(block.vertexAt(3), &all_polys[i]))) {
                DeletePoly(all_polys, max, j);

                if (max > 0)
                    max--;

                //goto AGAIN;
            }

            Rect adj = block.rect().adjusted(-BOUND, -BOUND, BOUND, BOUND);
            block.setRect(adj);
            count = 0;

            if (IsInPoly(block.vertexAt(0), &all_polys[i]))
                count++;

            if (IsInPoly(block.vertexAt(1), &all_polys[i]))
                count++;

            if (IsInPoly(block.vertexAt(2), &all_polys[i]))
                count++;

            if (IsInPoly(block.vertexAt(3), &all_polys[i]))
                count++;

            switch (count) {
                case 1:
                    if (!LDPUMA_Skip(hNotUseAntonCross))
                        PolysIsCrossed(&all_polys[i], &block, TRUE);
                    else
                        PolysIsCrossed(&all_polys[i], &block, FALSE);
                    break;
                case 2:
                    if (all_polys[i].alphabet() != block.number()) {
                        InsertRectangleInPoly(&block, &all_polys[i]);
                        all_polys[i].setAlphabet(block.number());
                        break;
                    }

                    else {
                        goto AGAIN;
                    }

                case 3:
                    break;
                case 4:
                    break;
                default:
                    break;
            }
        }
    }

    LDPUMA_FPuts(resFile_blocks, "  <4 Н Финал \n");
    PrevMin = -2000;
    Min = -1000;

    while (Min != PrevMin) {
        PrevMin = Min;
        Min = 65535;

        for (i = 0; i < max; i++) {
            if ((all_polys[i].vertexY(0) < Min)
                    && (all_polys[i].vertexY(0) > PrevMin)) {
                Min = all_polys[i].vertexY(0);
            }
        }

        for (i = 0; i < max; i++) {
            if (all_polys[i].vertexY(0) == Min) {
                all_polys[i].setNumber(++BlockNumber);
            }
        }
    }

    j = 0;

    for (i = 0; i < max; i++) {
        all_polys[i].setAlphabet(0);
        BlockNumber = all_polys[i].number();
        all_polys[i].setNumber(0);
        all_polys[i].setLight(TYPE_POSITIVE);
        all_polys[i].setOrientation(TYPE_LEFTRIGHT);

        if (all_polys[i].vertexCount() == 4) {
            j++;
            sprintf(tmp_str, "  <4 О 1 %4d %4d %4d %4d %d \n",
                    all_polys[i].vertexX(0), all_polys[i].vertexY(0),
                    all_polys[i].vertexX(1), all_polys[i].vertexY(1),
                    all_polys[i].vertexY(2) - all_polys[i].vertexY(1));
            LDPUMA_FPuts(resFile_blocks, tmp_str);
        }

        hBlock = CPAGE_CreateBlock(hPage, TYPE_TEXT, 0, 0, &all_polys[i], sizeof(PolyBlock));

        if (!hBlock) {
            SetReturnCode_rblock(CPAGE_GetReturnCode());
            rc = FALSE;
            break;
        }

        CPAGE_SetBlockUserNum(hBlock, BlockNumber);
    }

    sprintf(tmp_str, "  <4 Р %d %d %d \n", j, 0, 0);
    LDPUMA_FPuts(resFile_blocks, tmp_str);
    LDPUMA_FPuts(resFile_blocks, "  <4 К Финал \n");
    LDPUMA_FPuts(resFile_blocks, "  <3 К Фрагменты \n");
    LDPUMA_FPuts(resFile_blocks, "  <2 К Страница \n");
    /*Numeraciya Kartinok*/
    PrevMin = -2000;
    Min = -1000;

    while (Min != PrevMin) {
        PrevMin = Min;
        Min = 65535;

        for (i = 0; i < nPics; i++) {
            if ((pPics[i].vertexY(0) < Min)
                    && (pPics[i].vertexY(0) > PrevMin)) {
                Min = pPics[i].vertexY(0);
            }
        }

        for (i = 0; i < nPics; i++) {
            if (pPics[i].vertexY(0) == Min) {
                pPics[i].setNumber(++BlockNumber);
            }
        }
    }

    for (h = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE); h != NULL; h
            = CPAGE_GetBlockNext(hPage, h, TYPE_IMAGE)) {
        CPAGE_GetBlockData(h, TYPE_IMAGE, &block, sizeof(PolyBlock));

        for (i = 0; i < nPics; i++) {
            Same = TRUE;

            for (j = 0; j < pPics[i].vertexCount(); j++) {
                if (pPics[i].vertexAt(j) != block.vertexAt(j)) {
                    Same = FALSE;
                    break;
                }
            }

            if (Same) {
                BlockNumber = pPics[i].number();
                pPics[i].setNumber(0);
                CPAGE_SetBlockUserNum(h, BlockNumber);
            }
        }
    }

    //Негативы
    h = CPAGE_GetBlockFirst(hPage, TYPE_IMAGE);

    while (h) {
        h_next = CPAGE_GetBlockNext(hPage, h, TYPE_IMAGE);
        CPAGE_GetBlockData(h, TYPE_IMAGE, &block, sizeof(PolyBlock));

        if (block.isNegative()) {
            BlockNumber = CPAGE_GetBlockUserNum(h);
            CPAGE_DeleteBlock(hPage, h);
            block.setAlphabet(0);
            block.setNumber(0);
            hBlock = CPAGE_CreateBlock(hPage, TYPE_TEXT, 0, 0, &block,
                                       sizeof(PolyBlock));
            CPAGE_SetBlockUserNum(hBlock, BlockNumber);
        }

        h = h_next;
    }

    LT_FreeAllData();
    //CPAGE_BackUp(hPage); //Rom
    free(all_polys);
    free(pPics);
    return rc;
}

void CalculatePageIncline(Handle hCCOM, int32_t * lpNominator,
                          int32_t * lpDenominator)
{
    *lpDenominator = INCLINE_FACTOR;
    *lpNominator = nIncline;
}

int IsInPoly(const Point& a, const PolyBlock * p)
{
    return p->isInPoly(a);
}

int IsInPoly(const Point16& a, const PolyBlock *pPoly)
{
    Point p32;
    p32 = a;
    return IsInPoly(p32, pPoly);
}

