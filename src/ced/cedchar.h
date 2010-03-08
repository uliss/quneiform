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

#ifndef CEDCHAR_H_
#define CEDCHAR_H_

#include "globus.h"
#include "ced_struct.h"
#include "common/rect.h"

struct text_ref;

namespace CIF
{

class FUN_EXPO__ CEDChar
{
    public:
        CEDChar();

        /**
         * Returns character bounding rectangle
         */
        Rect boundingRect() const;

        /**
         * Returns font height
         */
        int fontHeight() const;

        /**
         * Checks if picture
         */
        bool isPicture() const;

        /**
         * Returns character parent number
         */
        int parentNumber() const;

        /**
         * Returns reference to character bounding rectangle
         */
        Rect& rBoundingRect();

        /**
         * Sets bounding rectangle
         */
        void setBoundingRect(const Rect& bbox);

        /**
         * Sets bounding rectabgle
         */
        void setBoundingRect(const EDBOX& bbox);

        /**
         * Sets font height
         */
        void setFontHeight(int height);

        /**
         * Sets character parent number
         */
        void setParentNumber(int number);

        CEDChar * prev, *next; //pointer to neibor elements in connected list
        int fontAttribs; //font parameters
        int fontNum;
        int fontLang;
        int foregroundColor;
        int backgroundColor;
        letterEx * alternatives; //array of alternatives
        int numOfAltern;
    private:
        //layout of symbol in input image (in pixel)
        Rect bbox_;
        int parent_number_;
        int font_height_;
};

inline Rect CEDChar::boundingRect() const {
    return bbox_;
}

inline int CEDChar::fontHeight() const {
    return font_height_;
}

inline int CEDChar::parentNumber() const {
    return parent_number_;
}

inline Rect& CEDChar::rBoundingRect() {
    return bbox_;
}

inline void CEDChar::setBoundingRect(const Rect& bbox) {
    bbox_ = bbox;
}

inline void CEDChar::setBoundingRect(const EDBOX& bbox) {
    bbox_.rleft() = bbox.x;
    bbox_.rtop() = bbox.y;
    bbox_.setWidth(bbox.w);
    bbox_.setHeight(bbox.h);
}

inline void CEDChar::setFontHeight(int height) {
    font_height_ = height;
}

inline void CEDChar::setParentNumber(int number) {
    parent_number_ = number;
}

}

#endif /* CEDCHAR_H_ */
