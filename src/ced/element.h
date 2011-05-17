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

#ifndef CIF_ELEMENT_H_
#define CIF_ELEMENT_H_

#include "globus.h"
#include "common/color.h"
#include "common/rect.h"
#include "common/serialize.h"

namespace cf
{

class CEDExporter;

class CLA_EXPO Element
{
    private:
        Element(const Element&);
        const Element& operator=(const Element&);
    public:
        Element(Element * parent = NULL);
        virtual ~Element();
    public:
        virtual void exportElement(CEDExporter& exp) = 0;
        virtual void exportChildren(CEDExporter& exp);

        /**
         * Returns element background color
         */
        const Color& backgroundColor() const;

        /**
         * Returns reference to bounding rectangle of element
         */
        Rect& boundingRect();

        /**
         * Returns reference to bounding rectangle of element
         */
        const Rect& boundingRect() const;

        /**
         * Returns element color
         */
        const Color& color() const;

        /**
         * Returns element height
         * @see width(), boundingRect()
         */
        int height() const;

        /**
         * Returns pointer to parent element
         * @see setParent()
         */
        Element * parent();
        const Element * parent() const;

        /**
         * Sets element background color
         * @see backgroundColor(), setColor(), color()
         */
        void setBackgroundColor(const Color& c);

        /**
         * Sets element bounding rectangle
         * @see boundingRect()
         */
        void setBoundingRect(const Rect& r);

        /**
         * Sets element color
         * @see color(), setBackgroundColor()
         */
        void setColor(const Color& c);

        /**
         * Sets parent object
         * @see parent()
         */
        void setParent(Element * parent);

        /**
         * Sets element size
         * @see boundingRect()
         */
        void setSize(const Size& sz);

        /**
         * Updates element bounding rectangle
         */
        virtual void updateBoundingRect();

        /**
         * Returns element width
         * @see height()
         */
        int width() const;
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            //ar & parent_;
            ar & make_nvp("color", color_);
            ar & make_nvp("background-color", bgcolor_);
            ar & make_nvp("bounding-rect", brect_);
        }
#endif
    private:
        Element * parent_;
        Color color_;
        Color bgcolor_;
        Rect brect_;
};

}

#endif /* CIF_ELEMENT_H_ */
