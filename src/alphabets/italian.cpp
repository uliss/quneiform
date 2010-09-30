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

#include "italian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new ItalianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_ITALIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_ITALIAN,
        AlphabetFactory::AlphabetTables("rec1.dat", "rec2.dat"));
}

namespace cf
{

ItalianAlphabet::ItalianAlphabet()
{
    addSymbol(AA_left_accent);
    addSymbol(EE_left_accent);
    addSymbol(EE_right_accent);
    addSymbol(II_left_accent);
    addSymbol(II_right_accent);
    addSymbol(II_roof_accent);
    addSymbol(OO_left_accent);
    addSymbol(OO_right_accent);
    addSymbol(UU_left_accent);
    addSymbol(UU_right_accent);
    addSymbol(a_left_accent);
    addSymbol(e_left_accent);
    addSymbol(e_right_accent);
    addSymbol(i_left_accent);
    addSymbol(i_right_accent);
    addSymbol(i_roof_accent);
    addSymbol(o_left_accent);
    addSymbol(o_right_accent);
    addSymbol(u_left_accent);
    addSymbol(u_right_accent);
}

language_t ItalianAlphabet::language() const
{
    return LANGUAGE_ITALIAN;
}

}
