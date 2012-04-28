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

#include "ibinarizator.h"
#include "rimage_debug.h"
#include "crimemory.h"
#include "rdib/ctdib.h"

namespace cf {

IBinarizator::IBinarizator() : src_(0)
{
}

IBinarizator::~IBinarizator()
{
}

void IBinarizator::setSource(CTDIB * dib)
{
    src_ = dib;
}

CTDIB * IBinarizator::source()
{
    return src_;
}

CTDIB * IBinarizator::createDestination()
{
    if (!src_) {
        RIMAGE_ERROR << " source image is not set\n";
        return 0;
    }

    CTDIB * dest = new CTDIB;

    if (!dest->SetExternals(RIMAGEAlloc, RIMAGEFree, RIMAGELock, RIMAGEUnlock)) {
        RIMAGE_ERROR << " set external error\n";
        delete dest;
        return NULL;
    }

    uint res_x = 0;
    uint res_y = 0;

    uint new_height = src_->GetLinesNumber();
    uint new_width = src_->GetLineWidth();
    src_->GetResolutionDPM(&res_x, &res_y);

    if (!dest->CreateDIBBegin((int) new_width, (int) new_height, 1)) {
        delete dest;
        return NULL;
    }

    dest->SetResolutionDPM(res_x, res_y);

    CTDIBRGBQUAD BWQuads[2] = {
        CTDIBRGBQUAD(0, 0, 0, 0),
        CTDIBRGBQUAD(0xff, 0xff, 0xff, 0)
    };

    if (!dest->SetRGBQuad(0, BWQuads[0])) {
        delete dest;
        return NULL;
    }

    if (!dest->SetRGBQuad(1, BWQuads[1])) {
        delete dest;
        return NULL;
    }

    if (!dest->CreateDIBEnd()) {
        delete dest;
        return NULL;
    }

    return dest;
}

}
