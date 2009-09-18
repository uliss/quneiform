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
 *              LTBREAK.C - Blocks breaking                                 *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "c_memory.h"
# include "layout.h"
# include "func.h"
# include "my_mem.h"

# include "dpuma.h"
#include "minmax.h"

extern Handle hBlocksBreaking;
extern Handle hNotWholeLine ;
extern Handle hHystogramNull ;
extern uint32_t ZagolovokBreakingCoeff;


# define MIN_BREAKED_BLOCK_HEIGHTES_WIDTH  4
# define MAX_NARROW_COLUMN_DISTANCE        4
# define MAX_BULLET_DISTANCE               8

// результат имеет максимумы в серединах линий, минимумы
// (~нули!) -- между линиями (~строками).
void BlockHorzHystogramBuild (BLOCK *p)
{
    int *pHystogram;
    int nHystColumns; // длина гистограммы

    ROOT *pRoot;
    int iTopColumn, iBottomColumn; // assert(botom>top)
    int iColumn;
    int nSum;

    nHystColumns = p -> Rect.yBottom - p -> Rect.yTop + 1;

    pHystogram = malloc ((nHystColumns + 1) * sizeof (int));

    if (pHystogram == NULL)
        ErrorNoEnoughMemory ("in LTBREAK.C,BlockHorzHystogramBuild,part 1");

    memset (pHystogram, 0, (nHystColumns + 1) * sizeof (int));

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        // пыль не учитываем
        if (IS_LAYOUT_DUST (* pRoot))
            continue;
        // относительное начало текущего рута (по у)
        iTopColumn    = (pRoot -> yRow - p -> Rect.yTop);
        // относительное окончание текущего рута (по высоте же)
        iBottomColumn = (pRoot -> yRow + pRoot -> nHeight - 1
                            - p -> Rect.yTop);
        pHystogram [iTopColumn] ++;
        pHystogram [iBottomColumn + 1] --;
        // то есть вокруг "боттом-линии" будет отрицательный пик,
        // вокруг "топ-линии" -- положительный,
        // а близкие топ- и боттом- линии взаимоуничтожаются
    }

    nSum = 0;

    // "интегрирование" ("dx==1") исходной гистограммы:
    // получаем, естественно, экстремумы в бывших нулях;
    // точнее -- максимумы в серединах линий, минимумы
    // (~нули!) -- между линиями
    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        nSum += pHystogram [iColumn];
        pHystogram [iColumn] = nSum;
    }

    p -> nActualHeight = 0;

    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        if (pHystogram [iColumn] != 0)
            p -> nActualHeight++; // ~количество реальных ординат
                                  // (высот)
    }

    if (p -> pHorzHystogram != NULL)
    {
        free (p -> pHorzHystogram);
        p -> pHorzHystogram = NULL;
    }

    p -> pHorzHystogram = pHystogram;
}

void BlockBuild_HystHorzHeightesSum (BLOCK *p)
{
    int nHystColumns;

    ROOT *pRoot;
    int iTopColumn, iBottomColumn;
    int iColumn;
    int nSum;

    nHystColumns = p -> Rect.yBottom - p -> Rect.yTop + 1;
    memset (pHystHorzHeightesSum, 0, (nHystColumns + 1) * sizeof (int));

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        if (IS_LAYOUT_DUST (* pRoot))
            continue;

        iTopColumn    = (pRoot -> yRow - p -> Rect.yTop);
        iBottomColumn = (pRoot -> yRow + pRoot -> nHeight - 1
                            - p -> Rect.yTop);

        pHystHorzHeightesSum [iTopColumn]        += pRoot -> nHeight;
        pHystHorzHeightesSum [iBottomColumn + 1] -= pRoot -> nHeight;
    }

    nSum = 0;

    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        nSum += pHystHorzHeightesSum [iColumn];
        pHystHorzHeightesSum [iColumn] = nSum;
    }
}

void BlockBuild_HystVertHeightesSum (BLOCK *p)
{
    int nHystColumns;
    ROOT *pRoot;
    int iLeftColumn, iRightColumn;
    int iColumn;
    int nSum;

    nHystColumns = p -> Rect.xRight - p -> Rect.xLeft + 1;
    memset (pHystVertHeightesSum, 0, (nHystColumns + 1) * sizeof (int));

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        if (IS_LAYOUT_DUST (* pRoot))
            continue;

        iLeftColumn  = (pRoot -> xColumn - p -> Rect.xLeft);
        iRightColumn = (pRoot -> xColumn + pRoot -> nWidth - 1
                            - p -> Rect.xLeft);

        pHystVertHeightesSum [iLeftColumn]      += pRoot -> nHeight;
        pHystVertHeightesSum [iRightColumn + 1] -= pRoot -> nHeight;
    }

    nSum = 0;

    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        nSum += pHystVertHeightesSum [iColumn];
        pHystVertHeightesSum [iColumn] = nSum;
    }
}

void BlockCalculateBreakingParameters (BLOCK *p)
{
    int nBlockHeights;

    // "горизонтальная гистограмма" --
    // гистограмма для разбиения по вертикали!
    BlockHorzHystogramBuild (p);

    // количество "актуальных высот" можно примерно считать
    // равным высоте блока (после "интегрирования" число нулей
    // должно стремительно упасть)
    nBlockHeights = p -> nActualHeight / p -> nAverageHeight;

    switch (nBlockHeights)
    {
        case 0:
        case 1:
            p -> nStartColumnWidth            = p -> nAverageHeight * 4;
            p -> nUnconditionalMinColumnWidth = p -> nStartColumnWidth;
            p -> nConditionalMinColumnWidth   = p -> nStartColumnWidth;
            break;

        case 2:
        case 3:
        case 4:
        case 5:
            p -> nStartColumnWidth            = p -> nAverageHeight * 2;
            p -> nUnconditionalMinColumnWidth = p -> nStartColumnWidth;
            p -> nConditionalMinColumnWidth   = p -> nStartColumnWidth;
            break;

        case 6:
        case 7:
        case 8:
        case 9:
            p -> nStartColumnWidth            = p -> nAverageHeight;
            p -> nUnconditionalMinColumnWidth = p -> nStartColumnWidth;
            p -> nConditionalMinColumnWidth   = p -> nStartColumnWidth / 2;
            break;

        default:
            p -> nStartColumnWidth            = p -> nAverageHeight;
            p -> nUnconditionalMinColumnWidth = p -> nAverageHeight;
            p -> nConditionalMinColumnWidth   = p -> nAverageHeight / 4;
            break;
    }

    if (p -> nStartColumnWidth == 0)
        p -> nStartColumnWidth = 1;

    if (p -> nUnconditionalMinColumnWidth == 0)
        p -> nUnconditionalMinColumnWidth = 1;

    if (p -> nConditionalMinColumnWidth == 0)
        p -> nConditionalMinColumnWidth = 1;
}

void BlockReCalculateBreakingParameters (BLOCK *p)
{
    if (p -> pHorzHystogram != NULL)
    {
        free (p -> pHorzHystogram);
        p -> pHorzHystogram = NULL;
    }

    BlockCalculateBreakingParameters (p);
}

void BlocksRestoreBreakedBlock (BLOCK *p, BLOCK *q, BLOCK *r)
{
    if (q -> pRoots == NULL)
    {
        p -> pRoots    = r -> pRoots;
        p -> pEndRoots = r -> pEndRoots;
    }
    else if (r -> pRoots == NULL)
    {
        p -> pRoots    = q -> pRoots;
        p -> pEndRoots = q -> pEndRoots;
    }
    else
    {
        p -> pRoots                = q -> pRoots;
        q -> pEndRoots -> u1.pNext = r -> pRoots;
        p -> pEndRoots             = r -> pEndRoots;
    }

    BlocksRemoveDescriptor (q);
    BlocksRemoveDescriptor (r);
}

Bool BlockBreakOnVertical (BLOCK *p, BLOCK **pq, BLOCK **pr,
                           int x, int nBreakingWidth)
{
    BLOCK *q, *r;
    ROOT *pRoot, *pNext;

    if (p -> nRoots <= 1)
        return (FALSE);

    q            = BlocksAddDescriptor ();
    q -> nNumber = ++nNextBlockNumber;
    q -> Type    = BLOCK_TEXT;

    r            = BlocksAddDescriptor ();
    r -> nNumber = ++nNextBlockNumber;
    r -> Type    = BLOCK_TEXT;

    if (p -> uFlags & BF_SMART_BREAKING_APPLIED)
    {
        q -> uFlags |= BF_SMART_BREAKING_APPLIED;
        r -> uFlags |= BF_SMART_BREAKING_APPLIED;
    }

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pNext)
    {
        pNext = pRoot -> u1.pNext;

        BlockAccountRoot (pRoot -> xColumn <= x ? q : r, pRoot);
    }


    if (q -> nRoots == 0 || r -> nRoots == 0)
    {
        BlocksRestoreBreakedBlock (p, q, r);
        return (FALSE);
    }

    BlockSetAverageHeight (q);
    BlockSetAverageHeight (r);
    BlockCalculateBreakingParameters (q);
    BlockCalculateBreakingParameters (r);

    if (q -> nRoots == q -> nDust ||
        r -> nRoots == r -> nDust ||

        // Cut lower block

        ! bOptionBusinessCardsLayout &&
        nBreakingWidth != 0 &&
        (nBreakingWidth < q -> nConditionalMinColumnWidth ||
         nBreakingWidth < r -> nConditionalMinColumnWidth)
        ||

        // Cut narrow column

        nBreakingWidth != 0 &&
        (q -> nAverageHeight * MIN_BREAKED_BLOCK_HEIGHTES_WIDTH >
                q -> Rect.xRight - q -> Rect.xLeft + 1
            &&
         nBreakingWidth < q -> nAverageHeight * MAX_NARROW_COLUMN_DISTANCE

            ||

         r -> nAverageHeight * MIN_BREAKED_BLOCK_HEIGHTES_WIDTH >
                r -> Rect.xRight - r -> Rect.xLeft + 1
            &&
         nBreakingWidth < r -> nAverageHeight * MAX_NARROW_COLUMN_DISTANCE)

         ||

         // Cut bullet

        q -> nRoots == 1 &&
        p -> nRoots > 10 &&
        q -> Rect.xRight  - q -> Rect.xLeft + 1 <= p -> nAverageHeight &&
        q -> Rect.yBottom - q -> Rect.yTop  + 1 <= p -> nAverageHeight

            ||
        p -> nRoots == 1 &&
        q -> nRoots > 10 &&
        p -> Rect.xRight  - p -> Rect.xLeft + 1 <= q -> nAverageHeight &&
        p -> Rect.yBottom - p -> Rect.yTop  + 1 <= q -> nAverageHeight)
    {
        BlocksRestoreBreakedBlock (p, q, r);
        return (FALSE);
    }
    else
    {
        BlocksRemoveDescriptor (p);
        if (pq != NULL) *pq = q;
        if (pr != NULL) *pr = r;
        return (TRUE);
    }
}

# define VERT_HYST_LOW_LEVEL(n)  ((n) / 4)
# define CONDITIONAL_BREAKING_FACTOR 3

Bool CorrectCondition (int iColumn)
{
    int nLeftMax;
    int nRightMax;

    if (iColumn < 1 || iColumn > nHystColumns - 2)
        return (FALSE);

    nLeftMax = pHystogram [iColumn - 1];

    if (iColumn >= 2 && pHystogram [iColumn - 2] > nLeftMax)
        nLeftMax = pHystogram [iColumn - 2];

    if (iColumn >= 3 && pHystogram [iColumn - 3] > nLeftMax)
        nLeftMax = pHystogram [iColumn - 3];

    if (nLeftMax >= CONDITIONAL_BREAKING_FACTOR * pHystogram [iColumn + 1])
    {
        Bool bOneString = TRUE;
        int i;

        for (i = iColumn + 1; i < nHystColumns; i++)
            if (pHystogram [i] > 3)
                bOneString = FALSE;

        if (! bOneString)
            return (TRUE);
    }

    nRightMax = pHystogram [iColumn + 1];

    if (iColumn <= nHystColumns - 3 && pHystogram [iColumn + 2] > nRightMax)
        nRightMax = pHystogram [iColumn + 2];

    if (iColumn <= nHystColumns - 4 && pHystogram [iColumn + 3] > nRightMax)
        nRightMax = pHystogram [iColumn + 3];

    if (nRightMax >= CONDITIONAL_BREAKING_FACTOR * pHystogram [iColumn - 1])
    {
        Bool bOneString = TRUE;
        int i;

        for (i = 0; i < iColumn; i++)
            if (pHystogram [i] > 3)
                bOneString = FALSE;

        if (! bOneString)
            return (TRUE);
    }

    return (FALSE);
}

Bool TryCutBlockOnVertical (BLOCK *p, int bcr_cut, Bool32 SecondStage)
{
    ROOT *pRoot;

    int  iLeftColumn, iRightColumn;
    int  iColumn;

    int  nMaximum;
    int  nLowLevel;

    int  nSpaceWidth;
    int  iSpace;
    int  nHystSum;
    Bool bNotSpace;

    int  iLeftLimit;
    int  iRightLimit;
    int  iBestPointSizeBreakingColumn;
    int  nPointSizeDifference;
    int  nBestPointSizeDifference = 0; /* To avoid warning on MPW C/C++ */

	int nSum;
	int nAverage;
	int nColumns;

    BLOCK *q, *r;

# ifdef LT_DEBUG
    if (p -> pHorzHystogram == NULL)
        ErrorInternal ("pHorzHystogram not builded in TryCutBlockOnVertical");
# endif

    nHystColumnWidth = p -> nStartColumnWidth;

    while (nHystColumnWidth >= p -> nConditionalMinColumnWidth)
    {
        /* Build vertical hystogram */

        nHystColumns = (p -> Rect.xRight - p -> Rect.xLeft + 1)
                       / nHystColumnWidth + 1;

        //memset (pHystogram, 0, nHystColumns * sizeof (int));
		memset (pHystogram, 0, (nHystColumns + 1) * sizeof (int));// Piter 08.07.99

        for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
        {
            iLeftColumn  = (pRoot -> xColumn
                            - p -> Rect.xLeft) / nHystColumnWidth;
            iRightColumn = (pRoot -> xColumn + pRoot -> nWidth - 1
                            - p -> Rect.xLeft) / nHystColumnWidth;

            pHystogram [iLeftColumn]      ++;
            pHystogram [iRightColumn + 1] --;
        }

        nMaximum = 0;
        nHystSum = 0;

        for (iColumn = 0; iColumn < nHystColumns; iColumn++)
        {
            nHystSum += pHystogram [iColumn];
            pHystogram [iColumn] = nHystSum;

            if (pHystogram [iColumn] > nMaximum)
                nMaximum = pHystogram [iColumn];
        }

        nLowLevel = VERT_HYST_LOW_LEVEL (nMaximum);

		/* Rom */
		if (SecondStage)
		{
			nSum = 0;
			for (iColumn = 0; iColumn < nHystColumns; iColumn++)
			{
				nSum += pHystogram [iColumn];
			}

			if(nHystColumns!= 0)
			{
				nAverage = nSum/nHystColumns;
			}

			nSum = 0;
			nColumns = 0;
			for (iColumn = 0; iColumn < nHystColumns; iColumn++)
			{
				if(pHystogram [iColumn] > nAverage)
				{
					nSum += pHystogram [iColumn];
					nColumns++;
				}
			}

			if(nColumns!= 0)
			{
				nAverage = nSum/nColumns;
			}

			for (iColumn = 0; iColumn < nHystColumns; iColumn++)
			{
				if(pHystogram [iColumn] < nAverage / 10)
				{
					pHystogram [iColumn] = 0;
				}
			}
		}
		/* Rom */

# ifdef LT_DEBUG
        //if (LT_DebugGraphicsLevel >= 4)
		if(!LDPUMA_Skip(hBlocksBreaking))
		{
			pDebugBlock = p; // Piter 02-10-98
            LT_GraphicsHystogramOutput ("Hystogram for vertical breaking");
		}
# endif

        bNotSpace = FALSE;

        for (iColumn = 0; iColumn < nHystColumns; iColumn++)
        {
            if (pHystogram [iColumn] == 0)
            {
                if (! bNotSpace)
                    continue;

                if (nHystColumnWidth < p -> nUnconditionalMinColumnWidth &&
                    ! CorrectCondition (iColumn))
                {
                    continue;
                }

                nSpaceWidth = 0;

                for (iSpace = iColumn;
                         iSpace < nHystColumns && pHystogram [iSpace] == 0;
                             iSpace++)
                {
                    nSpaceWidth += nHystColumnWidth;
                }

                if (iSpace == nHystColumns)
                    break;

                if (BlockBreakOnVertical (p, NULL, NULL,
                        iColumn * nHystColumnWidth + p -> Rect.xLeft,
                        nSpaceWidth))
                {
                    return (TRUE);
                }

                iColumn += nSpaceWidth;
            }
            else
            {
                bNotSpace = TRUE;
            }
        }

        for (iColumn = 0; iColumn < nHystColumns; iColumn++)
        {
            if (pHystogram [iColumn] < nLowLevel)
            {
                nHystColumnWidth /= 2;
                break;
            }
        }

        if (iColumn == nHystColumns)
            break;
    }

    /*
     *  Analysis with font accounting
     */

    if (! (bOptionPointSizeAnalysis && bOptionBusinessCardsLayout))
        return (FALSE);
    if (!bcr_cut)
       return (FALSE);    // 940228
        /* Build vertical hystogram */

    if (nHystColumnWidth != 1)
    {
        nHystColumns = p -> Rect.xRight - p -> Rect.xLeft + 1;

        memset (pHystogram, 0, nHystColumns * sizeof (int));

        for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
        {
            iLeftColumn  = pRoot -> xColumn - p -> Rect.xLeft;
            iRightColumn = pRoot -> xColumn + pRoot -> nWidth - 1
                            - p -> Rect.xLeft;

            pHystogram [iLeftColumn]      ++;
            pHystogram [iRightColumn + 1] --;
        }

        nHystSum = 0;

        for (iColumn = 0; iColumn < nHystColumns; iColumn++)
        {
            nHystSum += pHystogram [iColumn];
            pHystogram [iColumn] = nHystSum;
        }
    }

    BlockBuild_HystVertHeightesSum (p);

    HystogramMakeIntegral (pHystInt1,
                           pHystogram,
                           nHystColumns);

    HystogramMakeIntegral (pHystInt2,
                           pHystVertHeightesSum,
                           nHystColumns);

    iLeftLimit  = nHystColumns / 10;
    iRightLimit = nHystColumns - nHystColumns / 10;

    iBestPointSizeBreakingColumn = 0;

    for (iColumn = iLeftLimit; iColumn < iRightLimit; iColumn++)
    {
        int nAverageHeightBefore;
        int nAverageHeightAfter;

        if (pHystogram [iColumn] != 0)
            continue;

        if (pHystInt1 [iColumn] == 0 ||
            pHystInt1 [nHystColumns - 1] - pHystInt1 [iColumn] == 0)
        {
            continue;
        }

        nAverageHeightBefore = pHystInt2 [iColumn] / pHystInt1 [iColumn];
        nAverageHeightAfter  =
            (pHystInt2 [nHystColumns - 1] - pHystInt2 [iColumn]) /
            (pHystInt1 [nHystColumns - 1] - pHystInt1 [iColumn]);

        if (nAverageHeightBefore == 0 || nAverageHeightAfter == 0)
            continue;

        if (! (nAverageHeightBefore >= nAverageHeightAfter  * 2 ||
               nAverageHeightAfter  >= nAverageHeightBefore * 3))
        {
            continue;
        }

        nPointSizeDifference =
            MAX (nAverageHeightBefore * 1000 / nAverageHeightAfter,
                 nAverageHeightAfter  * 1000 / nAverageHeightBefore);

        if (iBestPointSizeBreakingColumn == 0 ||
            nPointSizeDifference > nBestPointSizeDifference)
        {
            iBestPointSizeBreakingColumn = iColumn;
            nBestPointSizeDifference     = nPointSizeDifference;
        }
    }

    if (iBestPointSizeBreakingColumn != 0 &&
        BlockBreakOnVertical
          (p, &q, &r, iBestPointSizeBreakingColumn + p -> Rect.xLeft, 0))
    {
# ifdef LT_DEBUG
        //if (LT_DebugGraphicsLevel >= 3)
		if(!LDPUMA_Skip(hBlocksBreaking))
        {
            pDebugBlock = q;
            LT_GraphicsBlockOutput2 ("Block was breaked using "
                                         "Vert PointSizeAnalysis");
            pDebugBlock = r;
            LT_GraphicsBlockOutput2 ("Block was breaked using "
                                         "Vert PointSizeAnalysis");
            LT_Getch ();
            LT_GraphicsClearScreen ();
        }
# endif
        return (TRUE);
    }

    return (FALSE);
}

Bool BlockBreakOnHorizontal (BLOCK *p, BLOCK **pq, BLOCK **pr,
                             int y)
{
    BLOCK *q, *r;
    ROOT *pRoot, *pNext;

    if (p -> nRoots <= 1)
        return (FALSE);

    q            = BlocksAddDescriptor ();
    q -> nNumber = ++nNextBlockNumber;
    q -> Type    = BLOCK_TEXT;

    r            = BlocksAddDescriptor ();
    r -> nNumber = ++nNextBlockNumber;
    r -> Type    = BLOCK_TEXT;

    if (p -> uFlags & BF_SMART_BREAKING_APPLIED)
    {
        q -> uFlags |= BF_SMART_BREAKING_APPLIED;
        r -> uFlags |= BF_SMART_BREAKING_APPLIED;
    }

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pNext)
    {
        pNext = pRoot -> u1.pNext;

        BlockAccountRoot ((pRoot -> yRow <= y ? q : r), pRoot);
    }

    if (q -> nRoots == 0 || r -> nRoots == 0)
    {
        BlocksRestoreBreakedBlock (p, q, r);
        return (FALSE);
    }

    BlockSetAverageHeight (q);
    BlockSetAverageHeight (r);
    BlockCalculateBreakingParameters (q);
    BlockCalculateBreakingParameters (r);

    BlocksRemoveDescriptor (p);
    if (pq != NULL) *pq = q;
    if (pr != NULL) *pr = r;
    return (TRUE);
}

Bool HorizontalBreakingCondition (BLOCK *p, int iBegin, int iEnd)
{
    int i;
    int x1, x2, y;
    int nLength;

    /* Regular case */

    if ((iEnd - iBegin + 1) * 4 >=  p -> nAverageHeight * 5)
        return (TRUE);
//    if ((iEnd - iBegin + 1) >=  p -> nAverageHeight * 5) // Piter
//        return (TRUE);

    /* Irregular case: space and line */

    if ((iEnd - iBegin + 1) * 4 <   p -> nAverageHeight * 3)
        return (FALSE);

    for (i = 0; i < nSeps; i++)
    {
        if (pSeps [i].Type != SEP_HORZ)
            continue;

        x1 = pSeps [i].xBegin;
        x2 = pSeps [i].xEnd;

        if (x1 > x2)
            EXCHANGE_INTS (x1, x2);

        y  = (pSeps [i].yBegin + pSeps [i].yEnd) / 2;

        if (y  < p -> Rect.yTop  + iBegin ||
            y  > p -> Rect.yTop  + iEnd   ||
            x1 > p -> Rect.xRight         ||
            x2 < p -> Rect.xLeft)
        {
            continue;
        }

        nLength = MIN (x2, p -> Rect.xRight) - MAX (x1, p -> Rect.xLeft);

        if (nLength > (p -> Rect.xRight - p -> Rect.xLeft + 1) * 2 / 3)
            return (TRUE);
    }

    return (FALSE);
}

Bool TryCutBlockOnHorizontal (BLOCK *p)
{
    int iColumn;
    int iZeroEnd;
    Bool bNotSpace;

    int  iBestPointSizeBreakingColumn;
    int  nPointSizeDifference;
    int  nBestPointSizeDifference = 0; /* To avoid warning on MPW C/C++ */

    BLOCK *q, *r;

# ifdef LT_DEBUG
    if (p -> pHorzHystogram == NULL)
        ErrorInternal ("pHorzHystogram not builded in TryCutBlockOnHorizontal");
# endif

    nHystColumnWidth = 1;
    nHystColumns     = p -> Rect.yBottom - p -> Rect.yTop + 1;

    memcpy (pHystogram, p -> pHorzHystogram, nHystColumns * sizeof (int));

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if(!LDPUMA_Skip(hBlocksBreaking))
        LT_GraphicsHystogramOutput ("Hystogram for horizontal breaking");
# endif

    bNotSpace = FALSE;
    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        if (pHystogram [iColumn] != 0)
        {
            bNotSpace = TRUE;
            continue;
        }

        if (! bNotSpace)
            continue;

        for (iZeroEnd = iColumn + 1;
                    iZeroEnd < nHystColumns && pHystogram [iZeroEnd] == 0;
                        iZeroEnd++)
        {
        }

        if (iZeroEnd == nHystColumns)
            break;

        if (HorizontalBreakingCondition (p, iColumn, iZeroEnd - 1) &&
            BlockBreakOnHorizontal (p, NULL, NULL, iColumn + p -> Rect.yTop))
        {
            return (TRUE);
        }

        iColumn = iZeroEnd;
    }

    /*
     *  Analysis with font accounting
     */

    if (! bOptionPointSizeAnalysis)
        return (FALSE);

    BlockBuild_HystHorzHeightesSum (p);

    HystogramMakeIntegral (pHystInt1,
                           pHystogram,
                           nHystColumns);

    HystogramMakeIntegral (pHystInt2,
                           pHystHorzHeightesSum,
                           nHystColumns);

    iBestPointSizeBreakingColumn = 0;

    for (iColumn = 0; iColumn < nHystColumns; iColumn++)
    {
        int nAverageHeightBefore;
        int nAverageHeightAfter;

        if (pHystogram [iColumn] != 0)
            continue;

        if (pHystInt1 [iColumn] == 0 ||
            pHystInt1 [nHystColumns - 1] - pHystInt1 [iColumn] <= 20)
        {
            continue;
        }

        nAverageHeightBefore = pHystInt2 [iColumn] / pHystInt1 [iColumn];
        nAverageHeightAfter  =
            (pHystInt2 [nHystColumns - 1] - pHystInt2 [iColumn]) /
            (pHystInt1 [nHystColumns - 1] - pHystInt1 [iColumn]);

        if (nAverageHeightBefore == 0 || nAverageHeightAfter == 0)
            continue;

        if (! ((nAverageHeightBefore-1)*2 >= nAverageHeightAfter  * ZagolovokBreakingCoeff ||
               (nAverageHeightAfter-1)*2  >= nAverageHeightBefore * ZagolovokBreakingCoeff))
        {
            continue;
        }

        nPointSizeDifference =
            MAX (nAverageHeightBefore * 1000 / nAverageHeightAfter,
                 nAverageHeightAfter  * 1000 / nAverageHeightBefore);

        if (iBestPointSizeBreakingColumn == 0 ||
            nPointSizeDifference > nBestPointSizeDifference)
        {
            iBestPointSizeBreakingColumn = iColumn;
            nBestPointSizeDifference     = nPointSizeDifference;
        }
    }

    iColumn = iBestPointSizeBreakingColumn;

    if (pHystInt1 [nHystColumns - 1] - pHystInt1 [iColumn] > 0 &&
        iColumn <=
            (pHystInt2 [nHystColumns - 1] - pHystInt2 [iColumn]) /
            (pHystInt1 [nHystColumns - 1] - pHystInt1 [iColumn]) / 2)
    {
        /* Strange case: its possibly dots above letters */
        return (FALSE);
    }

    if (BlockBreakOnHorizontal
          (p, &q, &r, iBestPointSizeBreakingColumn + p -> Rect.yTop))
    {
# ifdef LT_DEBUG
        //if (LT_DebugGraphicsLevel >= 3)
		if(!LDPUMA_Skip(hBlocksBreaking))
        {
            pDebugBlock = q;
            LT_GraphicsBlockOutput2 ("Block was breaked using "
                                         "Horz PointSizeAnalysis");
            pDebugBlock = r;
            LT_GraphicsBlockOutput2 ("Block was breaked using "
                                         "Horz PointSizeAnalysis");
            LT_Getch ();
            LT_GraphicsClearScreen ();
        }
# endif
        return (TRUE);
    }

    return (FALSE);
}

# define SEPARATOR_FLUCTUATION_DIVIDER 10

//Bool TryCutBlockByVerticalSeparator (BLOCK *p, int i)
Bool TryCutBlockByVerticalSeparator (BLOCK *p, int i, Bool NotWholeBlock)
{
    BLOCK *q, *r;

    int x = (pSeps [i].xBegin + pSeps [i].xEnd) / 2;
	int y = (pSeps [i].yBegin + pSeps [i].yEnd) / 2;
    int yFluc = (p -> Rect.yBottom - p -> Rect.yTop)
                    / SEPARATOR_FLUCTUATION_DIVIDER;
    int xFluc = (p -> Rect.xRight - p -> Rect.xLeft)
                    / SEPARATOR_FLUCTUATION_DIVIDER;


	if(NotWholeBlock)
	{
		if (x > p -> Rect.xLeft                             &&
			x < p -> Rect.xRight                            &&
			pSeps [i].yBegin - p -> Rect.yTop    <=   yFluc &&
			pSeps [i].yEnd   - p -> Rect.yBottom >= - yFluc &&
			BlockBreakOnVertical (p, &q, &r, x, 0))
		{
			q -> pRightSep = &pSeps [i];
			r -> pLeftSep  = &pSeps [i];
			return (TRUE);
		}
	}
	else
	{
		if(x < p -> Rect.xLeft                             ||
		   x > p -> Rect.xRight                            ||
		   pSeps[i].yBegin > p->Rect.yBottom			   ||
		   pSeps[i].yEnd   < p->Rect.yTop				   ||
		   pSeps [i].xBegin - p -> Rect.xLeft  <=   xFluc  ||
		   p -> Rect.xRight - pSeps [i].xEnd   <=   xFluc)
		{
			return FALSE;
		}
		if((pSeps[i].yBegin - p->Rect.yTop) > (p -> Rect.yBottom - pSeps [i].yEnd))
		{
			if(!BlockBreakOnHorizontal (p, &q, &r, pSeps[i].yBegin - 10))
			{
				return FALSE;
			}
		}
		else
		{
			if(!BlockBreakOnHorizontal (p, &q, &r, pSeps[i].yEnd + 10))
			{
				return FALSE;
			}
		}
		q -> pDownSep = &pSeps [i];
        r -> pUpSep   = &pSeps [i];
        return (TRUE);
	}

    return (FALSE);
}

Bool TryCutBlockByHorizontalSeparator (BLOCK *p, int i)
{
    BLOCK *q, *r;
    int y = (pSeps [i].yBegin + pSeps [i].yEnd) / 2;
    int xFluc = (p -> Rect.xRight - p -> Rect.xLeft)
                        / SEPARATOR_FLUCTUATION_DIVIDER;

    if (y > p -> Rect.yTop                             &&
        y < p -> Rect.yBottom                          &&
        pSeps [i].xBegin - p -> Rect.xLeft  <=   xFluc &&
        pSeps [i].xEnd   - p -> Rect.xRight >= - xFluc &&
        BlockBreakOnHorizontal (p, &q, &r, y))
    {
        q -> pDownSep = &pSeps [i];
        r -> pUpSep   = &pSeps [i];
        return (TRUE);
    }

    return (FALSE);
}

Bool BlockBreakByRectangle (BLOCK *p, BLOCK **pq, BLOCK **pr,
                            int xBegin, int yBegin, int xEnd, int yEnd)
{
    BLOCK *q, *r;
    ROOT *pRoot, *pNext;

    if (p -> nRoots <= 1)
        return (FALSE);

    q            = BlocksAddDescriptor ();
    q -> nNumber = ++nNextBlockNumber;
    q -> Type    = BLOCK_TEXT;

    r            = BlocksAddDescriptor ();
    r -> nNumber = ++nNextBlockNumber;
    r -> Type    = BLOCK_TEXT;

    if (p -> uFlags & BF_SMART_BREAKING_APPLIED)
    {
        q -> uFlags |= BF_SMART_BREAKING_APPLIED;
        r -> uFlags |= BF_SMART_BREAKING_APPLIED;
    }

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pNext)
    {
        pNext = pRoot -> u1.pNext;

        if (pRoot -> xColumn >= xBegin &&
            pRoot -> xColumn <  xEnd   &&
            pRoot -> yRow    >= yBegin &&
            pRoot -> yRow    <  yEnd)
        {
            BlockAccountRoot (r, pRoot);
        }
        else
        {
            BlockAccountRoot (q, pRoot);
        }
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

        q -> nEmbedding = p -> nEmbedding;
        r -> nEmbedding = p -> nEmbedding + 1;

        BlocksRemoveDescriptor (p);
        if (pq != NULL) *pq = q;
        if (pr != NULL) *pr = r;
        return (TRUE);
    }
}

Bool TryCutBlockByRectangleSeparator (BLOCK *p, int i)
{
    if (pSeps [i].xEnd   > p -> Rect.xLeft            &&
        pSeps [i].yEnd   > p -> Rect.yTop             &&
        pSeps [i].xBegin < p -> Rect.xRight           &&
        pSeps [i].yBegin < p -> Rect.yBottom          &&
        BlockBreakByRectangle (p, NULL, NULL,
                               pSeps [i].xBegin,
                               pSeps [i].yBegin,
                               pSeps [i].xEnd,
                               pSeps [i].yEnd))
    {
        return (TRUE);
    }

    return (FALSE);
}

Bool TrySpecialHorizontalCutting (BLOCK *p) /* Debugging not completed */
{
    int  nBlockWidth  = p -> Rect.xRight  - p -> Rect.xLeft + 1;
    int  nBlockHeight = p -> Rect.yBottom - p -> Rect.yTop  + 1;
    ROOT *pRoot;
    int  iLeftColumn, iRightColumn;
    int  yTop, yBottom;
    int  i;
    int  nMaximum;
    int  iLeftMaximum;
    int  iRightMaximum;
    int  iMinBreakingPoint;

# ifdef LT_DEBUG
    if (p -> pHorzHystogram == NULL)
        ErrorInternal ("pHorzHystogram not builded in TrySpecialHorizontalCutting");
# endif

    if (p -> nAverageHeight == 0)
        return (FALSE);

    if (p -> nActualHeight / p -> nAverageHeight < 10 ||
        nBlockWidth        / p -> nAverageHeight < 30)
    {
        return (FALSE);
    }

    /*
     *      Hystogram building
     */

    nHystColumns = nBlockWidth + 1;

    for (i = 0; i < nHystColumns; i++)
    {
        pHystogram   [i] = 0;
        pHystTops    [i] = nBlockHeight;
        pHystBottoms [i] = -1;
    }

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        iLeftColumn  = pRoot -> xColumn - p -> Rect.xLeft;
        iRightColumn = iLeftColumn + pRoot -> nWidth - 1;

        yTop    = pRoot -> yRow - p -> Rect.yTop;
        yBottom = yTop + pRoot -> nHeight - 1;

        for (i = iLeftColumn; i <= iRightColumn; i++)
        {
            pHystogram [i]++;

            if (yTop < pHystTops [i])
                pHystTops [i] = yTop;

            if (yBottom > pHystBottoms [i])
                pHystBottoms [i] = yBottom;
        }
    }

    /*
     *      Finding maximums in hystogram
     */


    nMaximum = 0;

    for (i = 0; i < nHystColumns; i++)
    {
        if (pHystogram [i] > nMaximum)
            nMaximum = pHystogram [i];
    }

    for (i = 0; i < nHystColumns && pHystogram [i] < nMaximum * 3 / 4; i++)
            ;

    if (i == nHystColumns)
        ErrorInternal ("i == nHystColumns in TrySpecialHorizontalCutting");

    iLeftMaximum = i;

    for (i = nHystColumns - 1; i >= 0 && pHystogram [i] < nMaximum * 3 / 4; i--)
            ;

    if (i == -1)
        ErrorInternal ("i == -1 in TrySpecialHorizontalCutting");

    iRightMaximum = i;

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 1)
	if(!LDPUMA_Skip(hBlocksBreaking))
    {
        pDebugBlock = p;
        LT_GraphicsSpecialCuttingOutput ("Special cutting condition");
    }
# endif

    /*
     *      Finding minimum break point
     */

    iMinBreakingPoint = -1;

    for (i = iLeftMaximum + 1; i < iRightMaximum; i++)
    {
        if (pHystogram [i] > nMaximum / 6)
            continue;

        if (pHystTops [i] == -1)
            continue;

        if (pHystTops [i] < nBlockHeight * 5 / 6)
            continue;

        if (p -> pHorzHystogram [pHystTops [i] - 1] != 0)
            continue;

        if (iMinBreakingPoint == -1 ||
            pHystTops [i] > pHystTops [iMinBreakingPoint])
        {
            iMinBreakingPoint = i;
        }
    }

    if (iMinBreakingPoint == -1)
        return (FALSE);

    return (BlockBreakOnHorizontal
            (
                p,
                NULL,
                NULL,
                pHystTops [iMinBreakingPoint] - 1 + p -> Rect.yTop
            ));
}

void BlocksBreak (void)
{
    BLOCK *p;
    int i;
    int nRootsInNotBreakedBlocks;

    if (!HystogramAllocateBody ())
        return;

    nRootsInNotBreakedBlocks = 0;
 AGAIN:
# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 4)
	if(!LDPUMA_Skip(hBlocksBreaking))
        LT_GraphicsBlocksOutput ("Blocks while breaking: again");
# endif
    for (p = pBlocksList; p != NULL; p = p -> pNext)
    {
        if (p -> uFlags & BF_NOT_BREAK_BLOCK)
			continue;

        for (i = 0; i < nSeps; i++)
        {
            switch (pSeps [i].Type)
            {
                default:
                    break;

                case SEP_VERT:
                    //if (TryCutBlockByVerticalSeparator   (p, i))  goto AGAIN;
					if (TryCutBlockByVerticalSeparator   (p, i, TRUE))
						goto AGAIN;
                    break;

                case SEP_HORZ:
                    if (TryCutBlockByHorizontalSeparator (p, i))
						goto AGAIN;
                    break;

                case SEP_RECT:
                    if (TryCutBlockByRectangleSeparator  (p, i))
						goto AGAIN;
                    break;
            }
        }


        //if (TryCutBlockOnVertical (p,0))          goto AGAIN;
		if (TryCutBlockOnVertical (p,0, FALSE))          goto AGAIN;

        if (bOptionSpecialHorizontalCutting &&
            TrySpecialHorizontalCutting (p))    goto AGAIN;

        if (TryCutBlockOnHorizontal (p))        goto AGAIN;

        if (bOptionSmartBreaking &&
            TrySmartBreaking (p))               goto AGAIN;

        if ( bOptionBusinessCardsLayout &&
             TryCutBlockOnVertical (p,1, FALSE))       goto AGAIN;

		// Rom
		//Handle hNotWholeLine = NULL;
		//Handle hHystogramNull = NULL;

		if(LDPUMA_Skip(hNotWholeLine))
		{
			for (i = 0; i < nSeps; i++)
			{
				switch (pSeps [i].Type)
				{
                default:
                    break;

                case SEP_VERT:
					if (TryCutBlockByVerticalSeparator   (p, i, FALSE))
						goto AGAIN;
                    break;

					/*
					case SEP_HORZ:
                    if (TryCutBlockByHorizontalSeparator (p, i))  goto AGAIN;
                    break;
					*/
				}
			}
		}

		/* Rom */
		if(LDPUMA_Skip(hHystogramNull))
		{
			if (TryCutBlockOnVertical (p,0, TRUE))          goto AGAIN;
		}



        p -> uFlags |= BF_NOT_BREAK_BLOCK;
        nRootsInNotBreakedBlocks += p -> nRoots;
        progress_set_percent (nRootsInNotBreakedBlocks * 100 / nRoots);

# ifdef LT_DEBUG
        //if (LT_DebugGraphicsLevel >= 4)
		if(!LDPUMA_Skip(hBlocksBreaking))
            LT_GraphicsBlocksOutput ("Blocks while breaking: set not break");
# endif
    }

    if (bOptionWhiteStripsBreaking)
        WhiteStripsBreaking ();

    HystogramFreeData ();
}

