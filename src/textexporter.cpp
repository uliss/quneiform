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
#include "textexporter.h"
#include "document.h"
#include "image.h"
#include "paragraph.h"
#include "cifstring.h"
#include "table.h"

using namespace std;

namespace CIF
{

TextExporter::TextExporter(ostream& os) : Exporter(), m_os(os) {}
TextExporter::~TextExporter() {}

void TextExporter::exportCell(const Cell & cell) const
{
    cell.BlockElement::exportTo(*this);
}

void TextExporter::exportDocument(const Document & doc) const
{
    doc.BlockElement::exportTo(*this);
}

void TextExporter::exportImage(const Image & img) const {}


void TextExporter::exportPage(const Page & page) const
{
    page.BlockElement::exportTo(*this);
}

void TextExporter::exportParagraph(const Paragraph & par) const
{
    par.BlockElement::exportTo(*this);
}

void TextExporter::exportRaw(const Raw & raw) const
{
    for(BlockIteratorPtr it(raw.createIterator()); !it->isDone(); it->next()) {
        it->current()->exportTo(*this);
        m_os << "\t";
    }
    m_os << "\n";
}

void TextExporter::exportString(const String & str) const
{
    m_os << str.text();
}

void TextExporter::exportTable(const Table & table) const
{
    m_os << "\n";
    table.BlockElement::exportTo(*this);    
}

}

