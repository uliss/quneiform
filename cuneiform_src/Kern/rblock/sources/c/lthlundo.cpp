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
 *              LTHLUNDO.C - hand layout backup / restore procedures        *
 *                                                                          *
 ***************************************************************************/

# include <string.h>

# include "c_memory.h"
# include "layout.h"
# include "my_mem.h"


# define MAX_BACKUP_STACK_DEPTH   10

BACKUP_COPY *pBC_First   = NULL;
BACKUP_COPY *pBC_Last    = NULL;
BACKUP_COPY *pBC_Current = NULL;

int nBC_Copies = 0;

void RestoreBackupCopy (BACKUP_COPY *pCopy)
{
    int i;

    /*
     *  Restore roots blocks allocation
     */

    for (i = 0; i < nRoots; i++)
    {
        pRoots [i].nBlock = pCopy -> pRootBackups [i].nBlock;
        pRoots [i].u1.pNext  = pCopy -> pRootBackups [i].pNext;
    }

    /*
     *  Clear current blocks list
     */

    while (pBlocksList != NULL)
        BlocksRemoveDescriptor (pBlocksList);

    /*
     *  Restore blocks.
     */

    for (i = 0; i < pCopy -> nBlocks; i++)
    {
        BLOCK *pBlock;
        BLOCK *pSaveNext;
        BLOCK *pSavePrev;

        pBlock = BlocksAddDescriptor ();
/*
        {
            char buf [128];

            sprintf (buf, "Restore: [%d, %d]-[%d, %d]",
                    (int) pCopy -> pBlocks [i].Rect.xLeft,
                    (int) pCopy -> pBlocks [i].Rect.yTop,
                    (int) pCopy -> pBlocks [i].Rect.xRight,
                    (int) pCopy -> pBlocks [i].Rect.yBottom);
            MESSAGE (buf);
        }
*/
        pSaveNext = pBlock -> pNext;
        pSavePrev = pBlock -> pPrev;

        *pBlock = pCopy -> pBlocks [i];

        pBlock -> pNext = pSaveNext;
        pBlock -> pPrev = pSavePrev;
    }
    /* Pit 09-29-94 03:10pm
     * Restore pictures information.
     */
	//DDD if(pCopy ->lPictures > 0 && pCopy -> pPictures != NULL)
    //DDD   undo_pictures(pCopy -> pPictures,pCopy ->lPictures);

    //DDDundo_sheets(pCopy->nSheets,pCopy->sht);
}

void FreeBackupCopy (BACKUP_COPY *p)
{
    if (p == NULL)
        return;

    if (p -> pRootBackups != NULL)
        free (p -> pRootBackups);

    if (p -> pBlocks != NULL)
        free (p -> pBlocks);
    if (p -> pPictures != NULL) // Pit 09-29-94 03:15pm
        free (p -> pPictures);
    if (p -> sht != NULL){
	//DDDfree_undo_sheets(p->nSheets,p -> sht);
	p->nSheets = 0;
	p->sht     = NULL;
        }

    free (p);

    nBC_Copies--;
}

void FreeBackupChain (BACKUP_COPY *p)
{
    BACKUP_COPY *pTemp;

    while (p != NULL)
    {
        pTemp = p;
        p = p -> pNext;
        FreeBackupCopy (pTemp);
    }
}

/*
void LayoutBackup (void)
{
    int nBlocks = 0;
    BLOCK  *pBlock;
    BACKUP_COPY *pCopy;
    int i;


//       Remove possibility of REDO


    BlocksSetRootsNumbers ();

    if (pBC_Current != NULL && pBC_Current -> pNext != NULL)
    {
        FreeBackupChain (pBC_Current -> pNext);
        pBC_Last = pBC_Current;
        pBC_Last -> pNext = NULL;
    }


//     Check current depth of backup stack


    if (nBC_Copies == MAX_BACKUP_STACK_DEPTH)
    {
        BACKUP_COPY *pTemp = pBC_First;
        pBC_First = pBC_First -> pNext;
        FreeBackupCopy (pTemp);

        if (pBC_First == NULL)
        {
            pBC_Last    = NULL;
            pBC_Current = NULL;
        }
        else
        {
            pBC_First -> pPrev = NULL;
        }
    }


//      Allocate memory for backup copy


    for (pBlock = pBlocksList; pBlock != NULL; pBlock = pBlock -> pNext)
        nBlocks++;

    pCopy = (BACKUP_COPY *) malloc (sizeof (BACKUP_COPY));

    if (pCopy == NULL)
        ErrorNoEnoughMemory ("in LTHLUNDO.C,LayoutBackup,part 1");

    if (nRoots != 0)
    {
        pCopy -> pRootBackups =
            (ROOT_BACKUP *) malloc (nRoots * sizeof (ROOT_BACKUP));

        if (pCopy -> pRootBackups == NULL)
        ErrorNoEnoughMemory ("in LTHLUNDO.C,LayoutBackup,part 2");
    }
    else
    {
        pCopy -> pRootBackups = NULL;
    }

    if (nBlocks != 0)
    {
        pCopy -> pBlocks = (BLOCK *) malloc (nBlocks * sizeof (BLOCK));

        if (pCopy -> pBlocks == NULL)
        ErrorNoEnoughMemory ("in LTHLUNDO.C,LayoutBackup,part 3");
    }
    else
    {
        pCopy -> pBlocks = NULL;
    }
//     Pit 09-29-94 02:59pm
//    Save pictures information.

     {
     puchar      buffer;

//DDD            pCopy -> lPictures = update_pictures(&buffer);
     if(pCopy -> lPictures > 0){
        if((pCopy -> pPictures =  malloc(pCopy -> lPictures))==NULL)
            ErrorNoEnoughMemory ("in LTHLUNDO.C,LayoutBackup,part 3.1");
        memcpy(pCopy -> pPictures,buffer,pCopy -> lPictures);
        }
     else  {
        pCopy -> lPictures = 0;
        pCopy -> pPictures = NULL;
           }
     }
     //DDDbackup_sheets(&(pCopy->nSheets),&(pCopy->sht));

//    Save roots blocks allocation


    for (i = 0; i < nRoots; i++)
    {
        pCopy -> pRootBackups [i].nBlock = pRoots [i].nBlock;
        pCopy -> pRootBackups [i].pNext  = pRoots [i].u1.pNext;
    }


//      Save blocks.
//      Clean extraneous information in each block copy while saving.


    for (pBlock = pBlocksList, i = 0;
             pBlock != NULL;
                 pBlock = pBlock -> pNext, i++)
    {
        pCopy -> pBlocks [i] = *pBlock;

        pCopy -> pBlocks [i].uFlags &=
            ~ (BF_INSERTED_INTO_EMBEDDING_LISTS       |
               BF_INSERTED_INTO_LEFT_AND_RIGHT_LISTS  |
               BF_INSERTED_INTO_TOP_AND_BOTTOM_LISTS);

        pCopy -> pBlocks [i].pHigherEmbedding           = NULL;
        pCopy -> pBlocks [i].pLowerEmbedding            = NULL;
        pCopy -> pBlocks [i].pHorzHystogram             = NULL;

//        {
//            char buf [128];

//            sprintf (buf, "Save: [%d, %d]-[%d, %d]",
//                    (int) pCopy -> pBlocks [i].Rect.xLeft,
//                    (int) pCopy -> pBlocks [i].Rect.yTop,
//                    (int) pCopy -> pBlocks [i].Rect.xRight,
//                    (int) pCopy -> pBlocks [i].Rect.yBottom);
//            MESSAGE (buf);
//        }

    }

    pCopy -> nBlocks = nBlocks;


    //  Insert copy descriptor into chain


    if (pBC_First == NULL)
    {
        pCopy -> pNext = NULL;
        pCopy -> pPrev = NULL;

        pBC_First   = pCopy;
        pBC_Last    = pCopy;
        pBC_Current = pCopy;
    }
    else
    {
        pCopy -> pNext = NULL;
        pCopy -> pPrev = pBC_Last;

        pBC_Last -> pNext = pCopy;

        pBC_Last    = pCopy;
        pBC_Current = pCopy;
    }

    nBC_Copies++;
}
*/


void LayoutUndo (void)
{
    if (pBC_Current == NULL || pBC_Current == pBC_First)
        return;

    pBC_Current = pBC_Current -> pPrev;

    RestoreBackupCopy (pBC_Current);
}

void LayoutRedo (void)
{
    if (pBC_Current == NULL || pBC_Current == pBC_Last)
        return;

    pBC_Current = pBC_Current -> pNext;

    RestoreBackupCopy (pBC_Current);
}

void LayoutBackupFreeData (void)
{
    FreeBackupChain (pBC_First);

    pBC_First   = NULL;
    pBC_Last    = NULL;
    pBC_Current = NULL;

    nBC_Copies = 0;
}
