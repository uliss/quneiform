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

namespace cf
{

BalticAlphabet::BalticAlphabet()
{
    // WWindows cp-1257
    // it's only a var for marcores
    const int language = LANGUAGE_ESTONIAN;
    addSymbol(low_quotes);
    addSymbol(liga_exm);
    addSymbol(liga_i);
    addSymbol(right_quocket);
    addSymbol(liga_TM);
    addSymbol(liga_CC);
    addSymbol(liga_CR);
    addSymbol(liga_bull_baltic);
}

}
