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

#ifndef IBINARIZATOR_H
#define IBINARIZATOR_H

#include <boost/shared_ptr.hpp>

#include "globus.h"

namespace cf {

class CTDIB;

class CLA_EXPO IBinarizator
{
public:
    IBinarizator();
    virtual ~IBinarizator();

    virtual CTDIB * binarize() = 0;

    /**
      * Sets source image for binarization
      */
    void setSource(CTDIB * dib);

    /**
      * Returns pointer to source image
      */
    CTDIB * source();
protected:
    CTDIB * createDestination();
private:
    CTDIB * src_;
};

typedef boost::shared_ptr<IBinarizator> BinarizatorPtr;

}

#endif // IBINARIZATOR_H
