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
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "c_memory.h"
# include "extract.h"
# include "my_mem.h"


# define DUST_LIST_MEMORY_ALLOCATION_QUANTUM 128
# define DUST_LIST_MEMORY_ALLOCATION_MASK    127
# define DUST_LIST_MEMORY_ALLOCATION_SHIFT   7

STRING String;
Bool bNeedFreeString = FALSE;
STRING *pStringsList     = NULL;
STRING *pStringsListEnd  = NULL;
STRING *pStringsUpList   = NULL;
STRING *pStringsDownList = NULL;

void StringPrepare (void)
{
    StringFree ();

    String.pLettersList =(int*) malloc (nRoots * sizeof (int));
    if (String.pLettersList == NULL)
        ErrorNoEnoughMemory ((uchar*)"in SESTRING.C,StringPrepare,part 1");

    String.pDustList =(int*) malloc (nRoots * sizeof (int));
    if (String.pDustList == NULL)
        ErrorNoEnoughMemory ((uchar*)"in SESTRING.C,StringPrepare,part 2");

    bNeedFreeString = TRUE;
}

void StringFree (void)
{
    if (bNeedFreeString)
    {
        if (String.pLettersList != NULL) free (String.pLettersList);
        if (String.pDustList    != NULL) free (String.pDustList);
        bNeedFreeString = FALSE;
    }
}

void StringNewDescriptor (void)
{
    String.uFlags           = SF_NULL;
    String.nLetters         = 0;
    String.nDust            = 0;
    String.nRecognized      = 0;
    String.nSpecialsLetters = 0;
}

Bool StringIncludes (STRING *p, STRING *q)
{
    int    i;
    ROOT * pRoot;
    int    nLettersSquare;
    int    nStringSquare;
    Bool   bNotIncludes;

    if ((p -> uFlags & SF_SPECIAL)     ||
        (q -> uFlags & SF_SPECIAL)     ||
        p -> yMin > q -> yMiddleTop    ||
        p -> yMax < q -> yMiddleBottom
       )
    {
        return FALSE;
    }

    nLettersSquare = 0;

    for (i = 0; i < q -> nLetters; i++)
    {
        pRoot           = & pRoots [q -> pLettersList [i]];
        nLettersSquare += pRoot -> nWidth * pRoot -> nHeight;
    }

    nStringSquare = (q -> xRight - q -> xLeft + 1)
                    * (q -> yBottom - q -> yTop + 1);

    if (bOptionBusinessCardsLayout)
    {
           bNotIncludes =
            (
                (q -> uFlags & SF_VERTCUT) &&
                q -> nLetters > 0  &&
                nLettersSquare   >= nStringSquare / 3
            )
            ||
            (
              q -> nLetters >= 5  &&
              q -> nRecognized >= q -> nLetters / 2  &&
              nLettersSquare   >= nStringSquare / 3
            );
    }
    else
    {
        bNotIncludes =
            q -> nLetters    >= 5                  &&
            q -> nRecognized >= q -> nLetters / 2  &&
            q -> yMiddleTop  >  p -> yMiddleBottom &&
            nLettersSquare   >= nStringSquare / 2;
    }

    if (bNotIncludes)
    {
# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 2)
        {
         char bf[64];
         sprintf (bf, "Not Incl fl %x l %d r %d" ,
           q->uFlags,q->nLetters, q->nRecognized );
            LT_GraphicsStringsOutput (bf);
        }
# endif
        return (FALSE);
    }
    else
    {
# ifdef SE_DEBUG
        if (SE_DebugGraphicsLevel >= 2)
        {
         char bf[64];
         sprintf (bf, "Incl fl %x l %d r %d" ,
           q->uFlags,q->nLetters, q->nRecognized );
            LT_GraphicsStringsOutput (bf);
        }
# endif
        return (TRUE);
    }
}

void StringRemove (STRING *p)
{
    int i;

    for (i = 0; i < p -> nLetters; i++)
        pRoots [p -> pLettersList [i]].bType &= ~ROOT_USED;

    if (p == pStringsList)
        pStringsList = pStringsList -> pNext;

    if (p == pStringsListEnd)
        pStringsListEnd = pStringsListEnd -> pPrev;

    if (p -> pPrev != NULL) p -> pPrev -> pNext = p -> pNext;
    if (p -> pNext != NULL) p -> pNext -> pPrev = p -> pPrev;

    if (p == pStringsUpList)
        pStringsUpList = pStringsUpList -> pDown;

    if (p == pStringsDownList)
        pStringsDownList = pStringsDownList -> pUp;

    if (p -> pUp   != NULL) p -> pUp   -> pDown = p -> pDown;
    if (p -> pDown != NULL) p -> pDown -> pUp   = p -> pUp;

    if (p -> pLettersList != NULL)
        free (p -> pLettersList);

    if (p -> pDustList != NULL)
        free (p -> pDustList);

    free (p);
}

void StringAccountRectangle1 (int iRoot)
{
    ROOT *pRoot = &pRoots [iRoot];

    if (! (String.uFlags & SF_RECT_ACCOUNTED))
    {
        String.xLeft   = pRoot -> xColumn;
        String.yTop    = pRoot -> yRow;
        String.xRight  = pRoot -> xColumn + pRoot -> nWidth - 1;
        String.yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
        String.uFlags  |= SF_RECT_ACCOUNTED;
    }
    else
    {
        if (String.xLeft > pRoot -> xColumn)
            String.xLeft   = pRoot -> xColumn;

        if (String.yTop  > pRoot -> yRow)
            String.yTop    = pRoot -> yRow;

        if (String.xRight < pRoot -> xColumn + pRoot -> nWidth - 1)
            String.xRight  = pRoot -> xColumn + pRoot -> nWidth - 1;

        if (String.yBottom < pRoot -> yRow + pRoot -> nHeight - 1)
            String.yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
    }
}

void StringAccountRectangle2 (STRING *pString, int iRoot)
{
    ROOT *pRoot = &pRoots [iRoot];

    if (! (pString -> uFlags & SF_RECT_ACCOUNTED))
    {
        pString -> xLeft   = pRoot -> xColumn;
        pString -> yTop    = pRoot -> yRow;
        pString -> xRight  = pRoot -> xColumn + pRoot -> nWidth - 1;
        pString -> yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
        pString -> uFlags  |= SF_RECT_ACCOUNTED;
    }
    else
    {
        if (pString -> xLeft > pRoot -> xColumn)
            pString -> xLeft   = pRoot -> xColumn;

        if (pString -> yTop  > pRoot -> yRow)
            pString -> yTop    = pRoot -> yRow;

        if (pString -> xRight < pRoot -> xColumn + pRoot -> nWidth - 1)
            pString -> xRight  = pRoot -> xColumn + pRoot -> nWidth - 1;

        if (pString -> yBottom < pRoot -> yRow + pRoot -> nHeight - 1)
            pString -> yBottom = pRoot -> yRow + pRoot -> nHeight - 1;
    }
}

void StringCountRecog (STRING *q)
{
    int i, j;
    q->nRecognized = 0;
    for (i = 0; i < q -> nLetters; i++)
    {
        j = q -> pLettersList [i];
        if (pRoots [j].bType & ROOT_LETTER)
          q->nRecognized++;
    }
}

void StringAddLetter1 (int iRoot)
{
    String.pLettersList [String.nLetters++] = iRoot;

    if (pRoots [iRoot].bType & ROOT_LETTER)
        String.nRecognized++;

    StringAccountRectangle1 (iRoot);
}

void StringAddDust2 (STRING *pString, int iRoot)
{
    if ((pString -> nDust & DUST_LIST_MEMORY_ALLOCATION_MASK) == 0)
    {
        pString -> pDustList =(int*)
            realloc (pString -> pDustList,
                    (size_t)
                    ((((pString -> nDust
                        >> DUST_LIST_MEMORY_ALLOCATION_SHIFT)
                       + 1)
                      << DUST_LIST_MEMORY_ALLOCATION_SHIFT)
                     * sizeof (int)));

        if (pString -> pDustList == NULL)
        ErrorNoEnoughMemory ((uchar*)"in SESTRING.C,StringAddDust2,part 1");
    }

    pString -> pDustList [pString -> nDust++] = iRoot;
}

STRING *StringAddToList (void)
{
    STRING *pNew;
    STRING *p;

    pNew =(STRING*) malloc (sizeof (STRING));
    if (pNew == NULL)
        ErrorNoEnoughMemory ((uchar*)"in SESTRING.C,StringAddToList,part 1");

    memcpy (pNew, &String, sizeof (String));

    if (String.nLetters != 0)
    {
        pNew -> pLettersList =(int*) malloc (String.nLetters * sizeof (int));

        if (pNew -> pLettersList == NULL)
        ErrorNoEnoughMemory ((uchar*)"in SESTRING.C,StringAddToList,part 2");

        memcpy (pNew -> pLettersList,
                String.pLettersList,
                String.nLetters * sizeof (int));
    }
    else
    {
        pNew -> pLettersList = NULL;
    }

    if (String.nDust != 0)
    {
        pNew -> pDustList =(int*) malloc (String.nDust * sizeof (int));

        if (pNew -> pDustList == NULL)
        ErrorNoEnoughMemory ((uchar*)"in SESTRING.C,StringAddToList,part 3");

        memcpy (pNew -> pDustList,
                String.pDustList,
                String.nDust * sizeof (int));
    }
    else
    {
        pNew -> pDustList = NULL;
    }

    if (pStringsList == NULL)
    {
        pStringsList    = pNew;
        pStringsListEnd = pNew;
        pNew -> pNext   = NULL;
        pNew -> pPrev   = NULL;
    }
    else
    {
        pStringsListEnd -> pNext = pNew;
        pNew -> pNext            = NULL;
        pNew -> pPrev            = pStringsListEnd;
        pStringsListEnd          = pNew;
    }

    if (pStringsUpList == NULL)
    {
        pStringsUpList = pStringsDownList = pNew;
        pNew -> pUp    = NULL;
        pNew -> pDown  = NULL;
    }
    else if (! (pStringsUpList -> uFlags & SF_NEED_DELETE) &&
             (pStringsUpList -> yMiddleTop > pNew -> yMiddleBottom ||
              pStringsUpList -> yMiddleBottom > pNew -> yMiddleTop &&
              pStringsUpList -> xLeft > pNew -> xLeft))
    {
        pNew -> pUp           = NULL;
        pNew -> pDown         = pStringsUpList;
        pStringsUpList -> pUp = pNew;
        pStringsUpList        = pNew;
    }
    else
    {
        for (p = pStringsUpList; p -> pDown != NULL; p = p -> pDown)
        {
            if (p -> pDown -> uFlags & SF_NEED_DELETE)
                continue;

            if (pNew -> yMiddleBottom <= p -> pDown -> yMiddleTop ||
                pNew -> yMiddleTop    <= p -> pDown -> yMiddleBottom &&
                pNew -> xLeft         <= p -> pDown -> xLeft)
            {
                break;
            }
        }

        if (p -> pDown == NULL)
        {
            pNew -> pUp               = pStringsDownList;
            pNew -> pDown             = NULL;
            pStringsDownList -> pDown = pNew;
            pStringsDownList          = pNew;
        }
        else
        {
            pNew -> pUp          = p;
            pNew -> pDown        = p -> pDown;
            p    -> pDown -> pUp = pNew;
            p    -> pDown        = pNew;
        }
    }

    return (pNew);
}

static int StringListCompProc (const void *p1, const void *p2)
{
    return (pRoots [*((int*)(p1))].xColumn - pRoots [*((int*)(p2))].xColumn);
}

void StringSortLetters (STRING *pString)
{
    q_sort ((char *) pString -> pLettersList,
           pString -> nLetters,
           sizeof (int),
           StringListCompProc);
}

void StringSortDust (STRING *pString)
{
    q_sort ((char *) pString -> pDustList,
           pString -> nDust,
           sizeof (int),
           StringListCompProc);
}

void StringOutput (void)
{
    extern void file_string(STRING * n);
    String.nBlock = nCurrentBlock;
    String.nUserNum = pCurrentBlock->nUserNum;// Piter 08-17-95 06:41pm
    String.yBottom++;
    String.xRight++;

    file_string(&String);
}

void StringsListOutput (void)
{
# ifdef LT_DUMP
    extern FILE *pfListing;
# endif

    while (pStringsUpList != NULL)
    {
# ifdef LT_DUMP
        fprintf (pfListing,
             "String: Letters: %d, Dust: %d, Rect: [%d, %d]-[%d, %d]\n",
             (int) pStringsUpList -> nLetters,
             (int) pStringsUpList -> nDust,
             (int) pStringsUpList -> xLeft,
             (int) pStringsUpList -> yTop,
             (int) pStringsUpList -> xRight,
             (int) pStringsUpList -> yBottom);
# endif
        memcpy (&String, pStringsUpList, sizeof (String));
        StringOutput ();
        StringRemove (pStringsUpList);
    }

    StringsFreeMemory ();
# ifdef LT_DUMP
    fprintf (pfListing, "\n");
# endif
}

void StringUpdate (STRING *pString)
{
    int i;

    pString -> uFlags &= ~(SF_RECT_ACCOUNTED | SF_REPRESENTATION_ACCOUNTED);

    for (i = 0; i < pString -> nLetters; i++)
    {
        pRoots [pString -> pLettersList [i]].bType &= ~ROOT_SPECIAL_LETTER;
        pRoots [pString -> pLettersList [i]].bType |= ROOT_USED;
        StringAccountRectangle2 (pString, pString -> pLettersList [i]);
    }

    StringSortLetters (pString);
    StringSortDust    (pString);
    StringCalculateParameters (pString);
    pString -> uFlags &= ~SF_NEED_UPDATE;
}

void StringsListUpdate (void)
{
    STRING *pString, *pNext;

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 4)
        LT_GraphicsStringsOutput ("Before remove");
# endif

    for (pString = pStringsList; pString != NULL; pString = pNext)
    {
        pNext = pString -> pNext;

        if (pString -> uFlags & SF_NEED_DELETE)
            StringRemove (pString);
    }

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 4)
        LT_GraphicsStringsOutput ("Before update");
# endif

    for (pString = pStringsList; pString != NULL; pString = pString -> pNext)
    {
        if (pString -> uFlags & SF_NEED_UPDATE)
            StringUpdate (pString);
    }

# ifdef SE_DEBUG
    if (SE_DebugGraphicsLevel >= 4)
        LT_GraphicsStringsOutput ("Updated");
# endif
}

void StringsFreeMemory (void)
{
    STRING *pTemp;

    while (pStringsList != NULL)
    {
        pTemp = pStringsList;
        pStringsList = pStringsList -> pNext;

        if (pTemp -> pLettersList != NULL)
            free (pTemp -> pLettersList);

        if (pTemp -> pDustList != NULL)
            free (pTemp -> pDustList);

        free (pTemp);
    }

    pStringsList     = NULL;
    pStringsListEnd  = NULL;
    pStringsUpList   = NULL;
    pStringsDownList = NULL;
    StringFree ();
}
