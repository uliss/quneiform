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

#include <cassert>
#include <algorithm>
#include "htmlexporter.h"
#include "common/helper.h"
#include "common/debug.h"
#include "ced/cedint.h"
#include "rout_own.h"
#include "xmltag.h"
#include "imageexporterfactory.h"
#include "config.h" // for CF_VERSION
namespace CIF
{

const std::string HTML_DOCTYPE("<!DOCTYPE html PUBLIC "
    "\"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
    "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    XmlExporter(page, opts) {

    ImageExporterPtr exp = ImageExporterFactory::instance().make();
    setImageExporter(exp);

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
    setSkipPictures(false);
    //    useIndents(true);
}

std::string HtmlExporter::fontStyleTag(int style) const {
    switch (style) {
    case FONT_BOLD:
        if (formatOptions().isBoldUsed())
            return "b";
        break;
    case FONT_ITALIC:
        if (formatOptions().isItalicUsed())
            return "i";
        break;
    case FONT_UNDERLINE:
        return "u";
    case FONT_SUB:
        return "sub";
    case FONT_SUPER:
        return "sup";
    default:
        return "";
    }
    return "";
}

void HtmlExporter::writeDoctype(std::ostream& os) {
    os << HTML_DOCTYPE;
}

void HtmlExporter::writeLineBreak(std::ostream& os, CEDLine * line) {
    // skip last line break
    if (lineLeftInParagraph() <= 1)
        return;

    if (isLineBreak(line))
        os << XmlTag("br") << "\n";
}

void HtmlExporter::writeMeta(std::ostream& os) {
    XmlTag meta_generator("meta");
    meta_generator["name"] = "Generator";
    meta_generator["content"] = "cuneiform-linux-" CF_VERSION;
    os << meta_generator << "\n";

    if (isCharsetConversion()) {
        XmlTag meta_charset("meta");
        meta_charset["name"] = "Content-Type";
        meta_charset["content"] = "text/html;charset=" + outputEncoding();
        os << meta_charset << "\n";
    }
}

void HtmlExporter::writePageBegin(std::ostream& os, CEDPage*) {
    writeDoctype(os);
    writeStartTag(os, "html", "\n");
    writeStartTag(os, "head", "\n");
    writeTitle(os);
    writeMeta(os);
    writeCloseTag(os, "head", "\n");
    writeStartTag(os, "body", "\n");

    if (os.fail())
        throw Exception("HtmlExporter failed");
}

void HtmlExporter::writePageEnd(std::ostream& os, CEDPage*) {
    writeCloseTag(os, "body", "\n");
    writeCloseTag(os, "html", "\n");
}

void HtmlExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    assert(par);

    XmlTag p("p");
    switch (par->alignment & ALIGN_MASK) {
    case ALIGN_CENTER:
        p["align"] = "center";
        break;
    case (ALIGN_LEFT | ALIGN_RIGHT):
        p["align"] = "justify";
        break;
    case ALIGN_RIGHT:
        p["align"] = "right";
    default:
        // "left" by default
        break;
    }

    p.writeBegin(os);
    TextExporter::writeParagraphBegin(os, par);
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    // writes closing tags to line buffer
    resetFontStyle(os);
    // write and flush line buffer to output stream
    writeLineBufferRaw(os);
    writeCloseTag(os, "p", "\n");
}

void HtmlExporter::writePicture(std::ostream& /*os*/, CEDChar * picture) {
    try {
        std::string path = savePicture(picture);
        assert(current_picture_);

        XmlTag img("img");
        img["src"] = escapeHtmlSpecialChars(path);
        img["alt"] = "";
        img["height"] = toString(current_picture_->pictSize.height());
        img["width"] = toString(current_picture_->pictSize.width());
        lineBuffer() << img << "\n";

    } catch (Exception& e) {
        Debug() << "[HtmlExporter::writePicture] failed: " << e.what() << std::endl;
    }
}

void HtmlExporter::writeTableBegin(std::ostream& os, CEDParagraph * /*table*/) {
    resetFontStyle(os);
    writeStartTag(os, "table");
}

void HtmlExporter::writeTableEnd(std::ostream& os, CEDParagraph * /*table*/) {
    writeCloseTag(os, "table");
}

void HtmlExporter::writeTitle(std::ostream& os) {
    os << XmlTag("title", "Cuneiform output") << "\n";
}

}
