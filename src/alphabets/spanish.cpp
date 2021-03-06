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

#include "spanish.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new SpanishAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_SPANISH, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_SPANISH,
        AlphabetFactory::AlphabetTables("rec1.dat", "rec2.dat"));
}

namespace cf
{

SpanishAlphabet::SpanishAlphabet()
{
    addSymbol(AA_right_accent);
    addSymbol(EE_right_accent);
    addSymbol(II_right_accent);
    addSymbol(OO_right_accent);
    addSymbol(UU_right_accent);
    addSymbol(UU_2dot_accent);
    addSymbol(NN_tild_accent);
    addSymbol(a_right_accent);
    addSymbol(e_right_accent);
    addSymbol(i_right_accent);
    addSymbol(o_right_accent);
    addSymbol(u_right_accent);
    addSymbol(u_2dot_accent);
    addSymbol(n_tild_accent);
}

language_t SpanishAlphabet::language() const
{
    return LANGUAGE_SPANISH;
}

}
