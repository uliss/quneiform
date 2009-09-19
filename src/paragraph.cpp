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
#include "paragraph.h"
#include "cifstring.h"
#include "image.h"
#include <iostream>

using namespace std;

namespace CIF
{

Paragraph::Paragraph() : BlockElement() {}

Paragraph::Paragraph(const std::string& text) : BlockElement() {
    addText(text);
}

void Paragraph::addText(const string & text)
{
    if (empty()) {
        push(make_element(new String(text)));
    }
    else {
        String * sptr;
        if ((sptr = dynamic_cast<String*>(back())) != 0)
            sptr->add(text);
        else
            push(make_element(new String(text)));
    }
}

void Paragraph::exportTo(const Exporter& exp) const
{
    exp.exportParagraph(*this);
}


bool Paragraph::isValid(const ElementPtr & el) const
{
    if (
        dynamic_cast<String*>(el.get()) == 0 &&
        dynamic_cast<Image*>(el.get()) == 0
    )
        return false;
    return true;
}

}


