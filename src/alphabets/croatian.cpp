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

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new CroatianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_CROATIAN, create);
const bool tbl = AlphabetFactory::instance().registerAlphabetTables(LANGUAGE_CROATIAN,
        AlphabetFactory::AlphabetTables("rec1cen.dat", "rec2cen.dat"));
}

namespace CIF
{

CroatianAlphabet::CroatianAlphabet()
{
    addSymbol(CROAT_D);
    addSymbol(CROAT_d);
    addSymbol(CROAT_SR);
    addSymbol(CROAT_sr);
    addSymbol(CROAT_CA);
    addSymbol(CROAT_ca);
    addSymbol(CROAT_ZR);
    addSymbol(CROAT_zr);
    addSymbol(CROAT_CR);
    addSymbol(CROAT_cr);
}

language_t CroatianAlphabet::language() const
{
    return LANGUAGE_CROATIAN;
}

}
