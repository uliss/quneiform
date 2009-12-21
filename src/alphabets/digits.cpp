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

#include "digits.h"
#include "alphabetfactory.h"

namespace
{
using namespace CIF;
Alphabet * create()
{
    return new DigitsAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANGUAGE_DIGITS, create);
}

namespace CIF
{

DigitsAlphabet::DigitsAlphabet()
{
    static const char * digits = "#0123456789";

    for (const char * c = digits; *c; c++)
        addSymbol(*c);
}

language_t DigitsAlphabet::language() const
{
    return LANGUAGE_DIGITS;
}

}
