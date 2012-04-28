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

#ifndef THRESHOLDBINARIZATOR_H
#define THRESHOLDBINARIZATOR_H

#include "ibinarizator.h"
#include "compat_defs.h"

namespace cf {

class CLA_EXPO ThresholdBinarizator : public IBinarizator
{
public:
    enum grayscale_method_t {
        AVERAGE = 0,
        LUMINANCE,
        LUMA,
        DESATURAION,
        DECOMPOSITION_MAX,
        DECOMPOSITION_MIN,
        CHANNEL_RED,
        CHANNEL_GREEN,
        CHANNEL_BLUE
    };

public:
    ThresholdBinarizator(int threshold = 0);

    CTDIB * binarize(int flags);

    grayscale_method_t grayscaleMethod() const;
    void setGrayscaleMethod(grayscale_method_t m);

    /**
      * Sets binarization threshold
      */
    void setThreshold(int value);

    /**
      * Returns binarization threshold
      */
    int threshold() const;
private:
    int threshold_;
    grayscale_method_t grayscale_method_;
};

}

#endif // THRESHOLDBINARIZATOR_H
