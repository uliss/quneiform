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

#include "common/serialize.h"

namespace CIF
{

class Letter
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

}

#endif /* LETTER_H_ */
