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
 *              LTMAIN.C - the main routine                                 *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <string.h>
# include <time.h>

# include "iolib.h"
# include "layout.h"
# include "extract.h"
# include "kernel.h"
# include "status.h"

# include "dpuma.h"

//#define LT_DUMP 1
//#pragma message(__FILE__"(34): здесь закомментировать '#define LT_DUMP 1'")

extern uint16_t run_options;

/*********************************/
extern Handle hPageBeforeEditing ;
extern Handle hPageMatrix ;
extern Handle hInitialBreaking ;
extern Handle hInclineCalculating ;
//Handle hBlocksAfterFirstExtraction = NULL;
extern Handle hBlocksBreaking ;
extern Handle hBlocksGlueing ;
extern Handle hFirstDustAbsorbtion ;
extern Handle hRemoveEmptyBlocks ;
extern Handle hSecondDustAbsorbtion ;

Bool bOptionForceOneColumn      = FALSE;
Bool bOptionBusinessCardsLayout = FALSE;

Bool bOptionPointSizeAnalysis                = TRUE;
Bool bOptionSmartBreaking                    = TRUE;
Bool bOptionInitialBreakingByPageMatrix      = TRUE;
Bool bOptionBlocksRemovingByPageMatrix       = TRUE;
Bool bOptionBlocksRemovingByDustDistribution = TRUE;
Bool bOptionSpecialHorizontalCutting         = TRUE;
Bool bOptionWhiteStripsBreaking              = TRUE;

# ifdef LT_DEBUG
int  LT_DebugGraphicsLevel = 0;

Bool bDebugOptionCalibratePictureRemovingCriteria = FALSE;
Bool bDebugOptionCalibrateDD_RemovingCriteria     = FALSE;
Bool bDebugOptionCalibrateLinearRemovingCriteria  = FALSE;

Bool bDebugTimeFlag = FALSE;
static clock_t tTimeTotal;
# endif

# ifdef LT_STAND_ALONE
# ifdef LT_DEBUG
static char szQuickHelpText [] =
"usage: layout [-ght] [<roots_file>]\n";

static char szHelpText [] =
"Layout Version 1.0  - Stand-alone program for page layout debugging\n"
"Written in 1991 by Yuri Panchul\n"
"\n"
"Usage: layout [-ght] [<roots_file>]\n";
# else
static char szQuickHelpText [] =
"usage: layout [-h] [<roots_file>]\n";

static char szHelpText [] =
"Layout Version 1.0  - Stand-alone program for page layout debugging\n"
"Written in 1991 by Yuri Panchul\n"
"\n"
"Usage: layout [-h] [<roots_file>]\n";
# endif
# endif

# ifdef LT_STAND_ALONE
char szRootsFilename [100] = DEFAULT_ROOTS_FILENAME;
# endif




void LT_FreeAllData (void)
{
    WSB_FreeData ();
    SmartBreakingFreeData ();
    HystogramFreeData ();
    SeparatorsFreeData ();
    BlocksFreeData ();
    InitialBreakingFreeData ();
    PageMatrixFreeData ();
    CompsFreeData ();
    IntervalsFreeData ();
//    InclinesFreeData ();
    RootsFreeData ();

# ifdef LT_DEBUG
    LT_GraphicsClose ();
# endif

    LayoutBackupFreeData ();
}

# ifdef LT_DUMP
FILE *pfListing;
# endif

void PageLayoutPart1 (void)
{
int i;
i=0; i=i;
# ifdef MA_DEBUG
    AllocationsAccountingOpen ();
# endif

# ifdef LT_DUMP
    pfListing = fopen ("layout.dmp", "w");

    if (pfListing == NULL)
        ErrorInternal ("Can't open dump file");

    fprintf (pfListing, "nRoots: %d\n", (int) nRoots);

    {
        int i;

        for (i = 0; i < nRoots; i++)
        {
            fprintf (pfListing,
                "x: %4d, y: %4d, w: %4d, h: %4d, block: %4d/%4d [%4x %4x] %4x\n",
                (int) pRoots [i].xColumn,
                (int) pRoots [i].yRow,
                (int) pRoots [i].nWidth,
                (int) pRoots [i].nHeight,
                (int) pRoots [i].nBlock,
                (int) pRoots [i].nUserNum,
                0, //(int) pRoots [i].wSegmentPtr,
                0, //(int) pRoots [i].wLength);
                (int) pRoots [i].bType);
        }
    }
    fclose (pfListing);

# endif

    if (nRoots == 0)
        //ErrorInternal ("Page is empty");
		return;

    nNextBlockNumber = FIRST_REGULAR_BLOCK_NUMBER;

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel > 0)
	if( !LDPUMA_Skip (hPageBeforeEditing) ||
		!LDPUMA_Skip (hPageMatrix) ||
		!LDPUMA_Skip (hInitialBreaking) ||
		!LDPUMA_Skip (hInclineCalculating) ||
		!LDPUMA_Skip (hBlocksBreaking) ||
		!LDPUMA_Skip (hBlocksGlueing) ||
		!LDPUMA_Skip (hFirstDustAbsorbtion) ||
		!LDPUMA_Skip (hRemoveEmptyBlocks) ||
		!LDPUMA_Skip (hSecondDustAbsorbtion))
	{
        LT_GraphicsOpen ();
	}

    if (bDebugTimeFlag)
        tTimeTotal = clock ();
# endif

    RootsSaveNonLayoutData ();
    CalculatePageParameters ();
    SeparatorsGet ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if (!LDPUMA_Skip(hPageBeforeEditing))
        LT_GraphicsRootsOutput ("Page");
# endif

    if (bOptionInitialBreakingByPageMatrix ||
        bOptionBlocksRemovingByPageMatrix)
    {
        PageMatrixBuild ();
    }

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if (!LDPUMA_Skip(hPageMatrix))
        LT_GraphicsPageMatrixOutput ("Page matrix");
# endif

    // выделить в матрице страницы компоненты связности;
    // настроить в рутах поле nBlock на хранение этой
    // полезной информации; сами компоненты связности -- не хранятся.
    InitialBreakingProceed ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 3)
	if (!LDPUMA_Skip(hInitialBreaking))
        LT_GraphicsBlocksOutput ("Initial breaking");
# endif

//    InclinesAccount ();
    RotatePageToIdeal ();
//    InclinesFreeData ();
    RootStripsCalculate ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 3)
	if(!LDPUMA_Skip(hInclineCalculating))
    {
        LT_GraphicsRootsOutput ("Roots after page rotation");
        LT_GraphicsRootStripsOutput ("Root strips");
    }
# endif

    BlocksExtract ();
	//BlockAnalyse();
# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
    //    LT_GraphicsBlocksOutput ("Blocks after extraction.1");
# endif
 /*    Pit 09-27-94 03:42pm
    new_picture(3,5,300,500);
    del_picture(0);
    new_picture(300,500,100,100);

    BlocksExtract ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
        LT_GraphicsBlocksOutput ("Blocks after extraction.2");
# endif
*/
    BlocksBreak ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if(!LDPUMA_Skip(hBlocksBreaking))
        LT_GraphicsBlocksOutput ("Blocks after breaking");
# endif

    BlocksAddVirtualSeparatorsBlocks ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
    //    LT_GraphicsBlocksOutput ("Blocks after adding separators");
# endif

    BlocksRemoveFrameLikeRoots ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
    //    LT_GraphicsBlocksOutput ("After removing frame like roots");
# endif

    BlocksGlue ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if(!LDPUMA_Skip(hBlocksGlueing))
        LT_GraphicsBlocksOutput ("Blocks after glueing");
# endif

    BlocksBuildEmbeddingLists ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 3)
    //{
    //    LT_GraphicsHighEmbeddingBlocksListOutput ("High embedding blocks list");
    //    LT_GraphicsLowEmbeddingBlocksListOutput  ("Low embedding blocks list");
    //}
# endif

    BlocksAbsorbDust ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if(!LDPUMA_Skip(hFirstDustAbsorbtion))
        LT_GraphicsBlocksOutput ("After first dust absorbtion");
# endif

    PageMatrixExcludeSeparators (TRUE);

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
    //    LT_GraphicsPageMatrixOutput ("Matrix after excluding separators");
# endif

    BlocksRemoveEmptyBlocks ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if(!LDPUMA_Skip(hRemoveEmptyBlocks))
        LT_GraphicsBlocksOutput ("After removing empty blocks");
# endif

    BlocksAbsorbDust ();
    BlocksDisAbsorbBoundaryDust ();

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel >= 2)
	if(!LDPUMA_Skip(hSecondDustAbsorbtion))
        LT_GraphicsBlocksOutput ("After second dust absorbtion");
# endif

    if (cut_page_left || cut_page_right)
    {
        BlocksCutPageEdges ();

# ifdef LT_DEBUG
     //   if (LT_DebugGraphicsLevel >= 2)
     //       LT_GraphicsBlocksOutput ("After BlocksCutPageEdges");
# endif
    }

    LayoutBackupFreeData ();
}

void PageLayoutPart2 (void)
{
    LayoutBackupFreeData ();

# ifdef LT_DUMP
    fprintf (pfListing, "Blocks:\n");

    {
        BLOCK *p;

        for (p = pBlocksList; p != NULL; p = p -> pNext)
            fprintf (pfListing,
                "[%d, %d]-[%d, %d] %d\n",
                (int) p -> Rect.xLeft,
                (int) p -> Rect.yTop,
                (int) p -> Rect.xRight,
                (int) p -> Rect.yBottom,
                (int) p -> Type);
    }
# endif

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 1)
        LT_GraphicsBlocksOutput ("Text blocks");
# endif

    BlocksBuildLeftAndRightLists ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 3)
    {
        LT_GraphicsLeftBlocksListOutput  ("Left blocks list");
        LT_GraphicsRightBlocksListOutput ("Right blocks list");
    }
# endif

    BlocksBuildTopAndBottomLists ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 3)
    {
        LT_GraphicsTopBlocksListOutput    ("Top blocks list");
        LT_GraphicsBottomBlocksListOutput ("Bottom blocks list");
    }
# endif

/******************************** ATAL 940414 Remove couple functions calls
    TreeBuild ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 3)
        LT_GraphicsTreeOutput ("Tree structure before edit");
# endif

    TreeEdit ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
        LT_GraphicsTreeOutput ("Tree structure");
# endif
*************************************************/

    TreePass ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
        LT_GraphicsBlocksOrderOutput ("Output blocks order");
# endif

    RootsRemoveFromRulers ();

# ifdef LT_DEBUG
    if (bDebugTimeFlag)
    {
//      printf ("\nLayout Time : %f\n",
//          (double) (clock () - tTimeTotal) / CLK_TCK);
    }
# endif

# ifdef LT_USE_STRINGS_EXTRACTION
    StringsExtract ();
# endif

    RootsRestoreNonLayoutData ();
    LT_FreeAllData ();

# ifdef LT_DEBUG
    if (bDebugTimeFlag)
    {
//      tTimeTotal = clock () - tTimeTotal;
//      printf ("\nLayout Time : %f\n", (double) tTimeTotal / CLK_TCK);
    }
# endif

# ifdef MA_DEBUG
    AllocationsAccountingClose ();
# endif

# ifdef LT_DUMP
    fprintf (pfListing, "\nAFTER LAYOUT:\n");
    fprintf (pfListing, "nRoots: %d\n", (int) nRoots);

    {
        int i;

        for (i = 0; i < nRoots; i++)
        {
            fprintf (pfListing,
                "x: %4d, y: %4d, w: %4d, h: %4d, block: %4d/%4d [%4x %4x]\n",
                (int) pRoots [i].xColumn,
                (int) pRoots [i].yRow,
                (int) pRoots [i].nWidth,
                (int) pRoots [i].nHeight,
                (int) pRoots [i].nBlock,
                (int) pRoots [i].nUserNum,
                0, //(int) pRoots [i].wSegmentPtr,
                0); //(int) pRoots [i].wLength);
        }
    }

    fclose (pfListing);
# endif
}

void PageStrings1 (void)
{
    int i;

# ifdef MA_DEBUG
    AllocationsAccountingOpen ();
# endif

    if (nRoots == 0)
		ErrorEmptyPage();
//        ErrorInternal ("Page is empty");

    nNextBlockNumber = FIRST_REGULAR_BLOCK_NUMBER;

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel > 0)
        LT_GraphicsOpen ();

    if (bDebugTimeFlag)
        tTimeTotal = clock ();
# endif

    RootsSaveNonLayoutData ();
    CalculatePageParameters ();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
        LT_GraphicsRootsOutput ("Roots");
# endif

//    InclinesAccount ();
    RotatePageToIdeal ();
//    InclinesFreeData ();
    RootStripsCalculate ();

    for (i = 0; i < nRoots; i++)
        pRoots [i].nBlock = FIRST_REGULAR_BLOCK_NUMBER;

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
        LT_GraphicsRootsOutput ("Roots after page rotation");
# endif

    BlocksExtract ();
	//BlockAnalyse();

# ifdef LT_DEBUG
    if (LT_DebugGraphicsLevel >= 2)
        LT_GraphicsBlocksOutput ("Blocks after extraction");
# endif
}

void PageStrings2 (void)
{
    RootsRemoveFromRulers ();

# ifdef LT_USE_STRINGS_EXTRACTION
    StringsExtract ();
# endif

    RootsRestoreNonLayoutData ();
    LT_FreeAllData ();

# ifdef LT_DEBUG
    if (bDebugTimeFlag)
        tTimeTotal = clock () - tTimeTotal;
# endif

# ifdef MA_DEBUG
    AllocationsAccountingClose ();
# endif
}

void LayoutPart1 (void)
{
extern SheetsCorrectRoots();
# ifdef LT_DEBUG
    switch (layout)
    {
        case 0:  LT_DebugGraphicsLevel = 0;  break;
        case 1:  LT_DebugGraphicsLevel = 1;  break;
        case 2:  LT_DebugGraphicsLevel = 2;  break;
        case 3:  LT_DebugGraphicsLevel = 3;  break;
        case 4:  LT_DebugGraphicsLevel = 4;  break;
        case 5:  LT_DebugGraphicsLevel = 1;  break;
        case 6:  LT_DebugGraphicsLevel = 1;  break;
        case 7:  LT_DebugGraphicsLevel = 1;  break;
        case 8:  LT_DebugGraphicsLevel = 1;  break;
        case 9:  LT_DebugGraphicsLevel = 0;  break;
        case 10: LT_DebugGraphicsLevel = 0;  break;
    }

    bDebugTimeFlag = FALSE;

    bDebugOptionCalibratePictureRemovingCriteria = FALSE;
    bDebugOptionCalibrateDD_RemovingCriteria     = FALSE;
    bDebugOptionCalibrateLinearRemovingCriteria  = FALSE;
# endif

# ifdef SE_DEBUG
    switch (layout)
    {
        case 0:  SE_DebugGraphicsLevel = 0;  break;
        case 1:  SE_DebugGraphicsLevel = 0;  break;
        case 2:  SE_DebugGraphicsLevel = 0;  break;
        case 3:  SE_DebugGraphicsLevel = 0;  break;
        case 4:  SE_DebugGraphicsLevel = 0;  break;
        case 5:  SE_DebugGraphicsLevel = 1;  break;
        case 6:  SE_DebugGraphicsLevel = 2;  break;
        case 7:  SE_DebugGraphicsLevel = 3;  break;
        case 8:  SE_DebugGraphicsLevel = 4;  break;
        case 9:  SE_DebugGraphicsLevel = 0;  break;
        case 10: SE_DebugGraphicsLevel = 0;  break;
    }
# endif

# ifdef MA_DEBUG
    switch (layout)
    {
        case 0:  MA_DebugLevel = 0;  break;
        case 1:  MA_DebugLevel = 0;  break;
        case 2:  MA_DebugLevel = 0;  break;
        case 3:  MA_DebugLevel = 0;  break;
        case 4:  MA_DebugLevel = 0;  break;
        case 5:  MA_DebugLevel = 0;  break;
        case 6:  MA_DebugLevel = 0;  break;
        case 7:  MA_DebugLevel = 0;  break;
        case 8:  MA_DebugLevel = 0;  break;
        case 9:  MA_DebugLevel = 1;  break;
        case 10: MA_DebugLevel = 2;  break;
    }
# endif

    bOptionForceOneColumn      = ((run_options & FORCE_ONE_COLUMN) != 0);
    bOptionBusinessCardsLayout = ((run_options & BCRLOUT) != 0);

    bOptionPointSizeAnalysis                = TRUE;
    bOptionSmartBreaking                    = TRUE;
    bOptionInitialBreakingByPageMatrix      = TRUE;
    bOptionBlocksRemovingByPageMatrix       = ! bOptionBusinessCardsLayout;
    bOptionBlocksRemovingByDustDistribution = ! bOptionBusinessCardsLayout;
    bOptionSpecialHorizontalCutting         = FALSE;
    bOptionWhiteStripsBreaking              = TRUE;

    if (bOptionForceOneColumn)
        PageStrings1 ();
    else
        PageLayoutPart1 ();

// маркируются ROOTы попадающие в ячейки таблиц
    //DDD SheetsCorrectRoots(); // Piter 08-17-95 07:29pm
    BlocksExtract ();
	//BlockAnalyse();

// ******** temporary ***************
    // Piter 08-28-95 05:58pm
    // Удаление фрагментов не входящих в таблицы
//{
//    BLOCK *p;
//    lrep:
//       for (p = pBlocksList; p != NULL; p = p -> pNext)
//  switch(p -> nUserNum)
//  {
//  case 0:
//        BlockRemove (p);
//        goto lrep;
//  }

//}//**************************************

}

void LayoutPart2 (void)
{
    if (bOptionForceOneColumn)
        PageStrings2 ();
    else
        PageLayoutPart2 ();
}
/**********************************************/
# ifdef LT_STAND_ALONE
/*
void PageLayout(void);

void main (int nArgc, char **pArgv)
{
    int  i;
    int nFilesDefined = 0;
    char *p;

    for (i = 1; i < nArgc; i++)
    {
        if (*pArgv[i] == '-')
        {
            for (p = pArgv[i] + 1; *p; p++)
                switch (*p)
                {
# ifdef LT_DEBUG
                    case 'g':
                        LT_DebugGraphicsLevel++;
                        break;
# endif
                    case 'h':
                        printf (szHelpText);
                        exit (0);
# ifdef LT_DEBUG
                    case 't':
                        bDebugTimeFlag = TRUE;
                        break;
# endif
                    default:
                        fprintf (stderr, "Bad option: \"%s\"\n%s\n",
                                pArgv [i], szQuickHelpText);
                        exit (-1);
                        break;
                }
        }
        else
        {
            switch (nFilesDefined)
            {
                case 0:
                    strcpy (szRootsFilename, pArgv [i]);
                    nFilesDefined++;
                    break;

                default:
                    fprintf (stderr, "Unnecessary filename: \"%s\"\n%s\n",
                                pArgv [i], szQuickHelpText);
                    exit (-1);
            }
        }
    }

    RootsLoadFile (szRootsFilename);
    PageLayout ();
    exit (0);
}
*/
# endif
