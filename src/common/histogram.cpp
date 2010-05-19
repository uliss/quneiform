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

#include "histogram.h"

namespace CIF
{

bool Histogram::findU(size_t begin, size_t end) const {
    if (begin >= hist_.size() || end > hist_.size() || begin == end)
        return false;

    bool left_descent = false;
    bool right_ascent = false;
    unsigned short current = hist_[begin];

    for (size_t i = begin; i < end; i++) {
        if (current < hist_[i]) {
            if (left_descent == false)
                current = hist_[i];
            else if (left_descent == true && right_ascent == false) {
                right_ascent = true;
                // found 'U'-like histogram
                // return
                return true;
            }
        } else if (current > hist_[i]) {
            if (left_descent == false) {
                current = hist_[i];
                left_descent = true;
            } else if (left_descent == true && right_ascent == false)
                current = hist_[i];
        }
    }

    return false;
}

void Histogram::lower(unsigned short value) {
    for (iterator it = hist_.begin(), end = hist_.end(); it != end; ++it) {
        if (*it > value)
            *it -= value;
        else
            *it = 0;
    }
}

void Histogram::minimize() {
    iterator it = min();
    if (it != end() || (*it) != 0)
        lower(*it);
}

void Histogram::raise(unsigned short value) {
    for (iterator it = hist_.begin(), end = hist_.end(); it != end; ++it) {
        *it += value;
    }
}

size_t Histogram::spaceCount() const {
    size_t space_count = 0;
    bool space_flag = false;
    for (size_t i = 0, sz = hist_.size(); i < sz; i++) {
        if (hist_[i] == 0 && !space_flag) {
            space_flag = true;
            space_count++;
        } else if (hist_[i] != 0 && space_flag) {
            space_flag = false;
        }
    }
    return space_count;
}

}
