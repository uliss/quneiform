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
 *              LTSMART.C - SmartBreaking service functions                 *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "c_memory.h"
# include "layout.h"
# include "my_mem.h"

# include "dpuma.h"

extern Handle hBlocksBreaking;
uchar *pSB_Matrix = NULL;
int  nSB_Width;
int  nSB_Height;
int  nSB_Size;
int  nSB_CellWidth;
int  nSB_CellHeight;

# define NEEDED_COMP_MIN_WIDTH  8
# define NEEDED_COMP_MIN_HEIGHT 8
# define NEEDED_REMAINED_SQUARE 10

Bool SB_MatrixAllocateBody (BLOCK *p, int nCellWidth, int nCellHeight)
{
    nSB_CellWidth  = nCellWidth;
    nSB_CellHeight = nCellHeight;

    if (nSB_CellWidth < 6 || nSB_CellHeight < 6)
        return (FALSE);

    nSB_Width  = (p -> Rect.xRight  - p -> Rect.xLeft + 1) / nSB_CellWidth  + 1;
    nSB_Height = (p -> Rect.yBottom - p -> Rect.yTop  + 1) / nSB_CellHeight + 1;

    if (nSB_Width  < NEEDED_COMP_MIN_WIDTH ||
        nSB_Height < NEEDED_COMP_MIN_HEIGHT)
    {
        return (FALSE);
    }

    nSB_Size = nSB_Width * nSB_Height;
    pSB_Matrix = malloc (nSB_Size);

    if (pSB_Matrix == NULL)
        ErrorNoEnoughMemory ("in LTSMART.C,SB_MatrixAllocateBody,part 1");

    memset (pSB_Matrix, WHITE_CELL, nSB_Size);
    return (TRUE);
}

Bool SB_MatrixBuild (BLOCK *p, int nCellWidth, int nCellHeight)
{
    ROOT *pRoot;
    RECTANGLE r;
    int y, o;
    int xExtension = nCellWidth  / 2;
    int yExtension = nCellHeight / 4;

# ifdef LT_DEBUG
    pDebugBlock = p;
# endif

    if (! SB_MatrixAllocateBody (p, nCellWidth, nCellHeight))
        return (FALSE);

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        r.xLeft   = (pRoot -> xColumn
                            - p -> Rect.xLeft - xExtension)
                            / nSB_CellWidth;

        if (r.xLeft < 0)
            r.xLeft = 0;

        r.xRight  = (pRoot -> xColumn + pRoot -> nWidth - 1
                            - p -> Rect.xLeft + xExtension)
                            / nSB_CellWidth;

        if (r.xRight >= nSB_Width)
            r.xRight = nSB_Width - 1;

        r.yTop    = (pRoot -> yRow
                            - p -> Rect.yTop - yExtension)
                            / nSB_CellHeight;

        if (r.yTop < 0)
            r.yTop = 0;

        pSB_Matrix [r.yTop * nSB_Width + r.xLeft] = BLACK_CELL;

        r.yBottom = (pRoot -> yRow + pRoot -> nHeight - 1
                            - p -> Rect.yTop + yExtension)
                            / nSB_CellHeight;

        if (r.yBottom >= nSB_Height)
            r.yBottom = nSB_Height - 1;

        for (y = r.yTop, o = y * nSB_Width;
                    y <= r.yBottom;
                        y++, o += nSB_Width)
        {
            memset (pSB_Matrix + o + r.xLeft,
                    BLACK_CELL,
                    (r.xRight - r.xLeft + 1));
        }
    }

    return (TRUE);
}

void SB_MatrixFreeData (void)
{
    if (pSB_Matrix != NULL)
    {
        free (pSB_Matrix);
        pSB_Matrix = NULL;
    }
}

/*
static int CalculateSuspicionCells (COMP *pComp)
{
    STRIP *pStrip;
    uchar  *pBegin, *pEnd, *p;
    int   nSuspicionCells;

    nSuspicionCells = 0;

    for (pStrip = pComp -> pStripsListBegin;
             pStrip != NULL;
                 pStrip = pStrip -> pNext)
    {
        pBegin = pSB_Matrix
                + pStrip -> y * nSB_Width
                + pStrip -> xBegin;

        pEnd   = pSB_Matrix
                + pStrip -> y * nSB_Width
                + pStrip -> xEnd;

        if (pStrip -> y == nSB_Height - 1)
        {
            nSuspicionCells += pStrip -> xEnd - pStrip -> xBegin + 1;
            memset (pBegin, SUSPICION_COMP_CELL, pEnd - pBegin + 1);
        }
        else
        {
            for (p = pBegin; p <= pEnd; p++)
                if (*(p + nSB_Width) == WHITE_CELL)
                {
                    nSuspicionCells++;
                    *p = SUSPICION_COMP_CELL;
                }
        }
    }

    return (nSuspicionCells);
}
*/

int CompsFindCompToCut (COMP **ppResult)
{
    COMP *pResult;
    COMP *p;
    int  nTotalSquare;

    if (pCompsList == NULL || pCompsList -> pNext == NULL)
        return (FCC_CANT_FOUND);

    nTotalSquare = 0;

    for (p = pCompsList; p != NULL; p = p -> pNext)
        nTotalSquare += p -> nSquare;

    pResult = NULL;

    for (p = pCompsList; p != NULL; p = p -> pNext)
    {
        if (p -> xRight  - p -> xLeft + 1 < NEEDED_COMP_MIN_WIDTH ||
            p -> yBottom - p -> yTop  + 1 < NEEDED_COMP_MIN_HEIGHT)
        {
            continue;
        }

        if (pResult == NULL || p -> nSquare > pResult -> nSquare)
            pResult = p;
    }

    if (pResult == NULL ||
        nTotalSquare - pResult -> nSquare < NEEDED_REMAINED_SQUARE)
    {
        return (FCC_NOT_FOUND);
    }

    *ppResult = pResult;
/*
    if (CalculateSuspicionCells (pResult) > pResult -> nSquare / 4)
        return (FCC_FOUND_SUSPICION);
*/
    return (FCC_FOUND);
}

Bool BlockBreakByMatrix (BLOCK *p, BLOCK **pq, BLOCK **pr)
{
    BLOCK *q, *r;
    ROOT *pRoot, *pNext;

    if (p -> nRoots <= 1)
        return (FALSE);

    q            = BlocksAddDescriptor ();
    q -> nNumber = ++nNextBlockNumber;
    q -> Type    = BLOCK_TEXT;
    q -> uFlags  |= BF_SMART_BREAKING_APPLIED;

    r            = BlocksAddDescriptor ();
    r -> nNumber = ++nNextBlockNumber;
    r -> Type    = BLOCK_TEXT;
    r -> uFlags  |= BF_SMART_BREAKING_APPLIED;

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pNext)
    {
        int x, y;
        pNext = pRoot -> u1.pNext;

        x = (pRoot -> xColumn - p -> Rect.xLeft) / nSB_CellWidth;
        y = (pRoot -> yRow    - p -> Rect.yTop)  / nSB_CellHeight;

        if (pSB_Matrix [x + y * nSB_Width] & MARKED_CELL)
            BlockAccountRoot (r, pRoot);
        else
            BlockAccountRoot (q, pRoot);
    }

    if (q -> nRoots == 0 || r -> nRoots == 0)
    {
        BlocksRestoreBreakedBlock (p, q, r);
        return (FALSE);
    }
    else
    {
        BlockSetAverageHeight (q);
        BlockSetAverageHeight (r);
        BlockCalculateBreakingParameters (q);
        BlockCalculateBreakingParameters (r);

        BlocksRemoveDescriptor (p);
        if (pq != NULL) *pq = q;
        if (pr != NULL) *pr = r;
        return (TRUE);
    }
}

# define SB_ITERATIONS   3

static int SB_CoefficientX_Nom [SB_ITERATIONS] = { 1, 2, 4 };
static int SB_CoefficientX_Den [SB_ITERATIONS] = { 1, 1, 1 };
static int SB_CoefficientY_Nom [SB_ITERATIONS] = { 1, 2, 4 };
static int SB_CoefficientY_Den [SB_ITERATIONS] = { 1, 1, 1 };

Bool TrySmartBreaking (BLOCK *pBlock)
{
    COMP *pComp;
    int  FCC_Status;
    int  nIter;
    Bool bSuccess;

    for (nIter = 0; nIter < SB_ITERATIONS; nIter++)
    {
        if (! SB_MatrixBuild
              (
                  pBlock,
                  pBlock -> nAverageHeight
                    * SB_CoefficientX_Nom [nIter]
                    / SB_CoefficientX_Den [nIter],
                  pBlock -> nAverageHeight
                    * SB_CoefficientY_Nom [nIter]
                    / SB_CoefficientY_Den [nIter]))
        {
            SmartBreakingFreeData ();
            return (FALSE);
        }

        CompsBuild
        (
            pSB_Matrix,
            nSB_Width,
            nSB_Height,
            nSB_Size,
            BLACK_CELL | MARKED_CELL | SUSPICION_COMP_CELL
        );

        FCC_Status = CompsFindCompToCut (&pComp);

        switch (FCC_Status)
        {
            case FCC_NOT_FOUND:
# ifdef LT_DEBUG
                //if (LT_DebugGraphicsLevel >= 3)
				if(!LDPUMA_Skip(hBlocksBreaking))
                    LT_GraphicsSB_MatrixOutput ("Matrix (not found)");
# endif
                SmartBreakingFreeData ();
                return (FALSE);

            case FCC_FOUND:
# ifdef LT_DEBUG
                //if (LT_DebugGraphicsLevel >= 3)
				if(!LDPUMA_Skip(hBlocksBreaking))
                    LT_GraphicsSB_MatrixOutput ("Matrix (found)");
# endif
                CompOR_Matrix (pComp, MARKED_CELL);
                bSuccess = BlockBreakByMatrix (pBlock, NULL, NULL);
                SmartBreakingFreeData ();
                return (bSuccess);

            case FCC_CANT_FOUND:
# ifdef LT_DEBUG
                //if (LT_DebugGraphicsLevel >= 3)
				if(!LDPUMA_Skip(hBlocksBreaking))
                    LT_GraphicsSB_MatrixOutput ("Matrix (can't found)");
# endif
                SmartBreakingFreeData ();
                return (FALSE);

            case FCC_FOUND_SUSPICION:
# ifdef LT_DEBUG
                //if (LT_DebugGraphicsLevel >= 3)
				if(!LDPUMA_Skip(hBlocksBreaking))
                    LT_GraphicsSB_MatrixOutput ("Matrix (not found)");
# endif
                SmartBreakingFreeData ();
                continue;
        }
    }

    return (FALSE);
}

void SmartBreakingFreeData (void)
{
    IntervalsFreeData ();
    CompsFreeData ();
    SB_MatrixFreeData ();
}
