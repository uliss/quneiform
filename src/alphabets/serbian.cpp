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

#include "serbian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new SerbianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_SERBIAN, create);
}

namespace CIF
{

SerbianAlphabet::SerbianAlphabet()
{
    addSymbol(SERB_HH);
    addSymbol(SERB_hh);
    addSymbol(SERB_L);
    addSymbol(SERB_l);
    addSymbol(SERB_N);
    addSymbol(SERB_n);
    addSymbol(SERB_H);
    addSymbol(SERB_h);
    addSymbol(SERB_U);
    addSymbol(SERB_u);
    addSymbol(SERB_J);
    addSymbol(SERB_j);
    addSymbol('J');
    addSymbol('j');
    addSymbol(liga_j);
}

language_t SerbianAlphabet::language() const
{
    return LANGUAGE_SERBIAN;
}

}
