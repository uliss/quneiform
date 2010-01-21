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

#include "estonian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new EstonianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_ESTONIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_ESTONIAN,
        AlphabetFactory::AlphabetTables("rec1blt.dat", "rec2blt.dat"));
}

namespace CIF
{

EstonianAlphabet::EstonianAlphabet()
{
    addSymbol(AA_2dot_accent);
    addSymbol(a_2dot_accent);
    addSymbol(OO_tild_accent);
    addSymbol(o_tild_accent);
    addSymbol(OO_2dot_accent);
    addSymbol(o_2dot_accent);
    addSymbol(SS_inv_roof_baltic);
    addSymbol(s_inv_roof_baltic);
    addSymbol(UU_2dot_accent);
    addSymbol(u_2dot_accent);
    addSymbol(ZZ_inv_roof_baltic);
    addSymbol(z_inv_roof_baltic);
    removeSymbol('C');
    removeSymbol('c');
    removeSymbol('Q');
    removeSymbol('q');
    removeSymbol('W');
    removeSymbol('w');
    removeSymbol('X');
    removeSymbol('x');
    removeSymbol('Y');
    removeSymbol('y');
}

language_t EstonianAlphabet::language() const
{
    return LANGUAGE_ESTONIAN;
}

}
