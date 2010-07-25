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
#include "odfstyleexporter.h"
#include "imageexporterfactory.h"
#include "rout_own.h"
#include "xmltag.h"
#include "ced/cedpage.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
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

static const std::string ODF_PICT_DIR = "Pictures/";

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
    XmlExporter(page, opts), zip_(NULL), style_exporter_(new OdfStyleExporter(page, opts)),
            prev_char_style_hash_(0), style_span_opened_(false) {
    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);
    setSkipPictures(false);
}

OdfExporter::~OdfExporter() {
    odfClose();
    delete style_exporter_;
}

void OdfExporter::addOdfAutomaticStyles() {
    XmlTag automatic_styles("office:automatic-styles");
    automatic_styles.writeBegin(outputStream());
    style_exporter_->exportTo(outputStream());
    automatic_styles.writeEnd(outputStream());
}

void OdfExporter::addOdfContent() {
    // saving old output stream
    std::ostream * old_stream = &outputStream();

    makePicturesDir();

    std::ostringstream buf;
    // setting new output stream
    setOutputStream(&buf);

    writeXmlDeclaration();
    XmlTag doc_content("office:document-content");
    setCommonOdfNamespaces(doc_content);
    doc_content.writeBegin(outputStream());
    addOdfAutomaticStyles();

    XmlTag body("office:body");
    body.writeBeginNL(outputStream());

    doExport(outputStream());

    body.writeEnd(outputStream());
    doc_content.writeEnd(outputStream());

    odfWrite("content.xml", buf.str());
    addOdfManifestFile("content.xml", "text/xml");

    // restore output stream
    setOutputStream(old_stream);
}

void OdfExporter::addOdfManifest() {
    // saving old output stream
    std::ostream * old_stream = &outputStream();

    zip_add_dir(zip_, "META-INF");

    std::ostringstream buf;
    // setting new output stream
    setOutputStream(&buf);

    writeXmlDeclaration();

    XmlTag manifest("manifest:manifest");
    manifest["xmlns:manifest"] = "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0";
    manifest.writeBeginNL(outputStream());

    addOdfManifestFile("/", "application/vnd.oasis.opendocument.text");

    XmlTag file_entry("manifest:file-entry");
    for (ManifestList::iterator it = files_.begin(), end = files_.end(); it != end; ++it) {
        file_entry["manifest:full-path"] = it->first;
        file_entry["manifest:media-type"] = it->second;
        outputStream() << file_entry << "\n";
    }

    manifest.writeEndNL(outputStream());
    odfWrite("META-INF/manifest.xml", buf.str());

    // restore output stream
    setOutputStream(old_stream);
}

void OdfExporter::addOdfManifestFile(const std::string& path, const std::string& type) {
    files_[path] = type;
}

void OdfExporter::addOdfMeta() {
    // saving old output stream
    std::ostream * old_stream = &outputStream();

    std::ostringstream buf;
    // setting new output stream
    setOutputStream(&buf);

    writeXmlDeclaration();
    writeMeta();
    odfWrite("meta.xml", buf.str());
    addOdfManifestFile("meta.xml", "text/xml");

    // restore output stream
    setOutputStream(old_stream);
}

void OdfExporter::addOdfMime() {
    odfWrite("mimetype", "application/vnd.oasis.opendocument.text");
}

void OdfExporter::addOdfSettings() {
    // saving old output stream
    std::ostream * old_stream = &outputStream();

    std::ostringstream buf;
    // setting new output stream
    setOutputStream(&buf);

    writeXmlDeclaration();
    XmlTag settings("office:document-settings");
    setCommonOdfNamespaces(settings);
    outputStream() << settings << "\n";

    odfWrite("settings.xml", buf.str());
    addOdfManifestFile("settings.xml", "text/xml");

    // restore output stream
    setOutputStream(old_stream);
}

void OdfExporter::addOdfStyles() {
    // saving old output stream
    std::ostream * old_stream = &outputStream();

    std::ostringstream buf;
    // setting new output stream
    setOutputStream(&buf);

    writeXmlDeclaration();

    XmlTag style("office:document-style");
    setCommonOdfNamespaces(style);
    outputStream() << style << "\n";

    odfWrite("styles.xml", buf.str());
    addOdfManifestFile("styles.xml", "text/xml");

    // restore output stream
    setOutputStream(old_stream);
}

void OdfExporter::exportTo(const std::string& fname) {
    setOutputFilename(fname);
    unlink(fname.c_str());
    odfOpen(fname);
    addOdfMime();
    addOdfSettings();
    addOdfStyles();
    addOdfContent();
    addOdfMeta();
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

void OdfExporter::makePicturesDir() {
    zip_add_dir(zip_, ODF_PICT_DIR.c_str());
    addOdfManifestFile(ODF_PICT_DIR, "");
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

void OdfExporter::writeCharacterBegin(CEDChar& chr) {
    size_t chr_hash = style_exporter_->hash(chr);

    if (style_exporter_->hasHash(chr_hash)) {
        if (prev_char_style_hash_ != chr_hash) {
            XmlTag span("text:span");

            if (style_span_opened_) {
                span.writeEnd(lineBuffer());
                style_span_opened_ = false;
            }

            span["text:style-name"] = style_exporter_->styleByHash(chr_hash);
            span.writeBegin(lineBuffer());
            style_span_opened_ = true;

            prev_char_style_hash_ = chr_hash;
        }
    }
}

void OdfExporter::writeLineBreak(CEDLine&) {
    writeSingleTag("text:line-break", Attributes(), "\n");
}

void OdfExporter::writeMeta() {
    XmlTag document_meta("office:document-meta");
    setCommonOdfNamespaces(document_meta);
    document_meta.writeBegin(outputStream());

    XmlTag office_meta("office:meta");
    office_meta.writeBegin(outputStream());

    outputStream() << XmlTag("meta:generator", "Cuneiform-" CF_VERSION) << "\n";
    writeMetaDate();
    writeMetaStatistics();

    office_meta.writeEnd(outputStream());
    document_meta.writeEnd(outputStream());
}

void OdfExporter::writeMetaDate() {
    std::string time = datetime();
    outputStream() << XmlTag("meta:creation-date", time) << "\n";
    outputStream() << XmlTag("dc:date", time) << "\n";
}

void OdfExporter::writeMetaStatistics() {
    XmlTag stat("meta:document-statistic");
    stat["meta:table-count"] = toString(numTables());
    stat["meta:image-count"] = toString(numPictures());
    stat["meta:object-count"] = "0";
    stat["meta:page-count"] = "1";
    stat["meta:paragraph-count"] = toString(numParagraphs());
    stat["meta:word-count"] = "0";
    stat["meta:character-count"] = toString(numChars());
    outputStream() << stat << "\n";
}

void OdfExporter::writePageBegin(CEDPage&) {
    writeStartTag("office:text", "\n");
    writeStartTag("text:page", "\n");
}

void OdfExporter::writePageEnd(CEDPage&) {
    writeCloseTag("text:page", "\n");
    writeCloseTag("office:text", "\n");
}

void OdfExporter::writeParagraphBegin(CEDParagraph& par) {
    XmlTag p("text:p");

    std::string style_name = style_exporter_->styleByElement(par);

    if (!style_name.empty())
        p["text:style-name"] = style_name;

    p.writeBegin(outputStream());
    XmlExporter::writeParagraphBegin(par);

    prev_char_style_hash_ = 0;
    style_span_opened_ = false;
}

void OdfExporter::writeParagraphEnd(CEDParagraph&) {
    if (style_span_opened_)
        lineBuffer() << "</text:span>";
    style_span_opened_ = false;

    writeLineBufferRaw();
    writeCloseTag("text:p", "\n");
}

void OdfExporter::writePicture(CEDPicture& picture) {
    try {
        std::ostringstream img_buf;
        savePictureData(picture, img_buf);
        // uliss: TODO seems not optimal copying
        std::string picture_name = makePictureName(picture);
        std::string path = ODF_PICT_DIR + picture_name;
        odfWrite(path, img_buf.str());
        addOdfManifestFile(path, imageExporter()->mime());

        XmlTag img("draw:image");
        img["xlink:href"] = escapeHtmlSpecialChars(path);
        img["xlink:type"] = "simple";
        img["xlink:show"] = "embed";
        img["xlink:actuate"] = "onLoad";

        XmlTag frame("draw:frame");
        frame["text:anchor-type"] = "paragraph";
        frame["draw:frame-name"] = picture_name;
        frame["draw:name"] = picture_name;
        frame["draw:z-index"] = "0";
        float xdpi = (float) page()->imageDpi().width();
        float ydpi = (float) page()->imageDpi().height();
        assert(0 < xdpi && xdpi < 3000 && 0 < ydpi && ydpi < 3000);
        frame["svg:width"] = toString((float) picture.width() / xdpi) + "in";
        frame["svg:height"] = toString((float) picture.height() / ydpi) + "in";

        frame.writeBegin(outputStream());
        outputStream() << img;
        frame.writeEnd(outputStream());

    } catch (Exception& e) {
        Debug() << "[OdfExporter::writePicture] failed: " << e.what() << std::endl;
    }
}

void OdfExporter::writeSectionBegin(CEDSection&) {
    //    writeStartTag(os, "text:section", "\n");
}

void OdfExporter::writeSectionEnd(CEDSection&) {
    //    writeCloseTag(os, "text:section", "\n");
}

void OdfExporter::writeTableBegin(CEDTable&) {
    writeStartTag("table:table", "\n");
}

void OdfExporter::writeTableEnd(CEDTable&) {
    writeCloseTag("table:table", "\n");
}

}
