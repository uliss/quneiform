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

#include "spanish.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
CIF::Alphabet * create() {
    return new CIF::SpanishAlphabet;
}

const bool registered = CIF::AlphabetFactory::instance().registerCreator(LANG_SPANISH, create);
}

namespace CIF {

SpanishAlphabet::SpanishAlphabet() {
    set(AA_right_accent);
    set(EE_right_accent);
    set(II_right_accent);
    set(OO_right_accent);
    set(UU_right_accent);
    set(UU_2dot_accent);
    set(NN_tild_accent);

    set(a_right_accent);
    set(e_right_accent);
    set(i_right_accent);
    set(o_right_accent);
    set(u_right_accent);
    set(u_2dot_accent);
    set(n_tild_accent);
}

}
