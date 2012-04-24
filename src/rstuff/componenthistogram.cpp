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
#include "common/cifconfig.h"
#include "ccom/ccom.h"
#include "cpage/cpage.h"

namespace cf
{

#define MSG_PREFIX "[RStuff::ComponentHistogram] "
#define RSTUFF_DEBUG(msg, value) if(Config::instance().debug()) {\
    Debug() << MSG_PREFIX << " " << msg << ": " << value << "\n";}

static const int DEFAULT_DPI = 300;

ComponentHistogram::ComponentHistogram() :
    common_height_count_(0),
    common_width_count_(0),
    common_height_(0),
    common_width_(0)
{
    memset(w_hist_, 0, sizeof(w_hist_[0]) * MAX_COMP_SIZE);
    memset(h_hist_, 0, sizeof(h_hist_[0]) * MAX_COMP_SIZE);
}

void ComponentHistogram::fill(CCOM_handle hCCOM)
{
    CCOM_comp * comp = CCOM_GetFirst(hCCOM, NULL);

    while(comp) {
        if(comp->h >= (int) MIN_COMP_SIZE && comp->h < (int) MAX_COMP_SIZE)
            h_hist_[comp->h]++;

        if(comp->w >= (int) MIN_COMP_SIZE && comp->w < (int) MAX_COMP_SIZE)
            w_hist_[comp->w]++;

        comp = CCOM_GetNext(comp, NULL);
    }
}

void ComponentHistogram::calculate()
{
    for(size_t i = (MIN_COMP_SIZE + 1); i < (MAX_COMP_SIZE - 1); i++) {
        const unsigned int height_sum = h_hist_[i - 1] + h_hist_[i] + h_hist_[i + 1];

        if(height_sum > common_height_count_) {
            common_height_ = i;
            common_height_count_ = height_sum;
        }

        const unsigned int width_sum = w_hist_[i - 1] + w_hist_[i] + w_hist_[i + 1];

        if(width_sum > common_width_count_) {
            common_width_ = i;
            common_width_count_ = width_sum;
        }
    }

    RSTUFF_DEBUG("common component height", common_height_);
    RSTUFF_DEBUG("common component width", common_width_);
    RSTUFF_DEBUG("common height count", common_height_count_);
    RSTUFF_DEBUG("common width count", common_width_count_);
}

bool ComponentHistogram::isXCorrectionNeeded(const PAGEINFO& page_info) const
{
    if(common_width_ < MIN_COMP_SIZE) {
        RSTUFF_DEBUG("component width is too small", "");
        return false;
    }

    if(common_width_count_  < MIN_COMMON_COMP_COUNT) {
        RSTUFF_DEBUG("not enough common width count", common_width_count_);
        return false;
    }

    return !((page_info.DPIX * 22) < (2 * DEFAULT_DPI * common_width_)
                  && (2 * page_info.DPIX * 22) > (DEFAULT_DPI * common_width_));
}

bool ComponentHistogram::isYCorrectionNeeded(const PAGEINFO& page_info) const
{
    if(common_height_ < MIN_COMP_SIZE) {
        RSTUFF_DEBUG("component height is too small", "");
        return false;
    }

    if(common_height_count_ < MIN_COMMON_COMP_COUNT) {
        RSTUFF_DEBUG("not enough common height count", common_height_count_);
        return false;
    }

    if((page_info.DPIY * 22) < (2 * DEFAULT_DPI * common_height_)
            && (2 * page_info.DPIY * 22) > (DEFAULT_DPI * common_height_))
        return false;
    else
        return true;
}

unsigned int ComponentHistogram::xDpi() const
{
    return (DEFAULT_DPI * common_width_ + 11) / 22;
}

unsigned int ComponentHistogram::yDpi() const
{
    return (DEFAULT_DPI * common_height_ + 11) / 22;
}

static void printHeader(std::ostream& os, const std::string& msg)
{
    os << '\n' << msg << '\n' << std::string(ComponentHistogram::MAX_COMP_SIZE, '=') << '\n';
}

static void printFooter(std::ostream& os)
{
    os << std::string(ComponentHistogram::MAX_COMP_SIZE, '=') << '\n';
    for(size_t i = 0; i < ComponentHistogram::MAX_COMP_SIZE; i += 10) {
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
    static const size_t ROWS = 10;
    static const char SPACE = ' ';
    static const char DOT = '*';
    const unsigned int max_row = (*std::max_element(hist, hist + MAX_COMP_SIZE));
    char graph[MAX_COMP_SIZE][ROWS];

    // fill with spaces
    for(size_t c = 0; c < MAX_COMP_SIZE; c++) {
        for(size_t r = 0; r < ROWS; r++)
            graph[c][r] = SPACE;
    }

    // add dots
    for(size_t c = MIN_COMP_SIZE; c < MAX_COMP_SIZE; c++) {
        uint row_height = (hist[c] * ROWS) / max_row;
        for(size_t r = 0; r < row_height; r++)
            graph[c][r] = DOT;
    }

    // print hist
    for(size_t r = ROWS; r > 0; r--) {
        for(size_t c = 0; c < MAX_COMP_SIZE; c++)
            os << graph[c][r - 1];

        os << '\n';
    }
}

std::vector<int> ComponentHistogram::heightHistogram() const
{
    return std::vector<int>(h_hist_, h_hist_ + MAX_COMP_SIZE);
}

std::vector<int> ComponentHistogram::widthHistogram() const
{
    return std::vector<int>(w_hist_, w_hist_ + MAX_COMP_SIZE);
}

}

std::ostream& operator<<(std::ostream& os, const cf::ComponentHistogram& hist)
{
    hist.print(os);
    return os;
}
