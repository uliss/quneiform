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
#include <vector>

#include "globus.h"
#include "ced_struct.h"
#include "common/letter.h"
#include "common/color.h"
#include "common/rect.h"
#include "common/serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/vector.hpp>
#endif

namespace CIF
{

class CLA_EXPO CEDChar
{
    public:
        CEDChar();
        virtual ~CEDChar();

        /**
         * Adds alternative to char
         */
        void addAlternative(const Letter& alt);

        /**
         * Returns alternative at given position
         * @throw std::out_of_range - if @a pos is wrong
         */
        Letter alternativeAt(size_t pos) const;

        /**
         * Returns number of alternatives
         */
        size_t alternativeCount() const;

        /**
         * Returns char background color
         */
        Color backgroundColor() const;

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
         * Returns font number in page table of fonts
         */
        int fontNumber() const;

        /**
         * Returns font style
         */
        int fontStyle() const;

        /**
         * Returns char foreground color
         */
        Color foregroundColor() const;

        /**
         * Checks if char has alternative
         */
        bool hasAlternatives() const;

        /**
         * Checks if picture
         */
        virtual bool isPicture() const;

        /**
         * Returns pointer to next char
         */
        CEDChar * next();

        /**
         * Returns character parent number
         * @see setParentNumber()
         */
        int parentNumber() const;

        /**
         * Returns picture number or -1 is it not a picture
         */
        int pictureNumber() const;

        /**
         * Returns pointer to previous char
         */
        CEDChar * prev();

        /**
         * Returns reference to character bounding rectangle
         */
        Rect& rBoundingRect();

        /**
         * Sets character alternative
         * @throw std::out_of_range - if @a pos invalid
         */
        void setAlternative(const Letter& letter, size_t pos);

        /**
         * Sets background color
         * @see backgroundColor()
         */
        void setBackgroundColor(const Color& color);

        /**
         * Sets background color
         */
        void setBackgroundColor(int color);

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
         * Sets font number
         */
        void setFontNumber(int number);

        /**
         * Sets font style
         */
        void setFontStyle(int style);

        /**
         * Sets foreground color
         * @see foregroundColor()
         */
        void setForegroundColor(const Color& color);

        /**
         * Sets foreground color
         */
        void setForegroundColor(int color);

        /**
         * Sets pointer to next char
         * In next char pointer to previous set to @b this
         */
        void setNext(CEDChar * next);

        /**
         * Sets character parent number
         * @see parentNumber()
         */
        void setParentNumber(int number);

        /**
         * Sets pointer to previous char
         * In pointed char member next set to this
         */
        void setPrev(CEDChar * prev);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar & bbox_;
            ar & parent_number_;
            ar & fground_color_;
            ar & bground_color_;
            ar & font_lang_;
            ar & font_height_;
            ar & font_style_;
            ar & font_number_;
            ar & alternatives_;
            ar & next_;
            ar & prev_;
        }
#endif
        //layout of symbol in input image (in pixel)
        Rect bbox_;
        int parent_number_;
        Color fground_color_;
        Color bground_color_;
        //font parameters
        language_t font_lang_;
        int font_height_;
        int font_style_;
        int font_number_;
        typedef std::vector<Letter> AlternativeList;
        AlternativeList alternatives_;
        //pointers to neighbor elements in connected list
        CEDChar *next_;
        CEDChar * prev_;
};

std::ostream& operator<<(std::ostream& os, const CEDChar& chr);

}

#endif /* CEDCHAR_H_ */
