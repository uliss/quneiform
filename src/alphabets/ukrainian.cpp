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

#include "ukrainian.h"
#include "alphabetfactory.h"
#include "lang_def.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new UkrainianAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_UKRAINIAN, create);
}

namespace CIF
{

UkrainianAlphabet::UkrainianAlphabet()
{
    const language_t language = LANGUAGE_UKRAINIAN;
    addSymbol('I');
    addSymbol('i');
    addSymbol(UKR_I);
    addSymbol(UKR_i);
    addSymbol(UKR_II);
    addSymbol(UKR_ii);
    addSymbol(UKR_E);
    addSymbol(UKR_e);
    addSymbol(UKR_G);
    addSymbol(UKR_g);
    addSymbol(liga_i);
}

language_t UkrainianAlphabet::language() const
{
    return LANGUAGE_UKRAINIAN;
}

}
