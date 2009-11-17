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

#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>

#include "cfcompat.h"
#include "rfrmt.h"
#include "crtfpage.h"
#include "rfrmt_prot.h"
#include "resource.h"
#include "dpuma.h"
#include "puma/formatoptions.h"

const char * gpSerifName = "MS Serif Cyr";
const char * gpSansSerifName = "MS Sans Serif Cyr";
const char * gpCourierName = "Courier Cyr";
Bool32 gbBold = TRUE;
Bool32 gbItalic = TRUE;
Bool32 gbSize = TRUE;
uint32_t gnFormat = 1; // 0 - не форматировать
// 1 - форматировать с фреймами и  колонками
// 2 - только фреймы

char UnRecogSymbol = '~';
uint32_t gnLanguage = LANG_RUSENG;

namespace CIF {

RFrmt::RFrmt() {
}

void RFrmt::format(const std::string& filename, Handle * edPage) {
    if (!RFRMT_Formatter(filename.c_str(), edPage))
        throw RFrmtException("RFRMT_Formatter failed");
}

void RFrmt::setLanguage(language_t language) {
    gnLanguage = language;
}

void RFrmt::setFormatOptions(const FormatOptions& opts) {
    static std::string sans, serif, mono;

    gbBold = opts.isBoldUsed();
    gbItalic = opts.isItalicUsed();
    gbSize = opts.isFontSizeUsed();
    UnRecogSymbol = (char) opts.unrecognizedChar();
    gnFormat = opts.formatMode();

    serif = opts.serifName();
    gpSerifName = serif.c_str();
    sans = opts.sansSerifName();
    gpSansSerifName = sans.c_str();
    mono = opts.monospaceName();
    gpCourierName = mono.c_str();
}

void RFrmt::saveRtf(const std::string& filename) {
    RFRMT_SaveRtf(filename.c_str(), 8);
}
}
