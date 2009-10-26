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

#include "dutch.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new Dutch;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_DUTCH, create);
}

namespace CIF {

Dutch::Dutch() {
    set(AA_left_accent);
    set(AA_2dot_accent);
    set(AA_roof_accent);
    set(CC_bottom_accent);
    set(EE_left_accent);
    set(EE_right_accent);
    set(EE_2dot_accent);
    set(EE_roof_accent);
    set(OO_2dot_accent);
    set(NN_tild_accent);
    set(II_roof_accent);
    set(II_2dot_accent);
    set(UU_roof_accent);
    set(UU_2dot_accent);

    set(a_left_accent);
    set(a_2dot_accent);
    set(a_roof_accent);
    set(c_bottom_accent);
    set(e_left_accent);
    set(e_right_accent);
    set(e_2dot_accent);
    set(e_roof_accent);
    set(o_2dot_accent);
    set(n_tild_accent);
    set(i_roof_accent);
    set(i_2dot_accent);
    set(u_roof_accent);
    set(u_2dot_accent);
}

}
