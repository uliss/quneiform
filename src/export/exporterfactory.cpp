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

#include "exporterfactory.h"
#include "htmlexporter.h"
#include "debugexporter.h"
#include "rtfexporter.h"
#include "edexporter.h"
#include "textexporter.h"
#include "hocrexporter.h"
#include "puma/pumadef.h"
#include "common/outputformat.h"
#include "config-user.h"

#ifdef CF_USE_ODF
#include "odfexporter.h"
#endif

namespace CIF
{

ExporterFactoryImpl::ExporterFactoryImpl() :
    page_(NULL) {
}

void ExporterFactoryImpl::setFormatOptions(const FormatOptions& opts) {
    format_options_ = opts;
}

void ExporterFactoryImpl::setPage(Handle page) {
    page_ = page;
}

Exporter * ExporterFactoryImpl::make(format_t format) {
    switch (format) {
    case FORMAT_DEBUG:
        return new DebugExporter(format_options_);
    case FORMAT_RTF:
        return new RtfExporter(page_);
    case FORMAT_EDNATIVE:
        return new EdExporter(page_);
    case FORMAT_HOCR:
        return new HocrExporter((CEDPage*) page_, format_options_);
    case FORMAT_HTML:
        return new HtmlExporter((CEDPage*) page_, format_options_);
    case FORMAT_SMARTTEXT:
        format_options_.setPreserveLineBreaks(true);
    case FORMAT_TEXT:
        return new TextExporter((CEDPage*) page_, format_options_);
#ifdef CF_USE_ODT
    case FORMAT_ODF:
        return new OdfExporter((CEDPage*) page_, format_options_);
#endif
    default:
        throw Exception("[ExporterFactoryImpl::make] Unsupported export format: " + OutputFormat::name(format), format);
    }
}

}
