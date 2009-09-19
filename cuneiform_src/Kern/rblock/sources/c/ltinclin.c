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
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTINCLIN.C - incline calculation                            *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "c_memory.h"
# include "layout.h"
# include "my_mem.h"
# include "tuner.h"
# include "sheet.h"
# include "cpage.h" // Rom
# include "cttypes.h" // Piter


int16_t nIncline;

int  *pnHeightesCounter;
int  nHeightesAllocated;
int  nHeightes;

static long lInclinesSum;
static int  nInclinesStrings;

# define BAR_MEMORY_ALLOCATION_QUANTUM  256
# define BAR_STEP                       16
# define MAX_EQUAL_HEIGHTES_DIFFERENCE  1
# define MAX_EQUAL_LOWERS_DIFFERENCE    4

struct _HorizontalBarRecord
       {
           Bool bAllocated;
           int  xLeft;
           int  nLower;
       };

typedef struct _HorizontalBarRecord HORIZONTAL_BAR_RECORD;

static HORIZONTAL_BAR_RECORD *pBar = NULL;
static int nBarRecords             = 0;

static int nMaxSkippedRecords;
static int nLetterRepresentativeHeight;

/****************************************************************************
 *   Representative letter height accounting                                *
 ***************************************************************************/
/*
# define HEIGHTES_MEMORY_ALLOCATION_QUANTUM  256

static Bool CalculateLetterRepresentativeHeight (void)
{
    int i;
    int  iHeight;

    int  iMaxRepresentativeIndex;
    int  nMaxCounter;

    pnHeightesCounter  = NULL;
    nHeightes          = 0;
    nHeightesAllocated = 0;

    // Build hystogram

    for (i = 0; i < nRoots; i++)
    {
        if (! (pRoots [i].bType & (ROOT_RECOGNIZED | ROOT_LETTER)))
                continue;

        iHeight = (int) pRoots [i].nHeight;

        if (iHeight >= nHeightesAllocated)
        {
            int nOldHeightesAllocated = nHeightesAllocated;

            nHeightesAllocated = (int)
                ((int32_t) (iHeight + HEIGHTES_MEMORY_ALLOCATION_QUANTUM)
                        * HEIGHTES_MEMORY_ALLOCATION_QUANTUM
                        / HEIGHTES_MEMORY_ALLOCATION_QUANTUM);

            pnHeightesCounter =
                realloc (pnHeightesCounter,
                          (size_t) nHeightesAllocated * sizeof (int));

            if (pnHeightesCounter == NULL)
                ErrorNoEnoughMemory ("in LTINCLIN.C,CalculateLetterRepresentativeHeight,part 1");

            memset (pnHeightesCounter + nOldHeightesAllocated,
                    0,
                    (nHeightesAllocated-nOldHeightesAllocated) * sizeof (int));
        }

        pnHeightesCounter [iHeight]++;

        if (iHeight >= nHeightes)
            nHeightes = iHeight + 1;
    }

    // Find max representative height

    iMaxRepresentativeIndex = -1;
    nMaxCounter             = 0;

    for (i = 0; i < nHeightes; i++)
    {
        if (pnHeightesCounter [i] > nMaxCounter)
        {
            iMaxRepresentativeIndex = i;
            nMaxCounter             = pnHeightesCounter [i];
        }
    }

    if (iMaxRepresentativeIndex == -1)
    {
        nLetterRepresentativeHeight = 0;
        return (FALSE);
    }

    nLetterRepresentativeHeight = iMaxRepresentativeIndex;
    nMaxSkippedRecords          = nLetterRepresentativeHeight * 4 / BAR_STEP;

    if (pnHeightesCounter != NULL)
    {
        free (pnHeightesCounter);
        pnHeightesCounter = NULL;
    }
    return (TRUE);
}

Bool FindString (int iFirst, int *piAfter, int *piLastBar)
{
    int iRoot;
    int iBar;
    int nLower;

    if (iFirst >= nRoots)
        return (FALSE);

    for (iRoot = iFirst; iRoot < nRoots; iRoot++)
    {
        if (//! (pRoots [iRoot].bType & (ROOT_RECOGNIZED | ROOT_LETTER)) ||
            abs (pRoots [iRoot].nHeight - nLetterRepresentativeHeight)
                   > MAX_EQUAL_HEIGHTES_DIFFERENCE)
        {
            continue;
        }

        iBar   = pRoots [iRoot].xColumn / BAR_STEP;
        nLower = pRoots [iRoot].yRow + pRoots [iRoot].nHeight - 1;

        if (iBar < nBarRecords && pBar [iBar].bAllocated)
        {
            if (abs (pBar [iBar].nLower - nLower)
                    > MAX_EQUAL_LOWERS_DIFFERENCE)
            {
                break;             // New line began
            }
            else
                continue;          // Skip equal bar cell
        }

        if (iBar >= nBarRecords)
        {
            int nOldBarRecords = nBarRecords;

            nBarRecords = (int) ((int32_t) (iBar + BAR_MEMORY_ALLOCATION_QUANTUM)
                                        * BAR_MEMORY_ALLOCATION_QUANTUM
                                        / BAR_MEMORY_ALLOCATION_QUANTUM);
            pBar = realloc (pBar,
                       (size_t) nBarRecords * sizeof (HORIZONTAL_BAR_RECORD));

            if (pBar == NULL)
                ErrorNoEnoughMemory ("in LTINCLIN.C,FindString,part 1");

            memset (pBar + nOldBarRecords,
                    0,
                    (nBarRecords - nOldBarRecords)
                        * sizeof (HORIZONTAL_BAR_RECORD));
        }

        pBar [iBar].bAllocated = TRUE;
        pBar [iBar].xLeft      = pRoots [iRoot].xColumn;
        pBar [iBar].nLower     = nLower;
    }

    *piAfter   = iRoot;
    *piLastBar = iBar;
    return (TRUE);
}


int CalculateIncline (int dy, int dx)
{
    if ((dx >= 0) == (dy >= 0))
        return (int)( ( (int32_t) dy * INCLINE_FACTOR + dx / 2) / dx  );
    else
        return (int)( ( (int32_t) dy * INCLINE_FACTOR - dx / 2) / dx  );
}

Bool SelectStringAndCalculateIncline (int iStart)
{
    int iLeft, iRight, oSkipped;
    int nLower = pBar [iStart].nLower;
    extern uchar fax1x2;

    for (iLeft = iStart; iLeft >= 0;)
    {
        pBar [iLeft].bAllocated = FALSE;

        for (oSkipped = 1;
                 oSkipped <= nMaxSkippedRecords && iLeft - oSkipped >= 0;
                     oSkipped++)
        {
            if (pBar [iLeft - oSkipped].bAllocated &&
                abs (pBar [iLeft - oSkipped].nLower - nLower)
                    <= MAX_EQUAL_LOWERS_DIFFERENCE)
            {
                goto NEXT_LEFT_LETTER;
            }
        }
        break;

      NEXT_LEFT_LETTER:
        iLeft -= oSkipped;
    }

    for (iRight = iStart; iRight < nBarRecords;)
    {
        pBar [iRight].bAllocated = FALSE;

        for (oSkipped = 1;
                 oSkipped <= nMaxSkippedRecords &&
                 iRight + oSkipped < nBarRecords;
                     oSkipped++)
        {
            if (pBar [iRight + oSkipped].bAllocated &&
                abs (pBar [iRight + oSkipped].nLower - nLower)
                    <= MAX_EQUAL_LOWERS_DIFFERENCE)
            {
                goto NEXT_RIGHT_LETTER;
            }
        }
        break;

      NEXT_RIGHT_LETTER:
        iRight += oSkipped;
    }

    if (fax1x2)
    {
        if (abs (pBar [iRight].xLeft - pBar [iLeft].xLeft) < INCLINE_FACTOR / 40)
            return (FALSE);
    }
    else
    {
        if (pBar [iRight].xLeft == pBar [iLeft].xLeft)
            return (FALSE);
    }

    nInclinesStrings++;
    lInclinesSum += CalculateIncline (pBar[iRight].nLower - pBar[iLeft].nLower,
                                      pBar[iRight].xLeft  - pBar[iLeft].xLeft);

    return (TRUE);
}

void MakeShadow (int *pShadow, int nLength, int nMS_Incline)
{
    int i;
    int iProjection;

    memset (pShadow, 0, (size_t) nLength * sizeof (int));

    for (i = 0; i < nRoots; i++)
    {
        if (//pRoots [i].bType & (ROOT_RECOGNIZED | ROOT_LETTER))// 03-15-94 01:30am,Pit
             abs (pRoots [i].nHeight - nLetterRepresentativeHeight)// New two string.
                   <= MAX_EQUAL_HEIGHTES_DIFFERENCE)               //
        {
            iProjection = (int) ((int32_t) nPageOffset
                + (int32_t) (pRoots [i].yRow + pRoots [i].nHeight)
                - (int32_t) pRoots [i].xColumn * nMS_Incline / INCLINE_FACTOR);

            if (iProjection >= 0 && iProjection < nLength)
                pShadow [iProjection]++;
        }
    }
}

int32_t CalculateShadowSum (int *pShadow, int nLength)
{
    int i;
    int32_t lResult;

    lResult = 0;

    for (i = 0; i < nLength; i++)
        lResult += pShadow [i] * pShadow [i];

    return (lResult);
}

# define MAX_INCLINE_CORRECTION (INCLINE_FACTOR / 40)

Bool CorrectIncline (int nRoughIncline, int *pnIncline)
{
    int  *pShadow;
    int  dIncline;
    int32_t lStartShadowSum;
    int32_t lPreviousShadowSum;
    int32_t lShadowSum;

    pShadow = malloc (nSuitablePageHeight * sizeof (int));

    if (pShadow == NULL)
                ErrorNoEnoughMemory ("in LTINCLIN.C,CorrectIncline,part 1");

    MakeShadow (pShadow, nSuitablePageHeight, nRoughIncline);
    lStartShadowSum = CalculateShadowSum (pShadow, nSuitablePageHeight);
    lPreviousShadowSum = lStartShadowSum;

    for (dIncline = 1; dIncline < MAX_INCLINE_CORRECTION; dIncline++)
    {
        MakeShadow (pShadow, nSuitablePageHeight, nRoughIncline + dIncline);
        lShadowSum = CalculateShadowSum (pShadow, nSuitablePageHeight);

        if (lShadowSum < lPreviousShadowSum)
                break;

        lPreviousShadowSum = lShadowSum;
    }

    if (dIncline == MAX_INCLINE_CORRECTION)
	{
		free(pShadow);
        return (FALSE);
	}

    if (dIncline != 1)
    {
        *pnIncline = nRoughIncline + dIncline - 1;
        free (pShadow);
        return (TRUE);
    }

    lPreviousShadowSum = lStartShadowSum;

    for (dIncline = -1; dIncline > -MAX_INCLINE_CORRECTION; dIncline--)
    {
        MakeShadow (pShadow, nSuitablePageHeight, nRoughIncline + dIncline);
        lShadowSum = CalculateShadowSum (pShadow, nSuitablePageHeight);

        if (lShadowSum < lPreviousShadowSum)
                break;

        lPreviousShadowSum = lShadowSum;
    }

    if (dIncline == -MAX_INCLINE_CORRECTION)
        return (FALSE);

    if (dIncline != -1)
    {
        *pnIncline = nRoughIncline + dIncline + 1;
        free (pShadow);
        return (TRUE);
    }

    *pnIncline = nRoughIncline;
    free (pShadow);
    return (TRUE);
}

Bool InclinesAccount (void)
{
    int  iFirstStringRoot;
    int  iAfterStringRoot;
    int  iLastBar;
    int  nRoughIncline;
    int  nCorrectedIncline;
    //int16_t  angle;
    Bool bSuccess;
	// *************** Rom
	Handle h = NULL;
	int32_t nNom,nDenom;
	Handle hPage = CPAGE_GetHandlePage(CPAGE_GetCurrentPage( ));
	h = CPAGE_TableGetFirst(hPage);
	if (h!=NULL)
	{
		if(CPAGE_TableGetSkew(hPage,h,&nNom,&nDenom) && nDenom)
		{
			nIncline = nNom*2048/nDenom;// Rom Get angle from CPAGE
			return TRUE;
		}
	}
	// *******************
    if (nRoots == 0)
        return (FALSE);

# ifdef SE_DEBUG
    if (pBar != NULL)
        ErrorInternal ("pBar != NULL in InclinesAccount");
# endif

    if (!CalculateLetterRepresentativeHeight ())
        return (FALSE);

    iFirstStringRoot = 0;
    iAfterStringRoot = 0;
    lInclinesSum     = 0;

    nInclinesStrings = 0;

    while (FindString (iFirstStringRoot, &iAfterStringRoot, &iLastBar))
    {
        SelectStringAndCalculateIncline (iLastBar);
        iFirstStringRoot = iAfterStringRoot;
    }

    // printf ("%d\n", (int) nInclinesStrings);

    if (pBar != NULL)
    {
        free (pBar);
        pBar                 = NULL;
        nBarRecords          = 0;
    }

    if (nInclinesStrings == 0)
    {
        bSuccess = FALSE;
    }
    else
    {
        nRoughIncline = (int) (lInclinesSum / nInclinesStrings);
        bSuccess = CorrectIncline (nRoughIncline, &nCorrectedIncline);
    }

    nIncline = bSuccess ? (int16_t) nCorrectedIncline : 0;
    // Piter 09-14-95 04:13pm
    //DDDif(GetSheetIncline(&angle))
    //DDD   nIncline = 2 * angle;

#ifdef PRIMAX
//if( nIncline>180 )
  nIncline=0;
#endif
    return (bSuccess);
}
*/

void IdealXY (int16_t x, int16_t y, int16_t *px, int16_t *py)
{
    *px = (int16_t) (x + (int32_t) y * nIncline / INCLINE_FACTOR);
    //*py = (int16_t) (y - (int32_t) *px * nIncline / INCLINE_FACTOR);
	*py = (int16_t) (y - (int32_t) x * nIncline / INCLINE_FACTOR); // Piter
}

void RealXY (int16_t x, int16_t y, int16_t *px, int16_t *py)
{
    *py = (int16_t) (y + (int32_t) x * nIncline / INCLINE_FACTOR);
    //*px = (int16_t) (x - (int32_t) *py * nIncline / INCLINE_FACTOR);
    *px = (int16_t) (x - (int32_t) y * nIncline / INCLINE_FACTOR); // Piter
}

void RotatePageToIdeal (void)
{
    int i;

    for (i = 0; i < nRoots; i++)
    {
/*if(  pRoots [i].xColumn < 177 &&
			 pRoots [i].yRow < 102 &&
			(pRoots [i].xColumn + pRoots [i].nWidth) > 177 &&
			(pRoots [i].yRow + pRoots [i].nHeight) > 102)
		{
			int a = 0;
		}*/
        IDEAL_XY (pRoots [i].xColumn, pRoots [i].yRow);
    }

    for (i = 0; i < nSeps; i++)
    {
        switch (pSeps [i].Type)
        {
            case SEP_VERT:
            case SEP_HORZ:
            case SEP_RECT:
                break;

            default:
                continue;
        }

        IDEAL_XY (pSeps [i].xBegin, pSeps [i].yBegin);
        IDEAL_XY (pSeps [i].xEnd,   pSeps [i].yEnd);

        if (pSeps [i].xBegin > pSeps [i].xEnd)
            EXCHANGE_INTS (pSeps [i].xBegin, pSeps [i].xEnd);

        if (pSeps [i].yBegin > pSeps [i].yEnd)
            EXCHANGE_INTS (pSeps [i].yBegin, pSeps [i].yEnd);
    }
}

/*
void InclinesFreeData (void)
{
    if (pnHeightesCounter != NULL)
    {
        free (pnHeightesCounter);
        pnHeightesCounter = NULL;
    }
}
*/
// Piter 08.07.99
void RotatePageToReal(void)
{
    int i;

    for (i = 0; i < nRoots; i++)
    {
/*if(  pRoots [i].xColumn < 177 &&
			 pRoots [i].yRow < 102 &&
			(pRoots [i].xColumn + pRoots [i].nWidth) > 177 &&
			(pRoots [i].yRow + pRoots [i].nHeight) > 102)
		{
			int a = 0;
		}*/
        REAL_XY (pRoots [i].xColumn, pRoots [i].yRow);
    }

    for (i = 0; i < nSeps; i++)
    {
        switch (pSeps [i].Type)
        {
            case SEP_VERT:
            case SEP_HORZ:
            case SEP_RECT:
                break;

            default:
                continue;
        }

        REAL_XY (pSeps [i].xBegin, pSeps [i].yBegin);
        REAL_XY (pSeps [i].xEnd,   pSeps [i].yEnd);

        if (pSeps [i].xBegin > pSeps [i].xEnd)
            EXCHANGE_INTS (pSeps [i].xBegin, pSeps [i].xEnd);

        if (pSeps [i].yBegin > pSeps [i].yEnd)
            EXCHANGE_INTS (pSeps [i].yBegin, pSeps [i].yEnd);
    }
}
