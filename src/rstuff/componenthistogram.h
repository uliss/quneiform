/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef COMPONENTHISTOGRAM_H
#define COMPONENTHISTOGRAM_H

#include <iostream>
#include <vector>

#include "ccom/ccomdefs.h"

class PAGEINFO;

namespace cf {

class ComponentHistogram
{
public:
    ComponentHistogram();

    void fill(CCOM_handle hCCOM);
    void calculate();

    bool isXCorrectionNeeded(const PAGEINFO& page_info) const;
    bool isYCorrectionNeeded(const PAGEINFO& page_info) const;
    unsigned int xDpi() const;
    unsigned int yDpi() const;
    void print(std::ostream& os) const;
    std::vector<int> heightHistogram() const;
    std::vector<int> widthHistogram() const;
public:
    static const size_t MIN_COMP_SIZE = 10;
    static const size_t MAX_COMP_SIZE = 100;
    static const size_t MIN_COMMON_COMP_COUNT = 100;
private:
    static void printHistogram(std::ostream& os, const unsigned int * h);
private:
    unsigned int h_hist_[MAX_COMP_SIZE]; // height histogram
    unsigned int w_hist_[MAX_COMP_SIZE]; // width histogram
    unsigned int common_height_count_;
    unsigned int common_width_count_;
    unsigned int common_height_;
    unsigned int common_width_;
};

}

std::ostream& operator<<(std::ostream& os, const cf::ComponentHistogram& hist);

#endif // COMPONENTHISTOGRAM_H
