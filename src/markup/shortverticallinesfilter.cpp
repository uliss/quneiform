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

/**********  Заголовок  ****************************************************************************/
/*  Автор,                                                                                         */
/*  комментарии                                                                                    */
/*  правка     :                                                                                   */
/*  Редакция   :  30.08.00                                                                         */
/*  Файл       :  'ShortVerticalLinesFilter.cpp'                                                   */
/*  Содержание :                                                                                   */
/*  Назначение :                                                                                   */
/*-------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "dpuma.h"
#include "shortverticallinesfilter.h"
#include "linedefs.h"
#include "cline/cline.h"
#include "ccom/ccom.h"

static const int PUMA_MAX_NUM_LINES = 2000;
static const int IDS_ERR_INITIATED_BY_ALLEX = 2029;

static Bool32 bShowDebug = FALSE;
static Bool32 bShowStepDebug = FALSE;
static Bool32 bShowDebugData = FALSE;

struct PUMALinesBuffer {
    LinesTotalInfo * LineInfoA;
    LinesTotalInfo * LineInfoB;
    void * HLinesBufferA;
    void * VLinefBufferA;
    void * HLinesBufferB;
    void * VLinefBufferB;
};

static PUMALinesBuffer gSVLBuffer;

void freeSVLBuffer() {
    free(gSVLBuffer.LineInfoA);
    free(gSVLBuffer.LineInfoB);
}

void initSVLBuffer() {
    gSVLBuffer.VLinefBufferA = NULL;
    gSVLBuffer.VLinefBufferB = NULL;
    gSVLBuffer.LineInfoA = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));
    gSVLBuffer.LineInfoB = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));
}

Bool32 ShortVerticalLinesProcess(svl_step_t Step, PRMPreProcessImage Image)
{
    Bool32 bRet = FALSE;

    if (Step == PUMA_SVL_FIRST_STEP && gSVLBuffer.LineInfoA) {
        gSVLBuffer.HLinesBufferA = gSVLBuffer.LineInfoA->Hor.Lns = NULL;
        if (gSVLBuffer.VLinefBufferA == NULL)
            gSVLBuffer.VLinefBufferA = gSVLBuffer.LineInfoA->Ver.Lns = (LineInfo *) calloc(
                    sizeof(LineInfo), PUMA_MAX_NUM_LINES);

        bRet = ReadSVLFromPageContainer(gSVLBuffer.LineInfoA, Image);
    }

    if (Step == PUMA_SVL_SECOND_STEP && gSVLBuffer.LineInfoB) {
        gSVLBuffer.HLinesBufferB = gSVLBuffer.LineInfoB->Hor.Lns = NULL;
        if (gSVLBuffer.VLinefBufferB == NULL)
            gSVLBuffer.VLinefBufferB = gSVLBuffer.LineInfoB->Ver.Lns = (LineInfo *) calloc(
                    sizeof(LineInfo), PUMA_MAX_NUM_LINES);

        bRet = ReadSVLFromPageContainer(gSVLBuffer.LineInfoB, Image);

        ////////////////
        // обработка и удаление тут
        if (bRet) {
            bRet = SVLFilter(gSVLBuffer.LineInfoA, gSVLBuffer.LineInfoB, Image);
        }
    }

    if (Step == PUMA_SVL_THRID_STEP) {
        if (gSVLBuffer.VLinefBufferA != NULL) {
            free(gSVLBuffer.VLinefBufferA);
        }

        if (gSVLBuffer.VLinefBufferB != NULL) {
            free(gSVLBuffer.VLinefBufferB);
        }

        gSVLBuffer.VLinefBufferA = NULL;
        gSVLBuffer.VLinefBufferB = NULL;
        bRet = TRUE;
    }

    if (bRet == FALSE) {
        SetReturnCode_rmarker(IDS_ERR_INITIATED_BY_ALLEX);
    }

    return bRet;
}

Bool32 ReadSVLFromPageContainer(LinesTotalInfo *LTInfo, PRMPreProcessImage Image)
{
    Bool32 bRet = TRUE;
    uint32_t nTagSize;
    nTagSize = sizeof(LinesTotalInfo);
    Bool32 fl_break;
    int num = 0;
    int count = 0;
    CLINE_handle hCLINE = Image->hCLINE;
    CLINE_handle hline;
    CPDLine cpdata;
    hline = CLINE_GetFirstLine(hCLINE);
    LTInfo->Hor.Cnt = 0;
    LTInfo->Ver.Cnt = 0;

    while (hline) {
        fl_break = FALSE;
        cpdata = CLINE_GetLineData(hline);

        if (!cpdata)
            hline = CLINE_GetNextLine(hline);

        else {
            if (count >= PUMA_MAX_NUM_LINES)
                fl_break = TRUE;

            else {
                count++;

                if (cpdata->Dir == LD_Horiz) {
                    if (LTInfo->Hor.Lns) {
                        num = LTInfo->Hor.Cnt;
                        LTInfo->Hor.Lns[num].A.rx() = cpdata->Line.Beg_X;
                        LTInfo->Hor.Lns[num].A.ry() = cpdata->Line.Beg_Y;
                        LTInfo->Hor.Lns[num].B.rx() = cpdata->Line.End_X;
                        LTInfo->Hor.Lns[num].B.ry() = cpdata->Line.End_Y;
                        LTInfo->Hor.Lns[num].Thickness = cpdata->Line.Wid10 / 10;
                        LTInfo->Hor.Lns[num].Flags = cpdata->Flags;
                        (LTInfo->Hor.Cnt)++;
                    }
                }

                else {
                    if (LTInfo->Ver.Lns) {
                        num = LTInfo->Ver.Cnt;
                        LTInfo->Ver.Lns[num].A.rx() = cpdata->Line.Beg_X;
                        LTInfo->Ver.Lns[num].A.ry() = cpdata->Line.Beg_Y;
                        LTInfo->Ver.Lns[num].B.rx() = cpdata->Line.End_X;
                        LTInfo->Ver.Lns[num].B.ry() = cpdata->Line.End_Y;
                        LTInfo->Ver.Lns[num].Thickness = cpdata->Line.Wid10 / 10;
                        LTInfo->Ver.Lns[num].Flags = cpdata->Flags;
                        (LTInfo->Ver.Cnt)++;
                    }
                }
            }

            hline = CLINE_GetNextLine(hline);
        }

        if (fl_break)
            break;
    }

    return TRUE;
    return bRet;
}

Bool32 SVLFilter(LinesTotalInfo *LtiA, LinesTotalInfo *LtiB, PRMPreProcessImage Image)
{
    Bool32 rc = TRUE;
    uint32_t LinesTotalA;
    uint32_t LinesTotalB;
    char str[255];
    int32_t j = 0;
    uint32_t SVLCount = 0;
    Rect16 ZoomRect;
    bShowDebug = !LDPUMA_Skip(Image->hDebugSVLines);
    bShowStepDebug = !LDPUMA_Skip(Image->hDebugSVLinesStep);
    bShowDebugData = !LDPUMA_Skip(Image->hDebugSVLinesData);

    if (LtiA == NULL || LtiB == NULL) {
        rc = FALSE;
        return rc;
    }

    LinesTotalA = LtiA->Ver.Cnt;
    LinesTotalB = LtiB->Ver.Cnt;

    if ((bShowDebug || bShowStepDebug) && bShowDebugData) {
        sprintf(str, "VSL: до поиска таблиц - %i, после - %i\n", LinesTotalA, LinesTotalB);
        LDPUMA_Console(str);
    }

    for (uint32_t i = 0; i < LinesTotalB; i++) {
        if (LtiB->Ver.Lns[i].Flags != LtiA->Ver.Lns[i].Flags) {
            if (!(LtiA->Ver.Lns[i].Flags & LI_IsTrue) && (LtiB->Ver.Lns[i].Flags & LI_IsTrue)) {
                if (SVLCount != 0 && bShowStepDebug)
                    LDPUMA_WaitUserInput(Image->hDebugSVLinesStep, NULL);

                if (bShowDebug || bShowStepDebug) {
                    j
                            = sprintf(
                                    str,
                                    "VSL: < %4.4i, %4.4i > < %4.4i, %4.4i > x %3.3i flag: from %#8.8x to %#8.8x",
                                    LtiB->Ver.Lns[i].A.x(), LtiA->Ver.Lns[i].A.y(),
                                    LtiB->Ver.Lns[i].B.x(), LtiB->Ver.Lns[i].B.y(),
                                    LtiB->Ver.Lns[i].Thickness, LtiA->Ver.Lns[i].Flags,
                                    LtiB->Ver.Lns[i].Flags);
                    j += sprintf(str + j, " - удалить");
                    j += sprintf(str + j, "\n");

                    if (bShowDebugData)
                        LDPUMA_Console(str);

                    LDPUMA_DrawLine(NULL, &LtiB->Ver.Lns[i].A, &LtiB->Ver.Lns[i].B,
                            0/*LtiB->Skew1024*/, 0x00ff00, LtiB->Ver.Lns[i].Thickness, 315);
                    ZoomRect.top = LtiB->Ver.Lns[i].A.y() <= LtiB->Ver.Lns[i].B.y()
                            ? LtiB->Ver.Lns[i].A.y()
                            : LtiB->Ver.Lns[i].B.y();
                    ZoomRect.bottom = LtiB->Ver.Lns[i].A.y() > LtiB->Ver.Lns[i].B.y()
                            ? LtiB->Ver.Lns[i].A.y()
                            : LtiB->Ver.Lns[i].B.y();
                    ZoomRect.left = LtiB->Ver.Lns[i].A.x() <= LtiB->Ver.Lns[i].B.x()
                            ? LtiB->Ver.Lns[i].A.x()
                            : LtiB->Ver.Lns[i].B.x();
                    ZoomRect.right = LtiB->Ver.Lns[i].A.x() > LtiB->Ver.Lns[i].B.x()
                            ? LtiB->Ver.Lns[i].A.x()
                            : LtiB->Ver.Lns[i].B.x();
                    ZoomRect.bottom -= ((ZoomRect.top - ZoomRect.bottom) / 3) >= ZoomRect.bottom
                            ? ZoomRect.bottom
                            : ((ZoomRect.top - ZoomRect.bottom) / 3);
                    ZoomRect.top += ((ZoomRect.top - ZoomRect.bottom) / 3);
                    ZoomRect.left -= ((ZoomRect.right - ZoomRect.left) / 3) >= ZoomRect.left
                            ? ZoomRect.left
                            : ((ZoomRect.right - ZoomRect.left) / 3);
                    ZoomRect.right += ((ZoomRect.right - ZoomRect.left) / 3);

                    if (bShowStepDebug)
                        LDPUMA_ZoomToRect(NULL, &ZoomRect);
                }

                if (!SVLComponentFilter(&LtiB->Ver.Lns[i], Image)) {
                    rc = FALSE;
                    break;
                }

                SVLCount++;
            }
        }
    }

    if (bShowDebug || bShowStepDebug) {
        if (j == 0 && bShowDebugData) {
            LDPUMA_Console("VSL: Нужных изменений не найдено\n");
        }

        else {
            LDPUMA_Console("VSL: Найдено %i линий. Нажми на что нибудь и пойдем дальше...\n",
                    SVLCount);
            LDPUMA_WaitUserInput(Image->hDebugSVLines, NULL);
            LDPUMA_DeleteLines(NULL, 315);
            LDPUMA_DeleteRects(NULL, 316);
            ZoomRect.top = 0;
            ZoomRect.bottom = 0;

            if (bShowStepDebug)
                LDPUMA_ZoomToRect(NULL, &ZoomRect);
        }
    }

    return rc;
}

Bool32 SVLComponentFilter(LineInfo *Line, PRMPreProcessImage Image)
{
    CCOM_comp * pcomp;
    CCOM_comp * pdeadcom;
    Bool32 GoodComp;
    Rect16 Rc;
    Rect16 Rl;
    int32_t nRc = 0;
    int32_t Filter = 0;
    int32_t j = 0;
    char str[255];
    int16_t Thick = Line->Thickness / 2;
    Bool32 bDieComponent = FALSE;
    Rl.left = Line->A.x();
    Rl.top = Line->A.y();
    Rl.right = Line->B.x();
    Rl.bottom = Line->B.y();

    if (Rl.left <= Rl.right) {
        Rl.left -= Thick;
        Rl.right += Thick;
    }

    else {
        Rl.left += Thick;
        Rl.right -= Thick;
    }

    pcomp = CCOM_GetFirst(Image->hCCOM, NULL);

    do {
        GoodComp = CompIsGood(Filter);

        if (GoodComp) {
            Rc.left = pcomp->left;
            Rc.right = pcomp->left + pcomp->w - 1;
            Rc.top = pcomp->upper;
            Rc.bottom = pcomp->upper + pcomp->h - 1;
            nRc++;

            if (IsRectIntersect(&Rl, &Rc)) {
                if (Image->gKillVSLComponents) {
                    pdeadcom = pcomp;
                    pcomp = CCOM_GetNext(pcomp, NULL);
                    bDieComponent = CCOM_Delete(Image->hCCOM, pdeadcom);
                }

                if (bShowDebug || bShowStepDebug) {
                    j = sprintf(str, "VSL: intersect component < %4.4i, %4.4i > < %4.4i, %4.4i >",
                            Rc.left, Rc.top, Rc.right, Rc.bottom);

                    if (bDieComponent)
                        j += sprintf(str + j, " +dead+");

                    j += sprintf(str + j, "\n");

                    if (bShowDebugData)
                        LDPUMA_Console(str);

                    LDPUMA_DrawRect(NULL, &Rc, 0, 0xff0000, 1, 316);
                    bDieComponent = FALSE;
                }
            }
        }

        if (!bDieComponent)
            pcomp = CCOM_GetNext(pcomp, NULL);

        else
            // Almi 18.09.00
            bDieComponent = FALSE;
    }
    while (pcomp != NULL);

    return TRUE;
}

Bool32 IsRectIntersect(Rect16 *A, Rect16 *B)
{
    Bool32 rc = FALSE;
    int32_t M1 = A->bottom >= A->top ? A->bottom : A->top;
    int32_t M2 = A->right >= A->left ? A->right : A->left;
    int32_t m1 = A->bottom >= A->top ? A->top : A->bottom;
    int32_t m2 = A->right >= A->left ? A->left : A->right;
    int32_t M3 = B->bottom >= B->top ? B->bottom : B->top;
    int32_t M4 = B->right >= B->left ? B->right : B->left;
    int32_t m3 = B->bottom >= B->top ? B->top : B->bottom;
    int32_t m4 = B->right >= B->left ? B->left : B->right;

    if ((((B->top >= m1) && (B->top <= M1)) || ((B->bottom >= m1) && (B->bottom <= M1)))
            && (((B->left >= m2) && (B->left <= M2)) || ((B->right >= m2) && (B->right <= M2)))) {
        rc = TRUE;
    }

    if ((((A->top >= m3) && (A->top <= M3)) || ((A->bottom >= m3) && (A->bottom <= M3)))
            && (((A->left >= m4) && (A->left <= M4)) || ((A->right >= m4) && (A->right <= M4)))) {
        rc = TRUE;
    }

    return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
// взято у Михайлова. лишнее выкинуто, остальное изменено
Bool32 CompIsGood(int32_t Filter)
{
    switch (Filter) {
    case 0:
        return TRUE;
    default:
        return FALSE;
    }
}
