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
#include <iostream>

#include "resolutionchecker.h"
#include "componenthistogram.h"
#include "common/debug.h"
#include "cpage/cpage.h"

namespace cf {

static inline bool isValidResolution(const PAGEINFO& info)
{
    static const size_t MINIMAL_RESOLUTION = 99;
    return info.DPIX > MINIMAL_RESOLUTION && info.DPIY > MINIMAL_RESOLUTION;
}

ResolutionChecker::ResolutionChecker(CCOM_handle ccom, Handle cpage) :
    ccom_(ccom), cpage_(cpage)
{
}

void ResolutionChecker::check()
{
    PAGEINFO page_info;
    if(!GetPageInfo(cpage_, &page_info))
        return;

    if(isValidResolution(page_info)) {
        cf::Debug() << "[RStuff] no resolution correction needed: "
                    << page_info.DPIX << "x" << page_info.DPIY << "\n";
        return;
    }

    ComponentHistogram hyst;
    hyst.fill(ccom_);
    hyst.calculate();

    bool changed = false;

    if(hyst.isYCorrectionNeeded(page_info)) {
        page_info.DPIY = hyst.yCorrection();
        changed = true;
    }

    if(hyst.isXCorrectionNeeded(page_info)){
        page_info.DPIX = hyst.xCorrection();
        changed = true;
    }

    if(changed) {
        SetPageInfo(cpage_, page_info);
        cf::Debug() << "[RStuff] new resolution: DPIX=" << page_info.DPIX
                    << ", DPIY=" << page_info.DPIY << "\n";
    }

    cf::Debug() << hyst;
}

}
