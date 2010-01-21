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

#include "portuguese.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new PortugueseAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_PORTUGUESE, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_PORTUGUESE,
        AlphabetFactory::AlphabetTables("rec1n.dat", "rec2n.dat"));
}

namespace CIF
{

PortugueseAlphabet::PortugueseAlphabet()
{
    addSymbol(AA_left_accent);
    addSymbol(AA_right_accent);
    addSymbol(AA_tild_accent);
    addSymbol(AA_roof_accent);
    addSymbol(CC_bottom_accent);
    addSymbol(EE_left_accent);
    addSymbol(EE_right_accent);
    addSymbol(EE_2dot_accent);
    addSymbol(EE_roof_accent);
    addSymbol(OO_left_accent);
    addSymbol(OO_right_accent);
    addSymbol(OO_tild_accent);
    addSymbol(OO_roof_accent);
    addSymbol(II_right_accent);
    addSymbol(II_2dot_accent);
    addSymbol(UU_roof_accent);
    addSymbol(a_left_accent);
    addSymbol(a_right_accent);
    addSymbol(a_tild_accent);
    addSymbol(a_roof_accent);
    addSymbol(c_bottom_accent);
    addSymbol(e_left_accent);
    addSymbol(e_right_accent);
    addSymbol(e_2dot_accent);
    addSymbol(e_roof_accent);
    addSymbol(o_left_accent);
    addSymbol(o_right_accent);
    addSymbol(o_tild_accent);
    addSymbol(o_roof_accent);
    addSymbol(i_right_accent);
    addSymbol(i_2dot_accent);
    addSymbol(u_roof_accent);
}

language_t PortugueseAlphabet::language() const
{
    return LANGUAGE_PORTUGUESE;
}

}
