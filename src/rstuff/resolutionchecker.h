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

#ifndef RESOLUTIONCHECKER_H
#define RESOLUTIONCHECKER_H

#include <vector>
#include <boost/function.hpp>

#include "ccom/ccom.h"

namespace cf {

class CLA_EXPO ResolutionChecker
{
public:
    ResolutionChecker(CCOM_handle ccom, Handle cpage);

    typedef boost::function<void (const std::vector<int>&)> HistogramCallback;

    /**
      * Checks and fixes page resolution
      */
    void check();
public:
    static void setHistogramHeightCallback(const HistogramCallback& clb);
    static void setHistogramWidthCallback(const HistogramCallback& clb);
private:
    CCOM_handle ccom_;
    Handle cpage_;
private:
    static HistogramCallback hist_height_callback_;
    static HistogramCallback hist_width_callback_;
};

}

#endif // RESOLUTIONCHECKER_H
