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
#include "lang_def.h"
#include "element.h"
#include "common/letter.h"
#include "common/serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#endif

namespace CIF
{

class CLA_EXPO CEDChar: public Element
{
    public:
        CEDChar(Element * parent = NULL);

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

        void exportElement(CEDExporter& exp);

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
         * Checks if char has alternative
         */
        bool hasAlternatives() const;

        /**
         * Sets character alternative
         * @throw std::out_of_range - if @a pos invalid
         */
        void setAlternative(const Letter& letter, size_t pos);

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
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar & boost::serialization::base_object<Element>(*this);
            ar & font_lang_;
            ar & font_height_;
            ar & font_style_;
            ar & font_number_;
            ar & alternatives_;
        }
#endif
        //font parameters
        language_t font_lang_;
        int font_height_;
        int font_style_;
        int font_number_;
        typedef std::vector<Letter> AlternativeList;
        AlternativeList alternatives_;
};

std::ostream& operator<<(std::ostream& os, const CEDChar& chr);

}

#endif /* CEDCHAR_H_ */
