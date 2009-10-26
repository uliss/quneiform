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

#include "latvian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new LatvianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_LATVIAN, create);
}

namespace CIF {

LatvianAlphabet::LatvianAlphabet() {
    set(AA_macron);
    set(a_macron);
    set(CC_inv_roof);
    set(c_inv_roof);
    set(EE_macron);
    set(e_macron);
    set(GG_bottom_accent);
    set(LATVIAN_g);
    set(II_macron);
    set(i_macron);
    set(KK_bottom_accent);
    set(k_bottom_accent);
    set(LL_bottom_accent);
    set(l_bottom_accent);
    set(NN_bottom_accent);
    set(n_bottom_accent);
    set(SS_inv_roof_baltic);
    set(s_inv_roof_baltic);
    set(UU_macron);
    set(u_macron);
    set(ZZ_inv_roof_baltic);
    set(z_inv_roof_baltic);

    unset('Q');
    unset('q');
    unset('W');
    unset('w');
    unset('X');
    unset('x');
    unset('Y');
    unset('y');
}

}
