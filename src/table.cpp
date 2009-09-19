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
#include "table.h"
#include "cifstring.h"
#include <iostream>

using namespace std;

namespace CIF
{

Table::Table()
        : BlockElement()
{}

void Table::addRaw(const vector<string> & raw)
{
    Raw * r = new Raw();
    ElementPtr rel(make_element(r));
    for (size_t i = 0; i < raw.size(); i++) {
        Cell * c = new Cell();
        String * s = new String(raw[i]);
        c->push(make_element(s));
        r->push(make_element(c));
    }
    push(rel);
}

void Table::exportTo(const Exporter & exp) const
{
    exp.exportTable(*this);
}

bool Table::isValid(const ElementPtr & ptr) const
{
    return dynamic_cast<const Raw*>(ptr.get()) != 0;
}

void Raw::exportTo(const Exporter & exp) const
{
    exp.exportRaw(*this);
}

bool Raw::isValid(const ElementPtr & ptr) const
{
    return dynamic_cast<Cell*>(ptr.get()) != 0;
}

void Cell::exportTo(const Exporter & exp) const
{
    exp.exportCell(*this);
}

bool Cell::isValid(const ElementPtr & el) const
{
    return true;  
}

}



