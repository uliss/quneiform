/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

пЮГПЕЬЮЕРЯЪ ОНБРНПМНЕ ПЮЯОПНЯРПЮМЕМХЕ Х ХЯОНКЭГНБЮМХЕ ЙЮЙ Б БХДЕ ХЯУНДМНЦН ЙНДЮ,
РЮЙ Х Б ДБНХВМНИ ТНПЛЕ, Я ХГЛЕМЕМХЪЛХ ХКХ АЕГ, ОПХ ЯНАКЧДЕМХХ ЯКЕДСЧЫХУ СЯКНБХИ:

      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ХЯУНДМНЦН ЙНДЮ ДНКФМШ НЯРЮБЮРЭЯЪ СЙЮГЮММНЕ
        БШЬЕ СБЕДНЛКЕМХЕ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х ОНЯКЕДСЧЫХИ
        НРЙЮГ НР ЦЮПЮМРХИ.
      * оПХ ОНБРНПМНЛ ПЮЯОПНЯРПЮМЕМХХ ДБНХВМНЦН ЙНДЮ Б ДНЙСЛЕМРЮЖХХ Х/ХКХ Б
        ДПСЦХУ ЛЮРЕПХЮКЮУ, ОНЯРЮБКЪЕЛШУ ОПХ ПЮЯОПНЯРПЮМЕМХХ, ДНКФМШ ЯНУПЮМЪРЭЯЪ
        СЙЮГЮММЮЪ БШЬЕ ХМТНПЛЮЖХЪ НА ЮБРНПЯЙНЛ ОПЮБЕ, ЩРНР ЯОХЯНЙ СЯКНБХИ Х
        ОНЯКЕДСЧЫХИ НРЙЮГ НР ЦЮПЮМРХИ.
      * мХ МЮГБЮМХЕ Cognitive Technologies, МХ ХЛЕМЮ ЕЕ ЯНРПСДМХЙНБ МЕ ЛНЦСР
        АШРЭ ХЯОНКЭГНБЮМШ Б ЙЮВЕЯРБЕ ЯПЕДЯРБЮ ОНДДЕПФЙХ Х/ХКХ ОПНДБХФЕМХЪ
        ОПНДСЙРНБ, НЯМНБЮММШУ МЮ ЩРНЛ он, АЕГ ОПЕДБЮПХРЕКЭМНЦН ОХЯЭЛЕММНЦН
        ПЮГПЕЬЕМХЪ.

щрю опнцпюллю опеднярюбкемю бкюдекэжюлх юбрнпяйху опюб х/хкх дпсцхлх кхжюлх "йюй
нмю еярэ" аег йюйнцн-кхан бхдю цюпюмрхи, бшпюфеммшу ъбмн хкх ондпюгслебюелшу,
бйкчвюъ цюпюмрхх йнллепвеяйни жеммнярх х опхцндмнярх дкъ йнмйпермни жекх, мн ме
нцпюмхвхбюъяэ хлх. мх бкюдекеж юбрнпяйху опюб х мх ндмн дпсцне кхжн, йнрнпне
лнфер хглемърэ х/хкх онбрнпмн пюяопнярпюмърэ опнцпюллс, мх б йнел яксвюе ме
мея╗р нрберярбеммнярх, бйкчвюъ кчаше наыхе, яксвюимше, яоежхюкэмше хкх
онякеднбюбьхе сашрйх, ябъгюммше я хяонкэгнбюмхел хкх онмеяеммше бякедярбхе
мебнглнфмнярх хяонкэгнбюмхъ опнцпюллш (бйкчвюъ онрепх дюммшу, хкх дюммше,
ярюбьхе мецндмшлх, хкх сашрйх х/хкх онрепх днунднб, онмеяеммше хг-гю деиярбхи
рперэху кхж х/хкх нрйюгю опнцпюллш пюанрюрэ янблеярмн я дпсцхлх опнцпюллюлх,
мн ме нцпюмхвхбюъяэ щрхлх яксвюълх), мн ме нцпюмхвхбюъяэ хлх, дюфе еякх рюйни
бкюдекеж хкх дпсцне кхжн ашкх хгбеыемш н бнглнфмнярх рюйху сашрйнб х онрепэ.

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

/*
  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
  шш                                                              шш
  шш     Copyright (C) 1990 Cognitive Technology Corporation.     шш
  шш	 All rights reserved. This program is proprietary and     шш
  шш     a trade secret of Cognitive Technology Corporation.      шш
  шш                                                              шш
  шшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшшш
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

/*# include <conio.h>*/
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
/*# include <dos.h>*/

# include "layout.h"
# include "extract.h"

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

# define NUMBER_TO_COLOR(n) ((n) % 6 + 1)

BLOCK   * pDebugBlock    = NULL;
RECTANGLE rDebugRect;
int       xDebugVertLine = -1;

static Bool bGraphicsModeWasChanged = FALSE;

void LT_GraphicsOpen (void)
{
    if (! bGraphicsModeWasChanged)
    {
        bGraphicsModeWasChanged = TRUE;
        _setvideomode (_VRES16COLOR);
    }
}

void LT_GraphicsClose (void)
{
    if (bGraphicsModeWasChanged)
    {
        _setvideomode (_DEFAULTMODE);
        bGraphicsModeWasChanged = FALSE;
    }
}

void LT_GraphicsClearScreen (void)
{
    _clearscreen (_GCLEARSCREEN);
}

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

static void BlockRectangle (BLOCK *p, short fill,
                            int xLeft, int yTop, int nScaling)
{
    switch (p -> Type)
    {
        case BLOCK_NULL:
            return;

        case BLOCK_PICTURE:
        case BLOCK_TEXT:
            _rectangle (fill,
                (p -> Rect.xLeft   - xLeft) / nScaling,
                (p -> Rect.yTop    - yTop)  / nScaling,
                (p -> Rect.xRight  - xLeft) / nScaling,
                (p -> Rect.yBottom - yTop)  / nScaling);
            break;

        case BLOCK_HORZ_SEPARATOR:
        case BLOCK_VERT_SEPARATOR:
            _setcolor (15);
            _rectangle (_GBORDER,
                (p -> Rect.xLeft   - xLeft) / nScaling,
                (p -> Rect.yTop    - yTop)  / nScaling,
                (p -> Rect.xRight  - xLeft) / nScaling,
                (p -> Rect.yBottom - yTop)  / nScaling);
            break;

        case BLOCK_RECT_SEPARATOR:
            _setcolor (14);
            _rectangle (_GBORDER,
                (p -> Rect.xLeft   - xLeft) / nScaling + 3,
                (p -> Rect.yTop    - yTop)  / nScaling + 3,
                (p -> Rect.xRight  - xLeft) / nScaling - 3,
                (p -> Rect.yBottom - yTop)  / nScaling - 3);
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

    switch (OutputMode)
    {
        case SEPOUT_LINE:   break;
        case SEPOUT_DASH_1: _setlinestyle (0xf0f0); break;
        case SEPOUT_DASH_2: _setlinestyle (0x0f0f); break;
    }

    switch (pSep -> Type)
    {
        default:
            break;

        case SEP_VERT:
        case SEP_HORZ:
            _moveto ((pSep -> xBegin - xLeft) / nScaling,
                     (pSep -> yBegin - yTop)  / nScaling);
            _lineto ((pSep -> xEnd   - xLeft) / nScaling,
                     (pSep -> yEnd   - yTop)  / nScaling);
            break;

        case SEP_RECT:
            _rectangle (_GBORDER,
                        (pSep -> xBegin - xLeft) / nScaling + 3,
                        (pSep -> yBegin - yTop)  / nScaling + 3,
                        (pSep -> xEnd   - xLeft) / nScaling - 3,
                        (pSep -> yEnd   - yTop)  / nScaling - 3);
            break;
    }

    _setlinestyle (0xffff);
}

static void SeparatorsOutput (int xLeft, int yTop, int nScaling)
{

}

static void RootsUpdate (int xLeft, int yTop, int nScaling)
{
    ROOT *pRoot;

    LT_GraphicsClearScreen ();

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        int nColor = 8;

        if (pRoot -> bType & ROOT_LETTER)       nColor |= 1;
        if (pRoot -> bType & ROOT_RECOGNIZED)   nColor |= 2;
        if (IS_LAYOUT_DUST (*pRoot))            nColor |= 4;

        if (pRoot -> bType & ROOT_DEBUG)        nColor = 14;

        _setcolor (nColor);

        _rectangle (_GFILLINTERIOR,
            (pRoot -> xColumn - xLeft) / nScaling,
            (pRoot -> yRow    - yTop)  / nScaling,
            (pRoot -> xColumn + pRoot ->nWidth  - 1 - xLeft) / nScaling,
            (pRoot -> yRow    + pRoot ->nHeight - 1 - yTop)  / nScaling);
    }

    SeparatorsOutput (xLeft, yTop, nScaling);
}

static void BlocksUpdate (int xLeft, int yTop, int nScaling)
{
    BLOCK *p;
    ROOT *pRoot;
    int  nColor;

    LT_GraphicsClearScreen ();

    for (p = pBlocksList; p != NULL; p = p -> pNext)
    {
        nColor = NUMBER_TO_COLOR (p -> nNumber) |
                 (((p -> uFlags & BF_NOT_BREAK_BLOCK) != 0) << 3);

        _setcolor (nColor);
        BlockRectangle (p, _GBORDER, xLeft, yTop, nScaling);

        SeparatorOutput (p -> pUpSep,    nColor, SEPOUT_DASH_1, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pDownSep,  nColor, SEPOUT_DASH_2, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pLeftSep,  nColor, SEPOUT_DASH_1, xLeft, yTop, nScaling);
        SeparatorOutput (p -> pRightSep, nColor, SEPOUT_DASH_2, xLeft, yTop, nScaling);
    }

    for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
        if (pRoot -> nBlock == REMOVED_BLOCK_NUMBER)
        {

        }
        else
        {
            if (pRoot -> nBlock == DUST_BLOCK_NUMBER)
                _setcolor (15);
            else
                _setcolor (NUMBER_TO_COLOR (pRoot -> nBlock));

            _rectangle (_GFILLINTERIOR,
                (pRoot -> xColumn - xLeft) / nScaling,
                (pRoot -> yRow    - yTop)  / nScaling,
                (pRoot -> xColumn + pRoot ->nWidth  - 1
                                        - xLeft) / nScaling,
                (pRoot -> yRow    + pRoot ->nHeight - 1
                                        - yTop)  / nScaling);
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

    nMaxValue  = 0;
    iMaxColumn = 0;

	/**/
	for (pRoot = pRoots; pRoot < pAfterRoots; pRoot++)
    {
		if(pRoot->nBlock == pDebugBlock->nNumber)
		{
			int nColor = 8;

			if (pRoot -> bType & ROOT_LETTER)       nColor |= 1;
			if (pRoot -> bType & ROOT_RECOGNIZED)   nColor |= 2;
			if (IS_LAYOUT_DUST (*pRoot))            nColor |= 4;

			if (pRoot -> bType & ROOT_DEBUG)        nColor = 14;

			_setcolor (nColor);

			_rectangle (_GFILLINTERIOR,
				(pRoot -> xColumn - xLeft) / nScaling,
				(pRoot -> yRow    - yTop)  / nScaling,
				(pRoot -> xColumn + pRoot ->nWidth  - 1 - xLeft) / nScaling,
				(pRoot -> yRow    + pRoot ->nHeight - 1 - yTop)  / nScaling);
		}
    }
	/**/

    for (i = 0; i < nHystColumns; i++)
    {
        if (nMaxValue < pHystogram [i])
        {
            nMaxValue  = pHystogram [i];
            iMaxColumn = i;
        }
    }

    if (nMaxValue < 10)
        nMaxValue = 10;

    _gettextsettings (&ts);

    for (i = 0; i < nHystColumns; i++)
    {
        if (pHystogram [i] == 0)
        {

        }
        else
        {
            _setcolor (9);

            _rectangle (_GFILLINTERIOR,
                (i * VIEW_WIDTH / nHystColumns + VIEW_LEFT
                                                     - xLeft) / nScaling,
                (VIEW_BOTTOM - pHystogram [i] * VIEW_HEIGHT / nMaxValue
                                                     - yTop)  / nScaling,
                ((i + 1) * VIEW_WIDTH / nHystColumns + VIEW_LEFT
                                                     - xLeft) / nScaling,
                (VIEW_BOTTOM
                                                     - yTop)  / nScaling);
        }
    }

    _setcolor (12);

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

    _setcolor (15);
    _settextalign (_RIGHT, _TOP);
    nCharHeight = VIEW_HEIGHT / 10 / nScaling / 2;
    _setcharsize (nCharHeight, nCharHeight / 2);

    for (i = 0; i < nMaxValue - nMaxValue / 20; i += MAX (nMaxValue / 10, 1))
    {
        _moveto ((VIEW_LEFT                                 - xLeft)/ nScaling,
                 (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling);
        _lineto ((VIEW_RIGHT                                - xLeft)/ nScaling,
                 (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling);

        if (i != 0)
        {
            sprintf (szBuffer, "%d\n", i);
            _grtext ((VIEW_LEFT - xLeft) / nScaling,
                (VIEW_BOTTOM - i * VIEW_HEIGHT / nMaxValue - yTop) / nScaling,
                 szBuffer);
        }
    }

    sprintf (szBuffer, "%d\n", nMaxValue);
    _grtext ((VIEW_LEFT - xLeft) / nScaling,
             (VIEW_TOP  - yTop)  / nScaling,
             szBuffer);

    _rectangle (_GBORDER,
                (VIEW_LEFT   - xLeft) / nScaling,
                (VIEW_TOP    - yTop)  / nScaling,
                (VIEW_RIGHT  - xLeft) / nScaling,
                (VIEW_BOTTOM - yTop)  / nScaling);

    _setcharsize (ts.height, ts.width);
}

static void CurrentStringUpdate (int xLeft, int yTop, int nScaling)
{
    ROOT *pRoot;
    int i;
    unsigned old_style;

    LT_GraphicsClearScreen ();

    for (i = 0; i < String.nLetters; i++)
    {
        pRoot = pRoots + String.pLettersList [i];

        _setcolor ((pRoot -> bType & ROOT_SPECIAL_LETTER) ? 15 : 10);
        _rectangle (_GFILLINTERIOR,
            (pRoot -> xColumn                       - xLeft) / nScaling,
            (pRoot -> yRow                          - yTop)  / nScaling,
            (pRoot -> xColumn + pRoot ->nWidth  - 1 - xLeft) / nScaling,
            (pRoot -> yRow    + pRoot ->nHeight - 1 - yTop)  / nScaling);
    }

    _setcolor (10);

    for (i = 0; i < String.nDust; i++)
    {
        pRoot = pRoots + String.pDustList [i];

        _rectangle (_GBORDER,
            (pRoot -> xColumn                        - xLeft) / nScaling - 1,
            (pRoot -> yRow                           - yTop)  / nScaling - 1,
            (pRoot -> xColumn + pRoot -> nWidth  - 1 - xLeft) / nScaling + 1,
            (pRoot -> yRow    + pRoot -> nHeight - 1 - yTop)  / nScaling + 1);
    }

    old_style = _getlinestyle ();
    _setlinestyle (0xcccc);

    _moveto ((String.xLeft  - xLeft) / nScaling,
             (String.yMin   - yTop)  / nScaling);
    _lineto ((String.xRight - xLeft) / nScaling,
             (String.yMin   - yTop)  / nScaling);

    _moveto ((String.xLeft  - xLeft) / nScaling,
             (String.yMax   - yTop)  / nScaling);
    _lineto ((String.xRight - xLeft) / nScaling,
             (String.yMax   - yTop)  / nScaling);
    _setlinestyle (old_style);

    _setcolor (12);
    _rectangle (_GBORDER,
                (String.xLeft   - xLeft) / nScaling,
                (String.yTop    - yTop)  / nScaling,
                (String.xRight  - xLeft) / nScaling,
                (String.yBottom - yTop)  / nScaling);

    _setcolor (15);
    _moveto ((String.xLeft         - xLeft) / nScaling,
             (String.yMiddleTop    - yTop)  / nScaling);
    _lineto ((String.xRight        - xLeft) / nScaling,
             (String.yMiddleTop    - yTop)  / nScaling);

    _moveto ((String.xLeft         - xLeft) / nScaling,
             (String.yMiddleBottom - yTop)  / nScaling);
    _lineto ((String.xRight        - xLeft) / nScaling,
             (String.yMiddleBottom - yTop)  / nScaling);
}

static void StringsUpdate (int xLeft, int yTop, int nScaling)
{
    STRING *pString;
    ROOT        *pRoot;
    int nString;
    int nColor;
    int nDustShift;
    int i;
    unsigned old_style;

    LT_GraphicsClearScreen ();

    for (pString = pStringsUpList, nString = 0;
             pString != NULL;
                 pString = pString -> pDown, nString++)
    {
        nColor     = NUMBER_TO_COLOR (nString);
        nDustShift = 2 * (nString % 6 + 1);

        _setcolor (nColor + 8);
        _rectangle (_GBORDER,
                    (pString -> xLeft   - xLeft) / nScaling,
                    (pString -> yTop    - yTop)  / nScaling,
                    (pString -> xRight  - xLeft) / nScaling,
                    (pString -> yBottom - yTop)  / nScaling);

        for (i = 0; i < pString -> nLetters; i++)
        {
            pRoot = pRoots + pString -> pLettersList [i];
            _setcolor ((pRoot -> bType & ROOT_SPECIAL_LETTER) ? 15 : nColor);
            _rectangle (_GFILLINTERIOR,
                (pRoot -> xColumn - xLeft) / nScaling + nDustShift,
                (pRoot -> yRow    - yTop)  / nScaling,
                (pRoot -> xColumn + pRoot ->nWidth  - 1
                                        - xLeft) / nScaling + nDustShift,
                (pRoot -> yRow    + pRoot ->nHeight - 1
                                        - yTop)  / nScaling);
        }

        _setcolor (nColor + 8);

        for (i = 0; i < pString -> nDust; i++)
        {
            pRoot = pRoots + pString -> pDustList [i];

            if (pRoot -> bType & ROOT_USED)
                continue;

            _rectangle (_GBORDER,
                (pRoot -> xColumn - xLeft) / nScaling + nDustShift,
                (pRoot -> yRow    - yTop)  / nScaling,
                (pRoot -> xColumn + pRoot ->nWidth - 1
                                        - xLeft) / nScaling + nDustShift,
                (pRoot -> yRow    + pRoot ->nHeight - 1
                                        - yTop)  / nScaling);
        }

        _setcolor (15);

        for (i = 0; i < pString -> nDust; i++)
        {
            pRoot = pRoots + pString -> pDustList [i];

            if (pRoot -> bType & ROOT_USED)
                continue;

            _rectangle (_GBORDER,
                (pRoot -> xColumn - xLeft) / nScaling,
                (pRoot -> yRow    - yTop)  / nScaling,
                (pRoot -> xColumn + pRoot ->nWidth - 1
                                        - xLeft) / nScaling,
                (pRoot -> yRow    + pRoot ->nHeight - 1
                                        - yTop)  / nScaling);
        }

        _setcolor (nColor + 8);

        old_style = _getlinestyle ();
        _setlinestyle (0xcccc);

        _moveto ((pString -> xLeft  - xLeft) / nScaling,
                 (pString -> yMin   - yTop)  / nScaling);
        _lineto ((pString -> xRight - xLeft) / nScaling,
                 (pString -> yMin   - yTop)  / nScaling);

        _moveto ((pString -> xLeft  - xLeft) / nScaling,
                 (pString -> yMax   - yTop)  / nScaling);
        _lineto ((pString -> xRight - xLeft) / nScaling,
                 (pString -> yMax   - yTop)  / nScaling);
        _setlinestyle (old_style);

        _setcolor (12);
        _rectangle (_GBORDER,
                    (pString -> xLeft   - xLeft) / nScaling,
                    (pString -> yTop    - yTop)  / nScaling,
                    (pString -> xRight  - xLeft) / nScaling,
                    (pString -> yBottom - yTop)  / nScaling);

        _setcolor (15);
        _moveto ((pString -> xLeft         - xLeft) / nScaling,
                 (pString -> yMiddleTop    - yTop)  / nScaling);
        _lineto ((pString -> xRight        - xLeft) / nScaling,
                 (pString -> yMiddleTop    - yTop)  / nScaling);

        _moveto ((pString -> xLeft         - xLeft) / nScaling,
                 (pString -> yMiddleBottom - yTop)  / nScaling);
        _lineto ((pString -> xRight        - xLeft) / nScaling,
                 (pString -> yMiddleBottom - yTop)  / nScaling);
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

    for (nString = 1, p = pStringsList; p != NULL; p = p -> pNext, nString++)
    {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? 15 : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft   - xLeft) / nScaling,
                    (p -> yTop    - yTop)  / nScaling,
                    (p -> xRight  - xLeft) / nScaling,
                    (p -> yBottom - yTop)  / nScaling);

        x = ((p -> xLeft + p -> xRight)  / 2 - xLeft) / nScaling;
        y = ((p -> yTop  + p -> yBottom) / 2 - yTop)  / nScaling;

        _setcolor (15);
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

    for (nString = 1, p = pStringsListEnd; p != NULL; p = p -> pPrev, nString++)
    {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? 15 : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft   - xLeft) / nScaling,
                    (p -> yTop    - yTop)  / nScaling,
                    (p -> xRight  - xLeft) / nScaling,
                    (p -> yBottom - yTop)  / nScaling);

        x = ((p -> xLeft + p -> xRight)  / 2 - xLeft) / nScaling;
        y = ((p -> yTop  + p -> yBottom) / 2 - yTop)  / nScaling;

        _setcolor (15);
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

    for (nString = 1, p = pStringsUpList; p != NULL; p = p -> pDown, nString++)
    {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? 15 : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft   - xLeft) / nScaling,
                    (p -> yTop    - yTop)  / nScaling,
                    (p -> xRight  - xLeft) / nScaling,
                    (p -> yBottom - yTop)  / nScaling);

        x = ((p -> xLeft + p -> xRight)  / 2 - xLeft) / nScaling;
        y = ((p -> yTop  + p -> yBottom) / 2 - yTop)  / nScaling;

        _setcolor (15);
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

    for (nString = 1, p = pStringsDownList; p != NULL; p = p -> pUp, nString++)
    {
        _setcolor (p -> uFlags & SF_NEED_DELETE ? 15 : NUMBER_TO_COLOR (nString));
        _rectangle (_GBORDER,
                    (p -> xLeft   - xLeft) / nScaling,
                    (p -> yTop    - yTop)  / nScaling,
                    (p -> xRight  - xLeft) / nScaling,
                    (p -> yBottom - yTop)  / nScaling);

        x = ((p -> xLeft + p -> xRight)  / 2 - xLeft) / nScaling;
        y = ((p -> yTop  + p -> yBottom) / 2 - yTop)  / nScaling;

        _setcolor (15);
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

    _setcolor (15);
    _rectangle (_GFILLINTERIOR,
                (0                     - xLeft) / nScaling,
                (0                     - yTop)  / nScaling,
                (nRasterWidth  * nStep - xLeft) / nScaling,
                (nRasterHeight * nStep - yTop)  / nScaling);

    _setcolor (0);

    for (x = 0; x < nRasterWidth; x++)
    {
        for (y = 0; y < nRasterHeight; y++)
        {
            if ((pRaster [y * nRasterByteWidth + x / 8] << x % 8) & 0x80)
            {
                _rectangle (_GFILLINTERIOR,
                    (x       * nStep - xLeft) / nScaling,
                    (y       * nStep - yTop)  / nScaling,
                    ((x + 1) * nStep - xLeft) / nScaling,
                    ((y + 1) * nStep - yTop)  / nScaling);
            }
        }
    }

    _setcolor (7);

    for (x = 0; x <= nRasterWidth; x++)
    {
        _moveto ((x * nStep             - xLeft) / nScaling,
                 (0                     - yTop)  / nScaling);
        _lineto ((x * nStep             - xLeft) / nScaling,
                 (nRasterHeight * nStep - yTop)  / nScaling);
    }

    for (y = 0; y <= nRasterHeight; y++)
    {
        _moveto ((0                    - xLeft) / nScaling,
                 (y * nStep            - yTop)  / nScaling);
        _lineto ((nRasterWidth * nStep - xLeft) / nScaling,
                 (y * nStep            - yTop)  / nScaling);
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

    _setcolor (15);
    _rectangle (_GFILLINTERIOR,
                (0                     - xLeft) / nScaling,
                (0                     - yTop)  / nScaling,
                (nRasterWidth  * nStep - xLeft) / nScaling,
                (nRasterHeight * nStep - yTop)  / nScaling);

    for (x = 0; x < nRasterWidth; x++)
    {
        for (y = 0; y < nRasterHeight; y++)
        {
            if ((pRaster [y * nRasterByteWidth + x / 8] << x % 8) & 0x80)
                _setcolor (0);
            else if (y == yRasterBreakLine)
                _setcolor (12);
            else if (y >= yRasterUpBreakLimit && y <= yRasterDownBreakLimit)
                _setcolor (14);
            else
                continue;

            _rectangle (_GFILLINTERIOR,
                (x       * nStep - xLeft) / nScaling,
                (y       * nStep - yTop)  / nScaling,
                ((x + 1) * nStep - xLeft) / nScaling,
                ((y + 1) * nStep - yTop)  / nScaling);
        }
    }

    _setcolor (7);

    for (x = 0; x <= nRasterWidth; x++)
    {
        _moveto ((x * nStep             - xLeft) / nScaling,
                 (0                     - yTop)  / nScaling);
        _lineto ((x * nStep             - xLeft) / nScaling,
                 (nRasterHeight * nStep - yTop)  / nScaling);
    }

    for (y = 0; y <= nRasterHeight; y++)
    {
        _moveto ((0                    - xLeft) / nScaling,
                 (y * nStep            - yTop)  / nScaling);
        _lineto ((nRasterWidth * nStep - xLeft) / nScaling,
                 (y * nStep            - yTop)  / nScaling);
    }

    _setcolor (9);

    for (y = 0; y <= nRasterHeight; y++)
    {
        _rectangle (_GFILLINTERIOR,
            ((nRasterWidth + 1) * nStep - xLeft) / nScaling,
            (y                  * nStep - yTop)  / nScaling,
            ((aRasterHystogram [y] + nRasterWidth + 1)
                                * nStep - xLeft) / nScaling,
            ((y + 1)            * nStep - yTop)  / nScaling);
    }
}

void LT_GraphicsTitle (const char *pTitle)
{
    _setcolor (15);
    _settextalign (_CENTER, _HALF);
    _grtext (320, 450, pTitle);
}


int LT_Getch (void)
{
char c;
    if((c=getchar())=='d'){
        void break_point_data (char * c);
        static char stop_here_byte=0;
        static unsigned short int * stop_here_check=0;

	 break_point_data(&stop_here_byte);
        stop_here_byte++; break_point_data(0);
        }
    return ((int)c);
}


static int yCurrentTop  = 0;
static int xCurrentLeft = 0;
static int nCurrentScaling = MAX_SCALING;

static void ScreenOutput (const char *pTitle,
                     void (*pProcUpdate) (int xLeft, int yTop, int nScaling))
{
    int nWidth   = 3000;
    int nHeight  = 3000;

    Bool bNeedUpdateScreen = TRUE;

    for (;;)
    {
        if (bNeedUpdateScreen)
        {
            (*pProcUpdate) (xCurrentLeft, yCurrentTop, nCurrentScaling);
            LT_GraphicsTitle (pTitle);
        }

        do
        {
            switch (LT_Getch ())
            {
                case ' ':
                    return;

                case KEY_LEFT:
                    if (xCurrentLeft > 0)
                    {
                        xCurrentLeft -= SCREEN_WIDTH / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }
                    break;

                case KEY_RIGHT:
                    /*
                    if (xCurrentLeft + SCREEN_WIDTH / STEPS * nCurrentScaling < nWidth)
                    */

                    if (xCurrentLeft < nWidth)
                    {
                        xCurrentLeft += SCREEN_WIDTH / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }
                    break;

                case KEY_UP:
                    if (yCurrentTop > 0)
                    {
                        yCurrentTop -= SCREEN_HEIGHT / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }
                    break;

                case KEY_DOWN:
                    /*
                    if (yCurrentTop + SCREEN_HEIGHT / STEPS * nCurrentScaling < nHeight)
                    */

                    if (yCurrentTop < nHeight)
                    {
                        yCurrentTop += SCREEN_HEIGHT / STEPS * nCurrentScaling;
                        bNeedUpdateScreen = TRUE;
                    }
                    break;

                case KEY_F1:
                    if (nCurrentScaling != MIN_SCALING)
                    {
//                      yCurrentTop      = 0;
//                      xCurrentLeft     = 0;
                        nCurrentScaling  /= 2;
                        bNeedUpdateScreen = TRUE;
                    }
                    break;

                case KEY_F2:
                    if (nCurrentScaling != MAX_SCALING)
                    {
//                      yCurrentTop      = 0;
//                      xCurrentLeft     = 0;
                        nCurrentScaling  *= 2;
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

void LT_GraphicsBlocksOutput (const char *pTitle)
{
    BlocksSetRootsNumbers ();

    ScreenOutput (pTitle, BlocksUpdate);

}

void LT_GraphicsHystogramOutput (const char *pTitle)
{
    ScreenOutput (pTitle, HystogramUpdate);
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


# endif

// Pit 11-21-94 04:56pm
#include "edp.h"
int16_t maxx,minx,maxy,miny;
int16_t scale;

