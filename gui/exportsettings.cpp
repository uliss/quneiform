/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "exportsettings.h"
#include "common/outputformat.h"

ExportSettings::FormatMap ExportSettings::format_map_;

ExportSettings::ExportSettings(FormatType format) : format_(format)
{
    if(format_map_.isEmpty())
        initFormatMap();
}

int ExportSettings::cfFormatType() const {
    Q_ASSERT(format_map_.contains(format_));

    return format_map_.value(format_, cf::FORMAT_TEXT);
}

QString ExportSettings::extension() const {
    return cf::OutputFormat(static_cast<cf::format_t>(format_map_.value(format_, PLAINTEXT))).extension().c_str();
}

ExportSettings::FormatType ExportSettings::format() const {
    return format_;
}

void ExportSettings::initFormatMap() {
    format_map_.insert(PLAINTEXT, cf::FORMAT_TEXT);
    format_map_.insert(SMARTTEXT, cf::FORMAT_SMARTTEXT);
    format_map_.insert(HTML, cf::FORMAT_HTML);
    format_map_.insert(HOCR, cf::FORMAT_HOCR);
    format_map_.insert(ODF, cf::FORMAT_ODF);
    format_map_.insert(DJVUTXT, cf::FORMAT_DJVUTXT);
    format_map_.insert(DJVUXML, cf::FORMAT_DJVUXML);
    format_map_.insert(NATIVE, cf::FORMAT_NATIVE_TXT);
    format_map_.insert(NATIVEXML, cf::FORMAT_NATIVE_XML);
    format_map_.insert(FB2, cf::FORMAT_FB2);
}

void ExportSettings::setFormat(FormatType format) {
    format_ = format;
}

QList<ExportSettings::FormatType> ExportSettings::supportedFormats()
{
    return format_map_.keys();
}
