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

namespace {
using namespace CIF;
Alphabet * create() {
    return new TurkishAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_TURKISH, create);
}

namespace CIF {

TurkishAlphabet::TurkishAlphabet() {
    // Турецкие лиги, отличные от западноевропейских
    set(liga_TM_turkish);
    set(liga_CC_turkish);
    set(liga_CR_turkish);
    set(liga_bull_turkish);

    // Турецкие буквы, отличные от западноевропейских
    set(GG_semicircle);
    set(g_semicircle);
    set(II_dot_accent);
    set(i_sans_accent);
    set(SS_bottom_accent_turkish);
    set(s_bottom_accent_turkish);
    unset('Q');
    unset('q');
    unset('W');
    unset('w');
    unset('X');
    unset('x');
}

}
