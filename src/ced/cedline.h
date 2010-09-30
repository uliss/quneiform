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

#include "globus.h"
#include "blockelement.h"

namespace cf
{

class CEDPicture;
class CEDChar;

class CLA_EXPO CEDLine: public BlockElement
{
    public:
        CEDLine(BlockElement * parent = NULL);
        CEDLine(BlockElement * parent, bool hardBreak, int fontHeight);

        /**
         * Inserts image to the end of line
         */
        void addImage(CEDPicture * img);

        /**
         * Returns char at given position
         * @throw std::runtime_error if element at given position is not CEDChar
         */
        CEDChar * charAt(size_t pos);

        /**
         * Returns default font height
         */
        int defaultFontHeight() const;

        /**
         * Returns true if line end with hyphen
         */
        bool endsWithHyphen() const;

        void exportElement(CEDExporter& exp);

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
         * Sets default font height
         */
        void setDefaultFontHeight(int height);

        /**
         * Sets line hard breaking
         * @see hardBreak()
         */
        void setHardBreak(bool value);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar.template register_type<CEDChar>();
            ar.template register_type<CEDPicture>();
            ar & make_nvp("block-element", boost::serialization::base_object<BlockElement>(*this));
            ar & make_nvp("hard-break", hard_break_);
            ar & make_nvp("default-font-height", default_font_height_);
        }
#endif
        bool hard_break_;
        int default_font_height_;
};

std::ostream& operator<<(std::ostream& os, const CEDLine& chr);

}

#endif /* CEDLINE_H_ */
