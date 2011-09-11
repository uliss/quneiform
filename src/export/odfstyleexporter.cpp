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

#include <cstdio>
#include "odfstyleexporter.h"
#include "xmlexporter.h"
#include "cfcompat.h"
// ced
#include "ced/cedchar.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
#include "ced/cedsection.h"
// common
#include "common/helper.h"
#include "common/tostring.h"
#include "common/fontstyle.h"

namespace cf
{

static inline std::string pixel2odf(int pixel) {
    return toString(pixel / 11.9) + "mm";
}

static inline std::string color2odf(const Color& c) {
    char buf[8];
    snprintf(buf, sizeof(buf), "#%02x%02x%02x", c.red(), c.green(), c.blue());
    return buf;
}

static void writeTextProperties(std::ostream& os, const XmlExporter::Attributes& attrs);

static void writeParagraphProperties(std::ostream& os, const CEDParagraph& p) {
    XmlExporter::Attributes attrs;

    if (p.lineSpace() > 0)
        attrs["style:line-spacing"] = pixel2odf(p.lineSpace());

    if (!p.backgroundColor().isNull())
        attrs["fo:background-color"] = color2odf(p.backgroundColor());

    switch (p.align()) {
    case ALIGN_JUSTIFY:
        attrs["fo:text-align"] = "justify";
        attrs["style:justify-single-word"] = "false";
        break;
    case ALIGN_CENTER:
        attrs["fo:text-align"] = "center";
        break;
    case ALIGN_RIGHT:
        attrs["fo:text-align"] = "right";
    case ALIGN_LEFT:
        attrs["fo:text-align"] = "left";
        break;
    default:
        break;
    }

    if (p.indent() > 0)
        attrs["fo:text-indent"] = pixel2odf(p.indent());

    XmlExporter::writeSingleTag(os, "style:paragraph-properties", attrs);
}

static void writeParagraphTextProperties(std::ostream& os, const CEDParagraph& p) {
    XmlExporter::Attributes text_attrs;
    if (!p.color().isNull())
        text_attrs["fo:color"] = color2odf(p.color());
    writeTextProperties(os, text_attrs);
}

static void writeSectionPropertiesStart(std::ostream& os) {
    XmlExporter::Attributes prop_attr;
    prop_attr["style:editable"] = "false";
    XmlExporter::writeStartTag(os, "style:section-properties", prop_attr);
}

static void writeSectionPropertiesClose(std::ostream& os) {
    XmlExporter::writeCloseTag(os, "style:section-properties");
}

static void writeSectionProperties(std::ostream& os, const CEDSection& s) {
    writeSectionPropertiesStart(os);

    XmlExporter::Attributes col_attr;
    col_attr["fo:column-count"] = toString(s.columnCount());
    col_attr["fo:column-gap"] = "0.8cm";
    XmlExporter::writeStartTag(os, "style:columns", col_attr);
    XmlExporter::writeCloseTag(os, "style:columns");

    writeSectionPropertiesClose(os);
}

static void writeStyleClose(std::ostream& os) {
    XmlExporter::writeCloseTag(os, "style:style");
}

static void writeStyleStart(std::ostream& os, const std::string& name, const std::string& family) {
    XmlExporter::Attributes attrs;
    attrs["style:name"] = name;
    attrs["style:family"] = family;
    attrs["style:parent-style-name"] = "Standard";
    XmlExporter::writeStartTag(os, "style:style", attrs);
}

static void writeTextProperties(std::ostream& os, const XmlExporter::Attributes& attrs) {
    XmlExporter::writeSingleTag(os, "style:text-properties", attrs);
}

OdfStyleExporter::OdfStyleExporter(CEDPagePtr page, const FormatOptions& opts) :
    StyleExporter(page, opts), font_koef_(1.0) {
    setSkipEmptyParagraphs(true);
    if (page)
        font_koef_ = static_cast<float> (page->imageDpi().width() / 200.0);
}

int OdfStyleExporter::fontSize2odf(int value) const {
    return static_cast<int> (value * font_koef_);
}

std::string OdfStyleExporter::makeStyleContent(const CEDChar& chr, const std::string& name) {
    std::ostringstream buf;

    writeStyleStart(buf, name, "text");
    writeTextProperties(buf, chr);
    writeStyleClose(buf);
    buf << std::endl;

    return buf.str();
}

std::string OdfStyleExporter::makeStyleContent(const CEDParagraph& par, const std::string& name) {
    std::ostringstream buf;

    writeStyleStart(buf, name, "paragraph");
    writeParagraphProperties(buf, par);
    writeParagraphTextProperties(buf, par);
    writeStyleClose(buf);
    buf << std::endl;

    return buf.str();
}

std::string OdfStyleExporter::makeStyleContent(const CEDSection& s, const std::string& name) {
    std::ostringstream buf;

    writeStyleStart(buf, name, "section");
    writeSectionProperties(buf, s);
    writeStyleClose(buf);
    buf << std::endl;

    return buf.str();
}

std::string OdfStyleExporter::makeStyle(const CEDChar& chr) {
    std::string chr_style = StyleExporter::makeStyle(chr);
    styles_[chr_style] = makeStyleContent(chr, chr_style);
    return chr_style;
}

std::string OdfStyleExporter::makeStyle(const CEDParagraph& par) {
    std::string par_style = StyleExporter::makeStyle(par);
    styles_[par_style] = makeStyleContent(par, par_style);
    return par_style;
}

std::string OdfStyleExporter::makeStyle(const CEDSection& s) {
    std::string sec_style = StyleExporter::makeStyle(s);
    styles_[sec_style] = makeStyleContent(s, sec_style);
    return sec_style;
}

void OdfStyleExporter::writePageEnd(CEDPage&) {
    assert(outputStream());

    *outputStream() << "<!-- cuneiform generated style -->\n";
    for (StyleMap::iterator it = styles_.begin(), end = styles_.end(); it != end; ++it)
        *outputStream() << it->second << '\n';
}

void OdfStyleExporter::writeTextProperties(std::ostream& os, const CEDChar& c) {
    XmlExporter::Attributes attrs;

    if (!c.color().isNull())
        attrs["fo:color"] = color2odf(c.color());

    if (!c.color().isNull())
        attrs["fo:background-color:"] = color2odf(c.backgroundColor());

    if (c.fontStyle() & FONT_ITALIC) {
        attrs["fo:font-style"] = "italic";
        attrs["style:font-style-asian"] = "italic";
        attrs["style:font-style-complex"] = "italic";
    }

    if (c.fontStyle() & FONT_BOLD) {
        attrs["fo:font-weight"] = "bold";
        attrs["style:font-weight-asian"] = "bold";
        attrs["style:font-weight-complex"] = "bold";
    }

    if (c.fontStyle() & FONT_UNDERLINE)
        attrs["style:text-underline-type"] = "single";

    if (c.fontStyle() & FONT_STRIKE)
        attrs["style:text-line-through-type"] = "single";

    if (c.fontStyle() & FONT_SUPER)
        attrs["style:text-position"] = "super";

    if (c.fontStyle() & FONT_SUB)
        attrs["style:text-position"] = "sub";

    if (formatOptions().isFontSizeUsed() && c.fontHeight() > 0) {
        std::string font_size = toString(fontSize2odf(c.fontHeight()));
        attrs["fo:font-size"] = font_size;
        attrs["style:font-size-asian"] = font_size;
        attrs["style:font-size-complex"] = font_size;
    }

    cf::writeTextProperties(os, attrs);
}

}
