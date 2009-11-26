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

#include "dutch.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new DutchAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_DUTCH, create);
}

namespace CIF
{

DutchAlphabet::DutchAlphabet()
{
    addSymbol(AA_left_accent);
    addSymbol(AA_2dot_accent);
    addSymbol(AA_roof_accent);
    addSymbol(CC_bottom_accent);
    addSymbol(EE_left_accent);
    addSymbol(EE_right_accent);
    addSymbol(EE_2dot_accent);
    addSymbol(EE_roof_accent);
    addSymbol(OO_2dot_accent);
    addSymbol(NN_tild_accent);
    addSymbol(II_roof_accent);
    addSymbol(II_2dot_accent);
    addSymbol(UU_roof_accent);
    addSymbol(UU_2dot_accent);
    addSymbol(a_left_accent);
    addSymbol(a_2dot_accent);
    addSymbol(a_roof_accent);
    addSymbol(c_bottom_accent);
    addSymbol(e_left_accent);
    addSymbol(e_right_accent);
    addSymbol(e_2dot_accent);
    addSymbol(e_roof_accent);
    addSymbol(o_2dot_accent);
    addSymbol(n_tild_accent);
    addSymbol(i_roof_accent);
    addSymbol(i_2dot_accent);
    addSymbol(u_roof_accent);
    addSymbol(u_2dot_accent);
}

language_t DutchAlphabet::language() const
{
    return LANG_DUTCH;
}

}
