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
#include "rout_own.h"
// ced
#include "ced/cedchar.h"
#include "ced/cedpage.h"
#include "ced/cedparagraph.h"
// common
#include "common/helper.h"
#include "common/tostring.h"

namespace cf
{

inline std::string pixel2odf(int pixel) {
    return toString(pixel / 11.9) + "mm";
}

inline std::string color2odf(const Color& c) {
    char buf[8];
    snprintf(buf, sizeof(buf), "#%02x%02x%02x", c.red(), c.green(), c.blue());
    return buf;
}

OdfStyleExporter::OdfStyleExporter(CEDPage * page, const FormatOptions& opts) :
    StyleExporter(page, opts), font_koef_(1.0) {
    setSkipEmptyParagraphs(true);
    if (page)
        font_koef_ = static_cast<float> (page->imageDpi().width() / 200.0);
}

int OdfStyleExporter::fontSize2odf(int value) const {
    return value * font_koef_;
}

std::string OdfStyleExporter::makeOdfStyle(const CEDChar& chr, const std::string& name) {
    std::ostringstream buf;

    XmlExporter::Attributes style_attrs;
    style_attrs["style:name"] = name;
    style_attrs["style:family"] = "text";
    style_attrs["style:parent-style-name"] = "Standard";
    XmlExporter::writeStartTag(buf, "style:style", style_attrs);

    XmlExporter::Attributes text_attr;

    if (!chr.color().isNull())
        text_attr["fo:color"] = color2odf(chr.color());

    if (!chr.color().isNull())
        text_attr["fo:background-color:"] = color2odf(chr.backgroundColor());

    if (chr.fontStyle() & FONT_ITALIC) {
        text_attr["fo:font-style"] = "italic";
        text_attr["style:font-style-asian"] = "italic";
        text_attr["style:font-style-complex"] = "italic";
    }

    if (chr.fontStyle() & FONT_BOLD) {
        text_attr["fo:font-weight"] = "bold";
        text_attr["style:font-weight-asian"] = "bold";
        text_attr["style:font-weight-complex"] = "bold";
    }

    if (chr.fontStyle() & FONT_UNDERLINE)
        text_attr["style:text-underline-type"] = "single";

    if (chr.fontStyle() & FONT_STRIKE)
        text_attr["style:text-line-through-type"] = "single";

    if (chr.fontStyle() & FONT_SUPER)
        text_attr["style:text-position"] = "super";

    if (chr.fontStyle() & FONT_SUB)
        text_attr["style:text-position"] = "sub";

    if (formatOptions().isFontSizeUsed() && chr.fontHeight() > 0) {
        std::string font_size = toString(fontSize2odf(chr.fontHeight()));
        text_attr["fo:font-size"] = font_size;
        text_attr["style:font-size-asian"] = font_size;
        text_attr["style:font-size-complex"] = font_size;
    }

    XmlExporter::writeSingleTag(buf, "style:text-properties", text_attr);
    buf << '\n';
    XmlExporter::writeCloseTag(buf, "style:style");
    return buf.str();
}

std::string OdfStyleExporter::makeOdfStyle(const CEDParagraph& par, const std::string& name) {
    std::ostringstream buf;

    XmlExporter::Attributes style_attrs;
    style_attrs["style:name"] = name;
    style_attrs["style:family"] = "paragraph";
    style_attrs["style:parent-style-name"] = "Standard";
    XmlExporter::writeStartTag(buf, "style:style", style_attrs);

    XmlExporter::Attributes par_attrs;

    if (par.lineSpace() > 0)
        par_attrs["style:line-spacing"] = pixel2odf(par.lineSpace());

    if (!par.backgroundColor().isNull())
        par_attrs["fo:background-color"] = color2odf(par.backgroundColor());

    switch (par.align()) {
    case ALIGN_JUSTIFY:
        par_attrs["fo:text-align"] = "justify";
        par_attrs["style:justify-single-word"] = "false";
        break;
    case ALIGN_CENTER:
        par_attrs["fo:text-align"] = "center";
        break;
    case ALIGN_RIGHT:
        par_attrs["fo:text-align"] = "right";
    case ALIGN_LEFT:
        par_attrs["fo:text-align"] = "left";
        break;
    default:
        break;
    }

    if (par.indent() > 0)
        par_attrs["fo:text-indent"] = pixel2odf(par.indent());
    XmlExporter::writeSingleTag(buf, "style:paragraph-properties", par_attrs);
    buf << '\n';

    XmlExporter::Attributes text_attrs;
    if (!par.color().isNull())
        text_attrs["fo:color"] = color2odf(par.color());
    XmlExporter::writeSingleTag(buf, "style:text-properties", text_attrs);
    buf << '\n';

    XmlExporter::writeCloseTag(buf, "style:style");
    return buf.str();
}

std::string OdfStyleExporter::makeStyle(const CEDChar& chr) {
    std::string chr_style = StyleExporter::makeStyle(chr);
    styles_[chr_style] = makeOdfStyle(chr, chr_style);
    return chr_style;
}

std::string OdfStyleExporter::makeStyle(const CEDParagraph& par) {
    std::string par_style = StyleExporter::makeStyle(par);
    styles_[par_style] = makeOdfStyle(par, par_style);
    return par_style;
}

void OdfStyleExporter::writePageEnd(CEDPage&) {
    assert(outputStream());

    *outputStream() << "<!-- cuneiform generated style -->\n";
    for (StyleMap::iterator it = styles_.begin(), end = styles_.end(); it != end; ++it)
        *outputStream() << it->second << '\n';
}

}
