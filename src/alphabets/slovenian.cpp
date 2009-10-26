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

#include "slovenian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new SlovenianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_SLOVENIAN, create);
}

namespace CIF {

SlovenianAlphabet::SlovenianAlphabet() {
    set(CC_inv_roof);
    set(c_inv_roof);
    set(SS_inv_roof_latin);
    set(s_inv_roof_latin);
    set(ZZ_inv_roof_latin);
    set(z_inv_roof_latin);
}

}
