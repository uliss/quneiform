/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "belarusian.h"
#include "alphabetfactory.h"
#include "ligas.h"
#include "utf8.h"

namespace
{
using namespace cf;
Alphabet * create()
{
    return new BelarusianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_BELARUSIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_BELARUSIAN,
        AlphabetFactory::AlphabetTables("rec1rus.dat", "rec2rus.dat"));
}

namespace cf {

BelarusianAlphabet::BelarusianAlphabet()
{
    const language_t language = LANGUAGE_BELARUSIAN;

    addSymbol(r_cu_d);
    addSymbol(r_cu_g);
    addSymbol(r_cu_m);
    addSymbol(r_cu_u);
    addSymbol(r_cu_z);
    addSymbol(r_cu_a);
    addSymbol('I');
    addSymbol('i');

    removeSymbol(Utf8<CYRILLIC_SMALL_LETTER_SHCHA>::CP1251);
    removeSymbol(Utf8<CYRILLIC_CAPITAL_LETTER_SHCHA>::CP1251);
    removeSymbol(Utf8<CYRILLIC_SMALL_LETTER_I>::CP1251);
    removeSymbol(Utf8<CYRILLIC_CAPITAL_LETTER_I>::CP1251);
    removeSymbol(UKR_I);
    removeSymbol(UKR_i);
    removeSymbol(UKR_II);
    removeSymbol(UKR_ii);
    removeSymbol(UKR_E);
    removeSymbol(UKR_e);
    removeSymbol(UKR_G);
    removeSymbol(UKR_g);

    addSymbol(u_bel);
    addSymbol(U_bel);
    addSymbol(liga_i);

    addSymbol(liga_exm);
    addSymbol(liga_qm);
    addSymbol(liga_uperc);
    addSymbol(liga_lperc);
}

language_t BelarusianAlphabet::language() const
{
    return LANGUAGE_BELARUSIAN;
}

}

