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

// RShellLinesCom.h: interface for the ShellLine's functions.
//	Description: ShellLine's functions and classes definition
//	Implemented: by B.M. Shahverdiev

#include "globus.h"
#include "cline.h"
#include "cpage/cpage.h"
#include "compat_defs.h"

#ifdef __RSHELLLINES_EXPORTS__
#define RSHELLLINES_FUNC  FUN_EXPO
#else
#define RSHELLLINES_FUNC  FUN_IMPO
#endif

enum
{
    VerticalLine = 0,
    HorizantalLine = 1
};

const int HalfOtstup = 4;
const int Otstup = 4;
const int MaxNumber = 4;
const int MIN_H_NO_CLEAN = 10;

struct DOT_STRUCT
{
    int32_t left;
    int32_t right;
    int32_t top;
    int32_t bottom;
    int32_t i_left;
    int32_t i_top;
    int32_t i_right;
    Bool flag;
};

typedef struct group_lines_mass
{
    CLINE_handle* hLinesMass;
    int numder_of_lines;
    int number_of_cp;
    int32_t* m_arCutPointsL;
    int32_t* m_arCutPointsR;
    int32_t* m_arFrequency;
    int32_t m_top;
    int32_t m_bottom;
    int32_t GroupOffset;
    int32_t GroupVerOffset;
} GLM;

void SetEventWidth(CLINE_handle hEvent, DEvent* pEvent, uint32_t* LineHist, uint32_t RectBegin,
        uint32_t LineLength); //


// Component's class
enum
{
    InitialPointHasCrossPoint = 1, //defines for Component
    MiddleHasCrossPoint = 2,
    EndPointHasCrossPoint = 4
};

// For Status
enum rshellines_status_t
{
    ST_Unknown = 0x00000000,
    ST_Basil_Short = 0x00000001,
    ST_Good = 0x00000002,
    ST_Bad = 0x00000004
};

// Line's class
RSHELLLINES_FUNC(void) InitLine(DLine* linedata); //
Bool SetLineNormDataTypes(NORM_DATATYPES, DLine* line); //
void GetNewStripeVerticalLine(DEvent* eventdata); //
void SetLineProcessingType(DLine* linedata); //
void SetWidth(CLINE_handle line, DLine* pCLine); //
void SetLength(CLINE_handle line, DLine* pCLine); //
void SetComponentsCrossPointFlag(CLINE_handle line, DLine* pCLine); //

RSHELLLINES_FUNC(Bool32) CheckSeparationPoints(CLINE_handle hLine, CLINE_handle hComp = NULL); //

void SetSeparationPoints(CLINE_handle hContainer, CLINE_handle hLine, DLine* pCLine, Bool32 PLines); //
void SetCutPoints(CLINE_handle line, DLine* pCLine); //
void SetNewLine(CLINE_handle hContainer, CPDLine pCLine, int32_t IntervalBeg, int32_t IntervalEnd,
        CLINE_handle hLineOld, Bool32 PossibleSymbolAboveLine); //
void SetDegree(DLine* line); //
Bool CheckPoint(CLINE_handle line, int CurPoint); //
void ConvertStructs(CLINE_handle line, DLine* pCLine, int32_t nIncline); //
void FindTailComponent(CLINE_handle line, DLine* pCLine); //
void GetCorrectLine_End_X(CLINE_handle line, DLine* pCLine); //
void GetComponentBeg_Y_End_Y(int32_t Beg, int32_t End, int32_t* Beg_X, int32_t* End_X,
        int32_t* Beg_Y, int32_t* End_Y, CLINE_handle hLine, Bool32 ProcessingType); //
Bool32 IfExistCutLine(CLINE_handle hContainer, DLine* pCLine, int left, int right); //

Bool IsItNewLine(CLINE_handle hContainer, DLine* pLineCP, int32_t GroupVerOffset,
        Bool32 IfDrawResult); //
Bool CheckLinesForExtensible(CLINE_handle hLine, GLM* hGroupEx); //
Bool CheckLinesImpositionForExtensible(CLINE_handle hLine, GLM* hGroupEx); //
Bool CheckLinesDegreeForExtensible(CLINE_handle hLine, GLM* hGroupEx); //

RSHELLLINES_FUNC(void) FindFriendLines(CLINE_handle hContainer, GLM* friendlinesmass); //
RSHELLLINES_FUNC(void) DrawFriendLines(CLINE_handle hContainer, GLM* friendlinesmass); //

RSHELLLINES_FUNC(void) FindGroupOfExtensibleLines(CLINE_handle hContainer, GLM* friendlinesmass,
        Bool32 IfDrawResult); //
void DrawGroupOfExtensibleLines(CLINE_handle hContainer, GLM* hGroup, int CountLines); //

RSHELLLINES_FUNC(void) FindLosedVerticalLines(CLINE_handle hContainer, GLM* friendlinesmass,
        CLINE_handle hFictContainer, int CountLines, Bool32 IfDrawResult); //
RSHELLLINES_FUNC(void) DrawLosedVerticalLines(GLM* friendlinesmass, int CountLines); //

//  void                     GetBigComps(Handle hCComp);
RSHELLLINES_FUNC(void) DrawBigComps(CLINE_handle hContainer); //

RSHELLLINES_FUNC(void) FindDotLines(Handle hCCOM, Handle hCPAGE, CLINE_handle hContainer); //
Bool GetDotComps(Handle hCCOM, Handle hCPAGE, Handle &hbuf, char* &pDotBuffer,
        int32_t *CountDotComps); //
void FindChains(Handle hCCOM, char* &pDotBuffer, int32_t CountDotComps, CLINE_handle hContainer); //
void CheckChains(Handle hCCOM, char* &pDotBuffer, int32_t BegIndex, int32_t CountDots,
        CLINE_handle hContainer); //
void AddNewDotLines(Handle hCCOM, char* &pDotBuffer, int32_t CountDots, CLINE_handle hContainer); //
void UnionBreakup(CLINE_handle hContainer); //

RSHELLLINES_FUNC(void) SetLinesAndCompsRelationship(CLINE_handle hContainer,
        CLINE_handle hFictContainer); //

RSHELLLINES_FUNC(void) DrowAllLines(CLINE_handle hContainer, Handle hDrowAllLines); //
/*  int                      SetImage(Handle hCPage);
 };
 */
RSHELLLINES_FUNC(Bool32) FindLineFrag(CLINE_handle processedline, Bool32 OnlyPosyAndStat,
        Bool32 Is2ndPath, CLINE_handle hContainer, Bool32 IfNeedFragment = TRUE,
        Bool32 IfStraightFrag = FALSE); //
RSHELLLINES_FUNC(Bool32) FindLineAttr(CLINE_handle line, DLine* pCLine, Bool32 AbleSeeOldAttr); //
RSHELLLINES_FUNC(Bool32)
        GetNextPartOfLine(CLINE_handle hContainer, CLINE_handle hLine/*, CPDLine oldlinedata*/); //

void AddLine2Group(GLM* hGroup, CLINE_handle hLine);
RSHELLLINES_FUNC(void) DeleteGroup(GLM* hGroup, int CountLines);
RSHELLLINES_FUNC(void) DrawFragsForAllLines(CLINE_handle hContainer,
        Handle hDebugDrawAllLineFragments);
RSHELLLINES_FUNC(int32_t) CountBlackRaster(CPDLine pLine, CPDLine pLineExt, Bool32 IsHor,
        int32_t delta, Handle hDrawRaster);
RSHELLLINES_FUNC(Bool32) SetExtLines(CLINE_handle hExtContainer, CLINE_handle hContainer,
        CLINE_handle* hLinesMass, int32_t CountLines);
RSHELLLINES_FUNC(Bool32) RSL_CorrectDoubleLines(CLINE_handle hLine1, CLINE_handle hLine2);
RSHELLLINES_FUNC(Bool32) RSL_SplitLine(CLINE_handle hLine, CLINE_handle hContainer);

//possible return values for RSL_VerifyShortLine
enum
{
    RSL_IS_LINE = 0,
    RSL_BIG_ANGLE_DIFF = 1,
    RSL_NO_COMP_FOUND = 2,
    RSL_BAD_RASTER_FORM = 3,
    RSL_BAD_LINE_PARAMS = 4
};

const int MAX_CROSS_POINTS = 4;
RSHELLLINES_FUNC(int32_t) RSL_VerifyShortLine(CPDLine pLine, Handle hCCOM, PAGEINFO* page_info,
        uchar lang, uchar debug_flags, int32_t *cross_point = NULL);
RSHELLLINES_FUNC(Bool) SL_GetRaster(CIF::Rect* rect, uchar** ppData, PAGEINFO* page_info);

