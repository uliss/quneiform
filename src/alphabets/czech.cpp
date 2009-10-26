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

#include "czech.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new CzechAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_CZECH, create);
}

namespace CIF {

CzechAlphabet::CzechAlphabet() {
    set(AA_right_accent);
    set(a_right_accent);
    set(CC_inv_roof);
    set(c_inv_roof);
    set(DD_inv_roof);
    set(d_inv_roof);
    set(EE_right_accent);
    set(e_right_accent);
    set(EE_inv_roof);
    set(e_inv_roof);
    set(II_right_accent);
    set(i_right_accent);
    set(NN_inv_roof);
    set(n_inv_roof);
    set(OO_right_accent);
    set(o_right_accent);
    set(RR_inv_roof);
    set(r_inv_roof);
    set(SS_inv_roof_latin);
    set(s_inv_roof_latin);
    set(TT_inv_roof);
    set(t_inv_roof);
    set(UU_right_accent);
    set(u_right_accent);
    set(UU_circle_accent);
    set(u_circle_accent);
    set(YY_right_accent);
    set(y_right_accent);
    set(ZZ_inv_roof_latin);
    set(z_inv_roof_latin);

}

}