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

#include "turkish.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new TurkishAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_TURKISH, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_TURKISH,
        AlphabetFactory::AlphabetTables("rec1tur.dat", "rec2tur.dat"));
}

namespace cf
{

TurkishAlphabet::TurkishAlphabet()
{
    // Турецкие лиги, отличные от западноевропейских
    addSymbol(liga_TM_turkish);
    addSymbol(liga_CC_turkish);
    addSymbol(liga_CR_turkish);
    addSymbol(liga_bull_turkish);
    // Турецкие буквы, отличные от западноевропейских
    addSymbol(GG_semicircle);
    addSymbol(g_semicircle);
    addSymbol(II_dot_accent);
    addSymbol(i_sans_accent);
    addSymbol(SS_bottom_accent_turkish);
    addSymbol(s_bottom_accent_turkish);
    removeSymbol('Q');
    removeSymbol('q');
    removeSymbol('W');
    removeSymbol('w');
    removeSymbol('X');
    removeSymbol('x');
}

language_t TurkishAlphabet::language() const
{
    return LANGUAGE_TURKISH;
}

}
