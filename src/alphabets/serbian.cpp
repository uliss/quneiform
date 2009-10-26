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

#include "serbian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
CIF::Alphabet * create() {
    return new CIF::SerbianAlphabet;
}

const bool registered = CIF::AlphabetFactory::instance().registerCreator(LANG_SERBIAN, create);
}

namespace CIF {

SerbianAlphabet::SerbianAlphabet() {
    set(SERB_HH);
    set(SERB_hh);
    set(SERB_L);
    set(SERB_l);
    set(SERB_N);
    set(SERB_n);
    set(SERB_H);
    set(SERB_h);
    set(SERB_U);
    set(SERB_u);
    set(SERB_J);
    set(SERB_j);
    set('J');
    set('j');
    set(liga_j);

}

}
