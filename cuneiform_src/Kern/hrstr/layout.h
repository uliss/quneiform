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
 *              LAYOUT.H - the header file                                  *
 *                                                                          *
 ***************************************************************************/

# ifndef __LAYOUT_H_INCLUDE
# define __LAYOUT_H_INCLUDE

# include "ltconfig.h"
# include "lterrors.h"
# include "roots.h"
# include "madebug.h"
# include "struct.h"

#include  "picture.h"
#include  "ctp.h"

void  q_sort( char *, size_t, size_t,
			  int (*)( const void *, const void * ) );

# ifdef LT_STAND_ALONE
# define DEFAULT_ROOTS_FILENAME  "a.rts"
# endif

# define APPEND_TO_DUMP {                                                \
                            FILE *f = fopen ("aaa", "a");                \
                            fprintf (f, "%s: %d\n", __FILE__, __LINE__); \
                            fclose (f);                                  \
                        }

/****************************************************************************
 *   Common structures                                                      *
 ***************************************************************************/

typedef struct
    {
        int x;
        int y;
    } LPOINT;

typedef struct
    {
        int xLeft;
        int yTop;
        int xRight;
        int yBottom;
    } RECTANGLE;

# define EXCHANGE_INTS(a, b) { int temp; temp = a; a = b; b = temp; }

/****************************************************************************
 *   Declarations                                                           *
 ***************************************************************************/

struct _RootStrip;
typedef struct _RootStrip ROOT_STRIP;

struct _Component;
typedef struct _Component COMP;

struct _Interval;
typedef struct _Interval INTERVAL;

struct _Strip;
typedef struct _Strip STRIP;

struct _Separator;
typedef struct _Separator SEPARATOR;

struct _Block;
typedef struct _Block BLOCK;

struct _RootBackup;
typedef struct _RootBackup ROOT_BACKUP;

struct _BackupCopy;
typedef struct _BackupCopy BACKUP_COPY;


/****************************************************************************
 *   Working with roots                                                     *
 ***************************************************************************/

struct _RootStrip
    {
        ROOT *pBegin;
        ROOT *pEnd;
    };

extern ROOT_STRIP *pRootStrips;
extern int nRootStrips;

extern int nRootStripsStep;
extern int nRootStripsOffset;

# ifdef LT_STAND_ALONE
Bool RootsLoadFile (char * pFilename);
# endif
void CalculatePageParameters (void);

void RootStripsCalculate (void);
void RootStripsGetLoopParameters
         (int yTop, int yBottom, ROOT **ppBegin, ROOT **ppAfter);

void RootsRemoveFromRulers (void);

void RootsSaveNonLayoutData (void);
void RootsRestoreNonLayoutData_ForDustAndRemoved (void);
void RootsRestoreNonLayoutData_ForBlock (BLOCK *p);
void RootsRestoreNonLayoutData (void);

void RootsFreeData (void);

extern int nRootSpaceWidth;
extern int nRootSpaceHeight;
extern RECTANGLE rRootSpace;

/****************************************************************************
 *   Inclines accounting                                                    *
 ***************************************************************************/

# define INCLINE_FACTOR  2048

extern int  *pnHeightesCounter;
extern int  nHeightesAllocated;
extern int  nHeightes;

extern INT nIncline;

Bool InclinesAccount (void);
void InclinesFreeData (void);

void IdealXY (INT x, INT y, INT *px, INT *py);
void RealXY (INT x, INT y, INT *px, INT *py);

void RotatePageToIdeal (void);

# define IDEAL_XY(x, y)   \
         {\
             x = (INT) (x + (LONG) y * nIncline / INCLINE_FACTOR);\
             y = (INT) (y - (LONG) x * nIncline / INCLINE_FACTOR);\
         }

# define REAL_XY(x, y)   \
         {\
             y = (INT) (y + (LONG) x * nIncline / INCLINE_FACTOR);\
             x = (INT) (x - (LONG) y * nIncline / INCLINE_FACTOR);\
         }

/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                  ComponentsExtracting                                  **
 **                                                                        **
 ****************************************************************************
 ***************************************************************************/

/****************************************************************************
 *   Intervals                                                              *
 ***************************************************************************/

struct _Interval
    {
        int   xBegin;
        int   xEnd;
        COMP  *pComp;
    };

extern INTERVAL *pIntervals;
extern int nIntervals;

extern INTERVAL *pPrevIntervals;
extern int nPrevIntervals;

void IntervalsInit (void);
void IntervalsBuild (int y);
void IntervalsStep (int y);
void IntervalsFreeData (void);

/****************************************************************************
 *   Strips                                                                 *
 ***************************************************************************/

struct _Strip
    {
        STRIP *pNext;

        int   y;
        int   xBegin;
        int   xEnd;
    };

# define STRIPS_POOL_FIRST_ALLOCATION      1000
# define STRIPS_POOL_ALLOCATION_QUANTUM    500

extern STRIP *pStripsPool;
extern int   nStripsPoolLength;
extern STRIP *pFreeStripsList;

STRIP *StripNew (void);
void StripDelete (STRIP *pStrip);

/****************************************************************************
 *   Components                                                             *
 ***************************************************************************/

struct _Component
    {
        COMP    *pNext;
        COMP    *pPrev;

        int     Number;

        int     xLeft;
        int     yTop;
        int     xRight;
        int     yBottom;
        Bool    bRectangleAccounted;

        STRIP   *pStripsListBegin;
        STRIP   *pStripsListEnd;

        int     nSquare;
    };

# define COMPS_POOL_FIRST_ALLOCATION     100
# define COMPS_POOL_ALLOCATION_QUANTUM   50

extern COMP *pCompsPool;
extern int  nCompsPoolLength;
extern COMP *pFreeCompsList;

extern COMP *pCompsList;

COMP *CompNew (void);
void CompDelete (COMP *pComp);
void CompAddInterval (COMP *pComp, int y, INTERVAL *pInterval);
void CompsConnect (COMP *pComp1, COMP *pComp2);
void CompOR_Matrix (COMP *pComp, int Mask);
void CompAND_Matrix (COMP *pComp, int Mask);

void CompsBuild
(
    uchar *pMatrix,
    int   nWidth,
    int   nHeight,
    int   nSize,
    int   BlackMask
);

void CompsFreeData (void);

/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                  Initial Breaking                                      **
 **                                                                        **
 ****************************************************************************
 ***************************************************************************/

/****************************************************************************
 *   PageMatrix                                                             *
 ***************************************************************************/

# define PMC_NULL     0
# define PMC_ROOT     1
# define PMC_DUST     2
# define PMC_PICTURE  4

# define PMC_DEBUG1   8
# define PMC_DEBUG2   16

# define PMC_FLAGS_MASK    (PMC_ROOT | PMC_DUST | PMC_PICTURE)

#ifdef  HUGE_IMAGE
  # define PAGE_MATRIX_WIDTH          1024        // 512
  # define PAGE_MATRIX_WIDTH_SHIFT    10          // 9
	# define PAGE_MATRIX_HEIGHT         1024

  # define PAGE_MATRIX_SIZE           (1024*1024) // 512
#else
	# define PAGE_MATRIX_WIDTH          256
	# define PAGE_MATRIX_WIDTH_SHIFT    8
	# define PAGE_MATRIX_HEIGHT         256

	# define PAGE_MATRIX_SIZE           65536
#endif
/* (PAGE_MATRIX_WIDTH * PAGE_MATRIX_HEIGHT) */

# define PAGE_COMP_SHIFT     4
# define PAGE_COMP_FACTOR    (1 << PAGE_COMP_SHIFT)
# define PAGE_COMP_MASK      (~((~0 >> PAGE_COMP_SHIFT) << PAGE_COMP_SHIFT))
# define XY_COMPRESS(x)      \
    ((x) < 0 ? -(-(x) >> PAGE_COMP_SHIFT) : (x) >> PAGE_COMP_SHIFT)
# define XY_UNCOMPRESS(x)      \
    ((x) < 0 ? -(-(x) << PAGE_COMP_SHIFT) : (x) << PAGE_COMP_SHIFT)

# define PAGE_MATRIX_REAL_WIDTH  (PAGE_MATRIX_WIDTH  << PAGE_COMP_SHIFT)
# define PAGE_MATRIX_REAL_HEIGHT (PAGE_MATRIX_HEIGHT << PAGE_COMP_SHIFT)

extern uchar *PageMatrix;   /* [PAGE_MATRIX_SIZE] */
extern Bool bPageMatrixInitialized;

void PageMatrixInit (int nWidth, int nHeight);
void SpecCompPut (MN *mn);
void PageMatrixPutRoots (void);
void PageMatrixHorzGlue (void);
void PageMatrixVertGlue (void);
void PageMatrixExcludeIntersections (void);
void PageMatrixBuild (void);
void PageMatrixPutSeparators (Bool bConvertToRealCoords);
void PageMatrixExcludeSeparators (Bool bConvertToRealCoords);
uchar PageMatrixFlagsByIdealXY (int xIdeal, int yIdeal);
void PageMatrixFreeData (void);

# define PAGE_MATRIX_MARK_SHIFT  3
# define COMP_SERIES_LENGTH      7

void InitialBreakingProceed (void);
void InitialBreakingFreeData (void);

/****************************************************************************
 *   Working with separators                                                *
 ***************************************************************************/

# define SEP_NULL    0
# define SEP_VERT    1
# define SEP_HORZ    2
# define SEP_RECT    3

# define SEPF_NULL    0
# define SEPF_IS_PART 1

struct _Separator
    {
        int      Type;
        unsigned uFlags;
        int      xBegin;
        int      yBegin;
        int      xEnd;
        int      yEnd;
        int      nWidth;
    };

extern SEPARATOR *pSeps;
extern int nSeps;

void SeparatorsGet (void);
void SeparatorsFreeData (void);

/****************************************************************************
 *   Working with blocks                                                    *
 ***************************************************************************/

/* Block type */

# define BLOCK_NULL           0
# define BLOCK_TEXT           1
# define BLOCK_HORZ_SEPARATOR 2
# define BLOCK_VERT_SEPARATOR 3
# define BLOCK_RECT_SEPARATOR 4
# define BLOCK_PICTURE        5  // Pit

/* Block Flags */

# define BF_NULL                                0

/* Internal flags */

# define BF_NOT_BREAK_BLOCK                     0x1
# define BF_NOT_GLUE_BLOCK                      0x2
# define BF_NOT_REMOVE_FRAME_LIKE_ROOTS         0x4
# define BF_INSERTED_INTO_EMBEDDING_LISTS       0x8
# define BF_INSERTED_INTO_LEFT_AND_RIGHT_LISTS  0x10
# define BF_INSERTED_INTO_TOP_AND_BOTTOM_LISTS  0x20

/* External flags */

# define BF_SMART_BREAKING_APPLIED              0x100
# define BF_WHITE_STRIPS_BREAKING_APPLIED       0x200
# define BF_GLUED_FROM_ONE_LINE_BLOCKS          0x400
# define BF_CROSSED                             0x800

struct _Block
{
    BLOCK *pNext;
    BLOCK *pPrev;

    INT        Type;
    unsigned   uFlags;
    int        nNumber;
    int        nUserNum;  // Number call of User ExractBlocks Piter 08-17-95 06:20pm
    //05-25-94 03:01pm,Pit
    uchar        language; // ­  Є Є®¬ п§лЄҐ ­ ЇЁб ­®
    RECTANGLE  Rect;

    int        nEmbedding;
    BLOCK      *pLowerEmbedding;
    BLOCK      *pHigherEmbedding;

    BLOCK      *pLeft;
    BLOCK      *pRight;
    BLOCK      *pUp;
    BLOCK      *pDown;

    ROOT       *pRoots;
    ROOT       *pEndRoots;

    int        nRoots;
    int        nLetters;
    int        nDust;
    int        nHeightesSum;
    int        nAverageHeight;

    int        nParents;
    int        nChildren;
    BLOCK      *pParent;

    SEPARATOR   *pUpSep;                  // Own block separators
    SEPARATOR   *pDownSep;
    SEPARATOR   *pLeftSep;
    SEPARATOR   *pRightSep;

    int        *pHorzHystogram;          // Hystogram for breaking

    int        nActualHeight;

    int        nStartColumnWidth;
    int        nUnconditionalMinColumnWidth;
    int        nConditionalMinColumnWidth;

    int        *pHystogram;              // Hystogram for strings extraction
    int        nHystColumns;
    int        hPicture;                   // Pit. ЋЇЁб ­ЁҐ Є авЁ­ЄЁ
};

extern BLOCK *pBlocksList;
extern BLOCK *pHighEmbeddingBlocksList;
extern BLOCK *pLowEmbeddingBlocksList;
extern BLOCK *pLeftBlocksList;
extern BLOCK *pRightBlocksList;
extern BLOCK *pTopBlocksList;
extern BLOCK *pBottomBlocksList;
extern int nNextBlockNumber;

Bool RectEq (RECTANGLE *p, RECTANGLE *q);
Bool RectIsCrossed (RECTANGLE *p, RECTANGLE *q);
void RectAsgPlus (RECTANGLE *p, RECTANGLE *q);
Bool RectIncludes (RECTANGLE *p, RECTANGLE *q);

BLOCK *BlocksAddDescriptor (void);
void BlocksRemoveDescriptor (BLOCK *p);
void BlockAccountRoot (BLOCK *p, ROOT *pRoot);
void BlockSetAverageHeight (BLOCK *p);
void BlockCalculateBreakingParameters (BLOCK *p);
void BlockReCalculateBreakingParameters (BLOCK *p);
void BlockSetRootsNumbers (BLOCK *p);
void BlocksSetRootsNumbers (void);
void BlocksRestoreBreakedBlock (BLOCK *p, BLOCK *q, BLOCK *r);
void BlockEnglish(BLOCK *p); // Pit 02-11-94

# ifdef LT_DEBUG
void BlocksSetRandomRootsNumbers (void);
# endif

void BlockRenameRoots (BLOCK *p, int nBlock);
void BlocksFreeData (void);
void BlocksBuildEmbeddingLists (void);
void BlocksBuildLeftAndRightLists (void);
void BlocksBuildTopAndBottomLists (void);

/****************************************************************************
 *   Hystogram                                                              *
 ***************************************************************************/

extern int  *pHystogram;
extern int  *pHystVertHeightesSum;
extern int  *pHystHorzHeightesSum;
extern int  *pHystInt1;
extern int  *pHystInt2;
extern int  *pHystTops;
extern int  *pHystBottoms;
extern int  nHystColumns;
extern int  nHystColumnWidth;

Bool HystogramAllocateBody (void);
void HystogramMakeIntegral (int *pInt, int *p, int nWidth);
void HystogramFreeData (void);

/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                  SmartBreaking                                         **
 **                                                                        **
 ****************************************************************************
 ***************************************************************************/

/****************************************************************************
 *   SmartBreaking matrix                                                   *
 ***************************************************************************/

# define WHITE_CELL           0
# define BLACK_CELL           1
# define MARKED_CELL          2
# define SUSPICION_COMP_CELL  4

extern uchar *pSB_Matrix;
extern int  nSB_Width;
extern int  nSB_Height;
extern int  nSB_Size;
extern int  nSB_CellWidth;
extern int  nSB_CellHeight;

# if defined (LT_DEBUG) || defined (SE_DEBUG)
extern BLOCK *pSB_Block;
# endif

Bool SB_MatrixBuild (BLOCK *pBlock, int nCellWidth, int nCellHeight);
void SB_MatrixFreeData (void);

# define FCC_NOT_FOUND        0
# define FCC_FOUND            1
# define FCC_CANT_FOUND       2
# define FCC_FOUND_SUSPICION  3

int  CompsFindCompToCut (COMP **ppResult);

Bool TrySmartBreaking (BLOCK *pBlock);
void SmartBreakingFreeData (void);

/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                  End of SmartBreaking                                  **
 **                                                                        **
 ****************************************************************************
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *                   White Strips Breaking                                  *
 *                                                                          *
 ***************************************************************************/

typedef struct _WSB_Point
{
    int xBegin;
    int xMain;
    int xEnd;

    int yMin;
    int yMax;
    int yAverage;
} WSB_POINT;

extern int  * pWSB_Hyst1;
extern int  * pWSB_Hyst2;
extern int    nWSB_HystColumns;
extern int    xWSB_HystOffset;

# define WSB_POINTS_POOL_ALLOCATION_QUANTUM    32

extern WSB_POINT * pWSB_Points;
extern int         nWSB_Points;

void WSB_BuildHystograms (void);
void WSB_AddPoint (WSB_POINT Point);
void WSB_BuildPoints (void);
Bool WSB_BreakBlock (BLOCK * p, WSB_POINT Point);
Bool WSB_CutUpperLine (BLOCK * p, WSB_POINT Point);
void WhiteStripsBreaking (void);
void WSB_FreeData (void);

/****************************************************************************
 *   Graphics output                                                        *
 ***************************************************************************/

# if defined (LT_DEBUG) || defined (SE_DEBUG)
void LT_GraphicsOpen (void);
void LT_GraphicsClearScreen (void);
void LT_GraphicsTitle (const char *pTitle);
int  LT_Getch (void);

void LT_GraphicsRootsOutput (const char *pTitle);
void LT_GraphicsRootStripsOutput (char *pTitle);
void LT_GraphicsPageMatrixOutput (char *pTitle);
void LT_GraphicsBlocksOutput (const char *pTitle);
void LT_GraphicsHystogramOutput (const char *pTitle);
void LT_GraphicsSpecialCuttingOutput (char *pTitle);
void LT_GraphicsSB_MatrixOutput (char *pTitle);
void LT_GraphicsWSB_Hystogram_1_Output (char *pTitle);
void LT_GraphicsWSB_Hystogram_2_Output (char *pTitle);
void LT_GraphicsWSB_PointsOutput (char *pTitle);
void LT_GraphicsPictureRemovingConditionsOutput (char * pTitle);
void LT_GraphicsDD_RemovingConditionsOutput (char * pTitle);
void LT_GraphicsTreeOutput (char *pTitle);
void LT_GraphicsHighEmbeddingBlocksListOutput (char *pTitle);
void LT_GraphicsLowEmbeddingBlocksListOutput (char *pTitle);
void LT_GraphicsLeftBlocksListOutput (char *pTitle);
void LT_GraphicsRightBlocksListOutput (char *pTitle);
void LT_GraphicsTopBlocksListOutput (char *pTitle);
void LT_GraphicsBottomBlocksListOutput (char *pTitle);
void LT_GraphicsBlocksOrderOutput (char *pTitle);
void LT_GraphicsCurrentStringOutput (const char *pTitle);
void LT_GraphicsStringsOutput (const char *pTitle);
void LT_GraphicsStringsForwardOrderOutput (const char *pTitle);
void LT_GraphicsStringsBackwardOrderOutput (const char *pTitle);
void LT_GraphicsStringsUpOrderOutput (const char *pTitle);
void LT_GraphicsStringsDownOrderOutput (const char *pTitle);
void LT_GraphicsRasterOutput (const char *pTitle);
void LT_GraphicsBreakingOutput (const char *pTitle);
void LT_ShowBlock(char *pTitle);

void LT_GraphicsBlockOutput2 (char *pTitle);

void LT_GraphicsPictureRemovingConditionsOutput2 (char *pTitle);
void LT_GraphicsDD_RemovingConditionsOutput2 (char *pTitle);
void LT_GraphicsLinearRemovingConditionsOutput2 (char *pTitle);
void LT_GraphicsClose (void);

extern BLOCK *pDebugBlock;
extern RECTANGLE rDebugRect;
extern int xDebugVertLine;
# endif

/****************************************************************************
 *   Blocks processing tools                                                *
 ***************************************************************************/

Bool BlockBreakOnVertical (BLOCK *p, BLOCK **pq, BLOCK **pr,
                           int x, int nBreakingWidth);
Bool BlockBreakOnHorizontal (BLOCK *p, BLOCK **pq, BLOCK **pr,
                             int y);
Bool BlockBreakByRectangle (BLOCK *p, BLOCK **pq, BLOCK **pr,
                            int xBegin, int yBegin, int xEnd, int yEnd);
BLOCK *BlocksGlueTwo (BLOCK *p, BLOCK *q);
void BlockRemove (BLOCK *p);

/****************************************************************************
 *   Layout backup / restore procedures                                     *
 ***************************************************************************/
#include "sheet.h"

struct _RootBackup
{
    INT  nBlock;
    ROOT *pNext;
};

struct _BackupCopy
{
    BACKUP_COPY  *pNext;
    BACKUP_COPY  *pPrev;

    ROOT_BACKUP  *pRootBackups;

    int          nBlocks;
    BLOCK        *pBlocks;

    long         lPictures; // size of pictures buffer Pit 09-29-94 03:05pm
    puchar        pPictures; //         pictures buffer

    int          nSheets;   // зЁб«® в Ў«Ёж
    SHT          *sht;      // Ёе ®ЇЁб вҐ«Ё
};

extern BACKUP_COPY *pBC_First;
extern BACKUP_COPY *pBC_Last;
extern BACKUP_COPY *pBC_Current;
extern int nBC_Copies;

void LayoutBackup (void);
void LayoutUndo (void);
void LayoutRedo (void);
void LayoutBackupFreeData (void);

/****************************************************************************
 *   Hand layout functions                                                  *
 ***************************************************************************/

void HandLayoutFreeData (void);

/****************************************************************************
 *   Main subroutines                                                       *
 ***************************************************************************/

void PageMatrixBuild (void);
void InitialBreakingProceed (void);
void BlocksExtract (void);
void BlocksBreak (void);
void BlocksGlue (void);
void BlocksRemoveEmptyBlocks (void);
void BlocksRemoveFrameLikeRoots (void);
void BlocksAbsorbDust (void);
void BlocksDisAbsorbBoundaryDust (void);
void BlocksCutPageEdges ();
void BlocksAddVirtualSeparatorsBlocks (void);
void TreeBuild (void);
void TreeEdit  (void);
void TreePass  (void);

void LT_FreeAllData (void);
void LayoutPart1 (void);
void LayoutPart2 (void);

# ifdef LT_DEBUG
extern Bool bDebugTimeFlag;
extern int  LT_DebugGraphicsLevel;
extern Bool bDebugOptionCalibratePictureRemovingCriteria;
extern Bool bDebugOptionCalibrateDD_RemovingCriteria;
extern Bool bDebugOptionCalibrateLinearRemovingCriteria;
# endif

extern Bool bOptionForceOneColumn;
extern Bool bOptionBusinessCardsLayout;

extern Bool bOptionPointSizeAnalysis;
extern Bool bOptionSmartBreaking;
extern Bool bOptionInitialBreakingByPageMatrix;
extern Bool bOptionBlocksRemovingByPageMatrix;
extern Bool bOptionBlocksRemovingByDustDistribution;
extern Bool bOptionSpecialHorizontalCutting;
extern Bool bOptionWhiteStripsBreaking;
# endif

