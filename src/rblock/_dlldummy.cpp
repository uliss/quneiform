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
 *              LTGRAPH.C - debug graphics                                  *
 *                                                                          *
 ***************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

#include "cttypes.h"
# include "layout.h"
# include "extract.h"
#include "lterrors.h"

#include "graphics.h"
#include "minmax.h"

# if defined (LT_DEBUG) || defined (SE_DEBUG)

# define SCREEN_MEMORY     ((uchar *) 0xA0000)
# define SCREEN_WIDTH      1500
# define SCREEN_HEIGHT     1500
# define SCREEN_BYTE_WIDTH (SCREEN_WIDTH / 8)
# define SCREEN_BYTE_SIZE  (SCREEN_BYTE_WIDTH * SCREEN_HEIGHT)
# define MAX_SCALING       1
# define MIN_SCALING       1

# define FIELD_WIDTH       (SCREEN_WIDTH  * MAX_SCALING)
# define FIELD_HEIGHT      (SCREEN_HEIGHT * MAX_SCALING)

# define STEPS 4

# define KEY_ESC        '\033'
# define KEY_UP         '\110'
# define KEY_RIGHT      '\115'
# define KEY_DOWN       '\120'
# define KEY_LEFT       '\113'
# define KEY_F1         '\073'
# define KEY_F2         '\074'
# define KEY_F3         '\075'
# define KEY_F4         '\076'
# define KEY_F5         '\077'
# define KEY_F6         '\100'
# define KEY_F7         '\101'
# define KEY_F8         '\102'
# define KEY_F9         '\103'
# define KEY_F10        '\104'

# define NUMBER_TO_COLOR(n) ((n) % 6 + 1) // n -> [1..7]
// почему бы не в [0..7] ? (не в [0..15] ?)

BLOCK * pDebugBlock = NULL;
RECTANGLE rDebugRect;
int xDebugVertLine = -1;

static Bool bGraphicsModeWasChanged = FALSE;

void LT_GraphicsOpen (void) // "bGraphicsModeWasChanged=ON"
{
    if (! bGraphicsModeWasChanged) {
        bGraphicsModeWasChanged = TRUE;
        _setvideomode (_VRES16COLOR); //_setvideomode=={}; :)
    }
}

void LT_GraphicsClose (void) // "bGraphicsModeWasChanged=OFF"
{
    if (bGraphicsModeWasChanged) {
        _setvideomode (_DEFAULTMODE); //_setvideomode пуста!
        bGraphicsModeWasChanged = FALSE;
    }
}

void LT_GraphicsClearScreen (void)
{
    _clearscreen (_GCLEARSCREEN); // аргумент фиктивен
}

// окошечко для просмотра: отступим на 50 пикс. от границ
# define VIEW_LEFT      (MAX_SCALING * 50)
# define VIEW_TOP       (MAX_SCALING * 50)
# define VIEW_RIGHT     (MAX_SCALING * (SCREEN_WIDTH  - 50))
# define VIEW_BOTTOM    (MAX_SCALING * (SCREEN_HEIGHT - 50))
# define VIEW_WIDTH     (VIEW_RIGHT  - VIEW_LEFT + 1)
# define VIEW_HEIGHT    (VIEW_BOTTOM - VIEW_TOP  + 1)

/* Matrix Drawing Modes */

# define MATRIX_DRAWING_NATIVE_MODE  1
# define MATRIX_DRAWING_MASK_MODE    2

/* Matrix Drawing Cells Flags for Mask Mode */

# define MDCF_NULL   0
# define MDCF_COLOR  1
# define MDCF_MASK   2

static void MatrixUpdate
(
    int xLeft,
    int yTop,
    int nScaling,
    uchar *pMatrix,
    int nWidth,
    int nHeight,
    Bool bGrid,
    int DrawingMode
)
{
    int x, y;
    int nFactor;
    static char OldMask [8];
    //1000 0001, 0100 0010, 0010 0100, 0001 1000, -//-
    // - такая построчная развертка битовой буквы 'X':
    static char Mask [8] = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};
    LT_GraphicsClearScreen ();

    if (nWidth == 0 || nHeight == 0 || pMatrix == NULL)
        return;

    // на деле получаем VIEW_WIDTH==1500-50*2==1400 (отступ от 1500 по 50 с двух сторон),
    // и при (nWidth==PAGE_MATRIX_WIDTH) nFactor=1400/1024 =1;
    nFactor = MIN (VIEW_WIDTH / nWidth, VIEW_HEIGHT / nHeight);
    _setcolor (_COLOR_BACKGROUND);
    //    _rectangle (_GBORDER, //не заливать границ
    //    //_rectangle (_GFILLINTERIOR,
    //        // оригинальный учет nFactor...
    //        (0          * nFactor - xLeft) / nScaling, //==0
    //        (0          * nFactor - yTop)  / nScaling, //==0
    //        (nWidth  * nFactor - xLeft) / nScaling,    //==1024
    //        (nHeight * nFactor - yTop)  / nScaling);   //==1024

    switch (DrawingMode) {
        case MATRIX_DRAWING_NATIVE_MODE:

            for (x = 0; x < nWidth; x++) {
                for (y = 0; y < nHeight; y++) { //from 0 to 1024...
                    if (pMatrix [x + y * nWidth] == 0) { // --//--
                        continue;
                    }

                    // NUMBER_TO_COLOR -- это отображение в 1..7
                    // (что как раз покрывает комбинацию PMC-флагов: root,dust,picture (1;2;4))
                    _setcolor
                    (NUMBER_TO_COLOR (pMatrix [x + y * nWidth] & 0xFF) & 7);
                    //                    _rectangle (_GFILLINTERIOR,
                    //                        (x       * nFactor - xLeft) / nScaling,
                    //                        (y       * nFactor - yTop)  / nScaling,
                    //                        ((x + 1) * nFactor - xLeft) / nScaling,
                    //                        ((y + 1) * nFactor - yTop)  / nScaling);
                    // точка -- это рез-т сжатия пр-ка
                    // в PAGE_COMP_FACTOR раз (XY_(UN)COMPRESS)
                    _rectangle (_GBORDER,
                                XY_UNCOMPRESS( (x * nFactor - xLeft) / nScaling ),
                                XY_UNCOMPRESS( (y * nFactor - yTop) / nScaling ),
                                XY_UNCOMPRESS( ((x + 1) * nFactor - xLeft) / nScaling ),
                                XY_UNCOMPRESS( ((y + 1) * nFactor - yTop) / nScaling )
                               );
                    // nFactor обычно (всегда?) == 1
                }
            }

            break;
        case MATRIX_DRAWING_MASK_MODE:
            _setcolor (_COLOR_GREEN);

            for (x = 0; x < nWidth; x++) {
                for (y = 0; y < nHeight; y++) {
                    if (pMatrix [x + y * nWidth] & MDCF_COLOR) {
                        _rectangle (_GFILLINTERIOR,
                                    (x * nFactor - xLeft) / nScaling,
                                    (y * nFactor - yTop) / nScaling,
                                    ((x + 1) * nFactor - xLeft) / nScaling,
                                    ((y + 1) * nFactor - yTop) / nScaling);
                    }
                }
            }

            _getfillmask (OldMask);
            _setfillmask (Mask);
            _setcolor (_COLOR_BLACK);

            for (x = 0; x < nWidth; x++) {
                for (y = 0; y < nHeight; y++) {
                    if (pMatrix [x + y * nWidth] & MDCF_MASK) {
                        _rectangle (_GFILLINTERIOR,
                                    (x * nFactor - xLeft) / nScaling,
                                    (y * nFactor - yTop) / nScaling,
                                    ((x + 1) * nFactor - xLeft) / nScaling,
                                    ((y + 1) * nFactor - yTop) / nScaling);
                    }
                }
            }

            _setfillmask (OldMask);
            break;
    }

    if (! bGrid)
        return;

    // рисование сетки. нормальное состояние -- "выключено".
    _setcolor (_COLOR_GRAY);

    for (x = 0; x < nWidth + 1; x++) {
        _moveto (
            (x * nFactor - xLeft) / nScaling,
            (0 * nFactor - yTop) / nScaling);
        _lineto (
            (x * nFactor - xLeft) / nScaling,
            (nHeight * nFactor - yTop) / nScaling);
    }

    for (y = 0; y < nHeight + 1; y++) {
        _moveto (
            (0 * nFactor - xLeft) / nScaling,
            (y * nFactor - yTop) / nScaling);
        _lineto (
            (nWidth * nFactor - xLeft) / nScaling,
            (y * nFactor - yTop) / nScaling);
    }
}

static void BlockRectangle (BLOCK *p, short fill,
                            int xLeft, int yTop, int nScaling)
{
    switch (p -> Type) {
        case BLOCK_NULL:
            return;
        case BLOCK_PICTURE:
        case BLOCK_TEXT:
            _rectangle (fill,
                        (p -> Rect.xLeft - xLeft) / nScaling,
                        (p -> Rect.yTop - yTop) / nScaling,
                        (p -> Rect.xRight - xLeft) / nScaling,
                        (p -> Rect.yBottom - yTop) / nScaling);
            break;
        case BLOCK_HORZ_SEPARATOR:
        case BLOCK_VERT_SEPARATOR:
            _setcolor (_COLOR_BACKGROUND);
            _rectangle (_GBORDER,
                        (p -> Rect.xLeft - xLeft) / nScaling,
                        (p -> Rect.yTop - yTop) / nScaling,
                        (p -> Rect.xRight - xLeft) / nScaling,
                        (p -> Rect.yBottom - yTop) / nScaling);
            break;
        case BLOCK_RECT_SEPARATOR:
            _setcolor (_COLOR_YELLOW);
            _rectangle (_GBORDER,
                        (p -> Rect.xLeft - xLeft) / nScaling + 3,
                        (p -> Rect.yTop - yTop) / nScaling + 3,
                        (p -> Rect.xRight - xLeft) / nScaling - 3,
                        (p -> Rect.yBottom - yTop) / nScaling - 3);
            break;
    }
}

# define SEPOUT_LINE     0
# define SEPOUT_DASH_1   1
# define SEPOUT_DASH_2   2

static void SeparatorOutput (SEPARATOR *pSep,
                             int nColor, int OutputMode,
                             int xLeft, int yTop, int nScaling)
{
    if (pSep == NULL)
        return;

    _setcolor (nColor);

    switch (OutputMode) {
        case SEPOUT_LINE:
            break;
        case SEPOUT_DASH_1:
            _setlinestyle (0xf0f0);
            break;
        case SEPOUT_DASH_2:
            _setlinestyle (0x0f0f);
            break;
    }

    switch (pSep -> Type) {
        default:
            break;
        case SEP_VERT:
        case SEP_HORZ:
            _moveto ((pSep -> xBegin - xLeft) / nScaling,
                     (pSep -> yBegin - yTop) / nScaling);
            _lineto ((pSep -> xEnd - xLeft) / nScaling,
                     (pSep -> yEnd - yTop) / nScaling);
            break;
        case SEP_RECT:
            _rectangle (_GBORDER,
                        (pSep -> xBegin - xLeft) / nScaling + 3,
                        (pSep -> yBegin - yTop) / nScaling + 3,
                        (pSep -> xEnd - xLeft) / nScaling - 3,
                        (pSep -> yEnd - yTop) / nScaling - 3);
            break;
    }

    _setlinestyle (0xffff);
}

static void SeparatorsOutput (int xLeft, int yTop, int nScaling)
{
    int i;
    int nColor;

    for (i = 0; i < nSeps; i++) {
        switch (pSeps [i].Type) {
            default:
                continue;
            case SEP_VERT:
                nColor = 15;
                break;
            case SEP_HORZ:
                nColor = 15;
                break;
            case SEP_RECT:
                nColor = 14;
                break;
        }

        SeparatorOutput (&pSeps[i], nColor, SEPOUT_LINE, xLeft, yTop, nScaling);
    }
}

static void RootsUpdate (int xLeft, int yTop, int nScaling)
{
    ROOT *pRoot;
    LT_GraphicsClearScreen ();

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        int nColor = _COLOR_DGRAY;

        if (pRoot -> bType & ROOT_LETTER) nColor |= 1; //=>BLUE

        if (pRoot -> bType & ROOT_RECOGNIZED) nColor |= 2; //=>GREEN

        if (IS_LAYOUT_DUST (*pRoot)) nColor |= 4; //=>RED

        if (pRoot -> bType & ROOT_DEBUG) nColor = _COLOR_YELLOW;

        _setcolor (nColor);
        _rectangle (_GBORDER, //_GFILLINTERIOR,
                    (pRoot -> xColumn - xLeft) / nScaling,
                    (pRoot -> yRow - yTop) / nScaling,
                    (pRoot -> xColumn + pRoot ->nWidth - 1 - xLeft) / nScaling,
                    (pRoot -> yRow + pRoot ->nHeight - 1 - yTop) / nScaling);
    }

    SeparatorsOutput (xLeft, yTop, nScaling);
}

static void RootStripsUpdate (int xLeft, int yTop, int nScaling)
{
    int iStrip;
    ROOT *pRoot;
    int nShift;
    LT_GraphicsClearScreen ();

    for (iStrip = 0; iStrip < nRootStrips; iStrip++) {
        _setcolor (NUMBER_TO_COLOR (iStrip) | 8);
        nShift = (iStrip % 3) * 2;
        _moveto
        (
            0,
            (iStrip * nRootStripsStep + nRootStripsOffset
             - yTop) / nScaling + 1
        );
        _lineto
        (
            SCREEN_WIDTH,
            (iStrip * nRootStripsStep + nRootStripsOffset
             - yTop) / nScaling + 1
        );
        _moveto
        (
            0,
            ((iStrip + 1) * nRootStripsStep + nRootStripsOffset
             - yTop) / nScaling - 1
        );
        _lineto
        (
            SCREEN_WIDTH,
            ((iStrip + 1) * nRootStripsStep + nRootStripsOffset
             - yTop) / nScaling - 1
        );

        if (pRootStrips [iStrip].pBegin == NULL)
            continue;

        for (pRoot = pRootStrips [iStrip].pBegin;
                pRoot <= pRootStrips [iStrip].pEnd;
                pRoot++) {
            _rectangle (_GBORDER,
                        (pRoot -> xColumn
                         - xLeft) / nScaling + nShift,
                        (pRoot -> yRow
                         - yTop) / nScaling + nShift,
                        (pRoot -> xColumn + pRoot -> nWidth - 1
                         - xLeft) / nScaling + nShift,
                        (pRoot -> yRow + pRoot -> nHeight - 1
                         - yTop) / nScaling + nShift);
        }

        /*
         for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
         {
         if ((pRoot -> yRow
         - nRootStripsOffset) / nRootStripsStep > iStrip
         ||
         (pRoot -> yRow + pRoot -> nHeight - 1
         - nRootStripsOffset) / nRootStripsStep < iStrip)
         {
         continue;
         }

         _rectangle (_GBORDER,
         (pRoot -> xColumn
         - xLeft) / nScaling + nShift,
         (pRoot -> yRow
         - yTop)  / nScaling + nShift,
         (pRoot -> xColumn + pRoot -> nWidth  - 1
         - xLeft) / nScaling + nShift,
         (pRoot -> yRow    + pRoot -> nHeight - 1
         - yTop)  / nScaling + nShift);
         }
         */
    }
}

static void PageMatrixUpdate (int xLeft, int yTop, int nScaling)
{
    MatrixUpdate
    (
        xLeft,
        yTop,
        nScaling,
        PageMatrix,
        PAGE_MATRIX_WIDTH, // 1024
        PAGE_MATRIX_HEIGHT, // 1024 (квадрат такой)
        FALSE, // Bool bGrid
        MATRIX_DRAWING_NATIVE_MODE // режим рисования, один из двух (18.10.01)
    );
}

static void BlocksUpdate (int xLeft, int yTop, int nScaling)
{
    BLOCK *p;
    ROOT *pRoot;
    int nColor;
    LT_GraphicsClearScreen ();

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        nColor = NUMBER_TO_COLOR (p -> nNumber) |
                 (((p -> uFlags & BF_NOT_BREAK_BLOCK) != 0) << 3);
        _setcolor (nColor);
        BlockRectangle (p, _GBORDER, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pUpSep, nColor, SEPOUT_DASH_1, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pDownSep, nColor, SEPOUT_DASH_2, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pLeftSep, nColor, SEPOUT_DASH_1, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pRightSep, nColor, SEPOUT_DASH_2, xLeft, yTop, nScaling);
    }

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        if (pRoot -> nBlock == REMOVED_BLOCK_NUMBER) {
            /*
             _setcolor (14);

             _rectangle (_GBORDER,
             (pRoot -> xColumn - xLeft) / nScaling,
             (pRoot -> yRow    - yTop)  / nScaling,
             (pRoot -> xColumn + pRoot ->nWidth  - 1
             - xLeft) / nScaling,
             (pRoot -> yRow    + pRoot ->nHeight - 1
             - yTop)  / nScaling);
             */
        }
        else {
            if (pRoot -> nBlock == DUST_BLOCK_NUMBER)
                _setcolor (_COLOR_BACKGROUND);

            else
                _setcolor (NUMBER_TO_COLOR (pRoot -> nBlock));

            _rectangle (_GBORDER,//_GFILLINTERIOR,
                        (pRoot -> xColumn - xLeft) / nScaling,
                        (pRoot -> yRow - yTop) / nScaling,
                        (pRoot -> xColumn + pRoot ->nWidth - 1
                         - xLeft) / nScaling,
                        (pRoot -> yRow + pRoot ->nHeight - 1
                         - yTop) / nScaling);
        }
    }
}

static void HystogramUpdate (int xLeft, int yTop, int nScaling)
{
    char szBuffer [32];
    int i;
    int nMaxValue;
    int iMaxColumn;
    struct textsettings ts;
    int nCharHeight;
    ROOT *pRoot;//Rom
    LT_GraphicsClearScreen ();
    nMaxValue = 0;
    iMaxColumn = 0;

    /**/
    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++) {
        if (pRoot->nBlock == pDebugBlock->nNumber) {
            int nColor = 8;

            if (pRoot -> bType & ROOT_LETTER) nColor |= 1;

            if (pRoot -> bType & ROOT_RECOGNIZED) nColor |= 2;

            if (IS_LAYOUT_DUST (*pRoot)) nColor |= 4;

            if (pRoot -> bType & ROOT_DEBUG) nColor = 14;

            _setcolor (nColor);
            _rectangle (_GFILLINTERIOR,
                        (pRoot -> xColumn - xLeft) / nScaling,
                        (pRoot -> yRow - yTop) / nScaling,
                        (pRoot -> xColumn + pRoot ->nWidth - 1 - xLeft) / nScaling,
                        (pRoot -> yRow + pRoot ->nHeight - 1 - yTop) / nScaling);
        }
    }

    /**/

    for (i = 0; i < nHystColumns; i++) {
        if (nMaxValue < pHystogram [i]) {
            nMaxValue = pHystogram [i];
            iMaxColumn = i;
        }
    }

    if (nMaxValue < 10)
        nMaxValue = 10;

    _gettextsettings (&ts);

    for (i = 0; i < nHystColumns; i++) {
        if (pHystogram [i] == 0) {
            /*
             _setcolor (12);

             _rectangle (_GFILLINTERIOR,
             (i * VIEW_WIDTH / nHystColumns + VIEW_LEFT
             - xLeft) / nScaling,
             (VIEW_TOP
             - yTop)  / nScaling,
             ((i + 1) * VIEW_WIDTH / nHystColumns + VIEW_LEFT
             - xLeft) / nScaling,
             (VIEW_BOTTOM
             - yTop)  / nScaling);
             */
        }
        else {
            _setcolor (_COLOR_BLUE);
            _rectangle (_GFILLINTERIOR,
                        (i * VIEW_WIDTH / nHystColumns + VIEW_LEFT
                         - xLeft) / nScaling,
                        (VIEW_BOTTOM - pHystogram [i] * VIEW_HEIGHT / nMaxValue
                         - yTop) / nScaling,
                        ((i + 1) * VIEW_WIDTH / nHystColumns + VIEW_LEFT
                         - xLeft) / nScaling,
                        (VIEW_BOTTOM
                         - yTop) / nScaling);
        }
    }

    _setcolor (_COLOR_RED);
    _moveto (((xDebugVertLine - pDebugBlock -> Rect.xLeft)
              * VIEW_WIDTH / nHystColumns
              + VIEW_WIDTH / nHystColumns / 2
              + VIEW_LEFT
              - xLeft) / nScaling, 0);
    _lineto (((xDebugVertLine - pDebugBlock -> Rect.xLeft)
              * VIEW_WIDTH / nHystColumns
              + VIEW_WIDTH / nHystColumns / 2
              + VIEW_LEFT
              - xLeft) / nScaling, SCREEN_HEIGHT);
    _setcolor (_COLOR_BACKGROUND);
    _settextalign (_RIGHT, _TOP);
    nCharHeight = VIEW_HEIGHT / 10 / nScaling / 2;
    _setcharsize (nCharHeight, nCharHeight / 2);

    for (i = 0; i < nMaxValue - nMaxValue / 20; i += MAX (nMaxValue / 10, 1)) {
        _moveto ((VIEW_LEFT - xLeft) / nScaling,
                 (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling);
        _lineto ((VIEW_RIGHT - xLeft) / nScaling,
                 (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling);

        if (i != 0) {
            sprintf (szBuffer, "%d\n", i);
            _grtext ((VIEW_LEFT - xLeft) / nScaling,
                     (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling,
                     szBuffer);
        }
    }

    sprintf (szBuffer, "%d\n", nMaxValue);
    _grtext ((VIEW_LEFT - xLeft) / nScaling,
             (VIEW_TOP - yTop) / nScaling,
             szBuffer);
    _rectangle (_GBORDER,
                (VIEW_LEFT - xLeft) / nScaling,
                (VIEW_TOP - yTop) / nScaling,
                (VIEW_RIGHT - xLeft) / nScaling,
                (VIEW_BOTTOM - yTop) / nScaling);
    _setcharsize (ts.height, ts.width);
}

static void SpecialCuttingUpdate (int xLeft, int yTop, int nScaling)
{
    int i;
    int nWidth = pDebugBlock -> Rect.xRight - pDebugBlock -> Rect.xLeft + 1;
    int nHeight = pDebugBlock -> Rect.yBottom - pDebugBlock -> Rect.yTop + 1;
    int nFactor;
    LT_GraphicsClearScreen ();

    if (nWidth == 0 || nHeight == 0)
        return;

    nFactor = MIN (VIEW_WIDTH / nWidth, VIEW_HEIGHT / nHeight);
    _setcolor (_COLOR_BLUE);
    _rectangle
    (
        _GBORDER,
        0,
        0,
        ((nWidth - 1) * nFactor - xLeft) / nScaling,
        ((nHeight - 1) * nFactor - yTop) / nScaling
    );

    for (i = 0; i < nHystColumns; i++) {
        if (pHystBottoms [i] == -1)
            continue;

        if (pDebugBlock -> pHorzHystogram [pHystTops [i] - 1] == 0)
            _setcolor (_COLOR_RED);

        else
            _setcolor (_COLOR_BLUE);

        _rectangle
        (
            _GFILLINTERIOR,
            (i * nFactor - xLeft) / nScaling,
            (pHystTops [i] * nFactor - yTop) / nScaling,
            ((i + 1) * nFactor - xLeft) / nScaling - 1,
            (pHystBottoms [i] * nFactor - yTop) / nScaling
        );
    }
}

static void SB_MatrixUpdate (int xLeft, int yTop, int nScaling)
{
    int nFactor;
    ROOT *pRoot;
    nFactor = MIN (VIEW_WIDTH / nSB_Width, VIEW_HEIGHT / nSB_Height);
    MatrixUpdate
    (
        xLeft,
        yTop,
        nScaling,
        pSB_Matrix,
        nSB_Width,
        nSB_Height,
        TRUE,
        MATRIX_DRAWING_NATIVE_MODE
    );
    _setcolor (NUMBER_TO_COLOR (1) | 8);

    for (pRoot = pDebugBlock -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext) {
        _rectangle (_GFILLINTERIOR,
                    ((pRoot -> xColumn
                      - pDebugBlock -> Rect.xLeft) * nFactor / nSB_CellWidth
                     - xLeft) / nScaling,
                    ((pRoot -> yRow
                      - pDebugBlock -> Rect.yTop) * nFactor / nSB_CellHeight
                     - yTop) / nScaling,
                    ((pRoot -> xColumn + pRoot -> nWidth - 1
                      - pDebugBlock -> Rect.xLeft) * nFactor / nSB_CellWidth
                     - xLeft) / nScaling,
                    ((pRoot -> yRow + pRoot -> nHeight - 1
                      - pDebugBlock -> Rect.yTop) * nFactor / nSB_CellHeight
                     - yTop) / nScaling);
    }
}

static void WSB_HystogramUpdate
(
    int xLeft,
    int yTop,
    int nScaling,
    int * pWSB_Hyst,
    int nMaxValue
)
{
    int i;
    char szBuffer [32];
    struct textsettings ts;
    int nCharHeight;
    LT_GraphicsClearScreen ();
    _setcolor (_COLOR_RED);

    for (i = 0; i < nWSB_Points; i++) {
        _rectangle (_GFILLINTERIOR,
                    (pWSB_Points [i].xBegin * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                     - xLeft) / nScaling,
                    (VIEW_TOP
                     - yTop) / nScaling,
                    (pWSB_Points [i].xEnd * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                     - xLeft) / nScaling,
                    (VIEW_BOTTOM
                     - yTop) / nScaling);
    }

    _gettextsettings (&ts);

    for (i = 0; i < nWSB_HystColumns; i++) {
        if (pWSB_Hyst [i] == 0)
            continue;

        if (i / 20 % 2 == 0)
            _setcolor (_COLOR_GREEN);

        else
            _setcolor (_COLOR_BLUE);

        _rectangle (_GFILLINTERIOR,
                    (i * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                     - xLeft) / nScaling,
                    (VIEW_BOTTOM - pWSB_Hyst [i] * VIEW_HEIGHT / nMaxValue
                     - yTop) / nScaling,
                    ((i + 1) * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                     - xLeft) / nScaling,
                    (VIEW_BOTTOM
                     - yTop) / nScaling);
    }

    _setcolor (_COLOR_BACKGROUND);
    _settextalign (_RIGHT, _TOP);
    nCharHeight = VIEW_HEIGHT / 16 / nScaling / 2;
    _setcharsize (nCharHeight, nCharHeight / 2);

    for (i = 0; i < nMaxValue - nMaxValue / 32; i += MAX(nMaxValue / 16, 1)) {
        _moveto ((VIEW_LEFT - xLeft) / nScaling,
                 (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling);
        _lineto ((VIEW_RIGHT - xLeft) / nScaling,
                 (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling);

        if (i != 0) {
            sprintf (szBuffer, "%d\n", i);
            _grtext ((VIEW_LEFT - xLeft) / nScaling,
                     (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling,
                     szBuffer);
        }
    }

    sprintf (szBuffer, "%d\n", nMaxValue);
    _grtext ((VIEW_LEFT - xLeft) / nScaling,
             (VIEW_TOP - yTop) / nScaling,
             szBuffer);
    _rectangle (_GBORDER,
                (VIEW_LEFT - xLeft) / nScaling,
                (VIEW_TOP - yTop) / nScaling,
                (VIEW_RIGHT - xLeft) / nScaling,
                (VIEW_BOTTOM - yTop) / nScaling);
    _setcharsize (ts.height, ts.width);
    _setcolor (_COLOR_BLACK);

    for (i = 0; i < nWSB_Points; i++) {
        _moveto
        (
            (pWSB_Points [i].xMain * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
             - xLeft) / nScaling,
            (VIEW_TOP
             - yTop) / nScaling
        );
        _lineto
        (
            (pWSB_Points [i].xMain * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
             - xLeft) / nScaling,
            (VIEW_BOTTOM
             - yTop) / nScaling
        );

        if (pWSB_Hyst == pWSB_Hyst2) {
            _moveto
            (
                (pWSB_Points [i].xBegin * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                 - xLeft) / nScaling,
                (VIEW_BOTTOM - pWSB_Points [i].yMin * VIEW_HEIGHT / nMaxValue
                 - yTop) / nScaling
            );
            _lineto
            (
                (pWSB_Points [i].xEnd * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                 - xLeft) / nScaling,
                (VIEW_BOTTOM - pWSB_Points [i].yMin * VIEW_HEIGHT / nMaxValue
                 - yTop) / nScaling
            );
            _moveto
            (
                (pWSB_Points [i].xBegin * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                 - xLeft) / nScaling,
                (VIEW_BOTTOM - pWSB_Points [i].yMax * VIEW_HEIGHT / nMaxValue
                 - yTop) / nScaling
            );
            _lineto
            (
                (pWSB_Points [i].xEnd * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                 - xLeft) / nScaling,
                (VIEW_BOTTOM - pWSB_Points [i].yMax * VIEW_HEIGHT / nMaxValue
                 - yTop) / nScaling
            );
            _moveto
            (
                (pWSB_Points [i].xBegin * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                 - xLeft) / nScaling,
                (VIEW_BOTTOM - pWSB_Points [i].yAverage * VIEW_HEIGHT / nMaxValue
                 - yTop) / nScaling
            );
            _lineto
            (
                (pWSB_Points [i].xEnd * VIEW_WIDTH / nWSB_HystColumns + VIEW_LEFT
                 - xLeft) / nScaling,
                (VIEW_BOTTOM - pWSB_Points [i].yAverage * VIEW_HEIGHT / nMaxValue
                 - yTop) / nScaling
            );
        }
    }
}

static void WSB_Hystogram_1_Update (int xLeft, int yTop, int nScaling)
{
    int i;
    int nMaxValue;
    nMaxValue = 0;

    for (i = 0; i < nWSB_HystColumns; i++) {
        if (nMaxValue < pWSB_Hyst1 [i])
            nMaxValue = pWSB_Hyst1 [i];
    }

    if (nMaxValue < 10)
        nMaxValue = 10;

    WSB_HystogramUpdate (xLeft, yTop, nScaling, pWSB_Hyst1, nMaxValue);
}

static void WSB_Hystogram_2_Update (int xLeft, int yTop, int nScaling)
{
    WSB_HystogramUpdate (xLeft, yTop, nScaling, pWSB_Hyst2, nPageHeight);
}

static void WSB_PointsUpdate (int xLeft, int yTop, int nScaling)
{
    int i;
    BlocksUpdate (xLeft, yTop, nScaling);
    _setcolor (_COLOR_RED);

    for (i = 0; i < nWSB_Points; i++) {
        _moveto
        (
            (pWSB_Points [i].xMain + xWSB_HystOffset - xLeft) / nScaling,
            0
        );
        _lineto
        (
            (pWSB_Points [i].xMain + xWSB_HystOffset - xLeft) / nScaling,
            SCREEN_HEIGHT
        );
    }
}

static void PictureRemovingConditionsUpdate (int xLeft, int yTop, int nScaling)
{
    int nFactor;
    ROOT *pRoot;
    uchar *pMatrix;
    int nWidth;
    int nHeight;
    int xSrc, ySrc, xDst, yDst;
    LPOINT TopLeft, TopRight, BottomLeft, BottomRight;
    RECTANGLE rReal;
    nWidth = rDebugRect.xRight - rDebugRect.xLeft + 1;
    nHeight = rDebugRect.yBottom - rDebugRect.yTop + 1;
    pMatrix = static_cast<uchar*> (malloc (nWidth * nHeight));
    memset (pMatrix, MDCF_NULL, nWidth * nHeight);

    if (pMatrix == NULL)
        ErrorNoEnoughMemory ("in LTGRAPH.C,PictureRemovingConditionsUpdate,part 1");

    nFactor = MIN (VIEW_WIDTH / nWidth, VIEW_HEIGHT / nHeight);

    for (ySrc = rDebugRect.yTop, yDst = 0;
            ySrc <= rDebugRect.yBottom;
            ySrc++, yDst++) {
        for (xSrc = rDebugRect.xLeft, xDst = 0;
                xSrc <= rDebugRect.xRight;
                xSrc++, xDst++) {
            if (PageMatrix [ySrc * PAGE_MATRIX_WIDTH + xSrc] & PMC_DEBUG1)
                pMatrix [yDst * nWidth + xDst] |= MDCF_COLOR;

            if (PageMatrix [ySrc * PAGE_MATRIX_WIDTH + xSrc] & PMC_PICTURE)
                pMatrix [yDst * nWidth + xDst] |= MDCF_MASK;
        }
    }

    MatrixUpdate
    (
        xLeft,
        yTop,
        nScaling,
        pMatrix,
        nWidth,
        nHeight,
        TRUE,
        MATRIX_DRAWING_MASK_MODE
    );
    _setcolor (_COLOR_DGREEN);

    for (pRoot = pDebugBlock -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext) {
        RECTANGLE r;
        r.xLeft = pRoot -> xColumn;
        r.yTop = pRoot -> yRow;
        REAL_XY (r.xLeft, r.yTop);
        r.xRight = r.xLeft + pRoot -> nWidth - 1;
        r.yBottom = r.yTop + pRoot -> nHeight - 1;
        _rectangle (_GFILLINTERIOR,
                    ((r.xLeft
                      - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
                     * nFactor / PAGE_COMP_FACTOR
                     - xLeft) / nScaling,
                    ((r.yTop
                      - (rDebugRect.yTop << PAGE_COMP_SHIFT))
                     * nFactor / PAGE_COMP_FACTOR
                     - yTop) / nScaling,
                    ((r.xRight
                      - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
                     * nFactor / PAGE_COMP_FACTOR
                     - xLeft) / nScaling,
                    ((r.yBottom
                      - (rDebugRect.yTop << PAGE_COMP_SHIFT))
                     * nFactor / PAGE_COMP_FACTOR
                     - yTop) / nScaling);
    }

    // Get rectangle of block in real coordinates
    TopLeft.x = pDebugBlock -> Rect.xLeft;
    TopLeft.y = pDebugBlock -> Rect.yTop;
    TopRight.x = pDebugBlock -> Rect.xRight;
    TopRight.y = pDebugBlock -> Rect.yTop;
    BottomLeft.x = pDebugBlock -> Rect.xLeft;
    BottomLeft.y = pDebugBlock -> Rect.yBottom;
    BottomRight.x = pDebugBlock -> Rect.xRight;
    BottomRight.y = pDebugBlock -> Rect.yBottom;
    REAL_XY (TopLeft.x , TopLeft.y);
    REAL_XY (TopRight.x , TopRight.y);
    REAL_XY (BottomLeft.x , BottomLeft.y);
    REAL_XY (BottomRight.x, BottomRight.y);
    rReal.xLeft = MIN (TopLeft.x, BottomLeft.x);
    rReal.yTop = MIN (TopLeft.y, TopRight.y);
    rReal.xRight = MAX (TopRight.x, BottomRight.x);
    rReal.yBottom = MAX (BottomLeft.y, BottomRight.y);
    _setcolor (_COLOR_RED);
    _moveto
    (
        ((TopLeft.x
          - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - xLeft) / nScaling,
        ((TopLeft.y
          - (rDebugRect.yTop << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - yTop) / nScaling
    );
    _lineto
    (
        ((TopRight.x
          - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - xLeft) / nScaling,
        ((TopRight.y
          - (rDebugRect.yTop << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - yTop) / nScaling
    );
    _lineto
    (
        ((BottomRight.x
          - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - xLeft) / nScaling,
        ((BottomRight.y
          - (rDebugRect.yTop << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - yTop) / nScaling
    );
    _lineto
    (
        ((BottomLeft.x
          - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - xLeft) / nScaling,
        ((BottomLeft.y
          - (rDebugRect.yTop << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - yTop) / nScaling
    );
    _lineto
    (
        ((BottomLeft.x
          - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - xLeft) / nScaling,
        ((BottomLeft.y
          - (rDebugRect.yTop << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - yTop) / nScaling
    );
    _lineto
    (
        ((TopLeft.x
          - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - xLeft) / nScaling,
        ((TopLeft.y
          - (rDebugRect.yTop << PAGE_COMP_SHIFT))
         * nFactor / PAGE_COMP_FACTOR
         - yTop) / nScaling
    );
    _setcolor (_COLOR_BLUE);
    _rectangle (_GBORDER,
                ((rReal.xLeft
                  - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
                 * nFactor / PAGE_COMP_FACTOR
                 - xLeft) / nScaling,
                ((rReal.yTop
                  - (rDebugRect.yTop << PAGE_COMP_SHIFT))
                 * nFactor / PAGE_COMP_FACTOR
                 - yTop) / nScaling,
                ((rReal.xRight
                  - (rDebugRect.xLeft << PAGE_COMP_SHIFT))
                 * nFactor / PAGE_COMP_FACTOR
                 - xLeft) / nScaling,
                ((rReal.yBottom
                  - (rDebugRect.yTop << PAGE_COMP_SHIFT))
                 * nFactor / PAGE_COMP_FACTOR
                 - yTop) / nScaling);
    free (pMatrix);
}

void ColumnChart
(
    int xLeft,
    int yTop,
    int xRight,
    int yBottom,
    int *pArray,
    int nColumns
)
{
    int i;
    int nBiggest;
    struct textsettings ts;
    int dxTextOffset;
    int dyTextOffset;
    char szTextBuffer [32];
    _setcolor (_COLOR_BACKGROUND);

    if (nColumns == 0 || pArray == NULL)
        ErrorInternal ("Bad data");

    nBiggest = pArray [0];

    for (i = 1; i < nColumns; i++) {
        if (pArray [i] > nBiggest)
            nBiggest = pArray [i];
    }

    if (nBiggest == 0)
        return;

    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);
    dxTextOffset = (xRight - xLeft) / nColumns / 2,
                   dyTextOffset = ts.height / 2;

    for (i = 0; i < nColumns; i++) {
        _setcolor (NUMBER_TO_COLOR (i));
        _rectangle
        (
            _GFILLINTERIOR,
            xLeft + i * (xRight - xLeft) / nColumns,
            yTop + (nBiggest - pArray [i]) * (yBottom - yTop) / nBiggest,
            xLeft + (i + 1) * (xRight - xLeft) / nColumns,
            yBottom
        );
        _setcolor (_COLOR_BACKGROUND);
        _rectangle
        (
            _GBORDER,
            xLeft + i * (xRight - xLeft) / nColumns,
            yTop + (nBiggest - pArray [i]) * (yBottom - yTop) / nBiggest,
            xLeft + (i + 1) * (xRight - xLeft) / nColumns,
            yBottom
        );
        sprintf (szTextBuffer, "%d", pArray [i]);
        _grtext
        (
            xLeft + i * (xRight - xLeft) / nColumns + dxTextOffset,
            yBottom + dyTextOffset,
            szTextBuffer
        );
    }

    _setcharsize (ts.height, ts.width);
}

# define B2SX(x) (((x) * nFactor - xLeft) / nScaling)
# define B2SY(y) (((y) * nFactor - yTop)  / nScaling)

static void DD_RemovingConditionsUpdate (int xLeft, int yTop, int nScaling)
{
    int nFactor;
    ROOT *pRoot;
    int nBlockWidth, nBlockHeight;
    int nMatrixWidth, nMatrixHeight;
    RECTANGLE rGrid;
    int nCellWidth, nCellHeight;
    int x, y;
    struct textsettings ts;
    extern int DQD_Matrix [];
    extern int DSD_Matrix [];
    LT_GraphicsClearScreen ();
    nBlockWidth = pDebugBlock -> Rect.xRight - pDebugBlock -> Rect.xLeft + 1;
    nBlockHeight = pDebugBlock -> Rect.yBottom - pDebugBlock -> Rect.yTop + 1;
    nFactor = MIN (VIEW_WIDTH / 2 / nBlockWidth, VIEW_HEIGHT / nBlockHeight);
    nMatrixWidth = rDebugRect.xRight - rDebugRect.xLeft + 1;
    nMatrixHeight = rDebugRect.yBottom - rDebugRect.yTop + 1;
    rGrid.xLeft = B2SX (0);
    rGrid.yTop = B2SY (0);
    rGrid.xRight = B2SX (nBlockWidth - 1);
    rGrid.yBottom = B2SY (nBlockHeight - 1);
    nCellWidth = (rGrid.xRight - rGrid.xLeft + 1) / nMatrixWidth;
    nCellHeight = (rGrid.yBottom - rGrid.yTop + 1) / nMatrixHeight;

    for (pRoot = pDebugBlock -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext) {
        RECTANGLE r;
        r.xLeft = pRoot -> xColumn;
        r.yTop = pRoot -> yRow;
        r.xRight = r.xLeft + pRoot -> nWidth - 1;
        r.yBottom = r.yTop + pRoot -> nHeight - 1;
        _setcolor (IS_LAYOUT_DUST (*pRoot) ? _COLOR_GREEN : _COLOR_DGREEN);
        _rectangle (_GFILLINTERIOR,
                    B2SX (r.xLeft - pDebugBlock -> Rect.xLeft),
                    B2SY (r.yTop - pDebugBlock -> Rect.yTop),
                    B2SX (r.xRight - pDebugBlock -> Rect.xLeft),
                    B2SY (r.yBottom - pDebugBlock -> Rect.yTop));
    }

    _setcolor (_COLOR_BACKGROUND);

    for (x = 1; x < nMatrixWidth; x++) {
        _moveto (rGrid.xLeft + x * nCellWidth, rGrid.yTop);
        _lineto (rGrid.xLeft + x * nCellWidth, rGrid.yBottom);
    }

    for (y = 1; y < nMatrixHeight; y++) {
        _moveto (rGrid.xLeft, rGrid.yTop + y * nCellHeight);
        _lineto (rGrid.xRight, rGrid.yTop + y * nCellHeight);
    }

    _rectangle (_GBORDER, rGrid.xLeft, rGrid.yTop, rGrid.xRight, rGrid.yBottom);
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (x = 0; x < nMatrixWidth; x++) {
        for (y = 0; y < nMatrixHeight; y++) {
            char szTextBuffer [16];

            if (DQD_Matrix [x + y * nMatrixWidth] == 0)
                continue;

            _setcolor (_COLOR_BLACK);
            _rectangle
            (
                _GFILLINTERIOR,
                rGrid.xLeft + x * nCellWidth + nCellWidth / 2 - ts.width * 2,
                rGrid.yTop + y * nCellHeight + nCellHeight / 2 - ts.height / 3,
                rGrid.xLeft + x * nCellWidth + nCellWidth / 2 + ts.width * 2,
                rGrid.yTop + y * nCellHeight + nCellHeight / 2 + ts.height / 3
            );
            sprintf (szTextBuffer, "%d %d",
                     DQD_Matrix [x + y * nMatrixWidth],
                     DSD_Matrix [x + y * nMatrixWidth]);
            _setcolor (_COLOR_BACKGROUND);
            _grtext
            (
                rGrid.xLeft + x * nCellWidth + nCellWidth / 2,
                rGrid.yTop + y * nCellHeight + nCellHeight / 2,
                szTextBuffer
            );
        }
    }

    _setcharsize (ts.height, ts.width);
    ColumnChart
    (
        SCREEN_WIDTH * 6 / 10,
        SCREEN_HEIGHT * 1 / 10,
        SCREEN_WIDTH * 9 / 10,
        SCREEN_HEIGHT * 4 / 10,
        DQD_Matrix,
        nMatrixWidth * nMatrixHeight
    );
    ColumnChart
    (
        SCREEN_WIDTH * 6 / 10,
        SCREEN_HEIGHT * 6 / 10,
        SCREEN_WIDTH * 9 / 10,
        SCREEN_HEIGHT * 9 / 10,
        DSD_Matrix,
        nMatrixWidth * nMatrixHeight
    );
}
static void ShowBlock (int xLeft, int yTop, int nScaling)
{
    ROOT *pRoot;
    int nFactor;
    int nBlockWidth, nBlockHeight;
    RECTANGLE rGrid;
    char szTextBuffer [80];
    nBlockWidth = pDebugBlock -> Rect.xRight - pDebugBlock -> Rect.xLeft + 1;
    nBlockHeight = pDebugBlock -> Rect.yBottom - pDebugBlock -> Rect.yTop + 1;
    nFactor = MIN (VIEW_WIDTH / 2 / nBlockWidth, VIEW_HEIGHT / nBlockHeight);
    LT_GraphicsClearScreen ();

    for (pRoot = pDebugBlock -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext) {
        RECTANGLE r;
        r.xLeft = pRoot -> xColumn;
        r.yTop = pRoot -> yRow;
        r.xRight = r.xLeft + pRoot -> nWidth - 1;
        r.yBottom = r.yTop + pRoot -> nHeight - 1;
        _setcolor (IS_LAYOUT_DUST (*pRoot) ? _COLOR_GREEN : _COLOR_DGREEN);
        _rectangle (_GFILLINTERIOR,
                    B2SX (r.xLeft - pDebugBlock -> Rect.xLeft),
                    B2SY (r.yTop - pDebugBlock -> Rect.yTop),
                    B2SX (r.xRight - pDebugBlock -> Rect.xLeft),
                    B2SY (r.yBottom - pDebugBlock -> Rect.yTop));
    }

    _setcolor(_COLOR_BACKGROUND);
    rGrid.xLeft = B2SX (0);
    rGrid.yTop = B2SY (0);
    rGrid.xRight = B2SX (nBlockWidth - 1);
    rGrid.yBottom = B2SY (nBlockHeight - 1);
    _rectangle (_GBORDER, rGrid.xLeft, rGrid.yTop, rGrid.xRight, rGrid.yBottom);
    sprintf (szTextBuffer, "Number:%i", pDebugBlock->nNumber);
    _grtext
    (
        rGrid.xLeft ,
        rGrid.yBottom + 15,
        szTextBuffer
    );
}

static BLOCK *pBlocksListBegin;
static int oBlockNext;

static void BlocksListUpdate (int xLeft, int yTop, int nScaling)
{
    BLOCK *p;
    int nBlock;
    struct textsettings ts;
    int x, y;
    char szTextBuffer [128];
    LT_GraphicsClearScreen ();
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (nBlock = 1, p = pBlocksListBegin;
            p != NULL;
            p = *(BLOCK **) ((uchar *) p + oBlockNext), nBlock++) {
        _setcolor (NUMBER_TO_COLOR (p -> nNumber));
        BlockRectangle (p, _GBORDER, xLeft, yTop, nScaling);
        x = ((p -> Rect.xLeft + p -> Rect.xRight) / 2 - xLeft) / nScaling;
        y = ((p -> Rect.yTop + p -> Rect.yBottom) / 2 - yTop) / nScaling;
        _setcolor (_COLOR_BACKGROUND);
        sprintf (szTextBuffer, "%d", nBlock);

        if (p->nUserNum)
            sprintf(szTextBuffer, "%d/%d", nBlock, p->nUserNum);

        _grtext (x, y, szTextBuffer);
    }

    _setcharsize (ts.height, ts.width);
}
/************** ATAL 940414

 static void TreeUpdate (int xLeft, int yTop, int nScaling)
 {
 BLOCK *p, *q;
 int i;
 struct textsettings ts;
 int x1, y1;
 int x2, y2;
 char szTextBuffer [128];

 LT_GraphicsClearScreen ();

 for (p = pBlocksList; p != NULL; p = p -> pNext)
 {
 _setcolor (NUMBER_TO_COLOR (p -> nNumber));
 BlockRectangle (p, _GBORDER, xLeft, yTop, nScaling);

 x1 = ((p -> Rect.xLeft + p -> Rect.xRight)  / 2 - xLeft) / nScaling;
 y1 = ((p -> Rect.yTop  + p -> Rect.yBottom) / 2 - yTop)  / nScaling;

 for (i = 0; i < p -> nChildren; i++)
 {
 q = p -> pChildren [i];

 x2 = ((q -> Rect.xLeft + q -> Rect.xRight)  / 2 - xLeft) / nScaling;
 y2 = ((q -> Rect.yTop  + q -> Rect.yBottom) / 2 - yTop)  / nScaling;
 _moveto (x1, y1);
 _lineto (x2, y2);
 }
 }

 _gettextsettings (&ts);
 _setcharsize (ts.height / 2, ts.width / 2);
 _settextalign (_LEFT, _BASE);
 _setcolor (15);

 for (p = pBlocksList; p != NULL; p = p -> pNext)
 {
 x1 = ((p -> Rect.xLeft + p -> Rect.xRight)  / 2 - xLeft) / nScaling;
 y1 = ((p -> Rect.yTop  + p -> Rect.yBottom) / 2 - yTop)  / nScaling;
 sprintf (szTextBuffer, "%d", p -> nParents);
 _grtext (x1, y1, szTextBuffer);
 }

 _setcharsize (ts.height, ts.width);
 }
 *************************/

static void BlocksOrderUpdate (int xLeft, int yTop, int nScaling)
{
    BLOCK *p;
    struct textsettings ts;
    int x, y;
    char szTextBuffer [128];
    LT_GraphicsClearScreen ();
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (p = pBlocksList; p != NULL; p = p -> pNext) {
        _setcolor (NUMBER_TO_COLOR (p -> nNumber));
        BlockRectangle (p, _GBORDER, xLeft, yTop, nScaling);

        if (p -> Type == BLOCK_TEXT || p -> Type == BLOCK_PICTURE) {
            x = ((p -> Rect.xLeft + p -> Rect.xRight) / 2 - xLeft) / nScaling;
            y = ((p -> Rect.yTop + p -> Rect.yBottom) / 2 - yTop) / nScaling;
            _setcolor (_COLOR_BACKGROUND);
            sprintf (szTextBuffer, "%d", p -> nNumber);

            if (p->nUserNum)
                sprintf(szTextBuffer, "%d/%d", p -> nNumber, p->nUserNum);

            _grtext (x, y, szTextBuffer);
        }
    }

    _setcharsize (ts.height, ts.width);
}

static void CurrentStringUpdate (int xLeft, int yTop, int nScaling)
{
    ROOT *pRoot;
    int i;
    unsigned old_style;
    LT_GraphicsClearScreen ();

    for (i = 0; i < String.nLetters; i++) {
        pRoot = pRoots + String.pLettersList [i];
        _setcolor ((pRoot -> bType & ROOT_SPECIAL_LETTER) ? _COLOR_BACKGROUND : _COLOR_GREEN);
        _rectangle (_GFILLINTERIOR,
                    (pRoot -> xColumn - xLeft) / nScaling,
                    (pRoot -> yRow - yTop) / nScaling,
                    (pRoot -> xColumn + pRoot ->nWidth - 1 - xLeft) / nScaling,
                    (pRoot -> yRow + pRoot ->nHeight - 1 - yTop) / nScaling);
    }

    _setcolor (_COLOR_GREEN);

    for (i = 0; i < String.nDust; i++) {
        pRoot = pRoots + String.pDustList [i];
        _rectangle (_GBORDER,
                    (pRoot -> xColumn - xLeft) / nScaling - 1,
                    (pRoot -> yRow - yTop) / nScaling - 1,
                    (pRoot -> xColumn + pRoot -> nWidth - 1 - xLeft) / nScaling + 1,
                    (pRoot -> yRow + pRoot -> nHeight - 1 - yTop) / nScaling + 1);
    }

    old_style = _getlinestyle ();
    _setlinestyle (0xcccc);
    _moveto ((String.xLeft - xLeft) / nScaling,
             (String.yMin - yTop) / nScaling);
    _lineto ((String.xRight - xLeft) / nScaling,
             (String.yMin - yTop) / nScaling);
    _moveto ((String.xLeft - xLeft) / nScaling,
             (String.yMax - yTop) / nScaling);
    _lineto ((String.xRight - xLeft) / nScaling,
             (String.yMax - yTop) / nScaling);
    _setlinestyle (old_style);
    _setcolor (_COLOR_RED);
    _rectangle (_GBORDER,
                (String.xLeft - xLeft) / nScaling,
                (String.yTop - yTop) / nScaling,
                (String.xRight - xLeft) / nScaling,
                (String.yBottom - yTop) / nScaling);
    _setcolor (_COLOR_BACKGROUND);
    _moveto ((String.xLeft - xLeft) / nScaling,
             (String.yMiddleTop - yTop) / nScaling);
    _lineto ((String.xRight - xLeft) / nScaling,
             (String.yMiddleTop - yTop) / nScaling);
    _moveto ((String.xLeft - xLeft) / nScaling,
             (String.yMiddleBottom - yTop) / nScaling);
    _lineto ((String.xRight - xLeft) / nScaling,
             (String.yMiddleBottom - yTop) / nScaling);
}

static void StringsUpdate (int xLeft, int yTop, int nScaling)
{
    STRING *pString;
    ROOT *pRoot;
    int nString;
    int nColor;
    int nDustShift;
    int i;
    unsigned old_style;
    LT_GraphicsClearScreen ();

    for (pString = pStringsUpList, nString = 0;
            pString != NULL;
            pString = pString -> pDown, nString++) {
        nColor = NUMBER_TO_COLOR (nString);
        nDustShift = 2 * (nString % 6 + 1);
        _setcolor (nColor + 8);
        _rectangle (_GBORDER,
                    (pString -> xLeft - xLeft) / nScaling,
                    (pString -> yTop - yTop) / nScaling,
                    (pString -> xRight - xLeft) / nScaling,
                    (pString -> yBottom - yTop) / nScaling);

        for (i = 0; i < pString -> nLetters; i++) {
            pRoot = pRoots + pString -> pLettersList [i];
            _setcolor ((pRoot -> bType & ROOT_SPECIAL_LETTER) ? _COLOR_BACKGROUND : nColor);
            _rectangle (_GFILLINTERIOR,
                        (pRoot -> xColumn - xLeft) / nScaling + nDustShift,
                        (pRoot -> yRow - yTop) / nScaling,
                        (pRoot -> xColumn + pRoot ->nWidth - 1
                         - xLeft) / nScaling + nDustShift,
                        (pRoot -> yRow + pRoot ->nHeight - 1
                         - yTop) / nScaling);
        }

        _setcolor (nColor + 8);

        for (i = 0; i < pString -> nDust; i++) {
            pRoot = pRoots + pString -> pDustList [i];

            if (pRoot -> bType & ROOT_USED)
                continue;

            _rectangle (_GBORDER,
                        (pRoot -> xColumn - xLeft) / nScaling + nDustShift,
                        (pRoot -> yRow - yTop) / nScaling,
                        (pRoot -> xColumn + pRoot ->nWidth - 1
                         - xLeft) / nScaling + nDustShift,
                        (pRoot -> yRow + pRoot ->nHeight - 1
                         - yTop) / nScaling);
        }

        _setcolor (_COLOR_BACKGROUND);

        for (i = 0; i < pString -> nDust; i++) {
            pRoot = pRoots + pString -> pDustList [i];

            if (pRoot -> bType & ROOT_USED)
                continue;

            _rectangle (_GBORDER,
                        (pRoot -> xColumn - xLeft) / nScaling,
                        (pRoot -> yRow - yTop) / nScaling,
                        (pRoot -> xColumn + pRoot ->nWidth - 1
                         - xLeft) / nScaling,
                        (pRoot -> yRow + pRoot ->nHeight - 1
                         - yTop) / nScaling);
        }

        _setcolor (nColor + 8);
        old_style = _getlinestyle ();
        _setlinestyle (0xcccc);
        _moveto ((pString -> xLeft - xLeft) / nScaling,
                 (pString -> yMin - yTop) / nScaling);
        _lineto ((pString -> xRight - xLeft) / nScaling,
                 (pString -> yMin - yTop) / nScaling);
        _moveto ((pString -> xLeft - xLeft) / nScaling,
                 (pString -> yMax - yTop) / nScaling);
        _lineto ((pString -> xRight - xLeft) / nScaling,
                 (pString -> yMax - yTop) / nScaling);
        _setlinestyle (old_style);
        _setcolor (_COLOR_RED);
        _rectangle (_GBORDER,
                    (pString -> xLeft - xLeft) / nScaling,
                    (pString -> yTop - yTop) / nScaling,
                    (pString -> xRight - xLeft) / nScaling,
                    (pString -> yBottom - yTop) / nScaling);
        _setcolor (_COLOR_BACKGROUND);
        _moveto ((pString -> xLeft - xLeft) / nScaling,
                 (pString -> yMiddleTop - yTop) / nScaling);
        _lineto ((pString -> xRight - xLeft) / nScaling,
                 (pString -> yMiddleTop - yTop) / nScaling);
        _moveto ((pString -> xLeft - xLeft) / nScaling,
                 (pString -> yMiddleBottom - yTop) / nScaling);
        _lineto ((pString -> xRight - xLeft) / nScaling,
                 (pString -> yMiddleBottom - yTop) / nScaling);
    }

    SeparatorsOutput (xLeft, yTop, nScaling);
}

static void StringsForwardOrderUpdate (int xLeft, int yTop, int nScaling)
{
    STRING *p;
    int nString;
    struct textsettings ts;
    int x, y;
    char szTextBuffer [128];
    LT_GraphicsClearScreen ();
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (nString = 1, p = pStringsList; p != NULL; p = p -> pNext, nString++) {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? _COLOR_BACKGROUND : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft - xLeft) / nScaling,
                    (p -> yTop - yTop) / nScaling,
                    (p -> xRight - xLeft) / nScaling,
                    (p -> yBottom - yTop) / nScaling);
        x = ((p -> xLeft + p -> xRight) / 2 - xLeft) / nScaling;
        y = ((p -> yTop + p -> yBottom) / 2 - yTop) / nScaling;
        _setcolor (_COLOR_BACKGROUND);
        sprintf (szTextBuffer, "%d", nString);
        _grtext (x, y, szTextBuffer);
    }

    _setcharsize (ts.height, ts.width);
}

static void StringsBackwardOrderUpdate (int xLeft, int yTop, int nScaling)
{
    STRING *p;
    int nString;
    struct textsettings ts;
    int x, y;
    char szTextBuffer [128];
    LT_GraphicsClearScreen ();
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (nString = 1, p = pStringsListEnd; p != NULL; p = p -> pPrev, nString++) {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? _COLOR_BACKGROUND : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft - xLeft) / nScaling,
                    (p -> yTop - yTop) / nScaling,
                    (p -> xRight - xLeft) / nScaling,
                    (p -> yBottom - yTop) / nScaling);
        x = ((p -> xLeft + p -> xRight) / 2 - xLeft) / nScaling;
        y = ((p -> yTop + p -> yBottom) / 2 - yTop) / nScaling;
        _setcolor (_COLOR_BACKGROUND);
        sprintf (szTextBuffer, "%d", nString);
        _grtext (x, y, szTextBuffer);
    }

    _setcharsize (ts.height, ts.width);
}

static void StringsUpOrderUpdate (int xLeft, int yTop, int nScaling)
{
    STRING *p;
    int nString;
    struct textsettings ts;
    int x, y;
    char szTextBuffer [128];
    LT_GraphicsClearScreen ();
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (nString = 1, p = pStringsUpList; p != NULL; p = p -> pDown, nString++) {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? _COLOR_BACKGROUND : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft - xLeft) / nScaling,
                    (p -> yTop - yTop) / nScaling,
                    (p -> xRight - xLeft) / nScaling,
                    (p -> yBottom - yTop) / nScaling);
        x = ((p -> xLeft + p -> xRight) / 2 - xLeft) / nScaling;
        y = ((p -> yTop + p -> yBottom) / 2 - yTop) / nScaling;
        _setcolor (_COLOR_BACKGROUND);
        sprintf (szTextBuffer, "%d", nString);
        _grtext (x, y, szTextBuffer);
    }

    _setcharsize (ts.height, ts.width);
}

static void StringsDownOrderUpdate (int xLeft, int yTop, int nScaling)
{
    STRING *p;
    int nString;
    struct textsettings ts;
    int x, y;
    char szTextBuffer [128];
    LT_GraphicsClearScreen ();
    _gettextsettings (&ts);
    _setcharsize (ts.height / 2, ts.width / 2);
    _settextalign (_CENTER, _HALF);

    for (nString = 1, p = pStringsDownList; p != NULL; p = p -> pUp, nString++) {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? _COLOR_BACKGROUND : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft - xLeft) / nScaling,
                    (p -> yTop - yTop) / nScaling,
                    (p -> xRight - xLeft) / nScaling,
                    (p -> yBottom - yTop) / nScaling);
        x = ((p -> xLeft + p -> xRight) / 2 - xLeft) / nScaling;
        y = ((p -> yTop + p -> yBottom) / 2 - yTop) / nScaling;
        _setcolor (_COLOR_BACKGROUND);
        sprintf (szTextBuffer, "%d", nString);
        _grtext (x, y, szTextBuffer);
    }

    _setcharsize (ts.height, ts.width);
}

static void RasterUpdate (int xLeft, int yTop, int nScaling)
{
    int x, y;
    int nStep;
    LT_GraphicsClearScreen ();
    nStep = MIN (FIELD_WIDTH / nRasterWidth, FIELD_HEIGHT / nRasterHeight);

    if (nStep == 0) nStep = 1;

    _setcolor (_COLOR_BACKGROUND);
    _rectangle (_GFILLINTERIOR,
                (0 - xLeft) / nScaling,
                (0 - yTop) / nScaling,
                (nRasterWidth * nStep - xLeft) / nScaling,
                (nRasterHeight * nStep - yTop) / nScaling);
    _setcolor (_COLOR_BLACK);

    for (x = 0; x < nRasterWidth; x++) {
        for (y = 0; y < nRasterHeight; y++) {
            if ((pRaster [y * nRasterByteWidth + x / 8] << x % 8) & 0x80) {
                _rectangle (_GFILLINTERIOR,
                            (x * nStep - xLeft) / nScaling,
                            (y * nStep - yTop) / nScaling,
                            ((x + 1) * nStep - xLeft) / nScaling,
                            ((y + 1) * nStep - yTop) / nScaling);
            }
        }
    }

    _setcolor (_COLOR_GRAY);

    for (x = 0; x <= nRasterWidth; x++) {
        _moveto ((x * nStep - xLeft) / nScaling,
                 (0 - yTop) / nScaling);
        _lineto ((x * nStep - xLeft) / nScaling,
                 (nRasterHeight * nStep - yTop) / nScaling);
    }

    for (y = 0; y <= nRasterHeight; y++) {
        _moveto ((0 - xLeft) / nScaling,
                 (y * nStep - yTop) / nScaling);
        _lineto ((nRasterWidth * nStep - xLeft) / nScaling,
                 (y * nStep - yTop) / nScaling);
    }
}

static void BreakingUpdate (int xLeft, int yTop, int nScaling)
{
    int x, y;
    int nStep;
    LT_GraphicsClearScreen ();
    nStep = MIN (FIELD_WIDTH / nRasterWidth, FIELD_HEIGHT / nRasterHeight);

    if (nStep == 0)
        nStep = 1;

    _setcolor (_COLOR_BACKGROUND);
    _rectangle (_GFILLINTERIOR,
                (0 - xLeft) / nScaling,
                (0 - yTop) / nScaling,
                (nRasterWidth * nStep - xLeft) / nScaling,
                (nRasterHeight * nStep - yTop) / nScaling);

    for (x = 0; x < nRasterWidth; x++) {
        for (y = 0; y < nRasterHeight; y++) {
            if ((pRaster [y * nRasterByteWidth + x / 8] << x % 8) & 0x80)
                _setcolor (_COLOR_BLACK);

            else if (y == yRasterBreakLine)
                _setcolor (_COLOR_RED);

            else if (y >= yRasterUpBreakLimit && y <= yRasterDownBreakLimit)
                _setcolor (_COLOR_YELLOW);

            else
                continue;

            _rectangle (_GFILLINTERIOR,
                        (x * nStep - xLeft) / nScaling,
                        (y * nStep - yTop) / nScaling,
                        ((x + 1) * nStep - xLeft) / nScaling,
                        ((y + 1) * nStep - yTop) / nScaling);
        }
    }

    _setcolor (_COLOR_GRAY);

    for (x = 0; x <= nRasterWidth; x++) {
        _moveto ((x * nStep - xLeft) / nScaling,
                 (0 - yTop) / nScaling);
        _lineto ((x * nStep - xLeft) / nScaling,
                 (nRasterHeight * nStep - yTop) / nScaling);
    }

    for (y = 0; y <= nRasterHeight; y++) {
        _moveto ((0 - xLeft) / nScaling,
                 (y * nStep - yTop) / nScaling);
        _lineto ((nRasterWidth * nStep - xLeft) / nScaling,
                 (y * nStep - yTop) / nScaling);
    }

    _setcolor (_COLOR_BLUE);

    for (y = 0; y <= nRasterHeight; y++) {
        _rectangle (_GFILLINTERIOR,
                    ((nRasterWidth + 1) * nStep - xLeft) / nScaling,
                    (y * nStep - yTop) / nScaling,
                    ((aRasterHystogram [y] + nRasterWidth + 1)
                     * nStep - xLeft) / nScaling,
                    ((y + 1) * nStep - yTop) / nScaling);
    }
}

void LT_GraphicsTitle (char *pTitle)
{
    //    _setcolor (_COLOR_BACKGROUND);
    _setcolor (_COLOR_TEXT);
    _settextalign (_CENTER, _HALF); // _HALF("_HALPH")==_CENTER
    _grtext (320, 450, pTitle);
}

static int yCurrentTop = 0;
static int xCurrentLeft = 0;
static int nCurrentScaling = MAX_SCALING;
//так как MAX_SCALING, как и MIN_SCALING, ==1,
// можно смело считать, что nCurrentScaling==1

static void ScreenOutput (const char *pTitle,
                          void (*pProcUpdate) (int xLeft, int yTop, int nScaling))
{
    int nWidth = 3000;
    int nHeight = 3000;
    Bool bNeedUpdateScreen = TRUE;

    for (;;) {
        if (bNeedUpdateScreen) {
            (*pProcUpdate) (xCurrentLeft, yCurrentTop, nCurrentScaling);
            LT_GraphicsTitle (pTitle);
        }

        do {
            switch (LT_Getch ()) {
                case ' ':
                    return;
                case KEY_LEFT:

                    if (xCurrentLeft > 0) {
                        xCurrentLeft -= SCREEN_WIDTH / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }

                    break;
                case KEY_RIGHT:
                    /*
                     if (xCurrentLeft + SCREEN_WIDTH / STEPS * nCurrentScaling < nWidth)
                     */

                    if (xCurrentLeft < nWidth) {
                        xCurrentLeft += SCREEN_WIDTH / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }

                    break;
                case KEY_UP:

                    if (yCurrentTop > 0) {
                        yCurrentTop -= SCREEN_HEIGHT / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }

                    break;
                case KEY_DOWN:
                    /*
                     if (yCurrentTop + SCREEN_HEIGHT / STEPS * nCurrentScaling < nHeight)
                     */

                    if (yCurrentTop < nHeight) {
                        yCurrentTop += SCREEN_HEIGHT / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }

                    break;
                case KEY_F1:

                    if (nCurrentScaling != MIN_SCALING) {
                        //                      yCurrentTop      = 0;
                        //                      xCurrentLeft     = 0;
                        nCurrentScaling /= 2;
                        bNeedUpdateScreen = TRUE;
                    }

                    break;
                case KEY_F2:

                    if (nCurrentScaling != MAX_SCALING) {
                        //                      yCurrentTop      = 0;
                        //                      xCurrentLeft     = 0;
                        nCurrentScaling *= 2;
                        bNeedUpdateScreen = TRUE;
                    }

                    break;
                case KEY_F10:
# ifdef LT_DEBUG
                    LT_DebugGraphicsLevel = 0;
# endif
# ifdef SE_DEBUG
                    SE_DebugGraphicsLevel = 0;
# endif
                    return;
                default:
                    bNeedUpdateScreen = FALSE;
                    break;
            }
        }
        while (!bNeedUpdateScreen);
    }
}

void LT_GraphicsRootsOutput (const char *pTitle)
{
    ScreenOutput (pTitle, RootsUpdate);
}

void LT_GraphicsRootStripsOutput (char *pTitle)
{
    ScreenOutput (pTitle, RootStripsUpdate);
}

void LT_GraphicsPageMatrixOutput (char *pTitle)
{
    ScreenOutput (pTitle, PageMatrixUpdate);
}

void LT_GraphicsBlocksOutput (const char *pTitle)
{
    BlocksSetRootsNumbers ();
    ScreenOutput (pTitle, BlocksUpdate);
    //  if (pBlocksList != NULL)
    //      BlocksSetRandomRootsNumbers ();
}

void LT_GraphicsHystogramOutput (const char *pTitle)
{
    ScreenOutput (pTitle, HystogramUpdate);
}

void LT_GraphicsSpecialCuttingOutput (char *pTitle)
{
    ScreenOutput (pTitle, SpecialCuttingUpdate);
}

void LT_GraphicsSB_MatrixOutput (char *pTitle)
{
    ScreenOutput (pTitle, SB_MatrixUpdate);
}

void LT_GraphicsWSB_Hystogram_1_Output (char *pTitle)
{
    ScreenOutput (pTitle, WSB_Hystogram_1_Update);
}

void LT_GraphicsWSB_Hystogram_2_Output (char *pTitle)
{
    ScreenOutput (pTitle, WSB_Hystogram_2_Update);
}

void LT_GraphicsWSB_PointsOutput (char *pTitle)
{
    BlocksSetRootsNumbers ();
    ScreenOutput (pTitle, WSB_PointsUpdate);
}

void LT_GraphicsPictureRemovingConditionsOutput (char * pTitle)
{
    ScreenOutput (pTitle, PictureRemovingConditionsUpdate);
}

void LT_GraphicsDD_RemovingConditionsOutput (char * pTitle)
{
    ScreenOutput (pTitle, DD_RemovingConditionsUpdate);
}

void LT_GraphicsHighEmbeddingBlocksListOutput (char *pTitle)
{
    BLOCK Dummy;
    pBlocksListBegin = pHighEmbeddingBlocksList;
    oBlockNext = (uchar *) & Dummy.pLowerEmbedding - (uchar *) & Dummy;
    ScreenOutput (pTitle, BlocksListUpdate);
}

void LT_GraphicsLowEmbeddingBlocksListOutput (char *pTitle)
{
    BLOCK Dummy;
    pBlocksListBegin = pLowEmbeddingBlocksList;
    oBlockNext = (uchar *) & Dummy.pHigherEmbedding - (uchar *) & Dummy;
    ScreenOutput (pTitle, BlocksListUpdate);
}

void LT_GraphicsLeftBlocksListOutput (char *pTitle)
{
    BLOCK Dummy;
    pBlocksListBegin = pLeftBlocksList;
    oBlockNext = (uchar *) & Dummy.pRight - (uchar *) & Dummy;
    ScreenOutput (pTitle, BlocksListUpdate);
}

void LT_GraphicsRightBlocksListOutput (char *pTitle)
{
    BLOCK Dummy;
    pBlocksListBegin = pRightBlocksList;
    oBlockNext = (uchar *) & Dummy.pLeft - (uchar *) & Dummy;
    ScreenOutput (pTitle, BlocksListUpdate);
}

void LT_GraphicsTopBlocksListOutput (char *pTitle)
{
    BLOCK Dummy;
    pBlocksListBegin = pTopBlocksList;
    oBlockNext = (uchar *) & Dummy.pDown - (uchar *) & Dummy;
    ScreenOutput (pTitle, BlocksListUpdate);
}

void LT_GraphicsBottomBlocksListOutput (char *pTitle)
{
    BLOCK Dummy;
    pBlocksListBegin = pBottomBlocksList;
    oBlockNext = (uchar *) & Dummy.pUp - (uchar *) & Dummy;
    ScreenOutput (pTitle, BlocksListUpdate);
}

/********** ATAL 940414
 void LT_GraphicsTreeOutput (char *pTitle)
 {
 ScreenOutput (pTitle, TreeUpdate);
 }
 ****************/

void LT_GraphicsBlocksOrderOutput (char *pTitle)
{
    ScreenOutput (pTitle, BlocksOrderUpdate);
}

void LT_GraphicsCurrentStringOutput (const char *pTitle)
{
    ScreenOutput (pTitle, CurrentStringUpdate);
}

void LT_GraphicsStringsOutput (const char *pTitle)
{
    ScreenOutput (pTitle, StringsUpdate);
}

void LT_GraphicsStringsForwardOrderOutput (const char *pTitle)
{
    ScreenOutput (pTitle, StringsForwardOrderUpdate);
}

void LT_GraphicsStringsBackwardOrderOutput (const char *pTitle)
{
    ScreenOutput (pTitle, StringsBackwardOrderUpdate);
}

void LT_GraphicsStringsUpOrderOutput (const char *pTitle)
{
    ScreenOutput (pTitle, StringsUpOrderUpdate);
}

void LT_GraphicsStringsDownOrderOutput (const char *pTitle)
{
    ScreenOutput (pTitle, StringsDownOrderUpdate);
}

void LT_GraphicsRasterOutput (const char *pTitle)
{
    ScreenOutput (pTitle, RasterUpdate);
}

void LT_GraphicsBreakingOutput (const char *pTitle)
{
    ScreenOutput (pTitle, BreakingUpdate);
}

void LT_GraphicsBlockOutput2 (char *pTitle)
{
    ROOT *pRoot;
    pTitle = pTitle;
    _setcolor (NUMBER_TO_COLOR (pDebugBlock -> nNumber) | 8);
    BlockRectangle (pDebugBlock, _GBORDER,
                    xCurrentLeft, yCurrentTop, nCurrentScaling);

    for (pRoot = pDebugBlock -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext) {
        _rectangle (_GFILLINTERIOR,
                    (pRoot -> xColumn
                     - xCurrentLeft) / nCurrentScaling,
                    (pRoot -> yRow
                     - yCurrentTop) / nCurrentScaling,
                    (pRoot -> xColumn + pRoot -> nWidth - 1
                     - xCurrentLeft) / nCurrentScaling,
                    (pRoot -> yRow + pRoot -> nHeight - 1
                     - yCurrentTop) / nCurrentScaling);
    }

    _setcolor (_COLOR_RED);
    _moveto ((xDebugVertLine - xCurrentLeft) / nCurrentScaling, 0);
    _lineto ((xDebugVertLine - xCurrentLeft) / nCurrentScaling, SCREEN_HEIGHT);
    LT_GraphicsTitle (pTitle);
}

void LT_GraphicsPictureRemovingConditionsOutput2 (char *pTitle)
{
    PictureRemovingConditionsUpdate (0, 0, MAX_SCALING);
    LT_GraphicsTitle (pTitle);
}
void LT_ShowBlock (char *pTitle)
{
    ShowBlock(0, 0, MAX_SCALING);
    LT_GraphicsTitle (pTitle);
}

void LT_GraphicsDD_RemovingConditionsOutput2 (char *pTitle)
{
    DD_RemovingConditionsUpdate (0, 0, MAX_SCALING);
    LT_GraphicsTitle (pTitle);
}

void LT_GraphicsLinearRemovingConditionsOutput2 (char *pTitle)
{
    int xLeft, yTop, nScaling;
    char szTextBuffer [32];
    BLOCK *p = pDebugBlock;
    xLeft = xCurrentLeft;
    yTop = yCurrentTop;
    nScaling = nCurrentScaling;
    // единственное место, где xCurrentLeft и yCurrentTop
    // имеют шанс отличиться от нуля...
    xCurrentLeft = p -> Rect.xLeft - 1;
    yCurrentTop = p -> Rect.yTop - 1;
    nCurrentScaling = MIN_SCALING;
    LT_GraphicsClearScreen ();
    LT_GraphicsBlockOutput2 (pTitle);
    xCurrentLeft = xLeft;
    yCurrentTop = yTop;
    nCurrentScaling = nScaling;
    sprintf (szTextBuffer,
             "%d+%d+%d=%d "
             "W %d H %d H/W %5.2f W/H %5.2f\n",
             p -> nLetters,
             p -> nRoots - p -> nLetters - p -> nDust,
             p -> nDust,
             p -> nRoots,
             p -> Rect.xRight - p -> Rect.xLeft + 1,
             p -> Rect.yBottom - p -> Rect.yTop + 1,
             (double) (p -> Rect.yBottom - p -> Rect.yTop + 1) /
             (p -> Rect.xRight - p -> Rect.xLeft + 1),
             (double) (p -> Rect.xRight - p -> Rect.xLeft + 1) /
             (p -> Rect.yBottom - p -> Rect.yTop + 1)
            );
    LT_GraphicsTitle (szTextBuffer);
}
# endif

// Pit 11-21-94 04:56pm
#include "edp.h"
int16_t maxx, minx, maxy, miny;
int16_t scale;

static void part_minmax(DP * part)
{
    DP * p;

    for (p = part->first; p; p++) {
        part_minmax(p);

        if (p == part->last)
            break;
    }

    for (p = part; p; p = p->next) {
        if (maxx < p->x + p->w)
            maxx = p->x + p->w;

        if (minx > p->x)
            minx = p->x;

        if (maxy < p->y + p->h)
            maxy = p->y + p->h;

        if (miny > p->y)
            miny = p->y;
    }
}

static void part_graph(DP * part, int16_t level)
{
    DP * p;

    for (p = part->first; p; p++) {
        part_graph(p, level + 1);
        _setcolor((p - part->first) % 8 + 7);

        if (p->type & HOR) {
            if (p->parent && p->parent->type & (VERT | TERM) && p->parent->y
                    && p->parent->h) {
                _moveto((p->x - minx) / scale, p->parent->y / scale);
                _lineto((p->x - minx) / scale, (p->parent->y + p->parent->h)
                        / scale);
                _moveto((p->x - minx + p->w) / scale, p->parent->y / scale);
                _lineto((p->x - minx + p->w) / scale, (p->parent->y
                                                       + p->parent->h) / scale);
            }

            else {
                _moveto((p->x - minx) / scale, 0);
                _lineto((p->x - minx) / scale, (maxy - miny) / scale);
                _moveto((p->x - minx + p->w) / scale, 0);
                _lineto((p->x - minx + p->w) / scale, (maxy - miny) / scale);
            }
        }

        else if (p->type & (VERT | TERM)) {
            if (p->parent && p->parent->type & (HOR) && p->parent->x
                    && p->parent->w) {
                _moveto(p->parent->x / scale, (p->y - miny) / scale);
                _lineto((p->parent->x + p->parent->w) / scale, (p->y - miny)
                        / scale);
                _moveto(p->parent->x / scale, (p->y - miny + p->h) / scale);
                _lineto((p->parent->x + p->parent->w) / scale, (p->y - miny
                                                                + p->h) / scale);
            }

            else {
                _moveto(0, (p->y - miny) / scale);
                _lineto((maxx - minx) / scale, (p->y - miny) / scale);
                _moveto(0, (p->y - miny + p->h) / scale);
                _lineto((maxx - minx) / scale, (p->y - miny + p->h) / scale);
            }
        }

        if (p == part->last)
            break;
    }
}

