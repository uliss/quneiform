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
#include "ced/cedpage.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "common/tostring.h"
#include "common/helper.h"
#include "common/debug.h"
#include "common/fontstyle.h"
#include "compat/filefunc.h"
#include "config-version.h" // for CF_VERSION

namespace cf
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
    XmlExporter(page, opts),
    style_exporter_(new OdfStyleExporter(page, opts)),
    prev_char_style_hash_(0),
    style_span_opened_(false)
{
    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);
    setSkipPictures(false);
}

void OdfExporter::writeOdfAutomaticStyles() {
    writeStartTag("office:automatic-styles");
    style_exporter_->exportTo(buffer());
    writeCloseTag("office:automatic-styles");
}

void OdfExporter::addOdfContent() {
    makePicturesDir();

    // saving old output stream
    std::ostream * old_stream = outputStream();

    std::ostringstream content;
    writeXmlDeclaration(content);

    // setting new output stream
    setOutputStream(&content);

    Attributes doc_attrs;
    setCommonOdfNamespaces(doc_attrs);
    writeStartTag("office:document-content", doc_attrs);

    // export automatic styles
    writeOdfAutomaticStyles();

    // write body
    writeStartTag("office:body", "\n");

    // export document body
    doExport(content);

    writeCloseTag("office:body");
    writeCloseTag("office:document-content");
    flushBuffer();

    odfWrite("content.xml", content.str());
    addOdfManifestFile("content.xml", "text/xml");

    // restore output stream
    setOutputStream(old_stream);
}

void OdfExporter::addOdfManifest() {
    std::ostringstream buf;
    writeXmlDeclaration(buf);

    Attributes manifest;
    manifest["xmlns:manifest"] = "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0";
    writeStartTag(buf, "manifest:manifest", manifest);

    addOdfManifestFile("/", "application/vnd.oasis.opendocument.text");

    for (ManifestList::iterator it = files_.begin(), end = files_.end(); it != end; ++it) {
        Attributes attrs;
        attrs["manifest:full-path"] = it->first;
        attrs["manifest:media-type"] = it->second;
        writeSingleTag(buf, "manifest:file-entry", attrs);
    }

    writeCloseTag(buf, "manifest:manifest");

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
    zip_.setCompression("mimetype", ZipCpp::NONE);
}

void OdfExporter::addOdfSettings() {
    std::ostringstream buf;

    writeXmlDeclaration(buf);

    Attributes attrs;
    setCommonOdfNamespaces(attrs);
    writeSingleTag(buf, "office:document-settings", attrs);

    odfWrite("settings.xml", buf.str());
    addOdfManifestFile("settings.xml", "text/xml");
}

void OdfExporter::addOdfStyles() {
    std::ostringstream buf;

    writeXmlDeclaration(buf);

    Attributes attrs;
    setCommonOdfNamespaces(attrs);
    writeSingleTag(buf, "office:document-styles", attrs);

    odfWrite("styles.xml", buf.str());
    addOdfManifestFile("styles.xml", "text/xml");
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
    odfSave();
}

void OdfExporter::exportTo(std::ostream& os) {
    std::string tmpfile = "tmp.odt.$$$";
    exportTo(tmpfile);
    std::ifstream f(tmpfile.c_str());
    os << f.rdbuf();
    unlink(tmpfile.c_str());
}

void OdfExporter::makePicturesDir() {
    addOdfManifestFile(ODF_PICT_DIR, "");
}

std::string OdfExporter::makeSectionName() const {
    return std::string("Section ") + toString(section_counter_);
}

void OdfExporter::odfSave() {
    zip_.save(fname_);
}

void OdfExporter::odfOpen(const std::string& fname) {
    fname_ = fname;
}

void OdfExporter::odfWrite(const std::string& fname, const std::string& data) {
    zip_.addFile(fname);
    zip_.setContent(fname, data);
}

void OdfExporter::setCommonOdfNamespaces(Attributes& attrs) const {
    attrs["xmlns:manifest"] = "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0";
    attrs["xmlns:office"] = "urn:oasis:names:tc:opendocument:xmlns:office:1.0";
    attrs["xmlns:style"] = "urn:oasis:names:tc:opendocument:xmlns:style:1.0";
    attrs["xmlns:text"] = "urn:oasis:names:tc:opendocument:xmlns:text:1.0";
    attrs["xmlns:table"] = "urn:oasis:names:tc:opendocument:xmlns:table:1.0";
    attrs["xmlns:xlink"] = "http://www.w3.org/1999/xlink";
    attrs["xmlns:dc"] = "http://purl.org/dc/elements/1.1/";
    attrs["xmlns:meta"] = "urn:oasis:names:tc:opendocument:xmlns:meta:1.0";
    attrs["xmlns:number"] = "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0";
    attrs["xmlns:draw"] = "urn:oasis:names:tc:opendocument:xmlns:drawing:1.0";
    attrs["xmlns:svg"] = "urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0";
    attrs["xmlns:fo"] = "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0";
    attrs["office:version"] = "1.2";
}

void OdfExporter::writeCharacterBegin(CEDChar& chr) {
    size_t chr_hash = style_exporter_->hash(chr);

    if (style_exporter_->hasHash(chr_hash)) {
        if (prev_char_style_hash_ != chr_hash) {
            if (style_span_opened_) {
                writeCloseTag("text:span");
                style_span_opened_ = false;
            }

            Attributes attrs;
            attrs["text:style-name"] = style_exporter_->styleByHash(chr_hash);
            writeStartTag("text:span", attrs);
            style_span_opened_ = true;

            prev_char_style_hash_ = chr_hash;
        }
    }
}

void OdfExporter::writeLineBreak() {
    writeSingleTag("text:line-break", Attributes(), "\n");
}

void OdfExporter::writeMeta(std::ostream& os) {
    Attributes document_meta;
    setCommonOdfNamespaces(document_meta);

    writeStartTag(os, "office:document-meta", document_meta);
    writeStartTag(os, "office:meta");

    writeMetaGenerator(os);
    writeMetaDate(os);
    writeMetaStatistics(os);

    writeCloseTag(os, "office:meta");
    writeCloseTag(os, "office:document-meta");
}

void OdfExporter::writeMetaDate(std::ostream& os) {
    std::string time = datetime();
    writeTag(os, "meta:creation-date", time);
    writeTag(os, "dc:date", time);
}

void OdfExporter::writeMetaGenerator(std::ostream& os) {
    writeTag(os, "meta:generator", "Cuneiform-" CF_VERSION);
}

void OdfExporter::writeMetaStatistics(std::ostream& os) {
    Attributes attrs;
    attrs["meta:table-count"] = toString(numTables());
    attrs["meta:image-count"] = toString(numPictures());
    attrs["meta:object-count"] = "0";
    attrs["meta:page-count"] = "1";
    attrs["meta:paragraph-count"] = toString(numParagraphs());
    attrs["meta:word-count"] = "0";
    attrs["meta:character-count"] = toString(numChars());
    writeSingleTag(os, "meta:document-statistic", attrs);
}

void OdfExporter::writePageBegin(CEDPage&) {
    section_counter_ = 0;
    writeStartTag("office:text", "\n");
}

void OdfExporter::writePageEnd(CEDPage&) {
    writeCloseTag("office:text", "\n");
}

void OdfExporter::writeParagraphBegin(CEDParagraph& par) {
    Attributes attrs;
    std::string style_name = style_exporter_->styleByElement(par);
    if (!style_name.empty())
        attrs["text:style-name"] = style_name;

    writeStartTag("text:p", attrs);
    XmlExporter::writeParagraphBegin(par);

    prev_char_style_hash_ = 0;
    style_span_opened_ = false;
}

void OdfExporter::writeParagraphEnd(CEDParagraph&) {
    if (style_span_opened_)
        writeCloseTag("text:span");

    style_span_opened_ = false;

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

        Attributes frame_attrs;
        frame_attrs["text:anchor-type"] = "paragraph";
        frame_attrs["draw:frame-name"] = picture_name;
        frame_attrs["draw:name"] = picture_name;
        frame_attrs["draw:z-index"] = "0";
        float xdpi = (float) page()->imageDpi().width();
        float ydpi = (float) page()->imageDpi().height();
        assert(0 < xdpi && xdpi < 3000 && 0 < ydpi && ydpi < 3000);
        frame_attrs["svg:width"] = toString((float) picture.width() / xdpi) + "in";
        frame_attrs["svg:height"] = toString((float) picture.height() / ydpi) + "in";

        writeStartTag("draw:frame", frame_attrs);

        Attributes img_attrs;
        img_attrs["xlink:href"] = escapeHtmlSpecialChars(path);
        img_attrs["xlink:type"] = "simple";
        img_attrs["xlink:show"] = "embed";
        img_attrs["xlink:actuate"] = "onLoad";
        writeSingleTag("draw:image", img_attrs);

        writeCloseTag("draw:frame");

    } catch (Exception& e) {
        Debug() << "[OdfExporter::writePicture] failed: " << e.what() << std::endl;
    }
}

void OdfExporter::writeSectionBegin(CEDSection& section) {
    section_counter_++;

    Attributes attrs;
    attrs["text:name"] = makeSectionName();

    std::string style_name = style_exporter_->styleByElement(section);
    if (!style_name.empty())
        attrs["text:style-name"] = style_name;

    writeStartTag("text:section", attrs, "\n");
}

void OdfExporter::writeSectionEnd(CEDSection&) {
    writeCloseTag("text:section", "\n");
}

void OdfExporter::writeTableBegin(CEDTable&) {
    writeStartTag("table:table", "\n");
}

void OdfExporter::writeTableEnd(CEDTable&) {
    writeCloseTag("table:table", "\n");
}

}
