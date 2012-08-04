/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <cmath>

#include "otsubinarizator.h"
#include "histogramcreator.h"
#include "rimage_debug.h"

namespace cf
{

OtsuBinarizator::OtsuBinarizator() :
    hist_(256)
{
}

CTDIB * OtsuBinarizator::binarize()
{
    hist_.clear();
    HistogramCreator::grayBrighness(hist_, *source());
    HistogramCreator::save(hist_, "2.bmp");
    calculateThreshold();
    return ThresholdBinarizator::binarize();
}

void OtsuBinarizator::calculateThreshold()
{
    size_t hist_weighted_sum = hist_.weightedSum();
    size_t hist_sum = hist_.sum();

    if (!hist_sum) {
        RIMAGE_ERROR_FUNC() << "empty histogram";
        const int DEFAULT_VALUE= 163;
        setThreshold(DEFAULT_VALUE);
        return;
    }

    size_t left_sum = 0;
    size_t right_sum = 0;
    size_t left_sum_weighted = 0;
    size_t right_sum_weighted = 0;
    double max_varience = 0;
    int threshold = 0;

    for(uint i = 0; i < 256; i++) {
        size_t hist_value = hist_[i];
        left_sum += hist_value;
        if(!left_sum) // to avoid zero division on mean_left calculation
            continue; // try next

        right_sum = hist_sum - left_sum;
        if(right_sum == 0) // to avoid zero division on mean_right calculation
            break;  // end algorithm

        left_sum_weighted += i * hist_value;
        right_sum_weighted = hist_weighted_sum - left_sum_weighted;

        double left_mean = (double) left_sum_weighted / (double) left_sum;
        double right_mean = (double) right_sum_weighted / (double) right_sum;

        double variance = left_sum * right_sum * (left_mean - right_mean) * (left_mean - right_mean);
        if(variance > max_varience) {
            max_varience = variance;
            threshold = i;
        }
    }

    RIMAGE_DEBUG_FUNC() << "calculated threshold is:" << threshold;
    setThreshold(threshold);
}

}
