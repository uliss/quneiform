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

#ifndef LETTER_H_
#define LETTER_H_

#include <iosfwd>

#include "common/serialize.h"
#include "globus.h"

namespace cf
{

class CLA_EXPO Letter
{
    public:
        enum
        {
            LOWEST_PROBABILITY = 254
        };

        Letter(unsigned char alt = 0, unsigned char prob = LOWEST_PROBABILITY) :
            alternative_(alt), probability_(prob) {
        }

        unsigned char getChar() const {
            return alternative_;
        }

        bool isHyphen() const {
            return alternative_ == '-';
        }

        /**
         * Replaces non-printable character to space
         */
        void normalizeNonPrintable() {
            if (alternative_ < ' ')
                alternative_ = ' ';
        }

        bool operator==(const Letter& letter) const
        {
            return alternative_ == letter.alternative_ &&
                    probability_ == letter.probability_;
        }

        bool operator!=(const Letter& letter) const
        {
            return !(this->operator==(letter));
        }

        unsigned char probability() const {
            return probability_;
        }

        void setChar(unsigned char alt) {
            alternative_ = alt;
        }

        void setProbability(unsigned char prob) {
            probability_ = prob;
        }
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("alternative", alternative_);
            ar & make_nvp("probability", probability_);
        }
#endif
        unsigned char alternative_; //  ASCII code.  >= ' '.
        unsigned char probability_;
};

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const Letter& letter);

}

#ifdef CF_SERIALIZE
BOOST_IS_BITWISE_SERIALIZABLE(cf::Letter)
#endif

#endif /* LETTER_H_ */
