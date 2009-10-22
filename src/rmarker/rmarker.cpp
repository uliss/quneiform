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

#include <cassert>
#include <cstring>

#include "cifconfig.h"
#include "common/debug.h"
#include "rmarker.h"
#include "dpuma.h"
#include "linesbuffer.h"
#include "rblock.h"
#include "rmfunc.h"
#include "rpic.h"
#include "un_buff.h"
#include "line_vp_util.h"
#include "line_vp_2_am.h"
#include "markdataoper.h"
#include "amt_geom.h"
#include "un_err.h"
#include "puma_err.h"
#include "rneg.h"
#include "rselstr.h"
#include "rline.h"
#include "cfio/cfio.h"
#include "bigimage.h"

#include "compat_defs.h"

namespace CIF {

using namespace CFIO;

void MySetNegative(void *vB, Handle hCPage) {
    int Ind = FindSuchAimedData(vB, UN_DT_Rect16, UN_DA_NegTablCap);
    if (Ind < 0)
        return;

    POLY_ block;
    memset(&block, 0, sizeof(block));
    UN_BUFF *pB = static_cast<UN_BUFF*> (vB);
    int nRc = pB->nPartUnits[Ind];
    Rect16 * pRc = (Rect16*) pB->vPart[Ind];
    for (int i = 0; i < nRc; i++) {
        block.com.type = TYPE_PICTURE; //Текст, Картинка, Таблица;
        block.com.count = 4;
        block.com.Flags |= POS_NEGTABCAP;
        block.com.Vertex[0].rx() = pRc[i].left();
        block.com.Vertex[0].ry() = pRc[i].top();
        block.com.Vertex[1].rx() = pRc[i].right();
        block.com.Vertex[1].ry() = pRc[i].top();
        block.com.Vertex[2].rx() = pRc[i].right();
        block.com.Vertex[2].ry() = pRc[i].bottom();
        block.com.Vertex[3].rx() = pRc[i].left();
        block.com.Vertex[3].ry() = pRc[i].bottom();
        CPAGE_CreateBlock(hCPage, TYPE_IMAGE, 0, 0, &block, sizeof(POLY_));
    }

}

RMarker::RMarker() :
    lines_total_info_(NULL), ccom_(NULL), cline_(NULL), cpage_(NULL), one_column_(true),
            kill_svl_components_(false), language_(LANG_RUSENG), pictures_(PUMA_PICTURE_ALL) {
    RNEG_Init(0, NULL);
}

RMarker::~RMarker() {
    RNEG_Done();
}

Handle RMarker::cCom() const {
    return ccom_;
}

Handle RMarker::cLine() const {
    return cline_;
}

Handle RMarker::cPage() const {
    return cpage_;
}

void RMarker::setCCom(Handle ccom) {
    ccom_ = ccom;
}

void RMarker::setCLine(Handle cline) {
    cline_ = cline;
}

void RMarker::setCPage(Handle cpage) {
    cpage_ = cpage;
}

void RMarker::pageMarkup() {
    buffer_.alloc();

    shortVerticalLinesProcessPass1();

    BigImage big_Image(cpage_);
    Handle h = CPAGE_GetBlockFirst(cpage_, CPAGE_GetInternalType("TYPE_BIG_COMP"));
    if (h) {
        CPAGE_GetBlockData(cpage_, h, CPAGE_GetInternalType("TYPE_BIG_COMP"), &big_Image,
                sizeof(BigImage));
        CPAGE_DeleteBlock(cpage_, h);
    }

    //Поиск очевидных картинок
    searchPictures(big_Image);
    //Поиск негативов
    searchNeg(big_Image);

    //Третий проход по линиям
    RLINE_LinesPass3(cpage_, cline_, ccom_, language_);

    ////снова подсчитываем короткие вертикальные линии и сравниваем с предыдущим результатом
    shortVerticalLinesProcessPass2();

    buffer_.free();

    Bool32 bEnableSearchPicture = pictures_;
    RBLOCK_SetImportData(RBLOCK_Bool32_SearchPicture, &bEnableSearchPicture);
    RBLOCK_SetImportData(RBLOCK_Bool32_OneColumn, &one_column_);

    if (!RBLOCK_ExtractTextBlocks(ccom_, cpage_, cline_))
        throw RMarkerException("RBLOCK_ExtractTextBlocks failed");
}

void RMarker::readSVLFromPageContainer(LinesTotalInfo * LTInfo) {
    assert(LTInfo);
    LTInfo->Hor.Cnt = 0;
    LTInfo->Ver.Cnt = 0;

    int count = 0;
    CLINE_handle hline = CLINE_GetFirstLine(cline_);
    while (hline) {
        CPDLine cpdata = CLINE_GetLineData(hline);
        if (!cpdata)
            hline = CLINE_GetNextLine(hline);
        else {
            if (count >= PUMAMaxNumLines)
                break;
            count++;
            if (cpdata->Dir == LD_Horiz && LTInfo->Hor.Lns) {
                int num = LTInfo->Hor.Cnt;
                LTInfo->Hor.Lns[num].A.rx() = cpdata->Line.Beg_X;
                LTInfo->Hor.Lns[num].A.ry() = cpdata->Line.Beg_Y;
                LTInfo->Hor.Lns[num].B.rx() = cpdata->Line.End_X;
                LTInfo->Hor.Lns[num].B.ry() = cpdata->Line.End_Y;
                LTInfo->Hor.Lns[num].Thickness = cpdata->Line.Wid10 / 10;
                LTInfo->Hor.Lns[num].Flags = cpdata->Flags;
                (LTInfo->Hor.Cnt)++;
            }
            else if (LTInfo->Ver.Lns) {
                int num = LTInfo->Ver.Cnt;
                LTInfo->Ver.Lns[num].A.rx() = cpdata->Line.Beg_X;
                LTInfo->Ver.Lns[num].A.ry() = cpdata->Line.Beg_Y;
                LTInfo->Ver.Lns[num].B.rx() = cpdata->Line.End_X;
                LTInfo->Ver.Lns[num].B.ry() = cpdata->Line.End_Y;
                LTInfo->Ver.Lns[num].Thickness = cpdata->Line.Wid10 / 10;
                LTInfo->Ver.Lns[num].Flags = cpdata->Flags;
                (LTInfo->Ver.Cnt)++;
            }

            hline = CLINE_GetNextLine(hline);
        }
    }
}

void RMarker::searchNeg(const BigImage& big_image) {
    RNEG_RecogNeg(big_image.ccom(), cpage_, big_image.imageName(), big_image.incline());
}

void RMarker::searchPictures(const BigImage& big_image) {
    if (pictures_ == PUMA_PICTURE_NONE)
        return;

    if (!RPIC_SearchPictures(ccom_, big_image.ccom(), cpage_))
        throw RMarkerException("RPIC_SearchPictures failed");
}

void RMarker::setKillSVLComponents(bool value) {
    kill_svl_components_ = value;
}

void RMarker::setLanguage(language_t lang) {
    language_ = lang;
}

void RMarker::setOneColumn(bool value) {
    one_column_ = value;
}

void RMarker::setPicturesMode(puma_picture_t mode) {
    pictures_ = mode;
}

void RMarker::shortVerticalLinesProcessPass1() {
    buffer_.HLinesBufferA = NULL;
    buffer_.LineInfoA->Hor.Lns = NULL;

    if (buffer_.VLinefBufferA == NULL)
        buffer_.VLinefBufferA = buffer_.LineInfoA->Ver.Lns = (LineInfo *) CFIO_DAllocMemory(
                (sizeof(LineInfo) * PUMAMaxNumLines), MAF_GALL_GPTR, "puma",
                "SVL step I lines pool");

    readSVLFromPageContainer(buffer_.LineInfoB);
}

void RMarker::shortVerticalLinesProcessPass2() {
    buffer_.HLinesBufferB = NULL;
    buffer_.LineInfoB->Hor.Lns = NULL;

    if (buffer_.VLinefBufferB == NULL)
        buffer_.VLinefBufferB = buffer_.LineInfoB->Ver.Lns = (LineInfo *) CFIO_DAllocMemory(
                (sizeof(LineInfo) * PUMAMaxNumLines), MAF_GALL_GPTR, "puma",
                "SVL step II lines pool");

    readSVLFromPageContainer(buffer_.LineInfoB);

    // обработка и удаление тут
    svlFilter(buffer_.LineInfoA, buffer_.LineInfoB);
}

void RMarker::svlComponentFilter(LineInfo *Line) {
    assert(Line);

    Rect16 Rl(Line->A, Line->B);
    int nRc = 0;
    int Thick = Line->Thickness / 2;
    Bool32 bDieComponent = FALSE;

    if (Rl.left() <= Rl.right()) {
        Rl.rleft() -= Thick;
        Rl.rright() += Thick;
    }
    else {
        Rl.rleft() += Thick;
        Rl.rright() -= Thick;
    }

    CCOM_comp * pcomp = CCOM_GetFirst(ccom_, NULL);
    do {
        Rect16 Rc(Point16(pcomp->left, pcomp->upper), pcomp->w - 1, pcomp->h - 1);
        nRc++;

        if (Rl.intersects(Rc)) {
            if (kill_svl_components_) {
                CCOM_comp * pdeadcom = pcomp;
                pcomp = CCOM_GetNext(pcomp, NULL);
                bDieComponent = CCOM_Delete(ccom_, pdeadcom);
            }

            if (Config::instance().debugHigh()) {
                fprintf(stderr, "VSL: intersect component < %4.4i, %4.4i > < %4.4i, %4.4i >",
                        Rc.left(), Rc.top(), Rc.right(), Rc.bottom());

                if (bDieComponent)
                    fprintf(stderr, " +dead+");

                fprintf(stderr, "\n");
                bDieComponent = FALSE;
            }
        }

        if (!bDieComponent)
            pcomp = CCOM_GetNext(pcomp, NULL);
        else
            // Almi 18.09.00
            bDieComponent = FALSE;

    }
    while (pcomp != NULL);
}

void RMarker::svlFilter(LinesTotalInfo *LtiA, LinesTotalInfo *LtiB) {
    assert(LtiA);
    assert(LtiB);

    uint SVLCount = 0;
    uint LinesTotalA = LtiA->Ver.Cnt;
    uint LinesTotalB = LtiB->Ver.Cnt;

    if (Config::instance().debugHigh())
        Debug() << "VSL: before table search - " << LinesTotalA << ", after -" << LinesTotalB
                << "\n";

    for (uint i = 0; i < LinesTotalB; i++) {
        if (LtiB->Ver.Lns[i].Flags == LtiA->Ver.Lns[i].Flags)
            continue;

        if (!(LtiA->Ver.Lns[i].Flags & LI_IsTrue) && (LtiB->Ver.Lns[i].Flags & LI_IsTrue)) {
            if (Config::instance().debugHigh()) {
                fprintf(
                        stderr,
                        "VSL: < %4.4i, %4.4i > < %4.4i, %4.4i > x %3.3i flag: from %#8.8x to %#8.8x - delete\n",
                        LtiB->Ver.Lns[i].A.x(), LtiA->Ver.Lns[i].A.y(), LtiB->Ver.Lns[i].B.x(),
                        LtiB->Ver.Lns[i].B.y(), LtiB->Ver.Lns[i].Thickness, LtiA->Ver.Lns[i].Flags,
                        LtiB->Ver.Lns[i].Flags);
            }

            svlComponentFilter(&LtiB->Ver.Lns[i]);
            SVLCount++;
        }

    }

    if (Config::instance().debugHigh()) {
        if (SVLCount == 0)
            Debug() << "VSL: Нужных изменений не найдено\n";
        else
            Debug() << "VSL: Найдено " << SVLCount << " линий.\n";
    }
}

}

