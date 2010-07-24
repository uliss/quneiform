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

#include "ced.h"
#include "element.h"
#include "globus.h"
#include "common/serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif

namespace CIF
{

class CLA_EXPO BlockElement: public Element
{
    public:
        BlockElement(BlockElement * parent = NULL);
        ~BlockElement();

        typedef boost::shared_ptr<Element> ElementPtr;
        typedef std::vector<ElementPtr> ElementList;

        /**
         * Adds element to the and of block
         */
        void addElement(ElementPtr e);
        void addElement(Element * e);

        /**
         * Removes all element children
         */
        void clear();

        /**
         * Returns pointer to element at given position
         */
        Element * elementAt(size_t pos);

        /**
         * Returns number of elements
         */
        size_t elementCount() const;

        /**
         * Returns true if element is empty
         */
        bool empty() const;

        void exportChildren(CEDExporter& exp);

        /**
         * Returns pointer to last element in container
         * @throw std::out_of_range if container is empty
         */
        Element * lastElement();
        const Element * lastElement() const;

        void setBorderBottom(ed_border_t bottom);
        void setBorderBottomWidth(unsigned short width);
        void setBorderLeft(ed_border_t left);
        void setBorderLeftWidth(unsigned short width);
        void setBorderRight(ed_border_t right);
        void setBorderRightWidth(unsigned short width);
        void setBorderTop(ed_border_t top);
        void setBorderTopWidth(unsigned short width);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar & boost::serialization::base_object<Element>(*this);
            ar & elements_;
            ar & border_left_;
            ar & border_top_;
            ar & border_right_;
            ar & border_bottom_;
            ar & border_left_wd_;
            ar & border_top_wd_;
            ar & border_right_wd_;
            ar & border_bottom_wd_;
        }
#endif
    protected:
        typedef ElementList::iterator iterator;
    private:
        ElementList elements_;
        char border_left_;
        char border_top_;
        char border_right_;
        char border_bottom_;
        unsigned short border_left_wd_;
        unsigned short border_top_wd_;
        unsigned short border_right_wd_;
        unsigned short border_bottom_wd_;
};

}

#endif /* BLOCKELEMENT_H_ */
