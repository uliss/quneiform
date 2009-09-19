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
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              SESTRING.C - working with string data                       *
 *                                                                          *
 *              07/27/92 - insert checking nSpecialsLetters == nLetters     *
 *                         into StringCalculateParameters                   *
 *                                                                          *
 ***************************************************************************/

# include <stdlib.h>
# include "extract.h"
# include "func.h"
# include "my_mem.h"
# include "newfunc.h"

# include "dpuma.h"
# include "cstr.h"
#include "minmax.h"


int nCurrentFillingRoots;

Bool32 StringsUpdatedByBukvica()
{
	STRING* p;
	int nString;
//	Rect16  rect1;
	uint32_t key = 111;
	uint32_t color = 23000;

	for (nString = 0, p = pStringsUpList; p != NULL; p = p -> pDown, nString++)
    {
		if((p->nLetters == 1)&&((&pRoots [p -> pLettersList [0]])->nHeight >64))
		//if(p->nLetters == 1)
		{
			int data = 0;
			CCOM_USER_BLOCK uBlock;

			uBlock.code = CCOM_UB_CAPDROPLN;
			uBlock.data = (char*)&data;
			//*uBlock.data = nString;

			if((p->pUp!=NULL)||(p->pDown!=NULL))
			{
				if(p->pUp == NULL)
				{
					*(int*)uBlock.data = nString+1;
				}
				else
				{
					if(p->pDown == NULL)
					{
						*(int*)uBlock.data = nString-1;
					}
					else
					{
						if(abs(p->pUp->yTop - p->yTop)<=abs(p->pDown->yTop - p->yTop))
						{
							*(int*)uBlock.data = nString-1;
						}
						else
						{
							*(int*)uBlock.data = nString+1;
						}
					}
				}
				p->uFlags += CSTR_STR_CapDrop;
			}

			uBlock.size = sizeof(data);
			CCOM_SetUserBlock((pRoots [p -> pLettersList [0]]).pComp, &uBlock);

			/*
			rect1.left   = (pRoots [p -> pLettersList [0]]).xColumn;
			rect1.top    = (pRoots [p -> pLettersList [0]]).yRow;
			rect1.right  = rect1.left + (pRoots [p -> pLettersList [0]]).nWidth;
			rect1.bottom = rect1.top + (pRoots [p -> pLettersList [0]]).nHeight;

			LDPUMA_DrawRect(NULL, &rect1, 0, color, 8, key);
			*/
			//pRoot = &pRoots [pString -> pLettersList [i]];
		}
	}

	/*
	LDPUMA_Console("RBLOCK_Анализ на строки  \n");
	LDPUMA_WaitUserInput(NULL, NULL);
	LDPUMA_DeleteRects(NULL, key);
	*/

	return TRUE;
}

void StringCalculateParameters (STRING *pString)
{
    int i;
    ROOT *pRoot;
    int  nSumHeight;
    int  ySumTop;
    int  nSumTopDispersion;
    int  nSumBottomDispersion;
    Bool bSpecialFound;

    pString -> language = pCurrentBlock -> language; // Pit 02-??-94

    if (pString -> nLetters == 0)
    {
        pString -> nMiddleHeight     = 0;
        pString -> yMiddleTop        = 0;
        pString -> yMiddleBottom     = 0;
        pString -> nTopDispersion    = 0;
        pString -> nBottomDispersion = 0;
        pString -> yMin              = 0;
        pString -> yMax              = 0;
        return;
    }

    pString -> nSpecialsLetters = 0;

  AGAIN:
    nSumHeight = 0;
    ySumTop    = 0;

    for (i = 0; i < pString -> nLetters; i++)
    {
        pRoot = pRoots + pString -> pLettersList [i];

        if (pRoot -> bType & (ROOT_SPECIAL_LETTER | ROOT_SPECIAL_DUST))
            continue;

        nSumHeight     += pRoot -> nHeight;
        ySumTop        += pRoot -> yRow;
    }

    pString -> nMiddleHeight = nSumHeight
                / (pString -> nLetters - pString -> nSpecialsLetters);
    pString -> yMiddleTop    = ySumTop
                / (pString -> nLetters - pString -> nSpecialsLetters);
    pString -> yMiddleBottom = (ySumTop + nSumHeight)
                / (pString -> nLetters - pString -> nSpecialsLetters);
    pString -> yMiddleLine   =
        (pString -> yMiddleTop + pString -> yMiddleBottom) / 2;

    nSumTopDispersion    = 0;
    nSumBottomDispersion = 0;

    for (i = 0; i < pString -> nLetters; i++)
    {
        pRoot = pRoots + pString -> pLettersList [i];

        if (pRoot -> bType & (ROOT_SPECIAL_LETTER | ROOT_SPECIAL_DUST))
            continue;

        nSumTopDispersion +=
            (pRoot -> yRow - pString -> yMiddleTop) *
            (pRoot -> yRow - pString -> yMiddleTop);

        nSumBottomDispersion +=
            (pRoot -> yRow + pRoot -> nHeight - pString -> yMiddleBottom) *
            (pRoot -> yRow + pRoot -> nHeight - pString -> yMiddleBottom);
    }

    pString -> nTopDispersion =
        (int) long_sqrt ((int32_t) (nSumTopDispersion
                      / (pString -> nLetters - pString -> nSpecialsLetters)));

    pString -> nBottomDispersion =
        (int) long_sqrt ((int32_t) (nSumBottomDispersion
                      / (pString -> nLetters - pString -> nSpecialsLetters)));

    pString -> yMin = pString -> yMiddleTop
                      - MAX (2 * pString -> nTopDispersion,
                             3 * pString -> nMiddleHeight / 4);

    pString -> yMax = pString -> yMiddleBottom
                      + MAX (2 * pString -> nBottomDispersion,
                             3 * pString -> nMiddleHeight / 4);

    bSpecialFound = FALSE;

    for (i = 0; i < pString -> nLetters; i++)
    {
        pRoot = pRoots + pString -> pLettersList [i];

        if (pRoot -> bType & (ROOT_SPECIAL_LETTER | ROOT_SPECIAL_DUST))
            continue;

        if (pRoot -> yRow                        > pString -> yMiddleLine ||
            pRoot -> yRow + pRoot -> nHeight - 1 < pString -> yMiddleLine)
        {
            pRoot -> bType |= ROOT_SPECIAL_DUST;
            bSpecialFound = TRUE;
            pString -> nSpecialsLetters++;
        }
        else if (pRoot -> yRow                        < pString -> yMin ||
                 pRoot -> yRow + pRoot -> nHeight - 1 > pString -> yMax)
        {
            pRoot -> bType |= ROOT_SPECIAL_LETTER;
            bSpecialFound = TRUE;
            pString -> nSpecialsLetters++;
        }

    }

    if (! bSpecialFound)
        return;

    if (pString -> nSpecialsLetters == pString -> nLetters)
    {
        for (i = 0; i < pString -> nLetters; i++)
        {
            pRoot = pRoots + pString -> pLettersList [i];

            if (pRoot -> bType & ROOT_SPECIAL_LETTER)
            {
                pRoot -> bType &= ~(ROOT_SPECIAL_LETTER | ROOT_SPECIAL_DUST);
                pString -> nSpecialsLetters--;
            }
        }
    }

    if (pString -> nSpecialsLetters == pString -> nLetters)
		return; // Piter 22.02.00
	//        ErrorInternal ("Strange statistics in StringCalculateParameters");

    goto AGAIN;
}

void StringsFill (void)
{
    ROOT *pRoot;
    int  iMax;
    int  i;
    int  y;
    Bool    CorrectHist=FALSE;

    for (;;)
    {
# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 4)
            BlockHystogramShow (pCurrentBlock);
# endif

        iMax = 0;

        for (i = 0; i < pCurrentBlock -> nHystColumns; i++)
        {
            if (pCurrentBlock -> pHystogram [i]
                   > pCurrentBlock -> pHystogram [iMax])
            {
                iMax = i;
            }
        }

        if (pCurrentBlock -> pHystogram [iMax] == 0)
            break;

        y = pCurrentBlock -> Rect.yTop + iMax;

        StringNewDescriptor ();

        for (CorrectHist=FALSE,pRoot = pCurrentBlock -> pRoots;
                 pRoot != NULL;
                     pRoot = pRoot -> u1.pNext)
        {
            if (pRoot -> bType & ROOT_USED)
                continue;

            //if (IS_LAYOUT_DUST (*pRoot) || pRoot -> nHeight <= 4)
			//if (IS_LAYOUT_DUST (*pRoot) && pRoot->nUserNum!=IS_IN_TABLE)
			if (IS_LAYOUT_DUST (*pRoot))
                continue;

            if (pRoot -> yRow <= y &&
                pRoot -> yRow + pRoot -> nHeight >= y)
            {

                StringAddLetter1 (pRoot - pRoots);
                pRoot -> bType |= ROOT_USED;

                BlockHystogramDiscountRoot (pCurrentBlock, pRoot);
                CorrectHist=TRUE;
            }
        }
        if( !CorrectHist ) // Oleg & Pit : зацикливание UPIC31
            break;
        StringCalculateParameters (&String);

# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 4)
            LT_GraphicsCurrentStringOutput ("Current string");
# endif
        StringSortLetters (&String);
        StringAddToList ();

        nCurrentFillingRoots += String.nLetters;
        progress_set_percent (nCurrentFillingRoots * 100 / nRoots);
    }
}

void StringsListEdit (void)
{
    STRING *p, *q;

  AGAIN_P:
    for (p = pStringsList; p != NULL; p = p -> pNext)
    {
        if(p->nUserNum != IS_IN_TABLE)
        {
        if (p -> nRecognized  == 0  &&
            p -> yBottom - p -> yTop + 1 < pCurrentBlock -> nAverageHeight)
        {
            if (p == pStringsList)
                {
                  StringRemove (p);
                  goto AGAIN_P;
                }
            else if (p == pStringsListEnd)
                {
                   StringRemove (p);
                   break;
                }
            else
                {
                    p = p -> pPrev;
                    StringRemove (p -> pNext);
                 }
        }
        }

      AGAIN_Q:
        for (q = p -> pNext; q != NULL; q = q -> pNext)
        {
            if (StringIncludes (p, q))
            {
                if (q -> xLeft < p -> xLeft)
                    p -> xLeft = q -> xLeft;

                if (q -> xRight > q -> xRight)
                    p -> xRight = q -> xRight;
                StringRemove (q);
                goto AGAIN_Q;
            }
        }
    }
}

static int nStripHeight;
static int nDustUpper, nDustLower;
static int nDustLeft, nDustRight;
static int nDustGap;

static Bool PassForDust (STRING *pString, ROOT *pRootsBegin, ROOT *pRootsAfter)
{
    Bool bStripWasExpanded = FALSE;
    ROOT *pRoot;

    for (pRoot = pRootsBegin; pRoot < pRootsAfter; pRoot++)
    {
        if (pRoot -> nBlock != nCurrentBlock &&
            pRoot -> nBlock != 0)
        {
            continue;
        }

        if ((pRoot -> bType & ROOT_USED)                      ||
            (pRoot -> bType & ROOT_SPECIAL_LETTER)            ||
            pRoot -> yRow                        > nDustLower ||
            pRoot -> yRow + pRoot -> nHeight - 1 < nDustUpper)
        {
            continue;
        }

        if (pRoot -> xColumn < nDustLeft)
        {
            if (nDustLeft - (pRoot -> xColumn + pRoot -> nWidth)
                    >= nDustGap)
            {
                continue;
            }

            nDustLeft         = pRoot -> xColumn;
            bStripWasExpanded = TRUE;
        }

        if (pRoot -> xColumn + pRoot -> nWidth - 1 > nDustRight)
        {
            if (pRoot -> xColumn - nDustRight >= nDustGap)
            {
                continue;
            }

            nDustRight        = pRoot -> xColumn + pRoot -> nWidth - 1;
            bStripWasExpanded = TRUE;
        }

        pRoot -> bType |= ROOT_USED;
        StringAddDust2 (pString, pRoot - pRoots);
    }

    return (bStripWasExpanded);
}

void StringDustAccount (STRING *pString)
{
    int i;
    ROOT *pLocalRootsBegin, *pLocalRootsAfter;
    Bool bExpanded1, bExpanded2;

    nStripHeight = pString -> yBottom - pString -> yTop + 1;
    nDustUpper   = pString -> yTop - nStripHeight / 2;
    nDustLower   = pString -> yBottom + nStripHeight / 2;
    nDustLeft    = pString -> xLeft;
    nDustRight   = pString -> xRight;
    nDustGap     = nDustLower - nDustUpper + 1;

    RootStripsGetLoopParameters
    (
        nDustUpper,
        nDustLower,
        &pLocalRootsBegin,
        &pLocalRootsAfter
    );

    if (pLocalRootsBegin == NULL)
        return;

    do
    {
        bExpanded1 = PassForDust (pString, pLocalRootsBegin, pLocalRootsAfter);
        bExpanded2 = PassForDust (pString, pAfterOriginalRoots, pAfterRoots);
    }
    while (bExpanded1 || bExpanded2);

    for (i = 0; i < pString -> nDust; i++)
        pRoots [pString -> pDustList [i]].bType &= ~ROOT_USED;

    StringSortDust (pString);
}

void StringsDustAccount (void)
{
    STRING *pString;

    for (pString = pStringsList; pString != NULL; pString = pString -> pNext)
    {
/*  STDD19 !!!
        if (! (pString -> uFlags & SF_SPECIAL))
            StringDustAccount (pString);
*/
        StringDustAccount (pString);
    }
}

Bool StringIsTrash (STRING *pString)
{
    int nBigDust;
    int nBigDustHeight;
    int i;
    ROOT *pRoot;

    if (pString -> nDust < pString -> nLetters)
        return (FALSE);

    nBigDust       = 0;
    nBigDustHeight = pString -> nMiddleHeight / 2;

    for (i = 0; i < pString -> nDust; i++)
    {
        pRoot = & pRoots [pString -> pDustList [i]];

        if (pRoot -> nHeight >= nBigDustHeight                          &&
            pRoot -> xColumn                       <= pString -> xRight &&
            pRoot -> xColumn + pRoot -> nWidth - 1 >= pString -> xLeft)
        {
            nBigDust++;
        }
    }
                                                       /* STDG20 */
    return (pString -> nLetters < 30 && nBigDust > pString -> nLetters ||
                                        nBigDust > 2 * pString -> nLetters);
}

void StringsRemoveTrash (void)
{
    STRING *pString;
    STRING *pNext;

    pString = pStringsList;

    while (pString != NULL)
    {
        pNext = pString -> pNext;

        if (StringIsTrash (pString))
            StringRemove (pString);

        pString = pNext;
    }
}

void StringsForming (void)
{
# ifdef SE_DEBUG
    if (pCurrentBlock -> pHystogram == NULL ||
        pCurrentBlock -> nHystColumns == 0)
    {
        ErrorInternal ("Empty hystogram");
    }
# endif

    StringPrepare ();
    StringsFill ();
    //RootsRestoreNonLayoutData_ForBlock (pCurrentBlock);

    if (bOptionBusinessCardsLayout)
    {
        StringFree ();
        StringsBreakOnVertical ();
        StringPrepare ();

# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 1)
            LT_GraphicsStringsOutput ("After breaking on vertical");
# endif
    }

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 2)
        LT_GraphicsStringsOutput ("Before edit");
# endif

    StringsListEdit ();

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 2)
        LT_GraphicsStringsOutput ("After edit");
# endif

    StringsProcessSpecials ();

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 2)
        LT_GraphicsStringsOutput ("After processing specials");
# endif

    StringsDustAccount ();

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 1)
        LT_GraphicsStringsOutput ("After dust accounting");
# endif

    StringFree ();

    if (! bOptionBusinessCardsLayout)
    {
        StringsBreakOnVertical ();

# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 1)
            LT_GraphicsStringsOutput ("After breaking on vertical");
# endif
    }

    StringsRemoveTrash ();

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 1)
        LT_GraphicsStringsOutput ("After removing trash strings");

    if (SE_DebugGraphicsLevel >= 3)
    {
        LT_GraphicsStringsForwardOrderOutput  ("Forward strings order");
        LT_GraphicsStringsBackwardOrderOutput ("Backward strings order");
        LT_GraphicsStringsUpOrderOutput       ("Up strings order");
        LT_GraphicsStringsDownOrderOutput     ("Down strings order");
    }
# endif

	StringsUpdatedByBukvica();
    StringsListOutput ();
}
