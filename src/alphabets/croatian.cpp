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

#include "croatian.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new CroatianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_CROATIAN, create);
}

namespace CIF {

CroatianAlphabet::CroatianAlphabet() {
    set(CROAT_D);
    set(CROAT_d);
    set(CROAT_SR);
    set(CROAT_sr);
    set(CROAT_CA);
    set(CROAT_ca);
    set(CROAT_ZR);
    set(CROAT_zr);
    set(CROAT_CR);
    set(CROAT_cr);
}

}
