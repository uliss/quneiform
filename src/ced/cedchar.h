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

#include <iostream>
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
         * Returns font language
         */
        language_t fontLanguage() const;

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
         * Sets font language
         */
        void setFontLanguage(language_t lang);

        /**
         * Sets character parent number
         */
        void setParentNumber(int number);

        CEDChar * prev, *next; //pointer to neighbor elements in connected list
        int fontAttribs; //font parameters
        int fontNum;
        int foregroundColor;
        int backgroundColor;
        letterEx * alternatives; //array of alternatives
        int numOfAltern;
    private:
        //layout of symbol in input image (in pixel)
        Rect bbox_;
        int parent_number_;
        int font_height_;
        language_t font_lang_;
};

std::ostream& operator<<(std::ostream& os, const CEDChar& chr);

}

#endif /* CEDCHAR_H_ */
