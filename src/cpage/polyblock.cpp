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

#include "polyblock.h"

namespace cf {
namespace cpage {

PolyBlock::PolyBlock() :
    alphabet_(0),
    negative_(TYPE_POSITIVE),
    orient_(TYPE_LEFTRIGHT)
{}

void PolyBlock::setLight(block_light_t type)
{
    negative_ = type;
}

void PolyBlock::setOrientation(block_orientation_t dir)
{
    orient_ = dir;
}

block_orientation_t PolyBlock::orientation() const
{
    return static_cast<block_orientation_t>(orient_);
}

int PolyBlock::alphabet() const
{
    return alphabet_;
}

void PolyBlock::setAlphabet(int alph)
{
    alphabet_ = alph;
}

}
}
