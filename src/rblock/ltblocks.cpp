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

/****************************************************************************
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTBLOCKS.C - Blocks forming                                 *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "c_memory.h"
# include "layout.h"
# include "lang.h"

#include "dpuma.h" //George, 12.11.01 (dd.mm.yy)
#include "minmax.h"

extern Handle hSetTrivAverHeight;//George, 12.11.01 (dd.mm.yy)

BLOCK *pBlocksList = NULL;
BLOCK *pHighEmbeddingBlocksList = NULL;
BLOCK *pLowEmbeddingBlocksList = NULL;
BLOCK *pLeftBlocksList = NULL;
BLOCK *pRightBlocksList = NULL;
BLOCK *pTopBlocksList = NULL;
BLOCK *pBottomBlocksList = NULL;
int nNextBlockNumber;

// создание нового блока или инициализация самого списка блоков
BLOCK *BlocksAddDescriptor(void) {
    if (pBlocksList == NULL) {
        pBlocksList = static_cast<BLOCK*> (malloc(sizeof(BLOCK)));
        if (pBlocksList == NULL) {
#ifdef DebugFile
            strcpy(StringError,"BlocksAddDescriptor,if(pBlocksList==NULL)");
#endif
            ErrorNoEnoughMemory("in LTBLOCKS.C,BlockAddDescriptor;part 1");
        }

        memset(pBlocksList, 0, sizeof(BLOCK));
    }
    else {
        pBlocksList -> pPrev = static_cast<BLOCK*> (malloc(sizeof(BLOCK)));
        if (pBlocksList -> pPrev == NULL) {
#ifdef DebugFile
            strcpy(StringError,"BlocksAddDescriptor,if(pPrev==NULL)");
#endif
            ErrorNoEnoughMemory("in LTBLOCKS.C,BlockAddDescriptor;part 2");
        }

        memset(pBlocksList -> pPrev, 0, sizeof(BLOCK));
        pBlocksList -> pPrev -> pNext = pBlocksList;
        pBlocksList = pBlocksList -> pPrev;
    }

    pBlocksList->language = language; // Pit 02-??-94

    return (pBlocksList);
}

void BlocksRemoveDescriptor(BLOCK *p) {
    if (p != NULL) {
        if (p == pBlocksList)
            pBlocksList = p -> pNext;

        if (p == pHighEmbeddingBlocksList)
            pHighEmbeddingBlocksList = p -> pLowerEmbedding;

        if (p == pLowEmbeddingBlocksList)
            pLowEmbeddingBlocksList = p -> pHigherEmbedding;

        if (p == pLeftBlocksList)
            pLeftBlocksList = p -> pRight;

        if (p == pRightBlocksList)
            pRightBlocksList = p -> pLeft;

        if (p == pTopBlocksList)
            pTopBlocksList = p -> pDown;

        if (p == pBottomBlocksList)
            pBottomBlocksList = p -> pUp;

        //	if (p -> pChildren != NULL)	// ATAL - no more children list
        //	    free (p -> pChildren);

        if (p -> pHorzHystogram != NULL)
            free(p -> pHorzHystogram);

        if (p -> pNext != NULL)
            p -> pNext -> pPrev = p -> pPrev;
        if (p -> pPrev != NULL)
            p -> pPrev -> pNext = p -> pNext;

        if (p -> pLowerEmbedding != NULL)
            p -> pLowerEmbedding -> pHigherEmbedding = p -> pHigherEmbedding;
        if (p -> pHigherEmbedding != NULL)
            p -> pHigherEmbedding -> pLowerEmbedding = p -> pLowerEmbedding;

        if (p -> pRight != NULL)
            p -> pRight -> pLeft = p -> pLeft;
        if (p -> pLeft != NULL)
            p -> pLeft -> pRight = p -> pRight;

        if (p -> pDown != NULL)
            p -> pDown -> pUp = p -> pUp;
        if (p -> pUp != NULL)
            p -> pUp -> pDown = p -> pDown;

        free(p);
    }
}

void TryingAddBorderDust(BLOCK *p, ROOT *pRoot, int32_t *yTop, int32_t *yBottom, int32_t *xLeft,
        int32_t *xRight) {

    if (pRoot -> xColumn < p -> Rect.left() - p->nAverageHeight)
        return;

    if (pRoot -> xColumn + pRoot -> nWidth - 1 > p -> Rect.right() + p->nAverageHeight)
        return;

    if (pRoot -> yRow < p -> Rect.top() - p->nAverageHeight)
        return;

    if (pRoot -> yRow + pRoot -> nHeight - 1 > p -> Rect.bottom() + p->nAverageHeight / 2)
        return;

    pRoot -> nBlock = p -> nNumber;

    if (pRoot -> xColumn < *xLeft)
        *xLeft = pRoot -> xColumn;

    if (pRoot -> xColumn + pRoot -> nWidth > *xRight)
        *xRight = pRoot -> xColumn + pRoot -> nWidth;

    if (pRoot -> yRow < *yTop)
        *yTop = pRoot -> yRow;

    if (pRoot -> yRow + pRoot -> nHeight - 1 > *yBottom)
        *yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
}
// ***************************************************


// вызывается в цикле по рутам:
// в каждом блоке, которому принадлежит рут,
// в числе прочего (кол-ва компонент типа) накапливаются:
// - сумма высот входящих в блок рутов (??);
// - обтягивающий прямоугольник;
void BlockAccountRoot(BLOCK *p, ROOT *pRoot) {
    // Piter
    if (IS_LAYOUT_DUST(*pRoot)) {
        p -> nDust++;
        //return; //rom
    }
    // end Piter

    // инициализация. если этот рут ("основной пр-к") -- первый.
    if (p -> nRoots == 0) {
        p -> nRoots = 0; // ??? ср. с условием (if())
        p -> nLetters = 0;
        p -> nDust = 0;
        p -> nHeightesSum = pRoot -> nHeight;

        p -> pRoots = pRoot;
        p -> pEndRoots = pRoot;
        pRoot -> u1.pNext = NULL; // руты лежат в памяти
        // последовательно, ограниченные сверху указателем
        // pAfterRoots, поэтому утери данных не произойдет.
        // начиная с этого момента, руты переписываются
        // в поблочные списки (в обратном порядке).

        p -> Rect.rleft() = pRoot -> xColumn;
        p -> Rect.rright() = pRoot -> xColumn + pRoot -> nWidth - 1;
        p -> Rect.rtop() = pRoot -> yRow;
        p -> Rect.rbottom() = pRoot -> yRow + pRoot -> nHeight - 1;
    }
    else {
        if (pRoot == p -> pRoots)// Piter
            return;

        pRoot -> u1.pNext = p -> pRoots; // то есть на предыдущий рут

        p -> pRoots = pRoot;
        p -> nHeightesSum += pRoot -> nHeight;// Piter

        // растяжение обтягивающего прямоугольника на текущий рут:
        if (pRoot -> xColumn < p -> Rect.left())
            p -> Rect.rleft() = pRoot -> xColumn;

        if (pRoot -> xColumn + pRoot -> nWidth - 1 > p -> Rect.right())
            p -> Rect.rright() = pRoot -> xColumn + pRoot -> nWidth - 1;

        if (pRoot -> yRow < p -> Rect.top())
            p -> Rect.rtop() = pRoot -> yRow;

        if (pRoot -> yRow + pRoot -> nHeight - 1 > p -> Rect.bottom())
            p -> Rect.rbottom() = pRoot -> yRow + pRoot -> nHeight - 1;
    }

    p -> nRoots++;

    if (pRoot -> bType & ROOT_LETTER)
        p -> nLetters++;
}

// если пылевых блоков столько же, сколько основных --
// то считается "высота среднего блока".
// иначе считается величина, смысл которой туманен.
// могу только сказать, что в определении нет непрерывности
// при nDust->nRoots (ибо среднее стремится к нулю).
void BlockSetAverageHeight(BLOCK *p) {
    if (!LDPUMA_Skip(hSetTrivAverHeight)) {
        // вычисление средних по стандарной формуле
        if (p -> nRoots == 0)
            p -> nAverageHeight = 1;
        else
            p -> nAverageHeight = MAX(p -> nHeightesSum / p -> nRoots, 1);

        // хотя.. если считать, что пыль образована из осколков
        // букв, то среднее разумно чуть увеличить.. только вот --
        // -- почему пропорционально nRoots/(nRoots-nDust) ?
    }
    else // старый вариант по умолчанию:
    {
        if (p -> nRoots - p -> nDust == 0) {
            if (p -> nRoots == 0)
                p -> nAverageHeight = 1;
            else
                p -> nAverageHeight = MAX(p -> nHeightesSum / p -> nRoots, 1);
        }
        else {
            // меня смущает, что при превышении числом пылинок
            // числа основных блоков среднее будет отрицательно.
            p -> nAverageHeight = MAX(p -> nHeightesSum / (p -> nRoots
                            - p -> nDust), 1);
            // то есть добавляя пылинку (точку), я почему-то
            // увеличиваю среднюю высоту. я (GF) отказываюсь это понимать.

            // хотя.. если считать, что пыль образована из осколков
            // букв, то среднее разумно чуть увеличить.. только вот --
            // -- почему пропорционально nRoots/(nRoots-nDust) ?
        }
    }
}

// на деле -- здесь, похоже, собирается первоначальный (единственный)
// текстовый блок -- которому еще предстоит быть разделенным..
void BlocksExtract() {
    BLOCK *p;
    ROOT *pRoot;

    BlocksFreeData();

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        // мелочь пропускаем
        if (pRoot -> nBlock == DUST_BLOCK_NUMBER || pRoot -> nBlock == REMOVED_BLOCK_NUMBER) {
            continue;
        }

        // по списку текстовых блоков происходит накопление
        // суммы высот рутов (?) и создается обтягивающий
        // эту группу рутов прямоугольник
        for (p = pBlocksList; p != NULL; p = p -> pNext) {
            if (p -> nNumber == pRoot -> nBlock) {
                BlockAccountRoot(p, pRoot);
                // перейти на следующий оборот внешнего цикла
                goto FOUND;
                //"break здесь и continue во внешнем"
            }
        }

        /* Block descriptor not found */
        p = BlocksAddDescriptor(); // создание нового блока
        // или инициализация самого списка блоков

        p -> Type = BLOCK_TEXT;
        p -> nNumber = pRoot -> nBlock;

        p -> nUserNum = pRoot ->nUserNum; // Piter
        BlockAccountRoot(p, pRoot);

        FOUND: ;
    }

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        // здесь происходит нечто странное, но,
        // полагая число пылинок (<=8x8) << числа рутов,
        // можем считать, что здесь вычисляется средняя
        // высота входящего в блок рута.
        BlockSetAverageHeight(p);
        BlockCalculateBreakingParameters(p);
    }
}

void BlockSetRootsNumbers(BLOCK *p) {
    ROOT *pRoot;

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
        pRoot -> nBlock = p -> nNumber;
}

void BlocksSetRootsNumbers(void) {
    BLOCK *p;

    for (p = pBlocksList; p != NULL; p = p -> pNext)
        BlockSetRootsNumbers(p);
}

# ifdef LT_DEBUG
void BlocksSetRandomRootsNumbers(void) {
    ROOT *pRoot;

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        if (pRoot -> nBlock != DUST_BLOCK_NUMBER && pRoot -> nBlock != REMOVED_BLOCK_NUMBER) {
            pRoot -> nBlock = rand() % 128 + FIRST_REGULAR_BLOCK_NUMBER;
        }
    }
}
# endif

BLOCK * BlockBuildNewWithoutRoot(BLOCK *p, ROOT *pRemovedRoot) {
    BLOCK *pNew;
    ROOT *pRoot, *pNext;

    if (pRemovedRoot == NULL)
        return (NULL);

    pRemovedRoot -> nBlock = REMOVED_BLOCK_NUMBER;

    pNew = BlocksAddDescriptor();
    pNew -> nNumber = ++nNextBlockNumber;
    pNew -> Type = BLOCK_TEXT;

    if (p -> uFlags & BF_SMART_BREAKING_APPLIED)
        pNew -> uFlags |= BF_SMART_BREAKING_APPLIED;

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pNext) {
        pNext = pRoot -> u1.pNext;

        if (pRoot != pRemovedRoot)
            BlockAccountRoot(pNew, pRoot);
    }

    if (pNew -> nRoots != 0) {
        BlockSetAverageHeight(pNew);
        BlockCalculateBreakingParameters(pNew);
    }
    else {
        BlocksRemoveDescriptor(pNew);
        pNew = NULL;
    }

    BlocksRemoveDescriptor(p);
    return (pNew);
}

static ROOT *GetFrameLikeRoot(BLOCK *p) {
    ROOT *pRoot;

    if (p -> Type != BLOCK_TEXT)
        return (NULL);

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext) {
        if (pRoot -> xColumn == p -> Rect.left() && pRoot -> yRow == p -> Rect.top()
                && pRoot -> xColumn + pRoot -> nWidth - 1 == p -> Rect.right() && pRoot -> yRow
                + pRoot -> nHeight - 1 == p -> Rect.bottom()) {
            return (pRoot);
        }
    }

    return (NULL);
}

void BlocksRemoveFrameLikeRoots(void) {
    BLOCK *p;
    BLOCK *pBuilded;
    ROOT *pFrameLikeRoot;

    AGAIN: for (p = pBlocksList; p != NULL; p = p -> pNext) {
        if (p -> Type != BLOCK_TEXT || (p -> uFlags & BF_NOT_REMOVE_FRAME_LIKE_ROOTS)) {
            continue;
        }

        pFrameLikeRoot = GetFrameLikeRoot(p);

        if (pFrameLikeRoot == NULL)
            continue;

        if (p -> nRoots - p -> nDust > 3 && pFrameLikeRoot -> nWidth > 2
                * pFrameLikeRoot -> nHeight) {
            pBuilded = BlockBuildNewWithoutRoot(p, pFrameLikeRoot);

            if (pBuilded != NULL)
                pBuilded -> uFlags |= BF_NOT_REMOVE_FRAME_LIKE_ROOTS;

            goto AGAIN;
        }
    }
}

void BlocksAbsorbDust(void) {
    BLOCK *p;
    ROOT *pRoot;
    ROOT *pRootsBegin, *pRootsAfter;
    int32_t xLeft, yTop, yBottom, xRight;

    BlocksSetRootsNumbers();

    for (p = pHighEmbeddingBlocksList; p != NULL; p = p -> pLowerEmbedding) {
        if (p -> Type != BLOCK_TEXT)
            continue;

        RootStripsGetLoopParameters(p -> Rect.top(), p -> Rect.bottom(), &pRootsBegin, &pRootsAfter);

        if (pRootsBegin == NULL)
            continue;

        for (pRoot = pRootsBegin; pRoot < pRootsAfter; pRoot++) {
            if (pRoot -> nBlock == DUST_BLOCK_NUMBER && pRoot -> yRow >= p -> Rect.top()
                    && pRoot -> xColumn >= p -> Rect.left() && pRoot -> yRow + pRoot -> nHeight - 1
                    <= p -> Rect.bottom() && pRoot -> xColumn + pRoot -> nWidth - 1
                    <= p -> Rect.right() && !(PageMatrixFlagsByIdealXY(pRoot -> xColumn,
                    pRoot -> yRow) & PMC_PICTURE)) {
                BlockAccountRoot(p, pRoot);
                pRoot -> nBlock = p -> nNumber;
            }
        }
    }

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        yTop = p -> Rect.top();
        xLeft = p -> Rect.left();
        yBottom = p -> Rect.bottom();
        xRight = p -> Rect.right();

        // ******************** Rom 23-02-99 ***************
        // Trying include border dust
        for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
            if (IS_LAYOUT_DUST(*pRoot)) {
                TryingAddBorderDust(p, pRoot, &yTop, &yBottom, &xLeft, &xRight);
            }
        }
        p -> Rect.rtop() = yTop;
        p -> Rect.rleft() = xLeft;
        p -> Rect.rbottom() = yBottom;
        p -> Rect.rright() = xRight;
        // *************************************************
    }
}

void BlocksDisAbsorbBoundaryDust(void) /* STDG4, STDG19 */
{
    BLOCK *p, *q;
    ROOT *pRoot, *pPrev;
    CIF::Rect rExt;
    int dxExt, dyExt;
    ROOT *pRootsBegin, *pRootsAfter;

    BlocksSetRootsNumbers();

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        for (q = p -> pNext; q != NULL; q = q -> pNext) {
            if (p->Rect.intersects(q -> Rect)) {
                p -> uFlags |= BF_CROSSED;
                q -> uFlags |= BF_CROSSED;
            }
        }
    }

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        if (p -> Type != BLOCK_TEXT || !(p -> uFlags & BF_CROSSED))
            continue;

        dxExt = 3 * MAX(p -> nAverageHeight, MAX_DUST_WIDTH);
        dyExt = MAX(p -> nAverageHeight, MAX_DUST_HEIGHT);

        rExt.rleft() = p -> Rect.left() - dxExt;
        rExt.rtop() = p -> Rect.top() - dyExt;
        rExt.rright() = p -> Rect.right() + dxExt;
        rExt.rbottom() = p -> Rect.bottom() + dyExt;

        RootStripsGetLoopParameters(rExt.top(), rExt.bottom(), &pRootsBegin, &pRootsAfter);

        if (pRootsBegin == NULL)
            continue;

        for (pRoot = pRootsBegin; pRoot < pRootsAfter; pRoot++) {
            if (pRoot -> nBlock != p -> nNumber && pRoot -> nBlock != DUST_BLOCK_NUMBER
                    && IS_LAYOUT_DUST(*pRoot) && pRoot -> yRow >= rExt.top() && pRoot -> xColumn
                    >= rExt.left() && pRoot -> yRow + pRoot->nHeight - 1 <= rExt.bottom()
                    && pRoot -> xColumn + pRoot->nWidth - 1 <= rExt.right()) {
                pRoot -> nBlock = DUST_BLOCK_NUMBER;
            }
        }
    }

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        if (p -> Type != BLOCK_TEXT)
            continue;

        while (p -> pRoots != NULL && p -> pRoots -> nBlock == DUST_BLOCK_NUMBER) {
            p -> nRoots--;
            p -> nDust--;

            if (p -> pRoots -> bType & ROOT_LETTER)
                p -> nLetters--;

            p -> pRoots = p -> pRoots -> u1.pNext;
        }

        if (p -> pRoots == NULL) {
            p -> pEndRoots = NULL;
            continue;
        }

        pPrev = p -> pRoots;
        pRoot = p -> pRoots -> u1.pNext;

        while (pRoot != NULL) {
            if (pRoot -> nBlock == DUST_BLOCK_NUMBER) {
                p -> nRoots--;
                p -> nDust--;

                if (pRoot -> bType & ROOT_LETTER)
                    p -> nLetters--;

                pPrev -> u1.pNext = pRoot -> u1.pNext;
                pRoot = pRoot -> u1.pNext;
            }
            else {
                pPrev = pRoot;
                pRoot = pRoot -> u1.pNext;
            }
        }

        if (p -> pEndRoots != pPrev)
            p -> pEndRoots = pPrev;
    }
}

void BlocksBuildEmbeddingLists() {
    BLOCK *p;
    BLOCK *pInserted;

    pHighEmbeddingBlocksList = NULL;
    pLowEmbeddingBlocksList = NULL;

    for (;;) {
        for (p = pBlocksList; p != NULL; p = p -> pNext) {
            if (p -> Type != BLOCK_TEXT)
                continue;

            if (!(p -> uFlags & BF_INSERTED_INTO_EMBEDDING_LISTS))
                break;
        }

        if (p == NULL)
            break;

        pInserted = p;

        for (; p != NULL; p = p -> pNext) {
            if (!(p -> uFlags & BF_INSERTED_INTO_EMBEDDING_LISTS) && p -> nEmbedding
                    > pInserted -> nEmbedding) {
                pInserted = p;
            }
        }

        if (pHighEmbeddingBlocksList == NULL) {
            pLowEmbeddingBlocksList = pInserted;
        }
        else {
            pHighEmbeddingBlocksList -> pHigherEmbedding = pInserted;
        }

        pInserted -> uFlags |= BF_INSERTED_INTO_EMBEDDING_LISTS;
        pInserted -> pLowerEmbedding = pHighEmbeddingBlocksList;
        pHighEmbeddingBlocksList = pInserted;
    }
}

void BlocksBuildLeftAndRightLists() {
    BLOCK *p;
    BLOCK *pInserted;

    pLeftBlocksList = NULL;
    pRightBlocksList = NULL;

    for (;;) {
        for (p = pBlocksList; p != NULL; p = p -> pNext) {
            if (!(p -> uFlags & BF_INSERTED_INTO_LEFT_AND_RIGHT_LISTS))
                break;
        }

        if (p == NULL)
            break;

        pInserted = p;

        for (; p != NULL; p = p -> pNext) {
            if (!(p -> uFlags & BF_INSERTED_INTO_LEFT_AND_RIGHT_LISTS) && p -> Rect.left()
                    > pInserted -> Rect.left()) {
                pInserted = p;
            }
        }

        if (pLeftBlocksList == NULL) {
            pRightBlocksList = pInserted;
        }
        else {
            pLeftBlocksList -> pLeft = pInserted;
        }

        pInserted -> uFlags |= BF_INSERTED_INTO_LEFT_AND_RIGHT_LISTS;
        pInserted -> pRight = pLeftBlocksList;
        pLeftBlocksList = pInserted;
    }
}

void BlocksBuildTopAndBottomLists() {
    BLOCK *p;
    BLOCK *pInserted;

    pTopBlocksList = NULL;
    pBottomBlocksList = NULL;

    for (;;) {
        for (p = pBlocksList; p != NULL; p = p -> pNext) {
            if (!(p -> uFlags & BF_INSERTED_INTO_TOP_AND_BOTTOM_LISTS))
                break;
        }

        if (p == NULL)
            break;

        pInserted = p;

        for (; p != NULL; p = p -> pNext) {
            if (!(p -> uFlags & BF_INSERTED_INTO_TOP_AND_BOTTOM_LISTS) && p -> Rect.top()
                    > pInserted -> Rect.top()) {
                pInserted = p;
            }
        }

        if (pTopBlocksList == NULL) {
            pBottomBlocksList = pInserted;
        }
        else {
            pTopBlocksList -> pUp = pInserted;
        }

        pInserted -> uFlags |= BF_INSERTED_INTO_TOP_AND_BOTTOM_LISTS;
        pInserted -> pDown = pTopBlocksList;
        pTopBlocksList = pInserted;
    }
}

void BlocksFreeData(void) {
    while (pBlocksList != NULL)
        BlocksRemoveDescriptor(pBlocksList);
}
