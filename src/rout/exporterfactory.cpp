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
#include "debugexporter.h"
#include "rtfexporter.h"
#include "edexporter.h"
#include "textexporter.h"
#include "puma/pumadef.h"

namespace CIF {

ExporterFactoryImpl::ExporterFactoryImpl() :
    page_(NULL) {
}

ExporterFactoryImpl::~ExporterFactoryImpl() {
}

void ExporterFactoryImpl::setFormatOptions(const FormatOptions& opts) {
    format_options_ = opts;
}

void ExporterFactoryImpl::setPage(Handle page) {
    page_ = page;
}

Exporter * ExporterFactoryImpl::make(int format) {
    switch (format) {
    case PUMA_DEBUG_TOTEXT:
        return new DebugExporter(format_options_);
        break;
    case PUMA_TORTF:
        return new RtfExporter(page_);
        break;
    case PUMA_TOEDNATIVE:
        return new EdExporter(page_);
        break;
    case PUMA_TOTEXT:
    case PUMA_TOSMARTTEXT:
    case PUMA_TOTABLETXT:
    case PUMA_TOTABLEDBF:
    case PUMA_TOHTML:
    case PUMA_TOHOCR:
        return new TextExporter(page_, format, format_options_);
        break;
    default:
        throw Exception("Unsupported export format");
    }
}

}
