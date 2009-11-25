/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef INCLINE_H_
#define INCLINE_H_

namespace CIF {

inline void IDEAL_XY(int& x, int& y, int Incline) {
    const int INCLINE_FACTOR = 2048;
    y -= x * Incline / INCLINE_FACTOR;
    x += y * Incline / INCLINE_FACTOR;
}

inline void REAL_XY(int& x, int& y, int Incline) {
    x -= y * Incline / INCLINE_FACTOR;
    y += x * Incline / INCLINE_FACTOR;
}

}

#endif /* INCLINE_H_ */
