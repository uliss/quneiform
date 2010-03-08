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
#include "imageexporterfactory.h"
#include "rout_own.h"
#include "xmltag.h"
#include "ced/cedpage.h"
#include "ced/cedchar.h"
#include "ced/cedparagraph.h"
#include "common/tostring.h"
#include "common/helper.h"
#include "common/debug.h"
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
const std::string ODF_PICT_DIR = "Pictures/";

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

    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);

    //useIndents(true);
    setSkipPictures(false);
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
    addOdfManifestFile("content.xml", "text/xml");
}

void OdfExporter::addOdfManifest() {
    zip_add_dir(zip_, "META-INF");

    std::ostringstream buf;
    writeXmlDeclaration(buf);

    XmlTag manifest("manifest:manifest");
    manifest["xmlns:manifest"] = "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0";
    manifest.writeBeginNL(buf);

    addOdfManifestFile("/", "application/vnd.oasis.opendocument.text");

    for (ManifestList::iterator it = files_.begin(), end = files_.end(); it != end; ++it) {
        XmlTag file_entry("manifest:file-entry");
        if (it->first == "/")
            file_entry["manifest:version"] = "1.0";

        file_entry["manifest:full-path"] = it->first;
        file_entry["manifest:media-type"] = it->second;
        buf << file_entry << "\n";
    }

    manifest.writeEndNL(buf);
    odfWrite("META-INF/manifest.xml", buf.str());
}

void OdfExporter::addOdfManifestFile(const std::string& path, const std::string& type) {
    files_[path] = type;
}

void OdfExporter::addOdfMeta() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    writeMeta(buf);
    odfWrite("meta.xml", buf.str());
    addOdfManifestFile("meta.xml", "text/xml");
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
    addOdfManifestFile("settings.xml", "text/xml");
}

void OdfExporter::addOdfStyles() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);
    XmlTag style("office:document-style");
    setCommonOdfNamespaces(style);
    buf << style << "\n";

    odfWrite("styles.xml", buf.str());
    addOdfManifestFile("styles.xml", "text/xml");
}

void OdfExporter::exportTo(const std::string& fname) {
    setOutputFilename(fname);
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

    zip_add_dir(zip_, ODF_PICT_DIR.c_str());
    addOdfManifestFile(ODF_PICT_DIR, "");
}

void OdfExporter::odfWrite(const std::string& fname, const std::string& data) {
    assert(zip_);
    BufList::iterator it = buffers_.insert(buffers_.end(), data);
    zip_source * src = zip_source_buffer(zip_, it->c_str(), it->size(), 0);

    if (!src)
        throw Exception("[OdfExporter::odfWrite] error: " + std::string(zip_strerror(zip_)));

    if (zip_add(zip_, fname.data(), src) < 0) {
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
    tag["xmlns:draw"] = "urn:oasis:names:tc:opendocument:xmlns:drawing:1.0";
    tag["xmlns:svg"] = "urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0";
    tag["xmlns:fo"] = "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0";
    tag["office:version"] = "1.2";
}

void OdfExporter::writeLineBreak(std::ostream& os, CEDLine * line) {
    // skip last line break
    if (lineLeftInParagraph() <= 1)
        return;

    if (isLineBreak(line))
        writeSingleTag(os, "text:line-break", Attributes(), "\n");
}

void OdfExporter::writeMeta(std::ostream& os) {
    XmlTag document_meta("office:document-meta");
    setCommonOdfNamespaces(document_meta);
    document_meta.writeBeginNL(os);

    XmlTag office_meta("office:meta");
    office_meta.writeBeginNL(os);

    os << XmlTag("meta:generator", "Cuneiform-" CF_VERSION) << "\n";
    writeMetaDate(os);
    writeMetaStatistics(os);

    office_meta.writeEndNL(os);
    document_meta.writeEndNL(os);
}

void OdfExporter::writeMetaDate(std::ostream& os) {
    std::string time = datetime();
    os << XmlTag("meta:creation-date", time) << "\n";
    os << XmlTag("dc:date", time) << "\n";
}

void OdfExporter::writeMetaStatistics(std::ostream& os) {
    XmlTag stat("meta:document-statistic");
    stat["meta:table-count"] = toString(numTables());
    stat["meta:image-count"] = toString(numPictures());
    stat["meta:object-count"] = "0";
    stat["meta:page-count"] = "1";
    stat["meta:paragraph-count"] = toString(numParagraphs());
    stat["meta:word-count"] = "0";
    stat["meta:character-count"] = toString(numChars());
    os << stat << "\n";
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
    writeCloseTag(os, "text:p", "");
}

void OdfExporter::writePicture(std::ostream& os, CEDChar * picture) {
    try {
        std::ostringstream img_buf;
        savePictureData(picture, img_buf);
        // uliss: TODO seems not optimal copying
        std::string path = ODF_PICT_DIR + pictureName(picture);
        odfWrite(path, img_buf.str());
        addOdfManifestFile(path, imageExporter()->mime());

        XmlTag img("draw:image");
        img["xlink:href"] = escapeHtmlSpecialChars(path);
        img["xlink:type"] = "simple";
        img["xlink:show"] = "embed";
        img["xlink:actuate"] = "onLoad";

        XmlTag frame("draw:frame");
        frame["text:anchor-type"] = "paragraph";
        frame["draw:name"] = pictureName(picture);
        frame["draw:z-index"] = "0";
        float xdpi = (float) page()->imageDpi().width();
        float ydpi = (float) page()->imageDpi().height();
        assert(0 < xdpi && xdpi < 3000 && 0 < ydpi && ydpi < 3000);
        frame["svg:width"] = toString((float) last_picture_size_.width() / xdpi) + "in";
        frame["svg:height"] = toString((float) last_picture_size_.height() / ydpi) + "in";

        frame.writeBeginNL(os);
        os << img << "\n";
        frame.writeEndNL(os);

    } catch (Exception& e) {
        Debug() << "[OdfExporter::writePicture] failed: " << e.what() << std::endl;
    }
}

}
