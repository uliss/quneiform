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

#include "danish.h"
#include "alphabetfactory.h"
#include "ligas.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new DanishAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_DANISH, create);
}

namespace CIF
{

DanishAlphabet::DanishAlphabet()
{
    addSymbol(AE_cap_deaf_sound);
    addSymbol(AA_circle_accent);
    addSymbol(OO_crossed);
    addSymbol(ae_deaf_sound);
    addSymbol(a_circle_accent);
    addSymbol(o_crossed);
}

language_t DanishAlphabet::language() const
{
    return LANGUAGE_DANISH;
}

}
