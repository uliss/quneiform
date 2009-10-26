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

#include "polish.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
CIF::Alphabet * create() {
    return new CIF::PolishAlphabet;
}

const bool registered = CIF::AlphabetFactory::instance().registerCreator(LANG_POLISH, create);
}

namespace CIF {

PolishAlphabet::PolishAlphabet() {
    set(POLISH_SS);
    set(POLISH_s);
    set(POLISH_ZZR);
    set(POLISH_zr);
    set(POLISH_ZZD);
    set(POLISH_zd);
    set(POLISH_CC);
    set(POLISH_c);
    set(POLISH_EE);
    set(POLISH_e);
    set(POLISH_NN);
    set(POLISH_n);
    set(POLISH_OO);
    set(POLISH_o);
    set(POLISH_LL);
    set(POLISH_l);
}

}
