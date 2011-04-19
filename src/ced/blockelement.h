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
#include <boost/serialization/shared_ptr.hpp>
#endif

namespace cf
{

class CLA_EXPO BlockElement: public Element
{
    public:
        BlockElement(BlockElement * parent = NULL);

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
          * Checks is given element exists in this block
          */
        bool hasElement(Element * e);

        /**
         * Returns pointer to last element in container
         * @throw std::out_of_range if container is empty
         */
        Element * lastElement();
        const Element * lastElement() const;

        /**
          * Returns page bottom margin
          * @see marginTop(), marginLeft(), marginRight()
          */
        int marginBottom() const;

        /**
          * Returns page left margin
          * @see marginBottom(), marginRight(), marginTop()
          */
        int marginLeft() const;

        /**
          * Returns page right margin
          * @see marginBottom(), marginLeft(), marginTop()
          */
        int marginRight() const;

        /**
          * Returns page top margin
          * @see marginBottom(), marginLeft(), marginRight()
          */
        int marginTop() const;

        /**
          * Returns page margins as a rectangle
          * @see marginBottom(), marginLeft(), marginRight(), marginTop()
          */
        Rect margins() const;

        void setBorderBottom(ed_border_t bottom);
        void setBorderBottomWidth(unsigned short width);
        void setBorderLeft(ed_border_t left);
        void setBorderLeftWidth(unsigned short width);
        void setBorderRight(ed_border_t right);
        void setBorderRightWidth(unsigned short width);
        void setBorderTop(ed_border_t top);
        void setBorderTopWidth(unsigned short width);
        void setMargins(const Rect& margins);
        void setMargins(int top, int right, int bottom, int left);
        void setMarginBottom(int bottom);
        void setMarginTop(int top);
        void updateBoundingRect();
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("element", boost::serialization::base_object<Element>(*this));
            ar & make_nvp("children", elements_);
            ar & make_nvp("border-left", border_left_);
            ar & make_nvp("border-top", border_top_);
            ar & make_nvp("border-right", border_right_);
            ar & make_nvp("border-bottom", border_bottom_);
            ar & make_nvp("border-left-width", border_left_wd_);
            ar & make_nvp("border-top-width", border_top_wd_);
            ar & make_nvp("border-right-width", border_right_wd_);
            ar & make_nvp("border-bottom-width", border_bottom_wd_);
            ar & make_nvp("margins", margins_);
        }
#endif
    protected:
        typedef ElementList::iterator iterator;
        typedef ElementList::const_iterator const_iterator;
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
        Rect margins_;
};

}

#endif /* BLOCKELEMENT_H_ */
