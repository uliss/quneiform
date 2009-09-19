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
#include "cifstring.h"
using namespace std;

namespace CIF
{

String::String(const std::string& str)
        : Element(), 
        m_text(str),
        m_font(),
        m_format(NONE),
        m_fontsize(NORMAL)        
{}

String::~String() {}

void String::add(const std::string & text)
{
    m_text += text;
}
void String::exportTo(const Exporter& exp) const
{
    exp.exportString(*this);
}

string String::text() const
{
    return m_text;
}

void String::setText(const string& value)
{
    m_text = value;
}

string String::font() const
{
    return m_font;
}

void String::setFont(const string& value)
{
    m_font = value;
}

int String::format() const
{
    return m_format;
}

void String::setFormat(int value)
{
    m_format = value;
}

String::FontSize String::fontSize() const
{
    return m_fontsize;
}

void String::setFontSize(const FontSize& value)
{
    m_fontsize = value;
}


}

