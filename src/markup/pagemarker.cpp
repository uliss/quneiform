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
#include <boost/current_function.hpp>

#include "pagemarker.h"
#include "rmfunc.h"
#include "svlprocessor.h"
#include "dpuma.h"
#include "common/recognizeoptions.h"
#include "common/debug.h"
#include "common/cifconfig.h"
#include "lns/lnsdefs.h"
#include "rblock/rblock.h"
#include "rline/rline.h"
#include "rneg/rneg.h"
#include "rpic/rpic.h"
#include "rselstr/rselstr.h"

namespace cf
{

int PageMarker::flags_ = 0;

PageMarker::PageMarker() :
    image_data_(NULL),
    cpage_(NULL),
    comp_cont_(NULL),
    cline_(NULL)
{
    RNEG_Init(0, NULL);
    image_data_ = new RMPreProcessImage;

#ifndef _NDEBUG
    setFlag(DEBUG_SVL);
    setFlag(DEBUG_SVL_STEP);
    setFlag(DEBUG_SVL_DATA);
#endif
}

PageMarker::~PageMarker()
{
    RNEG_Done();
    delete image_data_;
}

Handle PageMarker::cpage() {
    return cpage_;
}

void PageMarker::extractBlocks()
{
    if(hasFlag(SKIP_EXTRACT_BLOCKS)) {
        Debug() << "Markup: automatic layout skipped.\n";
        return;
    }

    Bool32 search_picture = image_data_->searchPictures ? TRUE : FALSE;
    RBLOCK_SetImportData(RBLOCK_Bool32_SearchPicture, &search_picture);
    RBLOCK_SetImportData(RBLOCK_Bool32_OneColumn, &(image_data_->gbOneColumn));

    if (!RBLOCK_ExtractTextBlocks(image_data_->hCCOM, image_data_->hCPAGE, image_data_->hCLINE)) {
        Debug() << BOOST_CURRENT_FUNCTION
                << " RBLOCK_ExtractTextBlocks failed with code: "
                << RBLOCK_GetReturnCode() << std::endl;

        throw Exception("extractBlocks failed.");
    }
}

void PageMarker::linePass3()
{
    RLINE_LinesPass3(image_data_->hCPAGE,
                     image_data_->hCLINE,
                     image_data_->hCCOM,
                     (uchar) image_data_->gnLanguage);
}

void PageMarker::processShortVerticalLines()
{
    SVLProcessor processor(image_data_);

    // count short lines
    processor.countSVLStep1();

    BigImage big_image(image_data_->hCPAGE);

    // obvious pictures search
    searchPictures(big_image.hCCOM);
    searchNegatives(big_image.hCCOM);
    linePass3();

    // снова подсчитываем короткие вертикальные линии
    processor.countSVLStep2();
    // и сравниваем с предыдущим результатом
    processor.filter();
}

void PageMarker::restoreLayout()
{
    image_data_->hCPAGE = CPAGE_RestorePage(TRUE, layout_filename_.c_str());

    if (image_data_->hCPAGE == NULL) {
        Debug() << BOOST_CURRENT_FUNCTION
                << " CPAGE_RestorePage failed with code: "
                << CPAGE_GetReturnCode() << std::endl;

        throw Exception("CPAGE_RestorePage failed");
    }

    CPAGE_SetCurrentPage(CPAGE_GetNumberPage(image_data_->hCPAGE));
    Debug() << "Layout restored from file: \"" << layout_filename_ << "\"\n";
}

void PageMarker::markupPage()
{
    processShortVerticalLines();

    if (hasFlag(DEBUG_LAYOUT_FROM_FILE))
        restoreLayout();
    else
        extractBlocks();

    cpage_ = image_data_->hCPAGE;
}

void PageMarker::searchNegatives(CCOM_cont * cont)
{
    if(hasFlag(SKIP_SEARCH_PICTURES))
        return;

    assert(image_data_);

    PAGEINFO info;
    GetPageInfo(image_data_->hCPAGE, &info);

    RNEG_RecogNeg(cont, image_data_->hCPAGE, (uchar*) info.szImageName, info.Incline2048);
}

void PageMarker::searchPictures(CCOM_cont * contBig) {
    if(hasFlag(SKIP_SEARCH_PICTURES))
        return;

    if(!image_data_->searchPictures)
        return;

    if(!RPIC_SearchPictures(image_data_->hCCOM, contBig, image_data_->hCPAGE)) {
        Debug() << BOOST_CURRENT_FUNCTION
                << "RPIC_SearchPictures failed with code: "
                << RPIC_GetReturnCode() << "\n";

        throw Exception("Picture search failed.");
    }
}

void PageMarker::setComponentContainer(CCOM_cont * cont) {
    comp_cont_ = cont;
}

void PageMarker::setCLine(Handle cline) {
    cline_ = cline;
}

void PageMarker::setCPage(Handle cpage) {
    cpage_ = cpage;
}

void PageMarker::setKillVSLComponents(bool value) {
    image_data_->gKillVSLComponents = value;
}

void PageMarker::setLayoutFilename(const std::string& fname) {
    layout_filename_ = fname;
}

void PageMarker::setOptions(const RecognizeOptions& opts) {
    image_data_->gbOneColumn = opts.oneColumn();
    image_data_->hCPAGE = cpage_;
    image_data_->hCCOM = comp_cont_;
    image_data_->hCLINE = cline_;
    image_data_->searchPictures = opts.pictureSearch();
    image_data_->gnLanguage = opts.language();
    image_data_->hDebugSVLines = hasFlag(DEBUG_SVL);
    image_data_->hDebugSVLinesStep = hasFlag(DEBUG_SVL_STEP);
    image_data_->hDebugSVLinesData = hasFlag(DEBUG_SVL_DATA);
}

}
