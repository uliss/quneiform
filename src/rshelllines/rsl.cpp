/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <cassert>

#include "rsl.h"
#include "rstuff/rstuff.h"
#include "cpage/cpage.h"
#include "common/debug.h"
#include "memorybuffer.h"
#include "cline.h"
#include "un_buff.h"
#include "rverline.h"

namespace CIF {

const int RSL_AboutLines_SizeMyBuff = 492000;
const int RSL_AboutLines_SizeWorkMem = 180000;

Rsl::Rsl() {

}

Rsl::~Rsl() {

}

void Rsl::aboutLines() {
    assert(image_);

    UN_BUFF MainBuff;
    AutoBuffer<char> Buffer(RSL_AboutLines_SizeMyBuff);
    AutoBuffer<char> WorkMem(RSL_AboutLines_SizeWorkMem);

    do {
        if (image_->pgneed_clean_line == NULL)
            return;

        /*  2. Инициализация.  */
        MainBuff.vBuff = Buffer.begin();
        MainBuff.SizeBuff = Buffer.size();
        MainBuff.vCurr = MainBuff.vBuff;
        MainBuff.SizeCurr = MainBuff.SizeBuff;

        *image_->pgneed_clean_line = FALSE;
        CLINE_handle hCLINE = *((CLINE_handle*) (image_->phCLINE));
        for (CLINE_handle hline = CLINE_GetFirstLine(hCLINE); hline; hline = CLINE_GetNextLine(
                hline)) {
            CPDLine cpdata = CLINE_GetLineData(hline);
            if (!cpdata)
                continue;
            if (cpdata->Flags & LI_IsTrue) {
                *image_->pgneed_clean_line = TRUE;
                break;
            }
        }

        if (*image_->pgneed_clean_line)
            Debug() << "RSource: Нужно снять линии.\n";
        else
            Debug() << "RSource: Не надо снимать линии!\n";
    }
    while (false);
}

void Rsl::setImage(RSPreProcessImage& image) {
    image_ = &image;
}

void Rsl::verifyNormalization() {
    assert(image_);

    if (!(*image_->pgrc_line))
        return;

    int val = image_->gnTables ? RVL_FutuTablCorr : RVL_Default;

    if (!RVERLINE_SetImportData(RVERLINE_DTRVERLINE_RegimeOfVerifyLines, &val)
            || !RVERLINE_MarkLines(*image_->phCCOM, image_->hCPAGE))
        throw RslException("RStuff::verifyNormalization failed");

    aboutLines();

    if (!(*image_->pgneed_clean_line))
        Debug() << "Warning: RSL said that the lines don't need to be erased from the picture.\n";
}
}
