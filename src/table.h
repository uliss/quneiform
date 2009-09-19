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
#ifndef CIFTABLE_H
#define CIFTABLE_H

#include "blockelement.h"
#include <vector>

namespace CIF
{

/**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
class Table : public BlockElement
{
    public:
        Table();
        void addRaw(const std::vector<std::string>& raw);
        void exportTo(const Exporter& exp) const;
    private:
        bool isValid(const ElementPtr& ptr) const;
};

class Raw : public BlockElement
{
    public:
        void exportTo(const Exporter& exp) const;
    private:
        bool isValid(const ElementPtr& ptr) const;
};

class Cell : public BlockElement
{
    public:
        void exportTo(const Exporter& exp) const;
        bool isValid(const ElementPtr& el) const;
};

}

#endif
