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

#define _RMARKER_CPP

#include <fstream>

#include "common/recognizeoptions.h"
#include "markpage.h"
#include "rneg/rneg.h"
#include "rmfunc.h"
#include "rneg/rneg.h"
#include "rselstr/rselstr.h"
#include "rline/rline.h"

namespace cf
{

static Handle hDebugCancelExtractBlocks = NULL;
static Handle hDebugCancelSearchPictures = NULL;
static Handle hDebugSVLines = NULL;
static Handle hDebugSVLinesStep = NULL;
static Handle hDebugSVLinesData = NULL;
static Handle hDebugLayoutFromFile = NULL;

RMarker::RMarker() :
    image_data_(NULL),
    cpage_(NULL),
    comp_cont_(NULL),
    cline_(NULL),
    kill_vsl_components_(true)
{
    RNEG_Init(0, NULL);
    image_data_ = new RMPreProcessImage;
}

RMarker::~RMarker()
{
    RNEG_Done();
    delete image_data_;
}

Handle RMarker::cpage() {
    return cpage_;
}

void RMarker::markupPage()
{
    static const size_t MEM_SIZE = 180000;
    static const size_t MAIN_SIZE = 500000;
    char * mem_buf = new char[MEM_SIZE];
    char * main_buf = new char[MAIN_SIZE];

    if(!RMARKER_PageMarkup(image_data_, mem_buf, MEM_SIZE, main_buf, MAIN_SIZE))
        throw Exception("markupPage failed");

    delete[] mem_buf;
    delete[] main_buf;

    cpage_ = image_data_->hCPAGE;
}

void RMarker::setComponentContainer(CCOM_cont * cont) {
    comp_cont_ = cont;
}

void RMarker::setCLine(Handle cline) {
    cline_ = cline;
}

void RMarker::setCPage(Handle cpage) {
    cpage_ = cpage;
}

void RMarker::setKillVSLComponents(bool value) {
    kill_vsl_components_ = value;
}

void RMarker::setLayoutFilename(const std::string& fname) {
    layout_filename_ = fname;
}

void RMarker::setOptions(const RecognizeOptions& opts) {
    image_data_->gbOneColumn = opts.oneColumn();
    image_data_->gKillVSLComponents = kill_vsl_components_;
    image_data_->hCPAGE = cpage_;
    image_data_->hCCOM = comp_cont_;
    image_data_->hCLINE = cline_;
    image_data_->gnPictures = opts.pictureSearch();
    image_data_->gnLanguage = opts.language();
    image_data_->hDebugCancelSearchPictures = hDebugCancelSearchPictures;
    image_data_->hDebugLayoutFromFile = hDebugLayoutFromFile;
    image_data_->hDebugCancelExtractBlocks = hDebugCancelExtractBlocks;
    image_data_->hDebugSVLines = hDebugSVLines;
    image_data_->hDebugSVLinesStep = hDebugSVLinesStep;
    image_data_->hDebugSVLinesData = hDebugSVLinesData;
    image_data_->szLayoutFileName = layout_filename_.c_str();
}

}

extern Handle hDebugPictures;
extern Handle hDebugNeg;
extern Handle hDebugLinePass3;
extern Handle hDebugLinePass2;
extern Handle hDebugVerifLine;
extern Handle hPrep;
extern Handle hEnd;

static uint32_t gwRC = 0;

Bool32 PageMarkup(PRMPreProcessImage Image)
{
    Bool32 rc = TRUE;
    gSVLBuffer.VLinefBufferA = NULL;
    gSVLBuffer.VLinefBufferB = NULL;
    gSVLBuffer.LineInfoA = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));
    gSVLBuffer.LineInfoB = (LinesTotalInfo*) calloc(1, sizeof(LinesTotalInfo));

    rc = ShortVerticalLinesProcess(PUMA_SVL_FIRST_STEP, Image);

    BigImage big_Image;
    //default Image:
    PAGEINFO info;
    GetPageInfo(Image->hCPAGE, &info);

    for (int i = 0; i < CPAGE_MAXNAME; i++)
        big_Image.ImageName[i] = info.szImageName[i];

    big_Image.hCCOM = NULL;
    Handle h = CPAGE_GetBlockFirst(Image->hCPAGE, TYPE_BIG_COMP);

    if (h) {
        CPAGE_GetBlockData(Image->hCPAGE, h, TYPE_BIG_COMP, &big_Image, sizeof(BigImage));
        CPAGE_DeleteBlock(Image->hCPAGE, h);
    }

    //Поиск очевидных картинок
    if (rc)
        rc = SearchPictures(Image, big_Image);

    //Поиск негативов
    if (rc)
        rc = SearchNeg(Image, big_Image, info.Incline2048);

    //Третий проход по линиям
    if (LDPUMA_Skip(hDebugLinePass3) && LDPUMA_Skip(hDebugVerifLine)
            && LDPUMA_Skip(hDebugLinePass2)) {
        if (rc)
            RLINE_LinesPass3(Image->hCPAGE, Image->hCLINE, Image->hCCOM, (uchar) Image->gnLanguage);
    }

    ////снова подсчитываем короткие вертикальные линии и сравниваем с предыдущим результатом
    if (rc) {
        rc = ShortVerticalLinesProcess(PUMA_SVL_SECOND_STEP, Image);
    }

    ShortVerticalLinesProcess(PUMA_SVL_THRID_STEP, Image);
    free(gSVLBuffer.LineInfoA);
    free(gSVLBuffer.LineInfoB);

    // blocks
    if (!LDPUMA_Skip(Image->hDebugLayoutFromFile)) {
        Image->hCPAGE = CPAGE_RestorePage(TRUE, (pchar) (Image->szLayoutFileName));

        if (Image->hCPAGE == NULL) {
            SetReturnCode_rmarker(CPAGE_GetReturnCode());
            rc = FALSE;
        }

        else {
            CPAGE_SetCurrentPage(CPAGE_GetNumberPage(Image->hCPAGE));
            LDPUMA_Console("Layout восстановлен из файла '%s'\n", Image->szLayoutFileName);
        }
    }
    else if (rc) {
        if (LDPUMA_Skip(Image->hDebugCancelExtractBlocks)) {
            Bool32 bEnableSearchPicture;
            bEnableSearchPicture = Image->gnPictures;
            RBLOCK_SetImportData(RBLOCK_Bool32_SearchPicture, &bEnableSearchPicture);
            RBLOCK_SetImportData(RBLOCK_Bool32_OneColumn, &(Image->gbOneColumn));

            if (!RBLOCK_ExtractTextBlocks(Image->hCCOM, Image->hCPAGE, Image->hCLINE)) {
                SetReturnCode_rmarker(RBLOCK_GetReturnCode());
                rc = FALSE;
            }
        }

        else
            LDPUMA_Console("Пропущен этап автоматического Layout.\n");
    }

    CCOM_DeleteContainer(big_Image.hCCOM);
    return rc;
}

void SetReturnCode_rmarker(uint32_t rc)
{
    gwRC = rc;
}

uint32_t GetReturnCode_rmarker(void)
{
    return gwRC;
}

Bool32 SearchNeg(PRMPreProcessImage Image, BigImage big_Image, int skew)
{
    if (!LDPUMA_Skip(hDebugNeg))
        return TRUE;

    RNEG_RecogNeg(big_Image.hCCOM, Image->hCPAGE, big_Image.ImageName, skew);
    return TRUE;
}

Bool32 SearchPictures(PRMPreProcessImage Image, BigImage big_Image)
{
    Bool32 rc = TRUE;

    if (!LDPUMA_Skip(hDebugPictures))
        return TRUE;

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

