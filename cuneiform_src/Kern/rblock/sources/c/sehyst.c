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
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              SEHYST.C - Blocks hystograms                                *
 *                                                                          *
 ***************************************************************************/

# include <string.h>
# include <time.h>

# include "c_memory.h"
# include "extract.h"
# include "my_mem.h"
# include "newfunc.h"


void BlocksHystogramsAllocate (void)
{
    BLOCK *p;

    for (p = pBlocksList; p != NULL; p = p -> pNext)
    {
        p -> nHystColumns = p -> Rect.yBottom - p -> Rect.yTop + 1;
        p -> pHystogram   = malloc (p -> nHystColumns * sizeof (int));

        if (p -> pHystogram == NULL)
            ErrorNoEnoughMemory ("in SEHYST.C,BlocksHystogramsAllocate,part 1");

        memset (p -> pHystogram, 0, p -> nHystColumns * sizeof (int));
    }
}

void BlocksHystogramsBuild (void)
{
    ROOT       *pRoot;
    BLOCK *pBlock;
    int iBegin;
    int iEnd;
    int i;

    BlocksHystogramsAllocate ();

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        if (pRoot -> nBlock == REMOVED_BLOCK_NUMBER)
            continue;

# ifdef SE_DEBUG
        if (pRoot -> nBlock > nBlocks)
            ErrorInternal ("Bad number of blocks");
# endif
        //if (IS_LAYOUT_DUST (*pRoot) || pRoot -> nHeight <= 4)
		//if (IS_LAYOUT_DUST (*pRoot)&& pRoot->nUserNum!=IS_IN_TABLE)
		if (IS_LAYOUT_DUST (*pRoot))
            continue;

        pBlock = pBlockPointer [pRoot -> nBlock];

        if (pBlock == NULL)
            continue;

        iBegin = pRoot -> yRow - pBlock -> Rect.yTop;
        iEnd   = (pRoot -> yRow + pRoot -> nHeight - 1) - pBlock -> Rect.yTop;

		if(iBegin<0)
		   iBegin=0;
	    if(iEnd>=pBlock->nHystColumns)
		   iEnd=pBlock->nHystColumns-1;

        for (i = iBegin; i <= iEnd; i++)
            pBlock -> pHystogram [i] += pRoot -> nWidth;
    }
}

void BlockHystogramDiscountRoot (BLOCK *pBlock, ROOT *pRoot)
{
    int iBegin;
    int iEnd;
    int i;

    iBegin = pRoot -> yRow - pBlock -> Rect.yTop;
    iEnd   = (pRoot -> yRow + pRoot -> nHeight - 1) - pBlock -> Rect.yTop;

	if(iBegin<0)
		iBegin=0;
	if(iEnd>=pBlock->nHystColumns)
		iEnd=pBlock->nHystColumns-1;

    for (i = iBegin; i <= iEnd; i++)
        pBlock -> pHystogram [i] -= pRoot -> nWidth;
}

void BlocksHystogramsFreeData (void)
{
    BLOCK *p;

    for (p = pBlocksList; p != NULL; p = p -> pNext)
    {
        if (p -> pHystogram != NULL)
        {
            free (p -> pHystogram);
            p -> pHystogram = NULL;
        }
    }
}

# ifdef SE_DEBUG
void BlockHystogramShow (BLOCK *pBlock)
{
    char szBuffer [128];

    HystogramAllocateBody ();
    nHystColumns     = pBlock -> nHystColumns;
    nHystColumnWidth = 1;

    memcpy (pHystogram,
            pBlock -> pHystogram,
            nHystColumns * sizeof (int));

    sprintf (szBuffer, "Block %d", pBlock -> nNumber);
    LT_GraphicsHystogramOutput (szBuffer);
    HystogramFreeData ();
}
# endif
