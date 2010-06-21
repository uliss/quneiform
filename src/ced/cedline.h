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

#ifndef CEDLINE_H_
#define CEDLINE_H_

#include <vector>
#include <boost/shared_ptr.hpp>
#include "globus.h"
#include "ced_struct.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#endif

namespace CIF
{

class CLA_EXPO CEDLine
{
    public:
        CEDLine();
        CEDLine(bool hardBreak, int fontHeight);

        /**
         * Returns char at given position
         */
        CEDChar * charAt(size_t pos);

        /**
         * Returns number of chars in line
         */
        int charCount();

        /**
         * Removes all chars from line
         */
        void clearChars();

        /**
         * Returns default font height
         */
        int defaultFontHeight() const;

        /**
         * Returns pointer to first char in line
         */
        CEDChar * first();

        /**
         * Returns true if line have hard break
         * @see setHardBreak()
         */
        bool hardBreak() const;

        /**
         * Inserts new symbol after current one. new symbol becomes current
         * @return pointer to new symbol
         */
        CEDChar * insertChar();

        /**
         * Inserts symbol after current one. new symbol becomes current
         * @return pointer to symbol
         */
        CEDChar * insertChar(CEDChar * chr);

        /**
         * Returns number of line from start of file
         */
        int internalNumber() const;

        /**
         * Returns number of parent in a file
         * @see setParentNumber()
         */
        int parentNumber() const;

        /**
         * Sets default font height
         */
        void setDefaultFontHeight(int height);

        /**
         * Sets line hard breaking
         * @see hardBreak()
         */
        void setHardBreak(bool value);

        /**
         * Sets parent number
         * @see parentNumber()
         */
        void setParentNumber(int number);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar & hard_break_;
            ar & internal_number_;
            ar & parent_number_;
            ar & default_font_height_;
            ar & chars_;
        }
#endif
        bool hard_break_;
        int internal_number_;
        int parent_number_;
        int default_font_height_;
        typedef boost::shared_ptr<CEDChar> CharPtr;
        typedef std::vector<CharPtr> CharList;
        CharList chars_;
};

std::ostream& operator<<(std::ostream& os, const CEDLine& chr);

}

#endif /* CEDLINE_H_ */
