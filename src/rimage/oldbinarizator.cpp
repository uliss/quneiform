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

#include "oldbinarizator.h"
#include "cribinarizator.h"
#include "rimage_debug.h"
#include "rdib/ctdib.h"

namespace cf {

OldBinarizator::OldBinarizator() : bin_(NULL)
{
    bin_ = new CRIBinarizator;
}

OldBinarizator::~OldBinarizator()
{
    delete bin_;
}

CTDIB * OldBinarizator::binarize(int flags)
{
    if(!source()) {
        RIMAGE_ERROR << " source image not set\n";
        return NULL;
    }

    CTDIB * dest = createDestination();

    // закидываем туда картинки
    if (!bin_->setRasters(source(), dest)) {
        RIMAGE_ERROR << " can't set dib rasters\n";
        delete dest;
        return NULL;
    }

    CTBinarize bType = CTBIN_UNKNOWN;
    switch(flags) {
    case BINARIZATOR_DEZA:
        bType = CTBIN_DEZA;
        break;
    case BINARIZATOR_KRONROD:
        bType = CTBIN_KRONROD;
        break;
    default:
        break;
    }

    // бинаризуем
    if (!bin_->Binarize(bType, flags)) {
        RIMAGE_ERROR << " binarization error\n";
        delete dest;
        return NULL;
    }

    return dest;
}

}
