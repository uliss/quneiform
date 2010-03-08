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
#include "xmltag.h"
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

const std::string ODF_STYLE_JUSTIFY = "P1";
const std::string ODF_STYLE_BOLD = "BOLD";
const std::string ODF_STYLE_ITALIC = "ITALIC";

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
    XmlExporter(page, opts), zip_(NULL) {
    //useIndents(true);
}

OdfExporter::~OdfExporter() {
    odfClose();
}

void OdfExporter::addOdfAutomaticStyles(std::ostream& os) {
    XmlTag automatic_styles("office:automatic-styles");
    automatic_styles.writeBeginNL(os);

    os << "<style:style style:name=\"" << ODF_STYLE_JUSTIFY << "\" "
        "style:family=\"paragraph\" "
        "style:parent-style-name=\"Standard\">\n"
        "<style:paragraph-properties "
        "fo:text-align=\"start\" "
        "style:justify-single-word=\"false\"/>\n"
        "</style:style>\n";

    os << "<style:style style:name=\"" << ODF_STYLE_BOLD << "\" style:family=\"text\">\n"
        "<style:text-properties "
        "fo:font-weight=\"bold\" "
        "style:font-weight-asian=\"bold\" "
        "style:font-weight-complex=\"bold\"/>\n"
        "</style:style>\n";

    automatic_styles.writeEndNL(os);
}

void OdfExporter::addOdfContent() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);

    XmlTag doc_content("office:document-content");
    setCommonOdfNamespaces(doc_content);
    doc_content.writeBeginNL(buf);

    addOdfAutomaticStyles(buf);

    XmlTag body("office:body");
    body.writeBeginNL(buf);

    doExport(buf);

    body.writeEndNL(buf);
    doc_content.writeEndNL(buf);

    odfWrite("content.xml", buf.str());
    files_.push_back("content.xml");
}

void OdfExporter::addOdfManifest() {
    zip_add_dir(zip_, "META-INF");

    std::ostringstream buf;
    writeXmlDeclaration(buf);

    XmlTag manifest("manifest:manifest");
    setCommonOdfNamespaces(manifest);
    manifest.writeBeginNL(buf);

    {
        XmlTag file_entry("manifest:file-entry");
        file_entry["manifest:media-type"] = "application/vnd.oasis.opendocument.text";
        file_entry["manifest:full-path"] = "/";
        file_entry["manifest:version"] = "1.0";
        buf << file_entry << "\n";

        file_entry.clear();
        file_entry["manifest:media-type"] = "text/xml";
        for (size_t i = 0; i < files_.size(); i++) {
            file_entry["manifest:full-path"] = files_[i];
            buf << file_entry << "\n";
        }
    }

    manifest.writeEndNL(buf);
    odfWrite("META-INF/manifest.xml", buf.str());
}

void OdfExporter::addOdfMeta() {
    std::ostringstream buf;
    std::string time = datetime();
    writeXmlDeclaration(buf);

    XmlTag document_meta("office:document-meta");
    setCommonOdfNamespaces(document_meta);
    document_meta.writeBeginNL(buf);

    XmlTag office_meta("office:meta");
    office_meta.writeBeginNL(buf);

    buf << XmlTag("meta:creation-date", time) << "\n";
    buf << XmlTag("dc:date", time) << "\n";

    XmlTag stat("meta:document-statistic");
    stat["meta:table-count"] = "0";
    stat["meta:image-count"] = "0";
    stat["meta:object-count"] = "0";
    stat["meta:page-count"] = "1";
    stat["meta:paragraph-count"] = "0";
    stat["meta:word-count"] = "0";
    stat["meta:character-count"] = "0";

    buf << stat << "\n";

    buf << XmlTag("meta:generator", "Cuneiform-" CF_VERSION) << "\n";

    office_meta.writeEndNL(buf);
    document_meta.writeEndNL(buf);

    odfWrite("meta.xml", buf.str());
    files_.push_back("meta.xml");
}

void OdfExporter::addOdfMime() {
    odfWrite("mimetype", "application/vnd.oasis.opendocument.text");
}

void OdfExporter::addOdfSettings() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    XmlTag settings("office:document-settings");
    setCommonOdfNamespaces(settings);
    buf << settings << "\n";

    odfWrite("settings.xml", buf.str());
    files_.push_back("settings.xml");
}

void OdfExporter::addOdfStyles() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    XmlTag style("office:document-style");
    setCommonOdfNamespaces(style);
    buf << style << "\n";

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

std::string OdfExporter::fontStyleTag(int style) const {
    switch (style) {
    case FONT_ITALIC:
        return ODF_STYLE_ITALIC;
    case FONT_BOLD:
        return ODF_STYLE_BOLD;
    default:
        return "";
    }
}

void OdfExporter::writeFontStyleBegin(std::ostream& os, int style) {
    //    XmlTag span("text:span");
    //    span["text:style-name"] = fontStyleTag(style);
    //    span.writeBegin(os);
}

void OdfExporter::writeFontStyleEnd(std::ostream& os, int style) {
    //    XmlTag span("text:span");
    //    span.writeEnd(os);
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

void OdfExporter::setCommonOdfNamespaces(Tag& tag) const {
    tag["xmlns:manifest"] = "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0";
    tag["xmlns:office"] = "urn:oasis:names:tc:opendocument:xmlns:office:1.0";
    tag["xmlns:style"] = "urn:oasis:names:tc:opendocument:xmlns:style:1.0";
    tag["xmlns:text"] = "urn:oasis:names:tc:opendocument:xmlns:text:1.0";
    tag["xmlns:table"] = "urn:oasis:names:tc:opendocument:xmlns:table:1.0";
    tag["xmlns:xlink"] = "http://www.w3.org/1999/xlink";
    tag["xmlns:dc"] = "http://purl.org/dc/elements/1.1/";
    tag["xmlns:meta"] = "urn:oasis:names:tc:opendocument:xmlns:meta:1.0";
    tag["xmlns:number"] = "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0";
    tag["office:version"] = "1.2";
}

void OdfExporter::writeLineBreak(std::ostream& os, CEDLine * line) {
    // skip last line break
    if (lineLeftInParagraph() <= 1)
        return;

    if (isLineBreak(line))
        writeSingleTag(os, "text:line-break", Attributes(), "\n");
}

void OdfExporter::writePageBegin(std::ostream& os, CEDPage*) {
    writeStartTag(os, "office:text", "\n");
}

void OdfExporter::writePageEnd(std::ostream& os, CEDPage*) {
    writeCloseTag(os, "office:text", "\n");
}

void OdfExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    XmlTag p("text:p");
    switch (par->alignment & ALIGN_MASK) {
    case (ALIGN_LEFT | ALIGN_RIGHT):
        p["text:style-name"] = ODF_STYLE_JUSTIFY;
        break;
    }

    p.writeBegin(os);
    XmlExporter::writeParagraphBegin(os, par);
}

void OdfExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    resetFontStyle(os);
    writeLineBufferRaw(os);
    writeCloseTag(os, "text:p", "\n\n");
}

}
