/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "percentcounter.h"
#include "common/debug.h"

namespace cf {

PercentCounter::PercentCounter(PercentCounter * parent) :
        parent_(parent),
        perc_(0),
        contrib_(100)
{
}

void PercentCounter::add(unsigned char perc) {
    if(perc_ >= 100 || ((perc_ + perc) > 100))
        Debug() << "[Warning] percent counter more than 100" << std::endl;

    if(perc == 0)
        return;

    // set value
    perc_ += perc;

    // callback
    if(cb_)
        cb_(perc_);

    // parent
    if(parent_) {
        int total = perc * contrib_;
        int pperc = total / 100;
        int tail  = total % 100;
        // round
        if(tail >= 50)
            pperc++;

        parent_->add(static_cast<unsigned char>(pperc));
    }
}

void PercentCounter::reset() {
    perc_ = 0;
}

void PercentCounter::setContribution(unsigned char contrib) {
    if(contrib > 100)
        Debug() << "[Warning] percent contribution more than 100" << std::endl;

   contrib_ = contrib;
}

void PercentCounter::setParent(PercentCounter * parent) {
    if(this == parent)
        return;

    parent_ = parent;
}

}
