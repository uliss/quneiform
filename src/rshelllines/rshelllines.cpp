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

// RShellLines.cpp: implementation of the ShellLine's functions.
//	Description: ShellLine's functions implementation
//	Implemented: by B.M. Shahverdiev

#include "resource.h"
#define __RSL__
#define __RSL_EXTERN__
#include "cttypes.h"
#include "linedefs.h"
#include "puma/pumadef.h"
#include "cpage/cpage.h"
#include "cimage/ctiimage.h"
#include "dpuma.h"
#include "rshelllinescom.h"
#include "rshelllines.h"
#include "rsl.h"
#include "rverline.h"
#include "un_buff.h"
#include "cfio/cfio.h"
#include "dsnap.h"
#include "rstuff/rstuff.h"

using namespace CIF::CFIO;
using namespace CIF;

#define CRIME_CUT   50
#define RESULT      2
#define RSL_HANDLE 1
#define RSL_VERLINE CPAGE_GetInternalType("RVL_VERIFY")

#define RSL_ERR_NO_MEMORY 1
#define     MAF_GALL_GPTR              0x0010
char cCommentBuffer[CFIO_MAX_COMMENT];

static uint16_t gwHeightRC = 0;
static uint16_t gwLowRC = 0;
Handle hSkipDeleteNoiseEvents = NULL;
Handle hPrintUnderlineTest = NULL;
Handle hSkipCheckUnderlining = NULL;
Handle hPrintUnderlines = NULL;
Handle hSkipCheckRaster = NULL;
Handle hPrintRasterCheck = NULL;
Handle hSkipCorrectPolynomia = NULL;
Handle hPrintCorrectPolynomia = NULL;
Handle hSkipDelFragLines = NULL;
Handle hPrintFragLines = NULL;
Handle hPreRSL_Root = NULL;
Handle hPreRSL_Control = NULL;
Handle hPreRSL_Debug = NULL;
Handle hDebugFragOwnerControl = NULL;

Bool32 FindLineFrag(CLINE_handle processedline, Bool32 OnlyPosyAndStat, Bool32 Is2ndPath,
        CLINE_handle hContainer, Bool32 IfNeedFragment, Bool32 IfStraightFrag) {
    return TRUE;
}

Bool32 FindLineAttr(CLINE_handle line, DLine* pCLine, Bool32 AbleSeeOldAttr) {
    //SetWidth(line, pCLine);
    return TRUE;
}

void SetReturnCode_rshelllines(uint16_t rc) {
    gwLowRC = rc;
}

uint16_t GetReturnCode_rshelllines() {
    return gwLowRC;
}

struct FictInterval
{
    int32_t Level;
    int32_t Pos;
    int32_t End;
    int32_t RelIndex;
};

void DeleteNoiseEvents(CLINE_handle hLine, DLine* pLine) {
    return;
}

void CheckUnderlining(CLINE_handle hLine, DLine* pLine, char* pSourceRaster) {
    return;
}

Bool32 CompareRasterParts(CPDLine pLine, char* pSourceRaster, Bool32 CheckSerif) {
    return TRUE;
}

Bool32 RSL_CorrectDoubleLines(CLINE_handle hLine1, CLINE_handle hLine2) {
    return TRUE;
}

Bool32 RSL_SplitLine(CLINE_handle hLine, CLINE_handle hContainer) {
    return TRUE;
}

Bool SL_IsPointInAB(Point *P, Point *A, Point *B) {
    return FALSE;
}

void CalculateRectForRaster(DLine* pCLine, PAGEINFO* page_info) {
}

void CalculatePolynomia(DLine* pCLine, PAGEINFO* page_info) {
}

void DrowRectForRaster(Rect* rect) {
}

void DrowPolynomia(DLine* pCLine) {
}

void DrowVerticalLineStripesIntervals(CLINE_handle line, Handle HndMyWindow) {
}

void CleaningRaster(DLine* pCLine, char* Buffer) {
}

void DrowCleanedRaster(DLine* pCLine, char* Buffer, BitmapInfoHeader* image_info,
        Handle* HndMyWindow) {
}

void DrowVerticalLineRaster(DLine* pCLine, char* Buffer, BitmapInfoHeader* image_info,
        Handle* HndMyWindow) {
}

void Transpose_bit_matrixes(pchar buf_in, pchar buf_out, Rect* rect) {
}

Bool GetLineStripesIntervals(CLINE_handle line, DLine* pCLine, pchar pRaster, Bool FlagVerticalLine) {
    return TRUE;
}

void FillingStripes(CLINE_handle line, int y, int16_t Count, uint16_t* pIntervals,
        Bool FlagVerticalLine, DLine* pCLine) {
}

void CheckDotLines(DLine* pCLine, Bool FlagVerticalLine) {
}

void TypeIntervals(int y, int16_t Count, uint16_t* pIntervals) {
}

void TypeStripe(CLINE_handle hStripe, DEvent* pStripe) {
}

Bool GetLineFragments(CLINE_handle line, DLine* pCLine) {
    return TRUE;
}

Handle DrawLineFragments(CLINE_handle line, CPDLine pCLine, Handle HndMyWindow, int ColorIndex,
        Bool32 ForAllLines)// = FALSE)
{
    return NULL;
}

void DrawFriendLines(CLINE_handle hContainer, GLM* friendlinesmass) {
}

void DrawGroupOfExtensibleLines(CLINE_handle hContainer, GLM* hGroup, int CountLines) {
}

void DrawLosedVerticalLines(GLM* friendlinesmass, int CountLines) {
}

void DrowAllLines(CLINE_handle hContainer, Handle hDrowAllLines) {
}

void DrawBigComps(CLINE_handle hContainer) {
}

void DrawFragsForAllLines(CLINE_handle hContainer, Handle hDebugDrawAllLineFragments) {
}

void InitLine(DLine* linedata) {
}

void FindDotLines(Handle hCCOM, Handle hCPAGE, CLINE_handle hContainer) {
}

Bool32 CheckSeparationPoints(CLINE_handle hLine, CLINE_handle hComp) {
    return FALSE;
}

Bool SL_GetRaster(Rect* rect, uchar** ppData, PAGEINFO* page_info) {
    return TRUE;
}

void SetLinesAndCompsRelationship(CLINE_handle hContainer, CLINE_handle hFictContainer) {
}

void FindFriendLines(CLINE_handle hContainer, GLM* friendlinesmass) {
}

void FindGroupOfExtensibleLines(CLINE_handle hContainer, GLM* friendlinesmass, Bool32 IfDrawResult) {
}

int32_t RSL_VerifyShortLine(CPDLine pLine, Handle hCCOM, PAGEINFO* page_info, uchar lang,
        uchar debug_flags, int32_t *cross_point) {
    return 0;
}

Bool32 GetNextPartOfLine(CLINE_handle hContainer, CLINE_handle hLine) {
    return FALSE;
}

Bool32 SetExtLines(CLINE_handle hExtContainer, CLINE_handle hContainer, CLINE_handle* hLinesMass,
        int32_t CountLines) {
    return FALSE;
}

int32_t CountBlackRaster(CPDLine pLine, CPDLine pLineExt, Bool32 IsHor, int32_t delta,
        Handle hDrawRaster) {
    return 0;
}

