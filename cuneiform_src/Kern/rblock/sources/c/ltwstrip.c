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
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTWSTRIP.C - white strips breaking                          *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <string.h>
# include "lterrors.h"
# include "layout.h"
# include "func.h"
# include "my_mem.h"
#include "minmax.h"


# ifdef LT_DEBUG
/*# include <conio.h>*/
# endif

int  * pWSB_Hyst1       = NULL;
int  * pWSB_Hyst2       = NULL;
int    nWSB_HystColumns = 0;
int    xWSB_HystOffset  = 0;

WSB_POINT * pWSB_Points = NULL;
int         nWSB_Points = 0;

void WSB_BuildHystograms (void)
{
    int   xMin, xMax;
    ROOT *pRoot;
    int   iBegin, iEnd;
    int   iColumn;
    int   nSum1, nSum2;

    WSB_FreeData ();

    if (nRoots == 0)
        return;

    xMin = pRoots [0].xColumn;
    xMax = pRoots [0].xColumn + pRoots [0].nWidth - 1;

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        if (IS_LAYOUT_DUST (*pRoot))
            continue;

        if (xMin > pRoot -> xColumn)
            xMin = pRoot -> xColumn;

        if (xMax < pRoot -> xColumn + pRoot -> nWidth - 1)
            xMax = pRoot -> xColumn + pRoot -> nWidth - 1;
    }

    nWSB_HystColumns = xMax - xMin + 1;
    xWSB_HystOffset  = xMin;

    pWSB_Hyst1 = malloc ((nWSB_HystColumns + 1) * sizeof (int));

    if (pWSB_Hyst1 == NULL)
        ErrorNoEnoughMemory ("in LTWSTRIP.C,WSB_BuildHystograms,part 1");

    pWSB_Hyst2 = malloc ((nWSB_HystColumns + 1) * sizeof (int));

    if (pWSB_Hyst2 == NULL)
        ErrorNoEnoughMemory ("in LTWSTRIP.C,WSB_BuildHystograms,part 2");

    memset (pWSB_Hyst1, 0, (nWSB_HystColumns + 1) * sizeof (int));
    memset (pWSB_Hyst2, 0, (nWSB_HystColumns + 1) * sizeof (int));

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        if (IS_LAYOUT_DUST (*pRoot))
            continue;

        iBegin = pRoot -> xColumn - xWSB_HystOffset;
        iEnd   = pRoot -> xColumn + pRoot -> nWidth - 1 - xWSB_HystOffset;

        pWSB_Hyst1 [iBegin]   ++;
        pWSB_Hyst1 [iEnd + 1] --;
        pWSB_Hyst2 [iBegin]   += pRoot -> nHeight;
        pWSB_Hyst2 [iEnd + 1] -= pRoot -> nHeight;
    }

    nSum1 = 0;
    nSum2 = 0;

    for (iColumn = 0; iColumn < nWSB_HystColumns; iColumn++)
    {
        nSum1 += pWSB_Hyst1 [iColumn];
        pWSB_Hyst1 [iColumn] = nSum1;
        nSum2 += pWSB_Hyst2 [iColumn];
        pWSB_Hyst2 [iColumn] = nSum2;
    }
}

void WSB_AddPoint (WSB_POINT Point)
{
    if (nWSB_Points % WSB_POINTS_POOL_ALLOCATION_QUANTUM == 0)
    {
        pWSB_Points = (WSB_POINT *)
            realloc
            (
                pWSB_Points,
                (size_t) ((nWSB_Points + WSB_POINTS_POOL_ALLOCATION_QUANTUM)
                          * sizeof (WSB_POINT))
            );

        if (pWSB_Points == NULL)
        ErrorNoEnoughMemory ("in LTWSTRIP.C,WSB_AddPoint,part 2");
    }

    pWSB_Points [nWSB_Points++] = Point;
}

# define WSB_MAX_PAGE_HEIGHT()    1000
# define WSB_HEIGHT_THRESH()      (nPageHeight / 20)
# define WSB_WIDTH_THRESH()       20
# define WSB_PEAK_HEIGHT_THRESH() (nPageHeight / 3)
# define WSB_STEP_BACK()          64
# define WSB_STEP_FORWARD()       64

void WSB_BuildPoints (void)
{
    int       x, xx;
    WSB_POINT Point;
    Bool      bPeakFromLeft;
    int       nLeftPeak;
    int       nRightPeak;
    BLOCK   * pBlock;

    if (nPageHeight < WSB_MAX_PAGE_HEIGHT())
        return;

    if (pWSB_Hyst1 == NULL || pWSB_Hyst2 == NULL)
        WSB_BuildHystograms ();

    for (x = 0; x < nWSB_HystColumns; x++)
    {
        if (pWSB_Hyst2 [x] > WSB_HEIGHT_THRESH ())
            continue;

        Point.xBegin = x;

        while (x < nWSB_HystColumns && pWSB_Hyst2 [x] <= WSB_HEIGHT_THRESH ())
        {
            x++;
        }

        Point.xEnd = x - 1;

        if (Point.xEnd - Point.xBegin + 1 < WSB_WIDTH_THRESH ())
            continue;

        if (Point.xEnd >= nWSB_HystColumns ||
            Point.xBegin <= 0)
        {
            continue;
        }

        nLeftPeak = 0;

        for (xx = Point.xBegin - 1;
                 xx >= 0 && xx > Point.xBegin - WSB_STEP_FORWARD ();
                     xx--)
        {
            if (nLeftPeak < pWSB_Hyst2 [xx])
                nLeftPeak = pWSB_Hyst2 [xx];
        }

        nRightPeak = 0;

        for (xx = Point.xEnd + 1;
                 xx < nWSB_HystColumns && xx < Point.xEnd + WSB_STEP_FORWARD ();
                     xx++)
        {
            if (nRightPeak < pWSB_Hyst2 [xx])
                nRightPeak = pWSB_Hyst2 [xx];
        }

        if (nRightPeak >= WSB_PEAK_HEIGHT_THRESH ())
        {
            bPeakFromLeft = FALSE;
        }
        else if (nLeftPeak >= WSB_PEAK_HEIGHT_THRESH ())
        {
            bPeakFromLeft = TRUE;
        }
        else
        {
            continue;
        }

        Point.yMin     = pWSB_Hyst2 [Point.xEnd];
        Point.yMax     = pWSB_Hyst2 [Point.xEnd];
        Point.yAverage = 0;

        for (xx = Point.xBegin; xx <= Point.xEnd; xx++)
        {
            if (Point.yMin > pWSB_Hyst2 [xx])
                Point.yMin = pWSB_Hyst2 [xx];

            if (Point.yMax < pWSB_Hyst2 [xx])
                Point.yMax = pWSB_Hyst2 [xx];

            Point.yAverage += pWSB_Hyst2 [xx] * pWSB_Hyst2 [xx];
        }

        Point.yAverage = (int) long_sqrt
            ((int32_t) Point.yAverage / (Point.xEnd - Point.xBegin + 1));

        if (bPeakFromLeft)
        {
            for (xx = Point.xBegin;
                    xx <= Point.xEnd && pWSB_Hyst2 [xx] > Point.yAverage;
                        xx++)
            {
            }

            for (;
                    xx < Point.xEnd                        &&
                    xx < Point.xBegin + WSB_STEP_BACK ()   &&
                    pWSB_Hyst2 [xx + 1] <= pWSB_Hyst2 [xx];
                        xx++)
            {
            }
        }
        else
        {
            for (xx = Point.xEnd;
                    xx >= Point.xBegin && pWSB_Hyst2 [xx] > Point.yAverage;
                        xx--)
            {
            }

            for (;
                    xx > Point.xBegin                      &&
                    xx > Point.xEnd - WSB_STEP_BACK ()     &&
                    pWSB_Hyst2 [xx - 1] <= pWSB_Hyst2 [xx];
                        xx--)
            {
            }
        }

        Point.xMain = xx;

        for (pBlock = pBlocksList; pBlock != NULL; pBlock = pBlock -> pNext)
        {
            if (pBlock -> Type != BLOCK_TEXT)
                continue;

            if (pBlock -> Rect.xLeft  < Point.xMain + xWSB_HystOffset &&
                pBlock -> Rect.xRight > Point.xMain + xWSB_HystOffset)
            {
                break;
            }
        }

        if (pBlock == NULL)
            continue;

        WSB_AddPoint (Point);
    }

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
    {
        LT_GraphicsWSB_Hystogram_1_Output ("Vertical Page Hystogram 1");
        LT_GraphicsWSB_Hystogram_2_Output ("Vertical Page Hystogram 2");
        LT_GraphicsWSB_PointsOutput       ("WhiteStripBreaking Points");
    }
# endif
}

Bool WSB_BreakBlock (BLOCK * p, WSB_POINT Point)
{
    BLOCK *q, *r;
    ROOT  *pRoot;

    int   iColumn;
    int   iLeftColumn, iRightColumn;
    int   nHystSum;

    int   iBreakingColumn;
    int   xBreakingLine;
    int   iLSpaceEnd, iRSpaceEnd;
    Bool  bLineCorrected;

    if (p -> uFlags & BF_WHITE_STRIPS_BREAKING_APPLIED)
        return FALSE;

# ifdef LT_DEBUG
    if (p -> pHorzHystogram == NULL)
        ErrorInternal ("pHorzHystogram not builded in WSB_BreakBlock");
# endif

    /*
     *  Hystograms building
     */

    nHystColumns = p -> Rect.xRight - p -> Rect.xLeft + 1;
    memset (pHystogram, 0, nHystColumns * sizeof (int));

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        iLeftColumn  = pRoot -> xColumn                       - p -> Rect.xLeft;
        iRightColumn = pRoot -> xColumn + pRoot -> nWidth - 1 - p -> Rect.xLeft;

        pHystogram [iLeftColumn]      += pRoot -> nHeight;
        pHystogram [iRightColumn + 1] -= pRoot -> nHeight;
    }

    nHystSum = 0;

    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        nHystSum += pHystogram [iColumn];
        pHystogram [iColumn] = nHystSum;
    }

    xBreakingLine   = Point.xMain + xWSB_HystOffset;
    iBreakingColumn = xBreakingLine - p -> Rect.xLeft;

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 4)
    {
        LT_GraphicsClearScreen ();
        pDebugBlock    = p;
        xDebugVertLine = xBreakingLine;
        LT_GraphicsBlockOutput2    ("Break, breaked with WSB");
        LT_Getch ();
        LT_GraphicsHystogramOutput ("Hystogram for WSB_BreakBlock");
        xDebugVertLine = -1;
    }
# endif

    xBreakingLine   = Point.xMain + xWSB_HystOffset;
    iBreakingColumn = xBreakingLine - p -> Rect.xLeft;

    /*
     *  Correction of breaking line
     */

    bLineCorrected = FALSE;

    if (pHystogram [iBreakingColumn] != 0)
    {
        int iColumn;

        for (iColumn = iBreakingColumn - 1;
                iColumn > iBreakingColumn - p -> nAverageHeight / 3 &&
                pHystogram [iColumn] != 0;
                    iColumn--)
        {
        }

        if (pHystogram [iColumn] != 0)
        {
            for (iColumn = iBreakingColumn + 1;
                    iColumn < iBreakingColumn + p -> nAverageHeight / 3 &&
                    pHystogram [iColumn] != 0;
                        iColumn++)
            {
            }
        }

        if (pHystogram [iColumn] == 0)
        {
            xBreakingLine   = iColumn + p -> Rect.xLeft;
            iBreakingColumn = iColumn;
            bLineCorrected  = TRUE;
        }
    }

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 3 && bLineCorrected)
    {
        LT_GraphicsClearScreen ();
        pDebugBlock    = p;
        xDebugVertLine = iBreakingColumn + p -> Rect.xLeft; ;
        LT_GraphicsBlockOutput2    ("Corrected WSB line");
        LT_Getch ();
        LT_GraphicsHystogramOutput
            ("WSB_BreakBlock Hyst with corrected line");
        xDebugVertLine = -1;
    }
# endif

    /*
     *  Discrimination
     */

    if (pHystogram [iBreakingColumn] != 0)
        goto NO_BREAK;

    for (iLSpaceEnd = iBreakingColumn - 1;
             iLSpaceEnd > 0 && pHystogram [iLSpaceEnd] == 0;
                 iLSpaceEnd--)
    {
    }

    for (iRSpaceEnd = iBreakingColumn + 1;
             iRSpaceEnd < nWSB_HystColumns && pHystogram [iRSpaceEnd] == 0;
                 iRSpaceEnd++)
    {
    }

    if (iRSpaceEnd - iLSpaceEnd - 1 < MAX (p -> nAverageHeight, 1))
        goto NO_BREAK;

    /*
     *  Breaking
     */

    if (! BlockBreakOnVertical (p, &q, &r, xBreakingLine, 0))
        goto NO_BREAK;

    q -> uFlags |= BF_WHITE_STRIPS_BREAKING_APPLIED;
    r -> uFlags |= BF_WHITE_STRIPS_BREAKING_APPLIED;
    q -> uFlags |= BF_NOT_GLUE_BLOCK;
    r -> uFlags |= BF_NOT_GLUE_BLOCK;
    return TRUE;

  NO_BREAK:
    p -> uFlags |= BF_WHITE_STRIPS_BREAKING_APPLIED;
    return FALSE;
}

Bool WSB_CutUpperLine (BLOCK * p, WSB_POINT Point)
{
    int ixColumn;
    int yAverageHystHeight;
    int iBlockBreakingColumn;
    int xBlockBreakingLine;

    ROOT * pRoot;
    ROOT * pBestRoot;
    int    nRootDistance;
    int    nBestRootDistance = 0; /* To avoid warning on MPW C/C++ */

    int xLeftBlockMin,   xRightBlockMin;
    int xLeftCorrected,  xRightCorrected;

    int iyColumn;

    int yUpperCutLine,   yLowerCutLine;
    int yFirstLineToCut, ySecondLineToCut;

# ifdef LT_DEBUG
    if (p -> pHorzHystogram == NULL)
        ErrorInternal ("pHorzHystogram not builded in WSB_BreakBlock");
# endif

    /*
     *  Precondition : Hystogram, builded by
     *     Bool WSB_BreakBlock (BLOCK * p, WSB_POINT Point)
     */

    /*
     *  1. Block discrimination heuristics
     */

    if (p -> Rect.yBottom - p -> Rect.yTop + 1 < p -> nAverageHeight * 6)
        return FALSE;

    /*
     *  2. Calculation corrected white strip for block
     */

    /**
     **  2.1. Average block hystogram height
     **/

    yAverageHystHeight = 0;

    for (ixColumn = 0; ixColumn < nHystColumns; ixColumn++)
        yAverageHystHeight += pHystogram [ixColumn] * pHystogram [ixColumn];

    yAverageHystHeight =
        (int) long_sqrt ((int32_t) yAverageHystHeight / nHystColumns);

    /**
     **  2.2. Find interval of block hystogram minimum
     **/

    iBlockBreakingColumn = Point.xMain + xWSB_HystOffset - p -> Rect.xLeft;
    xBlockBreakingLine   = Point.xMain + xWSB_HystOffset;

    if (pHystogram [iBlockBreakingColumn] > yAverageHystHeight / 3)
        return FALSE;

    for (ixColumn = iBlockBreakingColumn - 1;
             ixColumn >= 0                                              &&
             ixColumn >= iBlockBreakingColumn - p -> nAverageHeight * 2 &&
             pHystogram [ixColumn] <= yAverageHystHeight / 3;
                 ixColumn--)
    {
    }

    xLeftBlockMin = ixColumn + 1 + p -> Rect.xLeft;

    for (ixColumn = iBlockBreakingColumn + 1;
             ixColumn < nHystColumns                                    &&
             ixColumn <= iBlockBreakingColumn + p -> nAverageHeight * 2 &&
             pHystogram [ixColumn] <= yAverageHystHeight / 3;
                 ixColumn++)
    {
    }

    xRightBlockMin = ixColumn - 1 + p -> Rect.xLeft;

    /**
     **  2.3. Find intersection of page and block minimum intervals
     **/

    xLeftCorrected  = MAX (Point.xBegin + xWSB_HystOffset, xLeftBlockMin);
    xRightCorrected = MIN (Point.xEnd   + xWSB_HystOffset, xRightBlockMin);

    /**
     **  2.4. Discrimination corrected white strip
     **/

    if (xRightCorrected - xLeftCorrected + 1 < p -> nAverageHeight)
        return (FALSE);

    /*
     *  3. Find best root in the corrected white strip for block
     */

    pBestRoot = NULL;

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        if (pRoot -> xColumn                       >= xRightCorrected &&
            pRoot -> xColumn + pRoot -> nWidth - 1 <= xLeftCorrected)
        {
            continue;
        }

        if (IS_LAYOUT_DUST (*pRoot))
            continue;

        if (pRoot -> nHeight < p -> nAverageHeight / 2)
            continue;

        if (pRoot -> xColumn > xBlockBreakingLine)
        {
            nRootDistance = pRoot -> xColumn - xBlockBreakingLine;
        }
        else if (pRoot -> xColumn + pRoot -> nWidth - 1 < xBlockBreakingLine)
        {
            nRootDistance = xBlockBreakingLine
                            - (pRoot -> xColumn + pRoot -> nWidth - 1);
        }
        else
        {
            nRootDistance = 0;
        }

        if (pBestRoot == NULL                      ||
            nRootDistance < nBestRootDistance      ||
            nRootDistance == nBestRootDistance &&
                pRoot -> nWidth > pBestRoot -> nWidth)
        {
            pBestRoot         = pRoot;
            nBestRootDistance = nRootDistance;
        }
    }

    if (pBestRoot == NULL)
        return (FALSE);

    /*
     *  4. Find line horizontal strip for founded root
     */

    iyColumn = pBestRoot -> yRow - p -> Rect.yTop - 1;

    while (iyColumn >= 0 && p -> pHorzHystogram [iyColumn] != 0)
        iyColumn--;

    yUpperCutLine = iyColumn + p -> Rect.yTop;

    iyColumn = pBestRoot -> yRow + pBestRoot -> nHeight - 1
               - p -> Rect.yTop + 1;

    while (iyColumn < p -> Rect.yBottom - p -> Rect.yTop + 1 &&
           p -> pHorzHystogram [iyColumn] != 0)
    {
        iyColumn++;
    }

    yLowerCutLine = iyColumn + p -> Rect.yTop;

    /*
     *  5. Discriminate line horizontal strip
     */

    if (yLowerCutLine - yUpperCutLine < p -> nAverageHeight    ||
        yLowerCutLine - yUpperCutLine > p -> nAverageHeight * 3)
    {
        return (FALSE);
    }

    /*
     *  6. Select lines to cut and Cut!
     */

    if (yUpperCutLine <= p -> Rect.yTop &&
        yLowerCutLine >= p -> Rect.yBottom)
    {
        return (FALSE);
    }

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
    {
        LT_GraphicsClearScreen ();
        pDebugBlock    = p;
        xDebugVertLine = Point.xMain + xWSB_HystOffset;
        LT_GraphicsBlockOutput2 ("?? WSB Cut Line ??");
        LT_Getch ();
        LT_GraphicsHystogramOutput ("?? WSB Cut Line");
        xDebugVertLine = -1;
    }
# endif

    if (yUpperCutLine <= p -> Rect.yTop)
        return (BlockBreakOnHorizontal (p, NULL, NULL, yLowerCutLine));

    if (yLowerCutLine >= p -> Rect.yBottom)
        return (BlockBreakOnHorizontal (p, NULL, NULL, yUpperCutLine));

    if (yUpperCutLine - p -> Rect.yTop
            < p -> Rect.yBottom - yLowerCutLine)
    {
        yFirstLineToCut  = yLowerCutLine;
        ySecondLineToCut = yUpperCutLine;
    }
    else
    {
        yFirstLineToCut  = yUpperCutLine;
        ySecondLineToCut = yLowerCutLine;
    }

    if      (BlockBreakOnHorizontal (p, NULL, NULL, yFirstLineToCut))
        return (TRUE);
    else if (BlockBreakOnHorizontal (p, NULL, NULL, ySecondLineToCut))
        return (TRUE);
    else
        return (FALSE);
}

void WhiteStripsBreaking (void)
{
    BLOCK   * pBlock;
    int       iPoint;
    WSB_POINT Point;

    WSB_BuildHystograms ();
    WSB_BuildPoints ();

    if (nWSB_Points == 0)
    {
        WSB_FreeData ();
        return;
    }

    for (iPoint = 0; iPoint < nWSB_Points; iPoint++)
    {
        for (pBlock = pBlocksList; pBlock != NULL; pBlock = pBlock -> pNext)
            pBlock -> uFlags &= ~BF_WHITE_STRIPS_BREAKING_APPLIED;

        Point = pWSB_Points [iPoint];

      AGAIN:
        for (pBlock = pBlocksList; pBlock != NULL; pBlock = pBlock -> pNext)
        {
            if (pBlock -> Type != BLOCK_TEXT)
                continue;

            if (pBlock -> uFlags & BF_WHITE_STRIPS_BREAKING_APPLIED)
                continue;

            if (pBlock -> Rect.xLeft  >= Point.xMain + xWSB_HystOffset ||
                pBlock -> Rect.xRight <= Point.xMain + xWSB_HystOffset)
            {
                continue;
            }

            if (WSB_BreakBlock (pBlock, Point))
                goto AGAIN;

            if (WSB_CutUpperLine (pBlock, Point))
                goto AGAIN;
        }
    }

    WSB_FreeData ();
}

void WSB_FreeData (void)
{
    if (pWSB_Hyst1 != NULL)
        free (pWSB_Hyst1);

    if (pWSB_Hyst2 != NULL)
        free (pWSB_Hyst2);

    pWSB_Hyst1       = NULL;
    pWSB_Hyst2       = NULL;
    nWSB_HystColumns = 0;
    xWSB_HystOffset  = 0;

    if (pWSB_Points != NULL)
        free (pWSB_Points);

    pWSB_Points      = NULL;
    nWSB_Points      = 0;
}
