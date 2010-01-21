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

#include "lithuanian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new LithuanianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_LITHUANIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_LITHUANIAN,
        AlphabetFactory::AlphabetTables("rec1blt.dat", "rec2blt.dat"));
}

namespace CIF
{

LithuanianAlphabet::LithuanianAlphabet()
{
    const int language = LANGUAGE_LITHUANIAN;
    addSymbol(AA_bottom_accent);
    addSymbol(a_bottom_accent);
    addSymbol(CC_inv_roof);
    addSymbol(c_inv_roof);
    addSymbol(EE_bottom_accent);
    addSymbol(e_bottom_accent);
    addSymbol(EE_dot_accent);
    addSymbol(e_dot_accent);
    addSymbol(II_bottom_accent);
    addSymbol(i_bottom_accent);
    addSymbol(SS_inv_roof_baltic);
    addSymbol(s_inv_roof_baltic);
    addSymbol(UU_bottom_accent);
    addSymbol(u_bottom_accent);
    addSymbol(UU_macron);
    addSymbol(u_macron);
    addSymbol(ZZ_inv_roof_baltic);
    addSymbol(z_inv_roof_baltic);
    removeSymbol('Q');
    removeSymbol('q');
    removeSymbol('W');
    removeSymbol('w');
    removeSymbol('X');
    removeSymbol('x');
}

language_t LithuanianAlphabet::language() const
{
    return LANGUAGE_LITHUANIAN;
}

}
