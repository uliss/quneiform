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
#include "document.h"
#include <cassert>

using namespace std;

namespace CIF
{

Document::Document()
        : m_encoding("utf8"),
        m_title("CIFocr document"),
        m_author("CIFocr")
{
    setLanguage("english");
}

void Document::addLanguage(const string & lang)
{
    m_langs.push_back(lang);
}

string Document::author() const
{
    return m_author;
}

void Document::exportTo(Exporter& exp) const
{
    exp.exportDocument(*this);
}

string Document::encoding() const
{
    return m_encoding;
}

Document::LangList Document::languageList() const
{
    return m_langs;
}

void Document::setAuthor(const string & author)
{
    m_author = author;
}

void Document::setEncoding(const string& value)
{
    m_encoding = value;
}

void Document::setLanguage(const string & lang)
{
    m_langs.clear();
    m_langs.push_back(lang);
}

void Document::setTitle(const string& value)
{
    m_title = value;
}

string Document::title() const
{
    return m_title;
}

bool Document::isValid(const ElementPtr & ptr) const
{
    if (dynamic_cast<Page*>(ptr.get()) != 0)
        return true;
    return false;
}

}

