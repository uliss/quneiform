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

#include "russian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
CIF::Alphabet * create() {
    return new CIF::RussianAlphabet;
}

const bool registered = CIF::AlphabetFactory::instance().registerCreator(LANG_RUSSIAN, create);
}

namespace CIF {

RussianAlphabet::RussianAlphabet() {
    // DOS cp866
    static const char alph_russian[256] = {
     // 0 1 2 3 4 5 6 7 8 9 A B C D E F
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 1
        0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,    // 2 "!#%&()+-./"
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // 3 "0123456789:;<=>?"
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 4
        0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,    // 5 "[]_"
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 6
        0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,    // 7 "{|}"
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // 8 from "А"
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // 9 to "Я"
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // a from "а" to "п"
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // b
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // c
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // d
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,    // e from "р" to "я"
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0     // f
     };

    initFromTable(alph_russian);

    const static language_t language = LANG_RUSSIAN;

    addSymbol(r_cu_d);
    addSymbol(r_cu_g);
    addSymbol(r_cu_m);
    addSymbol(r_cu_u);
    addSymbol(r_cu_z);
    addSymbol(r_cu_a);

    addSymbol(liga_exm);
    addSymbol(liga_qm);
    addSymbol(liga_uperc);
    addSymbol(liga_lperc);
}

}
