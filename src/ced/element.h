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

namespace CIF
{

class CEDExporter;

class CLA_EXPO Element
{
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
         * Returns bounding rectangle of element
         */
        const Rect& boundingRect() const;

        /**
         * Returns element color
         */
        const Color& color() const;

        /**
         * Returns element height
         */
        int height() const;

        /**
         * Returns pointer to parent element
         */
        Element * parent();
        const Element * parent() const;

        /**
         * Sets element background color
         */
        void setBackgroundColor(const Color& c);

        /**
         * Sets element bounding rectangle
         */
        void setBoundingRect(const Rect& r);

        /**
         * Sets element color
         */
        void setColor(const Color& c);

        /**
         * Sets parent object
         */
        void setParent(Element * parent);

        /**
         * Returns element width
         */
        int width() const;
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            //ar & parent_;
            ar & color_;
            ar & bgcolor_;
            ar & brect_;
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
