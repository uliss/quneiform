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
 *                       M A D E B U G                                      *
 *                                                                          *
 *              Tool for memory allocation debugging                        *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              MADEBUG.C - functions for memory management checking        *
 *                                                                          *
 ***************************************************************************/

//  # include <conio.h>
# include <stdio.h>
# include <stdlib.h>

# include "c_memory.h"
# include "lterrors.h"
# include "madebug.h"
# ifdef PC_TYPE
# ifdef MA_DEBUG
# undef malloc
# undef calloc
# undef realloc
# undef free

int MA_DebugLevel = 0;

static size_t AvailableAmount [MAX_HEAP_CHECKINGS];
static int    nChecking = 0;

static unsigned long ulMinPointer = ~0;
static unsigned long ulMaxPointer = 0;

static int    nMallocs;
static int    nMalloc0s;
static size_t stMallocSize;
static int    nCallocs;
static int    nCalloc0s;
static size_t stCallocSize;
static int    nReallocs;
static size_t stReallocSize;
static int    nRealloc0s;
static size_t stRealloc0Size;
static int    nFrees;
static size_t stFreeSize;
static int    nFree0s;

static int    nAllocated;
static size_t stAllocated;
static int    nMaxAllocated;
static size_t stMaxAllocated;

static void AccountSumAllocation (void)
{
    nAllocated  = nMallocs + nCallocs + nRealloc0s - (nFrees - nFree0s),
    stAllocated = stMallocSize + stCallocSize + stReallocSize - stFreeSize;

    if (nAllocated > nMaxAllocated)
        nMaxAllocated = nAllocated;

    if (stAllocated > stMaxAllocated)
        stMaxAllocated = stAllocated;
}

void *my_calloc (size_t n, size_t size, char *pFilename, int nLine)
{
    void *p;

    nCallocs++;
    if (n == 0 || size == 0) nCalloc0s++;

    p = calloc (n, size);

    if (MA_DebugLevel >= 2)
    {
        printf ("calloc  (%10d, %10d)  return (%10p) : %s (%d)\n",
                    n, size, p, pFilename, nLine);
    }

    if (p != NULL) stCallocSize += _msize (p);
    AccountSumAllocation ();
    return (p);
}

void my_free(void *ptr, char *pFilename, int nLine)
{
    nFrees++;
    if (ptr != NULL) stFreeSize += _msize (ptr);

    if (ptr == NULL)
    {
        nFree0s++;
    }

    if (MA_DebugLevel >= 2)
    {
        printf ("free    (%10p (%9d)) : %s (%d)\n",
                ptr, ptr != NULL ? _msize (ptr) : 0, pFilename, nLine);
    }

    if (ptr != NULL)
    {
        if ((unsigned long) ptr < ulMinPointer)
            ulMinPointer = (unsigned long) ptr;

        if ((unsigned long) ptr > ulMaxPointer)
            ulMaxPointer = (unsigned long) ptr;
    }

    AccountSumAllocation ();
    free (ptr);
}

void *my_malloc(size_t size, char *pFilename, int nLine)
{
    void *p;

    nMallocs++;
    if (size == 0) nMalloc0s++;

    p = malloc (size);

    if (MA_DebugLevel >= 2)
    {
        printf ("malloc  (%10d)              return (%10p) : %s (%d)\n",
                size, p, pFilename, nLine);
    }

    stMallocSize += p != NULL ? _msize (p) : 0;
    AccountSumAllocation ();
    return (p);
}

void *my_realloc(void *ptr, size_t size, char *pFilename, int nLine)
{
    void *p;

    nReallocs++;
    if (ptr != NULL) stReallocSize -= _msize (ptr);

    if (ptr == NULL)
    {
        nRealloc0s++;
        stRealloc0Size += size;
    }

    if (ptr != NULL)
    {
        if ((unsigned long) ptr < ulMinPointer)
            ulMinPointer = (unsigned long) ptr;

        if ((unsigned long) ptr > ulMaxPointer)
            ulMaxPointer = (unsigned long) ptr;
    }

    p = realloc (ptr, size);

    if (MA_DebugLevel >= 2)
    {
        printf ("realloc (%10p, %10d)  return (%10p) : %s (%d)\n",
                ptr, size, p, pFilename, nLine);
    }

    stReallocSize += p != NULL ? _msize (p) : 0;
    AccountSumAllocation ();
    return (p);
}

void AllocationsAccountingOpen (void)
{
    if (nChecking >= MAX_HEAP_CHECKINGS)
    {
        if (MA_DebugLevel >= 1)
            printf ("Array of heap checkings overflow\n");
    }
    else
    {
     //   AvailableAmount [nChecking++] = _memavl ();		 // AK WatcomC ? This module not need?
    }

    nMallocs       = 0;
    nMalloc0s      = 0;
    stMallocSize   = 0;
    nCallocs       = 0;
    nCalloc0s      = 0;
    stCallocSize   = 0;
    nReallocs      = 0;
    stReallocSize  = 0;
    nRealloc0s     = 0;
    stRealloc0Size = 0;
    nFrees         = 0;
    stFreeSize     = 0;
    nFree0s        = 0;

    nAllocated     = 0;
    stAllocated    = 0;
    nMaxAllocated  = 0;
    stMaxAllocated = 0;
}

void AllocationsAccountingClose (void)
{
    int i;

    if (MA_DebugLevel < 1)
        return;

    if (nChecking >= MAX_HEAP_CHECKINGS)
        printf ("Array of heap checkings overflow\n");
    else
//        AvailableAmount [nChecking++] = _memavl ();	//AK 11.03.97

    printf ("Current heap checkings array:\n");

    for (i = 0; i < nChecking; i++)
    {
        printf ("%s: %u\n",
                    (i & 1) == 0 ? "Begin " : "End   ",
                    AvailableAmount [i]);
    }

    printf ("\nArea: %lx-%lx (%ld-%ld)\n",
                ulMinPointer, ulMaxPointer,
                ulMinPointer, ulMaxPointer);

    printf ("\nFunction       Calls      Size\n");
    printf ("----------------------------------\n");

    printf ("malloc         %7d    %8d\n", nMallocs,   stMallocSize);
    printf ("malloc (0)     %7d\n", nMalloc0s);
    printf ("calloc         %7d    %8d\n", nCallocs,   stCallocSize);
    printf ("calloc (0, 0)  %7d\n", nCalloc0s);
    printf ("realloc (NULL) %7d    %8d\n", nRealloc0s, stRealloc0Size);
    printf ("realloc        %7d    %8d\n", nReallocs,  stReallocSize);
    printf ("free (NULL)    %7d      \n", nFree0s);
    printf ("free           %7d    %8d\n", nFrees,     stFreeSize);
    printf ("----------------------------------\n");
    printf ("result         %7d    %8d\n", nAllocated, stAllocated);
    printf ("----------------------------------\n");
    printf ("maximum calls  %7d    %8s\n", nMaxAllocated, "");
    printf ("maximum size   %7s    %8d\n\n", "", stMaxAllocated);
}
# endif
# endif
