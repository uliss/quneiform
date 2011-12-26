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

#include <string.h>
#include <algorithm>
#include <iomanip>

#include "componenthistogram.h"
#include "common/debug.h"
#include "ccom/ccom.h"
#include "cpage/cpage.h"

namespace cf
{

#define MSG_PREFIX "[RStuff::ComponentHistogram] "

ComponentHistogram::ComponentHistogram() :
    max_height_peak_(0),
    max_width_peak_(0),
    common_comp_height_(0),
    common_comp_width_(0)
{
    memset(w_hist_, 0, sizeof(w_hist_[0]) * MAX_COMPONENT);
    memset(h_hist_, 0, sizeof(h_hist_[0]) * MAX_COMPONENT);
}

void ComponentHistogram::fill(CCOM_handle hCCOM)
{
    CCOM_comp * comp = CCOM_GetFirst(hCCOM, NULL);

    while(comp) {
        if(comp->h >= (int) MIN_COMPONENT && comp->h < (int) MAX_COMPONENT)
            h_hist_[comp->h]++;

        if(comp->w >= (int) MIN_COMPONENT && comp->w < (int) MAX_COMPONENT)
            w_hist_[comp->w]++;

        comp = CCOM_GetNext(comp, NULL);
    }
}

void ComponentHistogram::calculate()
{
    for(size_t i = (MIN_COMPONENT + 1); i < (MAX_COMPONENT - 1); i++) {
        const unsigned int height_sum = h_hist_[i - 1] + h_hist_[i] + h_hist_[i + 1];

        if(height_sum > max_height_peak_) {
            common_comp_height_ = i;
            max_height_peak_ = height_sum;
        }

        const unsigned int width_sum = w_hist_[i - 1] + w_hist_[i] + w_hist_[i + 1];

        if(width_sum > max_width_peak_) {
            common_comp_width_ = i;
            max_width_peak_ = width_sum;
        }
    }

    Debug() << MSG_PREFIX << "common component height: " << common_comp_height_ << "\n";
    Debug() << MSG_PREFIX << "common component width: " << common_comp_width_ << "\n";
    Debug() << MSG_PREFIX << "most frequent height count: " << max_height_peak_ << "\n";
    Debug() << MSG_PREFIX << "most frequent width count: " << max_width_peak_ << "\n";
}

bool ComponentHistogram::hasHeightPeak() const
{
    return (common_comp_height_ > MIN_COMPONENT) && (max_height_peak_ > MAX_COMPONENT);
}

bool ComponentHistogram::isXCorrectionNeeded(const PAGEINFO& page_info) const
{
    return (common_comp_width_ > MIN_COMPONENT) && (max_width_peak_ > MAX_COMPONENT)
            && !( (page_info.DPIX * 22) < (2 * 300 * common_comp_width_)
                  && (2 * page_info.DPIX * 22) > (300 * common_comp_width_));
}

bool ComponentHistogram::isYCorrectionNeeded(const PAGEINFO& page_info) const
{
    if (hasHeightPeak() && !( (page_info.DPIY * 22) < (2 * 300 * common_comp_height_)
                         && (2 * page_info.DPIY * 22) > (300 * common_comp_height_)))
        return true;
    else
        return false;
}

unsigned int ComponentHistogram::xCorrection() const
{
    return (300 * common_comp_width_ + 11) / 22;
}

unsigned int ComponentHistogram::yCorrection() const
{
    return (300 * common_comp_height_ + 11) / 22;
}

static void printHeader(std::ostream& os, const std::string& msg)
{
    os << '\n' << msg << '\n' << std::string(ComponentHistogram::MAX_COMPONENT, '=') << '\n';
}

static void printFooter(std::ostream& os)
{
    os << std::string(ComponentHistogram::MAX_COMPONENT, '=') << '\n';
    for(size_t i = 0; i < ComponentHistogram::MAX_COMPONENT; i += 10) {
        os << i << std::string(8, ' ');
    }
    os << '\n';
}

void ComponentHistogram::print(std::ostream& os) const
{
    printHeader(os, "Height histogram:");
    printHistogram(os, h_hist_);
    printFooter(os);

    printHeader(os, "Width histogram:");
    printHistogram(os, w_hist_);
    printFooter(os);
}

void ComponentHistogram::printHistogram(std::ostream& os, const unsigned int * hist)
{
    static const int ROWS = 10;
    static const char SPACE = ' ';
    static const char DOT = '*';
    const unsigned int max_row = (*std::max_element(hist, hist + MAX_COMPONENT));
    char graph[MAX_COMPONENT][ROWS];

    // fill with spaces
    for(size_t c = 0; c < MAX_COMPONENT; c++) {
        for(size_t r = 0; r < ROWS; r++)
            graph[c][r] = SPACE;
    }

    // add dots
    for(size_t c = MIN_COMPONENT; c < MAX_COMPONENT; c++) {
        uint row_height = (hist[c] * ROWS) / max_row;
        for(size_t r = 0; r < row_height; r++)
            graph[c][r] = DOT;
    }

    // print hist
    for(size_t r = ROWS; r > 0; r--) {
        for(size_t c = 0; c < MAX_COMPONENT; c++)
            os << graph[c][r - 1];

        os << '\n';
    }
}

}

std::ostream& operator<<(std::ostream& os, const cf::ComponentHistogram& hist)
{
    hist.print(os);
    return os;
}
