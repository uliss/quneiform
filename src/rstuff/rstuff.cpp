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

#include "rstuff.h"
#include "rsfunc.h"
#include "rsglobaldata.h"
#include "rverline.h"
#include "smetric.h"
#include "rline.h"

#include "common/debug.h"

namespace CIF {

FixedBuffer<unsigned char, RStuff::MainBufferSize> RStuff::main_buffer_;
FixedBuffer<unsigned char, RStuff::WorkBufferSize> RStuff::work_buffer_;

RStuff::RStuff() {
    gLTInfo = new LinesTotalInfo;

    if (!SMetric_Init(0xFFFF, 0))
        throw RStuffException("SMetric_Init failed");
    if (!RLINE_Init(0, 0))
        throw RStuffException("RLINE_Init failed");

    SetMainBuff(main_buffer_.begin(), RStuff::MainBufferSize);
    SetWorkBuff(work_buffer_.begin(), RStuff::WorkBufferSize);
}

RStuff::~RStuff() {
    delete gLTInfo;
    gLTInfo = NULL;

    if (!SMetric_Done())
        return;

    if (!RLINE_Done())
        return;
}

void RStuff::binarize() {

}

void RStuff::layout() {
    //    Layout(image_);
}

void RStuff::normalize() {
    PreProcessImage(image_);
    SearchLines(image_);
    CalcIncline(image_);
    OrtoMove(image_);
    CreateContainerBigComp(image_);
    SearchNewLines(image_);
    KillLines(image_);
    // убиваем остатки линии после сняти
//    LineKiller(image_);
}

void RStuff::removeLines() {
    if (!(*image_->pgrc_line) || !(*image_->pgneed_clean_line))
        return;
    puchar pDIB = NULL;
    bool rc = RemoveLines(image_, &pDIB);
    if (rc)
        *image_->pgpRecogDIB = pDIB;
}

void RStuff::setImageData(RSPreProcessImage& data) {
    image_ = &data;
}

void RStuff::setSpecialProject(uchar NoSpecPrj) {
    db_spec_prj = NoSpecPrj;
}

void RStuff::verifyNormalization() {
    if (!(*image_->pgrc_line))
        return;

    int val = image_->gnTables ? RVL_FutuTablCorr : RVL_Default;

    if (!RVERLINE_SetImportData(RVERLINE_DTRVERLINE_RegimeOfVerifyLines, &val)
            || !RVERLINE_MarkLines(*image_->phCCOM, image_->hCPAGE))
        throw RStuffException("RStuff::verifyNormalization failed");

    Bool32 BadScan = FALSE;
    int32_t ScanQual = 0;
    AboutLines(image_, &BadScan, &ScanQual);

    if (!(*image_->pgneed_clean_line))
        Debug() << "Warning: RSTUFF said that line clean is unneeded\n";
}

}
