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
#include "common/cifconfig.h"
#include "cpage/cpage.h"
#include "common/debug.h"
#include "memorybuffer.h"
#include "cline.h"
#include "un_buff.h"
#include "rverline.h"

namespace CIF {

const int RSL_AboutLines_SizeMyBuff = 492000;
const int RSL_AboutLines_SizeWorkMem = 180000;

Rsl::Rsl() :
    ccom_(NULL), cline_(NULL), cpage_(NULL), need_clean_line_(false), table_mode_(
            PUMA_TABLE_DEFAULT) {

}

Rsl::~Rsl() {

}

void Rsl::aboutLines() {
    UN_BUFF MainBuff;
    AutoBuffer<char> Buffer(RSL_AboutLines_SizeMyBuff);
    AutoBuffer<char> WorkMem(RSL_AboutLines_SizeWorkMem);

    /*  2. Инициализация.  */
    MainBuff.vBuff = Buffer.begin();
    MainBuff.SizeBuff = Buffer.size();
    MainBuff.vCurr = MainBuff.vBuff;
    MainBuff.SizeCurr = MainBuff.SizeBuff;

    need_clean_line_ = false;
    CLINE_handle hCLINE = *((CLINE_handle*) cline_);
    for (CLINE_handle hline = CLINE_GetFirstLine(hCLINE); hline; hline = CLINE_GetNextLine(hline)) {
        CPDLine cpdata = CLINE_GetLineData(hline);
        if (!cpdata)
            continue;
        if (cpdata->Flags & LI_IsTrue) {
            need_clean_line_ = true;
            break;
        }
    }

    if (Config::instance().debugHigh())
        Debug() << need_clean_line_
                ? "RSource: Нужно снять линии.\n"
                : "RSource: Не надо снимать линии!\n";

}

void Rsl::setCCom(Handle * ccom) {
    ccom_ = ccom;
}

void Rsl::setCLine(Handle * cline) {
    cline_ = cline;
}

void Rsl::setCPage(Handle cpage) {
    cpage_ = cpage;
}

void Rsl::setTableMode(puma_table_t mode) {
    table_mode_ = mode;
}

void Rsl::verifyNormalization() {
    //    if (!(*image_->pgrc_line))
    //        return;

    int val = table_mode_ ? RVL_FutuTablCorr : RVL_Default;

    if (!RVERLINE_SetImportData(RVERLINE_DTRVERLINE_RegimeOfVerifyLines, &val)
            || !RVERLINE_MarkLines(*ccom_, cpage_))
        throw RslException("RStuff::verifyNormalization failed");

    aboutLines();

    if (Config::instance().debug() && !need_clean_line_)
        Debug() << "Warning: RSL said that the lines don't need to be erased from the picture.\n";
}
}
