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
#include "djvutxtexporter.h"
#include "djvuxmlexporter.h"
#include "textexporter.h"
#include "hocrexporter.h"

#include "common/serialize.h"
#ifdef CF_SERIALIZE
#include "cuneiformexporter.h"
#include "cuneiformxmlexporter.h"
#endif

#include "smarttextexporter.h"
#include "summaryexporter.h"
#include "puma/pumadef.h"
#include "ced/cedpage.h"
#include "common/outputformat.h"
#include "odfexporter.h"
#include "fb2exporter.h"

namespace cf
{

ExporterFactoryImpl::ExporterFactoryImpl() {
}

void ExporterFactoryImpl::setFormatOptions(const FormatOptions& opts) {
    format_options_ = opts;
}

void ExporterFactoryImpl::setPage(CEDPagePtr page) {
    page_ = page;
}

ExporterPtr ExporterFactoryImpl::make(format_t format) {
    ExporterPtr exp;
    switch (format) {
    case FORMAT_DEBUG:
        exp.reset(new DebugExporter(format_options_));
        break;
    case FORMAT_HOCR:
        exp.reset(new HocrExporter(page_, format_options_));
        break;
    case FORMAT_HTML:
        exp.reset(new HtmlExporter(page_, format_options_));
        break;
    case FORMAT_SMARTTEXT:
        exp.reset(new SmartTextExporter(page_, format_options_));
        break;
    case FORMAT_TEXT:
        exp.reset(new TextExporter(page_, format_options_));
        break;
    case FORMAT_ODF:
        exp.reset(new OdfExporter(page_, format_options_));
        break;
    case FORMAT_SUMMARY:
        exp.reset(new SummaryExporter(page_, format_options_));
        break;
    case FORMAT_DJVUTXT:
        exp.reset(new DjvuTxtExporter(page_, format_options_));
        break;
    case FORMAT_DJVUXML:
        exp.reset(new DjvuXmlExporter(page_, format_options_));
        break;
#ifdef CF_SERIALIZE
    case FORMAT_NATIVE_TXT:
        exp.reset(new CuneiformExporter(page_, format_options_));
        break;
    case FORMAT_NATIVE_XML:
        exp.reset(new CuneiformXmlExporter(page_, format_options_));
        break;
#endif
    case FORMAT_FB2:
        exp.reset(new FB2Exporter(page_, format_options_));
        break;
    default:
        throw Exception("[ExporterFactoryImpl::make] Unsupported export format: "
                + OutputFormat::name(format), format);
    }
    return exp;
}

}
