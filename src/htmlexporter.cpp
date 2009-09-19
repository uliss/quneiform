/***************************************************************************
 *   Copyright (C) 2008 by Сергей Полтавский   *
 *   poltava@gentoo   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "htmlexporter.h"
#include "document.h"
#include "image.h"
#include "paragraph.h"
#include "cifstring.h"
#include "table.h"

#include <ctime>
#include <iostream>
#include <sstream>

using namespace std;

namespace CIF
{

HtmlExporter::HtmlExporter(ostream& os)
        : Exporter(),
        m_out(os)
{}

HtmlExporter::~HtmlExporter()
{
    flush(m_out);
}

void str_replace(string& str, const string& from, const string& to)
{
    size_t pos_start = 0, pos_found;
    while ((pos_found = str.find(from, pos_start)) != string::npos) {
        str.replace(pos_found, 1, to);
        pos_start = pos_found + to.length();
    }
}

string HtmlExporter::escape(const string & str) const
{
    string ret(str);
    str_replace(ret, "&", "&amp;");
    str_replace(ret, "<", "&lt;");
    str_replace(ret, ">", "&gt;");
    str_replace(ret, "\"", "&quot;");
    return ret;
}

void HtmlExporter::exportCell(const Cell & cell) const
{
    m_out << "<td>";
    cell.BlockElement::exportTo(*this);
    m_out << "</td>";
}

void HtmlExporter::exportHead(const string& title, const string& encoding) const
{
    m_out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\""
    "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" << endl;
    m_out << "<html>\n<head>\n";
    m_out << "<meta name=\"Generator\" content=\"CIFocr\"/>\n";
    m_out << "<meta name=\"Generation-date\" content=\"" << datetime() << "\"/>\n";
    m_out << "<meta http-equiv=\"Content-type\" content=\"text/html; charset=" << encoding << "\"/>\n";
    m_out << "<title>" << escape(title) << "</title>\n";
    m_out << "<style>\n"
    "\ttable{border:solid 1px gray;border-collapse:collapse;margin:10pt 0;}\n"
    "\ttd{border:solid 1px gray;}\n"
    "</style>\n";
    m_out << "</head>\n<body>\n";
}

void HtmlExporter::exportDocument(const Document & doc) const
{
    exportHead(doc.title(), doc.encoding());

    for (BlockIteratorPtr it(doc.createIterator()); !it->isDone(); it->next())
        it->current()->exportTo(*this);

    m_out << "\n</body>\n</html>\n";
}

void HtmlExporter::exportImage(const Image & img) const
{
    string fname = generate_image_name("png");
    m_out << "<img src=\"" << fname << "\" alt=\"\" height=\"" << img.height() << "\" width=\"" << img.width() << "\"/>\n";
    if (isSkipImagesSave())
        return;
    img.save(fname);
}

void HtmlExporter::exportPage(const Page & page) const
{
    m_out << "<!-- page -->\n";
    for (BlockIteratorPtr it(page.createIterator()); !it->isDone(); it->next())
        it->current()->exportTo(*this);
}

void HtmlExporter::exportParagraph(const Paragraph & par) const
{
    m_out << "\n<p>";
    par.BlockElement::exportTo(*this);
    m_out << "</p>\n";
}

void HtmlExporter::exportString(const String & str) const
{
    int fmt = str.format();

    if (!str.font().empty())
        m_out << "<span style=\"font-family:" << escape(str.font()) << "\">";
    if (fmt & String::UNDERLINED) m_out << "<i>";
    if (fmt & String::BOLD) m_out << "<b>";
    if (fmt & String::ITALIC) m_out << "<i>";

    m_out << escape(str.text());

    if (fmt & String::ITALIC) m_out << "</i>";
    if (fmt & String::BOLD) m_out << "</b>";
    if (fmt & String::UNDERLINED) m_out << "</u>";
    if (!str.font().empty())
        m_out << "</font>";
}

void HtmlExporter::exportRaw(const Raw & raw) const
{
    m_out << "<tr>";
    raw.BlockElement::exportTo(*this);
    m_out << "</tr>\n";
}

void HtmlExporter::exportTable(const Table & table) const
{
    m_out << "<table>\n";
    m_out << "<tbody>\n";
    table.BlockElement::exportTo(*this);
    m_out << "</tbody>\n";
    m_out << "</table>\n";
}

}

