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

extern uint16_t run_options;

/*********************************/
extern Handle hPageBeforeEditing ;
extern Handle hInclineCalculating ;
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
    HystogramFreeData ();
    BlocksFreeData ();
//    InclinesFreeData ();
    RootsFreeData ();

# ifdef LT_DEBUG
    LT_GraphicsClose ();
# endif
}

# ifdef LT_DUMP
FILE *pfListing;
# endif

void PageStrings1 (void)
{
uint32_t i;

# ifdef MA_DEBUG
    AllocationsAccountingOpen ();
# endif

    if (nRoots == 0)
//        ErrorInternal ((uchar*)"Page is empty");
		ErrorEmptyPage();

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
//extern SheetsCorrectRoots();
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

        PageStrings1 ();
// ¬ аЄЁаговбп ROOTл Ї®Ї ¤ ойЁҐ ў пзҐ©ЄЁ в Ў«Ёж
    BlocksExtract ();
}

void LayoutPart2 (void)
{
        PageStrings2 ();
}

