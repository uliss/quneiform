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

#include "rfrmt_prot.h"

#include "formatter.h"
#include "rfrmtoptions.h"
#include "crtfpage.h"

#include "cfcompat.h"

namespace CIF
{

bool Formatter::extended_mode_ = false;
char Formatter::unrecognized_char = '~';

Formatter::Formatter(const FormatOptions& opt) {
    setOptions(opt);
}

CEDPage * Formatter::format(const std::string& fileName) {
    FILE * fp = create_temp_file();

    if (fp == NULL)
        throw std::runtime_error("[Formatter::format] could not create temp file");

    setInnerOptions();

    if (!writeFormatFile(fp)) {
        fclose(fp);
        throw std::runtime_error("[Formatter::format] cannot write format file");
    }

    CEDPage * page = readFormatFile(fileName, fp);

    if (fclose(fp) != 0)
        throw std::runtime_error("[Formatter::format] cannot close format file");

    return page;
}

FormatOptions Formatter::options() const {
    return opts_;
}

CEDPage * Formatter::readFormatFile(const std::string& imageName, FILE * fp) {
    CRtfPage page;

    if (RfrmtOptions::hasFlag(USE_FRAME_AND_COLUMN)) {
        if (!page.FindPageTree(fp, NULL))
            throw std::runtime_error("[Formatter::readFormatFile] read error");
    } else if (!page.ReadInternalFile(fp))
        throw std::runtime_error("[Formatter::readFormatFile] read error");

    page.setFormatOptions(opts_);
    page.setImageName(imageName);

    page.CorrectKegl();
    page.ChangeKegl();

    //  page.addTables();
    page.addPictures();

    // in manual layout user can establish own order of the fragments
    if (RfrmtOptions::useNone())
        page.SortUserNumber();

    return page.Write();
}

void Formatter::setFontOptions() const {
    if (!opts_.isBoldUsed())
        RfrmtOptions::setFlag(NOBOLD);

    if (!opts_.isItalicUsed())
        RfrmtOptions::setFlag(NOCURSIV);

    if (!opts_.isFontSizeUsed())
        RfrmtOptions::setFlag(NOSIZE);
}

void Formatter::setInnerOptions() const {
    setFontOptions();

    extended_mode_ = false;

    RfrmtOptions::setFormatMode(0);

    // set to true in debug mode
    RfrmtOptions::setLineTransfer(false);

    if (opts_.formatMode() == PUMA_FORMAT_ALL && extended_mode_ == FALSE)
        RfrmtOptions::setFlag(USE_FRAME_AND_COLUMN);
    else
        RfrmtOptions::setFlag(USE_NONE);

}

void Formatter::setOptions(const FormatOptions& opts) {
    opts_ = opts;
    unrecognized_char = (char) opts.unrecognizedChar();
}

bool Formatter::writeFormatFile(FILE * fp) {
    return CreateInternalFileForFormatter(fp) == TRUE;
}

}
