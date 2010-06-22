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

#ifndef BLOCKELEMENT_H_
#define BLOCKELEMENT_H_

#include <vector>
#include <boost/shared_ptr.hpp>

#include "element.h"
#include "globus.h"

namespace CIF
{

class CLA_EXPO BlockElement: public Element
{
    public:
        BlockElement(Element * parent = NULL);
        ~BlockElement();

        typedef boost::shared_ptr<Element> ElementPtr;
        typedef std::vector<ElementPtr> ElementList;

        /**
         * Removes all element children
         */
        void clear();

        /**
         * Returns number of elements
         */
        size_t elementCount() const;

        /**
         * Returns true if element is empty
         */
        bool empty() const;
    protected:
        typedef ElementList::iterator iterator;
        Element * at(size_t pos);
        void push_back(ElementPtr e);
        void push_back(Element * e);
    private:
        ElementList elements_;
};

}

#endif /* BLOCKELEMENT_H_ */
