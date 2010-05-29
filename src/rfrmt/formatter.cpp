/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include <stdexcept>

#include "rfrmt.h"
#include "rfrmt_prot.h"

#include "formatter.h"
#include "rfrmtoptions.h"

#include "cfcompat.h"

extern Bool32 gbBold;
extern Bool32 gbItalic;
extern Bool32 gbSize;
extern uint32_t gnFormat;
extern const char* gpSerifName;
extern const char* gpSansSerifName;
extern const char* gpCourierName;
extern uint32_t CountTable;

std::string WriteRtfImageName;
uint32_t ExFlagMode;
std::string lpMyNameSerif;
std::string lpMyNameNonSerif;
std::string lpMyNameMono;

namespace CIF
{

Formatter::Formatter(const FormatOptions& opt) {
    setOptions(opt);
}

CEDPage * Formatter::format(const std::string& fileName) const {
    CEDPage * page = NULL;

    FILE *internal_file = create_temp_file();

    if (internal_file == NULL)
        throw std::runtime_error("[Formatter::format] could not create temp file");

    setInnerOptions();
    WriteRtfImageName = fileName;

    if (CreateInternalFileForFormatter(internal_file) == FALSE) {
        fclose(internal_file);
        throw std::runtime_error("[Formatter::format] cannot create format file");
    }

    if (gnFormat == 1 && ExFlagMode == FALSE)
        CIF::RfrmtOptions::setFlag(CIF::USE_FRAME_AND_COLUMN);
    else
        CIF::RfrmtOptions::setFlag(CIF::USE_NONE);

    lpMyNameSerif = gpSerifName;
    lpMyNameNonSerif = gpSansSerifName;
    lpMyNameMono = gpCourierName;

    if (!FullRtf(internal_file, NULL, &page)) {
        fclose(internal_file);
        throw std::runtime_error("[Formatter::format] formatting failed");
    }

    if (fclose(internal_file) != 0)
        throw std::runtime_error("[Formatter::format] cannot close format file");

    return page;
}

FormatOptions Formatter::options() const {
    return opts_;
}

void Formatter::setInnerOptions() const {
    ExFlagMode = FALSE;

    RfrmtOptions::setFormatMode(0);

    if (!opts_.isBoldUsed())
        RfrmtOptions::setFlag(NOBOLD);

    if (!opts_.isItalicUsed())
        RfrmtOptions::setFlag(NOCURSIV);

    if (!opts_.isFontSizeUsed())
        RfrmtOptions::setFlag(NOSIZE);

    // set to true in debug mode
    RfrmtOptions::setLineTransfer(false);
}

void Formatter::setOptions(const FormatOptions& opts) {
    opts_ = opts;
    RFRMT_SetFormatOptions(opts);
}

}
