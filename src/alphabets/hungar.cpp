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

#include "hungar.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new HungarAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_HUNGARIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_HUNGARIAN,
        AlphabetFactory::AlphabetTables("rec1cen.dat", "rec2cen.dat"));
}

namespace cf
{

HungarAlphabet::HungarAlphabet()
{
    addSymbol(AA_right_accent);
    addSymbol(a_right_accent);
    addSymbol(EE_right_accent);
    addSymbol(e_right_accent);
    addSymbol(II_right_accent);
    addSymbol(i_right_accent);
    addSymbol(OO_right_accent);
    addSymbol(o_right_accent);
    addSymbol(OO_2dot_accent);
    addSymbol(o_2dot_accent);
    addSymbol(OO_double_right);
    addSymbol(o_double_right);
}

language_t HungarAlphabet::language() const
{
    return LANGUAGE_HUNGARIAN;
}

}
