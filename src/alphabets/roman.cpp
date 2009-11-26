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

#include "roman.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new RomanAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_ROMAN, create);
}

namespace CIF
{

RomanAlphabet::RomanAlphabet()
{
    const int language = LANG_ROMAN;
    addSymbol(AA_semicircle);
    addSymbol(a_semicircle);
    addSymbol(AA_roof_accent);
    addSymbol(a_roof_accent);
    addSymbol(II_roof_accent);
    addSymbol(i_roof_accent);
    addSymbol(SS_bottom_accent);
    addSymbol(s_bottom_accent);
    addSymbol(TT_bottom_accent);
    addSymbol(t_bottom_accent);
}

language_t RomanAlphabet::language() const
{
    return LANG_ROMAN;
}

}
