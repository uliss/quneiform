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

#include <sstream>
#include <cassert>
#include <ctime>
#include "common/debug.h"
#include "odfexporter.h"
#include "config.h" // for CF_VERSION
namespace CIF
{

std::string datetime(const std::string& format = "%Y-%m-%dT%H:%M:%S") {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[100];
    const size_t buffer_length = sizeof(buffer) - 2;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, buffer_length, format.c_str(), timeinfo);
    buffer[buffer_length - 1] = '\0';
    return buffer;
}

OdfExporter::OdfExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), zip_(NULL) {
}

OdfExporter::~OdfExporter() {
    odfClose();
}

void OdfExporter::addOdfMeta() {
    std::ostringstream buf;
    std::string time = datetime();
    buf << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<office:document-meta xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
        " xmlns:xlink=\"http://www.w3.org/1999/xlink\""
        " xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
        " xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\""
        " office:version=\"1.2\">"
        "<office:meta><meta:creation-date>" << time << "</meta:creation-date>"
        "<meta:document-statistic meta:table-count=\"0\" meta:image-count=\"0\""
        " meta:object-count=\"0\" meta:page-count=\"1\" meta:paragraph-count=\"0\""
        " meta:word-count=\"0\" meta:character-count=\"0\"/>"
        " <dc:date>" << time << "</dc:date>";
    buf << "<meta:generator>Cuneiform-" << CF_VERSION << "</meta:generator>";
    buf << "</office:meta></office:document-meta>";

    odfWrite("meta.xml", buf.str());
}

void OdfExporter::addOdfMime() {
    odfWrite("mimetype", "application/vnd.oasis.opendocument.text");
}

void OdfExporter::exportTo(const std::string& fname) {
    odfOpen(fname);
    addOdfMeta();
    addOdfMime();
    odfClose();
}

void OdfExporter::exportTo(std::ostream& /*os*/) {
    throw Exception("[OdfExporter::exportTo] export to stream not supported yet");
}

void OdfExporter::odfClose() {
    if (zip_) {
        if (-1 == zip_close(zip_))
            Debug() << "[OdfExporter::odfClose] close error\n";
        zip_ = NULL;
    }
}

void OdfExporter::odfOpen(const std::string& fname) {
    int zip_error;
    zip_ = zip_open(fname.c_str(), ZIP_CREATE /*| ZIP_EXCL*/, &zip_error);
    if (!zip_) {
        switch (zip_error) {
        case ZIP_ER_EXISTS:
            throw Exception("[OdfExporter::odfOpen] file already exists: " + fname);
        default:
            throw Exception("[OdfExporter::odfOpen] can't open file: " + fname);
        }
    }
}

void OdfExporter::odfWrite(const std::string& fname, const std::string& data) {
    zip_source * src = zip_source_buffer(zip_, data.c_str(), data.length(), 0);

    if (!src)
        throw Exception("[OdfExporter::odfWrite] error: " + std::string(zip_strerror(zip_)));

    if (zip_add(zip_, fname.c_str(), src) < 0) {
        zip_source_free(src);
        throw Exception("[OdfExporter::addOdfMime] error: " + std::string(zip_strerror(zip_)));
    }

    Debug() << data;
}

}
