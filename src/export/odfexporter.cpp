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

#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <ctime>
#include "odfexporter.h"
#include "rout_own.h"
#include "ced/cedchar.h"
#include "ced/cedparagraph.h"
#include "common/tostring.h"
#include "compat/filefunc.h"
#include "config.h" // for CF_VERSION
#ifndef CF_VERSION
#define CF_VERSION ""
#endif

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
    XmlExporter(page, opts), zip_(NULL), lines_left_(0) {
    //useIndents(true);
}

OdfExporter::~OdfExporter() {
    odfClose();
}

void OdfExporter::addOdfContent() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    Attributes attrs;
    setCommonOdfNamespaces(attrs);

    writeStartTag(buf, "office:document-content", attrs, "\n");
    writeStartTag(buf, "office:body");

    doExport(buf);

    writeCloseTag(buf, "office:body");
    writeCloseTag(buf, "office:document-content");

    odfWrite("content.xml", buf.str());
    files_.push_back("content.xml");
}

void OdfExporter::addOdfManifest() {
    zip_add_dir(zip_, "META-INF");

    std::ostringstream buf;
    writeXmlDeclaration(buf);
    Attributes attrs;
    setCommonOdfNamespaces(attrs);
    writeStartTag(buf, "manifest:manifest", attrs, "\n");

    {
        Attributes attrs;
        attrs["manifest:media-type"] = "application/vnd.oasis.opendocument.text";
        attrs["manifest:full-path"] = "/";
        attrs["manifest:version"] = "1.0";
        writeSingleTag(buf, "manifest:file-entry", attrs, "\n");

        attrs["manifest:media-type"] = "text/xml";

        for (size_t i = 0; i < files_.size(); i++) {
            attrs["manifest:full-path"] = files_[i];
            writeSingleTag(buf, "manifest:file-entry", attrs, "\n");
        }
    }

    writeCloseTag(buf, "manifest:manifest", "\n");
    odfWrite("META-INF/manifest.xml", buf.str());
}

void OdfExporter::addOdfMeta() {
    std::ostringstream buf;
    std::string time = datetime();
    writeXmlDeclaration(buf);
    Attributes attrs;
    setCommonOdfNamespaces(attrs);
    writeStartTag(buf, "office:document-meta", attrs, "\n");
    writeStartTag(buf, "office:meta", "\n");
    writeTag(buf, "meta:creation-date", time, Attributes(), "\n");
    writeTag(buf, "dc:date", time, Attributes(), "\n");

    Attributes stat;
    stat["meta:table-count"] = "0";
    stat["meta:image-count"] = "0";
    stat["meta:object-count"] = "0";
    stat["meta:page-count"] = "1";
    stat["meta:paragraph-count"] = "0";
    stat["meta:word-count"] = "0";
    stat["meta:character-count"] = "0";
    writeSingleTag(buf, "meta:document-statistic", stat, "\n");
    writeTag(buf, "meta:generator", "Cuneiform-" CF_VERSION, Attributes(), "\n");
    writeCloseTag(buf, "office:meta", "\n");
    writeCloseTag(buf, "office:document-meta", "\n");

    odfWrite("meta.xml", buf.str());
    files_.push_back("meta.xml");
}

void OdfExporter::addOdfMime() {
    odfWrite("mimetype", "application/vnd.oasis.opendocument.text");
}

void OdfExporter::addOdfSettings() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    Attributes attrs;
    setCommonOdfNamespaces(attrs);
    writeSingleTag(buf, "office:document-settings", attrs);

    odfWrite("settings.xml", buf.str());
    files_.push_back("settings.xml");
}

void OdfExporter::addOdfStyles() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    Attributes attrs;
    setCommonOdfNamespaces(attrs);
    writeSingleTag(buf, "office:document-style", attrs);

    odfWrite("styles.xml", buf.str());
    files_.push_back("styles.xml");
}

void OdfExporter::exportTo(const std::string& fname) {
    unlink(fname.c_str());
    odfOpen(fname);
    addOdfMime();
    addOdfMeta();
    addOdfSettings();
    addOdfStyles();
    addOdfContent();
    addOdfManifest();
    odfClose();
}

void OdfExporter::exportTo(std::ostream& os) {
    std::string tmpfile = "tmp.odt.$$$";
    exportTo(tmpfile);
    std::ifstream f(tmpfile.c_str());
    os << f.rdbuf();
    unlink(tmpfile.c_str());
}

void OdfExporter::odfClose() {
    zip_close(zip_);
    zip_ = NULL;
    buffers_.clear();
}

void OdfExporter::odfOpen(const std::string& fname) {
    int zip_error = 0;
    zip_ = zip_open(fname.c_str(), ZIP_CREATE, &zip_error);
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
    assert(zip_);
    BufList::iterator it = buffers_.insert(buffers_.end(), data);
    zip_source * src = zip_source_buffer(zip_, it->c_str(), it->size(), 0);

    if (!src)
        throw Exception("[OdfExporter::odfWrite] error: " + std::string(zip_strerror(zip_)));

    if (zip_add(zip_, fname.c_str(), src) < 0) {
        zip_source_free(src);
        throw Exception("[OdfExporter::addOdfMime] error: " + std::string(zip_strerror(zip_)));
    }
}

void OdfExporter::setCommonOdfNamespaces(Attributes& attrs) {
    attrs["xmlns:manifest"] = "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0";
    attrs["xmlns:office"] = "urn:oasis:names:tc:opendocument:xmlns:office:1.0";
    attrs["xmlns:style"] = "urn:oasis:names:tc:opendocument:xmlns:style:1.0";
    attrs["xmlns:text"] = "urn:oasis:names:tc:opendocument:xmlns:text:1.0";
    attrs["xmlns:table"] = "urn:oasis:names:tc:opendocument:xmlns:table:1.0";
    attrs["xmlns:xlink"] = "http://www.w3.org/1999/xlink";
    attrs["xmlns:dc"] = "http://purl.org/dc/elements/1.1/";
    attrs["xmlns:meta"] = "urn:oasis:names:tc:opendocument:xmlns:meta:1.0";
    attrs["xmlns:number"] = "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0";
    attrs["office:version"] = "1.2";
}

void OdfExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);
    // writeFontStyle(os, chr->fontAttribs);
    os << escapeSpecialChar(chr->alternatives->alternative);
}

void OdfExporter::writeLineBreak(std::ostream& os) {
    // skip last line break
    lines_left_--;
    if (lines_left_ < 1)
        return;

    if (isLineBreak())
        writeSingleTag(os, "text:line-break");
}

void OdfExporter::writePageBegin(std::ostream& os) {
    writeStartTag(os, "office:text", "\n");
}

void OdfExporter::writePageEnd(std::ostream& os) {
    writeCloseTag(os, "office:text", "\n");
}

void OdfExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    Attributes attrs;
    switch (par->alignment & ALIGN_MASK) {

    }

    writeStartTag(os, "text:p", attrs);
    lines_left_ = par->GetCountLine();
}

void OdfExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    writeCloseTag(os, "text:p", "\n");
}

}
