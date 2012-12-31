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

#ifndef PICTURE_H
#define PICTURE_H

#include "cttypes.h"
#include "cpagedefs.h"
#include "common/point.h"

struct PolyBlock;

namespace cf {
namespace cpage {

class Picture
{
public:
    Picture();
    void appendCorner(const cf::Point& pt);
    cf::Point cornerAt(size_t pos) const;
    size_t cornerCount() const;
    void rotateCorner(size_t pos, int skew2048);
    void set(const PolyBlock& polygon);
private:
    size_t number_;
    cf::Point corners_[CPAGE_MAXCORNER];
};

}
}

#endif // PICTURE_H
