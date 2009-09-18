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
# include "my_mem.h"

#include "minmax.h"

BLOCK *pBlocksList              = NULL;
BLOCK *pHighEmbeddingBlocksList = NULL;
BLOCK *pLowEmbeddingBlocksList  = NULL;
BLOCK *pLeftBlocksList          = NULL;
BLOCK *pRightBlocksList         = NULL;
BLOCK *pTopBlocksList           = NULL;
BLOCK *pBottomBlocksList        = NULL;
int nNextBlockNumber;

BLOCK *BlocksAddDescriptor (void)
{
    if (pBlocksList == NULL)
    {
        pBlocksList = (BLOCK*)malloc (sizeof (BLOCK));
        if (pBlocksList == NULL)
        {
        #ifdef DebugFile
                STRCPY(StringError,"BlocksAddDescriptor,if(pBlocksList==NULL)");
        #endif
        ErrorNoEnoughMemory ((uchar*)"in LTBLOCKS.C,BlockAddDescriptor;part 1");
        }

        memset (pBlocksList, 0, sizeof (BLOCK));
    }
    else
    {
        pBlocksList -> pPrev =(BLOCK*) malloc (sizeof (BLOCK));
        if (pBlocksList -> pPrev == NULL)
        {
        #ifdef DebugFile
                STRCPY(StringError,"BlocksAddDescriptor,if(pPrev==NULL)");
        #endif
        ErrorNoEnoughMemory ((uchar*)"in LTBLOCKS.C,BlockAddDescriptor;part 2");
        }

        memset (pBlocksList -> pPrev, 0, sizeof (BLOCK));
        pBlocksList -> pPrev -> pNext = pBlocksList;
        pBlocksList = pBlocksList -> pPrev;
    }

    pBlocksList->language=language; // Pit 02-??-94

    return (pBlocksList);
}

void BlocksRemoveDescriptor (BLOCK *p)
{
    if (p != NULL)
    {
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


        if (p -> pHorzHystogram != NULL)
            free (p -> pHorzHystogram);

        if (p -> pNext != NULL) p -> pNext -> pPrev = p -> pPrev;
        if (p -> pPrev != NULL) p -> pPrev -> pNext = p -> pNext;

        if (p -> pLowerEmbedding != NULL)
            p -> pLowerEmbedding -> pHigherEmbedding = p -> pHigherEmbedding;
        if (p -> pHigherEmbedding != NULL)
            p -> pHigherEmbedding -> pLowerEmbedding = p -> pLowerEmbedding;

        if (p -> pRight != NULL) p -> pRight -> pLeft  = p -> pLeft;
        if (p -> pLeft  != NULL) p -> pLeft  -> pRight = p -> pRight;

        if (p -> pDown != NULL) p -> pDown -> pUp   = p -> pUp;
        if (p -> pUp   != NULL) p -> pUp   -> pDown = p -> pDown;

        free (p);
    }
}

void BlockAccountRoot (BLOCK *p, ROOT *pRoot)
{
	// Piter
    if (IS_LAYOUT_DUST (*pRoot))
	{
        p -> nDust++;
	}
// end Piter
    if (p -> nRoots == 0)
    {
        p -> nRoots       = 0;
        p -> nLetters     = 0;
        p -> nDust        = 0;
        p -> nHeightesSum = pRoot -> nHeight;

        p -> pRoots    = pRoot;
        p -> pEndRoots = pRoot;
        pRoot -> u1.pNext = NULL;

		p -> Rect.xLeft   = pRoot -> xColumn;
		p -> Rect.xRight  = pRoot -> xColumn + pRoot -> nWidth - 1;
		p -> Rect.yTop    = pRoot -> yRow;
		p -> Rect.yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
    }
    else
    {
		if(pRoot == p -> pRoots)// Piter
			return;

		pRoot -> u1.pNext = p -> pRoots;

        p -> pRoots    = pRoot;
		p -> nHeightesSum += pRoot -> nHeight;// Piter

		if (pRoot -> xColumn < p -> Rect.xLeft)
			p -> Rect.xLeft   = pRoot -> xColumn;

		if (pRoot -> xColumn + pRoot -> nWidth - 1 > p -> Rect.xRight)
			p -> Rect.xRight  = pRoot -> xColumn + pRoot -> nWidth - 1;

		if (pRoot -> yRow < p -> Rect.yTop)
			p -> Rect.yTop    = pRoot -> yRow;

		if (pRoot -> yRow + pRoot -> nHeight - 1 > p -> Rect.yBottom)
			p -> Rect.yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
    }

    p -> nRoots++;

    if (pRoot -> bType & ROOT_LETTER) p -> nLetters++;
}

void BlockSetAverageHeight (BLOCK *p)
{
    if (p -> nRoots - p -> nDust == 0)
    {
        if (p -> nRoots == 0)
            p -> nAverageHeight = 1;
        else
            p -> nAverageHeight =
                MAX(p -> nHeightesSum / p -> nRoots, 1);
    }
    else
    {
        p -> nAverageHeight =
            MAX(p -> nHeightesSum / (p -> nRoots - p -> nDust), 1);
    }
}

void BlocksExtract ()
{
    BLOCK *p;
    ROOT *pRoot;

    BlocksFreeData ();

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        if (pRoot -> nBlock == DUST_BLOCK_NUMBER ||
            pRoot -> nBlock == REMOVED_BLOCK_NUMBER)
        {
            continue;
        }

        for (p = pBlocksList; p != NULL; p = p -> pNext)
        {
            if (p -> nNumber == pRoot -> nBlock)
            {
                BlockAccountRoot (p, pRoot);
                goto FOUND;
            }
        }

        /* Block descriptor not found */
        p = BlocksAddDescriptor ();
        p -> Type = BLOCK_TEXT;
        p -> nNumber = pRoot -> nBlock;

		p -> nUserNum = pRoot ->nUserNum; // Piter
      BlockAccountRoot (p, pRoot);

FOUND:
        ;
    }


    for (p = pBlocksList; p != NULL; p = p -> pNext)
    {
        BlockSetAverageHeight (p);
        BlockCalculateBreakingParameters (p);
    }
}

void BlockSetRootsNumbers (BLOCK *p)
{
    ROOT *pRoot;

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
        pRoot -> nBlock = p -> nNumber;
}

void BlocksSetRootsNumbers (void)
{
    BLOCK *p;

    for (p = pBlocksList; p != NULL; p = p -> pNext)
        BlockSetRootsNumbers (p);
}

static ROOT *GetFrameLikeRoot (BLOCK *p)
{
    ROOT *pRoot;

    if (p -> Type != BLOCK_TEXT)
        return (NULL);

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        if (pRoot -> xColumn                        == p -> Rect.xLeft   &&
            pRoot -> yRow                           == p -> Rect.yTop    &&
            pRoot -> xColumn + pRoot -> nWidth  - 1 == p -> Rect.xRight  &&
            pRoot -> yRow    + pRoot -> nHeight - 1 == p -> Rect.yBottom)
        {
            return (pRoot);
        }
    }

    return (NULL);
}

void BlocksFreeData (void)
{
    while (pBlocksList != NULL)
        BlocksRemoveDescriptor (pBlocksList);
}
