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

#include "bulgar.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new BulgarAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_BULGAR, create);
}

namespace CIF {

BulgarAlphabet::BulgarAlphabet() {
    // В Болгарском нет трех русских букв
    removeSymbol(r_EE_2dot);
    removeSymbol(r_e_2dot);
    removeSymbol(0xDB); // Ы in cp1251
    removeSymbol(0xFB); // ы
    removeSymbol(0xDD); // Э in cp1251
    removeSymbol(0xFD); // э
}

language_t BulgarAlphabet::language() const {
    return LANG_BULGAR;
}

}

