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

namespace
{
using namespace cf;
Alphabet * create()
{
    return new CzechAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_CZECH, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_CZECH,
        AlphabetFactory::AlphabetTables("rec1cen.dat", "rec2cen.dat"));
}

namespace cf
{

CzechAlphabet::CzechAlphabet()
{
    addSymbol(AA_right_accent);
    addSymbol(a_right_accent);
    addSymbol(CC_inv_roof);
    addSymbol(c_inv_roof);
    addSymbol(DD_inv_roof);
    addSymbol(d_inv_roof);
    addSymbol(EE_right_accent);
    addSymbol(e_right_accent);
    addSymbol(EE_inv_roof);
    addSymbol(e_inv_roof);
    addSymbol(II_right_accent);
    addSymbol(i_right_accent);
    addSymbol(NN_inv_roof);
    addSymbol(n_inv_roof);
    addSymbol(OO_right_accent);
    addSymbol(o_right_accent);
    addSymbol(RR_inv_roof);
    addSymbol(r_inv_roof);
    addSymbol(SS_inv_roof_latin);
    addSymbol(s_inv_roof_latin);
    addSymbol(TT_inv_roof);
    addSymbol(t_inv_roof);
    addSymbol(UU_right_accent);
    addSymbol(u_right_accent);
    addSymbol(UU_circle_accent);
    addSymbol(u_circle_accent);
    addSymbol(YY_right_accent);
    addSymbol(y_right_accent);
    addSymbol(ZZ_inv_roof_latin);
    addSymbol(z_inv_roof_latin);
}

language_t CzechAlphabet::language() const
{
    return LANGUAGE_CZECH;
}

}
