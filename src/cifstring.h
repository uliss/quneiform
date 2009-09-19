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
#ifndef CIFSTRING_H
#define CIFSTRING_H

#include <string>
#include "element.h"

namespace CIF
{

/**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
class String : public Element
{
    public:
        enum Format {
            NONE = 0x0,
            ITALIC = 0x1,
            BOLD = 0x2,
            UNDERLINED = 0x4
        };

        enum FontSize {
            NORMAL,
            SMALL,
            BIG,
            HUGE
        };

        String(const std::string& str = "");
        ~String();
        void add(const std::string& text);
        std::string font() const;
        FontSize fontSize() const;
        int format() const;
        void exportTo(const Exporter& exp) const;
        void setFont(const std::string& value);
        void setFontSize(const FontSize& value);
        void setFormat(int flags);
        void setText(const std::string& value);
        std::string text() const;
    private:
        std::string m_text;
        std::string m_font;
        int m_format;
        FontSize m_fontsize;
};

}

#endif
