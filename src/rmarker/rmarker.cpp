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

#include <fstream>
#include <cassert>

#define _RMARKER_CPP

#include "dpuma.h"
#include "rblock.h"
#include "markpage.h"
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

#include "compat_defs.h"

using namespace CIF::CFIO;
using namespace CIF;

#define TYPE_FON      CPAGE_GetInternalType("TYPE_FON")

extern Handle DebugFill;
extern Handle hVertCells;
extern Handle hNegaCells;
extern Handle hVertCellsAuto;
extern Handle hNegaCellsAuto;
extern Handle hDebugUpDown;
extern Handle hDebugPictures;
extern Handle hPrintFileVertCells;
extern Handle hDebugNeg;
extern Handle hDebugLinePass3;
extern Handle hDebugLinePass2;
extern Handle hDebugVerifLine;
extern Handle hNoGiveNeg;
extern Handle hNoSeePict;

extern Handle hPrep;
extern Handle hPicture;
extern Handle hNegative;
extern Handle hPrintCrossedPics;
extern Handle hVCutInZones;
extern Handle hSVLP;
extern Handle hBlocks;
extern Handle hLines3;
extern Handle hNegaTestCells;
extern Handle hVertTestCells;
extern Handle hFon;
extern Handle hEnd;

static uint32_t gwRC = 0;
Bool dpDebugUpDown;

void SetReturnCode_rmarker(uint32_t rc) {
    gwRC = rc;
}

uint32_t GetReturnCode_rmarker(void) {
    return gwRC;
}

Bool32 SearchNeg(PRMPreProcessImage Image, BIG_IMAGE big_Image, int skew) {
    if (!LDPUMA_Skip(hDebugNeg))
        return TRUE;

    RNEG_RecogNeg(big_Image.hCCOM, Image->hCPAGE, big_Image.ImageName, skew);
    return TRUE;
}

Bool32 SearchPictures(PRMPreProcessImage Image, BIG_IMAGE big_Image) {
    Bool32 rc = TRUE;

    if (!LDPUMA_Skip(hDebugPictures))
        return TRUE;

    //	rc = ProgressStep(1,/*GetResourceString(IDS_PRG_OPEN),*/10);

    if (rc && LDPUMA_Skip(Image->hDebugCancelSearchPictures)) {
        if (Image->gnPictures) {
            if (!RPIC_SearchPictures(Image->hCCOM, big_Image.hCCOM, Image->hCPAGE)) {
                uint32_t RPicRetCode = RPIC_GetReturnCode();

                SetReturnCode_rmarker(RPicRetCode);
                rc = FALSE;
            }
        }
    }
    return rc;
}

int GetCountNumbers(int num) {
    int count = 0;
    if (num == 0)
        return 1;
    for (; num > 0; num = num / 10)
        count++;
    return count;
}

void MySetNegative(void *vB, Handle hCPage) {
    int i, Ind, nRc;
    Bool WasNegTabl;
    Rect16 *pRc;
    POLY_ block = { 0 };
    UN_BUFF *pB;
    pB = (UN_BUFF *) vB;
    Ind = FindSuchAimedData(vB, UN_DT_Rect16, UN_DA_NegTablCap);
    WasNegTabl = (Ind >= 0);
    if (WasNegTabl) {
        nRc = pB->nPartUnits[Ind];
        pRc = (Rect16*) pB->vPart[Ind];
        for (i = 0; i < nRc; i++) {
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
}

namespace CIF {

RMarker::RMarker() :
    image_(NULL) {
    RNEG_Init(0, NULL);
    // FIXME
    remove("crossed.txt");
}

RMarker::~RMarker() {
    RNEG_Done();
}

void RMarker::pageMarkup() {
    assert(image_);

    Bool32 rc = TRUE;

    gSVLBuffer.VLinefBufferA = NULL;
    gSVLBuffer.VLinefBufferB = NULL;
    gSVLBuffer.LineInfoA = (LinesTotalInfo*) CFIO_DAllocMemory(sizeof(LinesTotalInfo),
            MAF_GALL_GPTR, "puma", "SVL step I lines info pool");
    gSVLBuffer.LineInfoB = (LinesTotalInfo*) CFIO_DAllocMemory(sizeof(LinesTotalInfo),
            MAF_GALL_GPTR, "puma", "SVL step II lines info pool");

    shortVerticalLinesProcess();

    Handle h = NULL;
    BIG_IMAGE big_Image;

    //default Image:
    PAGEINFO info = { 0 };
    GetPageInfo(image_->hCPAGE, &info);
    for (int i = 0; i < CPAGE_MAXNAME; i++)
        big_Image.ImageName[i] = info.szImageName[i];
    big_Image.hCCOM = NULL;

    h = CPAGE_GetBlockFirst(image_->hCPAGE, TYPE_BIG_COMP);
    if (h) {
        CPAGE_GetBlockData(image_->hCPAGE, h, TYPE_BIG_COMP, &big_Image, sizeof(BIG_IMAGE));
        CPAGE_DeleteBlock(image_->hCPAGE, h);
    }

    //Поиск очевидных картинок
    if (rc)
        rc = SearchPictures(image_, big_Image);

    //Поиск негативов
    if (rc)
        rc = SearchNeg(image_, big_Image, info.Incline2048);

    //Третий проход по линиям
    if (LDPUMA_Skip(hDebugLinePass3) && LDPUMA_Skip(hDebugVerifLine)
            && LDPUMA_Skip(hDebugLinePass2)) {
        if (rc)
            RLINE_LinesPass3(image_->hCPAGE, image_->hCLINE, image_->hCCOM,
                    (uchar) image_->gnLanguage);
    }

    ////снова подсчитываем короткие вертикальные линии и сравниваем с предыдущим результатом
    if (rc) {
        rc = ShortVerticalLinesProcess(PUMA_SVL_SECOND_STEP, image_);
    }

    ShortVerticalLinesProcess(PUMA_SVL_THRID_STEP, image_);

    CFIO_FreeMemory(gSVLBuffer.LineInfoA);
    CFIO_FreeMemory(gSVLBuffer.LineInfoB);

    if (!LDPUMA_Skip(image_->hDebugLayoutFromFile)) {
        image_->hCPAGE = CPAGE_RestorePage(TRUE, (pchar) (image_->szLayoutFileName));
        if (image_->hCPAGE == NULL) {
            SetReturnCode_rmarker(CPAGE_GetReturnCode());
            rc = FALSE;
        }
        else {
            CPAGE_SetCurrentPage(CPAGE_GetNumberPage(image_->hCPAGE));
            LDPUMA_Console("Layout восстановлен из файла '%s'\n", image_->szLayoutFileName);
        }
    }
    else {
        if (rc) {
            if (LDPUMA_Skip(image_->hDebugCancelExtractBlocks)) {
                Bool32 bEnableSearchPicture;
                bEnableSearchPicture = image_->gnPictures;
                RBLOCK_SetImportData(RBLOCK_Bool32_SearchPicture, &bEnableSearchPicture);
                RBLOCK_SetImportData(RBLOCK_Bool32_OneColumn, &(image_->gbOneColumn));

                if (!RBLOCK_ExtractTextBlocks(image_->hCCOM, image_->hCPAGE, image_->hCLINE)) {
                    SetReturnCode_rmarker(RBLOCK_GetReturnCode());
                    rc = FALSE;
                }

            }
            else
                LDPUMA_Console("Пропущен этап автоматического Layout.\n");
        }
    }

    CCOM_DeleteContainer(big_Image.hCCOM);
}

void RMarker::setImageData(RMPreProcessImage& image) {
    image_ = &image;
}

void RMarker::shortVerticalLinesProcess() {
    ShortVerticalLinesProcess(PUMA_SVL_FIRST_STEP, image_);
}

}

