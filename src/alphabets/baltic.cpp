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

#include "baltic.h"
#include "ligas.h"

namespace CIF {

BalticAlphabet::BalticAlphabet() {
    // WWindows cp-1257
    // it's only a var for marcores
    const int language = LANG_ESTONIAN;
    set(low_quotes);
    set(liga_exm);
    set(liga_i);
    set(right_quocket);
    set(liga_TM);
    set(liga_CC);
    set(liga_CR);
    set(liga_bull_baltic);
}

}
