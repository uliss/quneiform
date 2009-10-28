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
    addSymbol(AA_macron);
    addSymbol(a_macron);
    addSymbol(CC_inv_roof);
    addSymbol(c_inv_roof);
    addSymbol(EE_macron);
    addSymbol(e_macron);
    addSymbol(GG_bottom_accent);
    addSymbol(LATVIAN_g);
    addSymbol(II_macron);
    addSymbol(i_macron);
    addSymbol(KK_bottom_accent);
    addSymbol(k_bottom_accent);
    addSymbol(LL_bottom_accent);
    addSymbol(l_bottom_accent);
    addSymbol(NN_bottom_accent);
    addSymbol(n_bottom_accent);
    addSymbol(SS_inv_roof_baltic);
    addSymbol(s_inv_roof_baltic);
    addSymbol(UU_macron);
    addSymbol(u_macron);
    addSymbol(ZZ_inv_roof_baltic);
    addSymbol(z_inv_roof_baltic);

    removeSymbol('Q');
    removeSymbol('q');
    removeSymbol('W');
    removeSymbol('w');
    removeSymbol('X');
    removeSymbol('x');
    removeSymbol('Y');
    removeSymbol('y');
}

}
