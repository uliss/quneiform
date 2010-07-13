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
#include "xmltag.h"
#include "ced/cedparagraph.h"
#include "common/helper.h"
#include "common/tostring.h"

namespace CIF
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
    StyleExporter(page, opts) {
    setSkipEmptyParagraphs(true);
}

OdfStyleExporter::StylePtr OdfStyleExporter::makeOdfStyle(const CEDParagraph& par,
        const std::string& name) {
    StylePtr style(new XmlTag("style:style"));
    style->setAttribute("style:name", name);
    style->setAttribute("style:family", "paragraph");
    style->setAttribute("style:parent-style-name", "Standard");
    XmlTag * par_prop = new XmlTag("style:paragraph-properties");

    if (par.lineSpace() > 0)
        par_prop->setAttribute("style:line-spacing", pixel2odf(par.lineSpace()));

    if (!par.backgroundColor().isNull())
        par_prop->setAttribute("fo:background-color", color2odf(par.backgroundColor()));

    switch (par.align()) {
    case ALIGN_JUSTIFY:
        par_prop->setAttribute("fo:text-align", "justify");
        par_prop->setAttribute("style:justify-single-word", "false");
        break;
    case ALIGN_CENTER:
        par_prop->setAttribute("fo:text-align", "center");
        break;
    case ALIGN_RIGHT:
        par_prop->setAttribute("fo:text-align", "right");
    case ALIGN_LEFT:
        par_prop->setAttribute("fo:text-align", "left");
        break;
    default:
        break;
    }

    if (par.indent() > 0)
        par_prop->setAttribute("fo:text-indent", pixel2odf(par.indent()));

    //    par_prop->setAttribute("fo:margin-left", "4cm");

    XmlTag * text_prop = new XmlTag("style:text-properties");
    if (!par.color().isNull())
        text_prop->setAttribute("fo:color", color2odf(par.color()));

    style->addChild(par_prop);
    style->addChild(text_prop);
    return style;
}

std::string OdfStyleExporter::makeStyle(const CEDParagraph& par) {
    std::string par_style = StyleExporter::makeStyle(par);
    styles_[par_style] = makeOdfStyle(par, par_style);
    return par_style;
}

void OdfStyleExporter::writePageEnd(std::ostream& os, CEDPage&) {
    os << "<!-- cuneiform generated style -->\n";
    for (StyleMap::iterator it = styles_.begin(), end = styles_.end(); it != end; ++it) {
        os << *(it->second) << "\n";
    }
}

}
