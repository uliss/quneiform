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

#include "english.h"
#include "alphabetfactory.h"

namespace {
using namespace CIF;
Alphabet * create() {
    return new EnglishAlphabet;
}

const bool registered = AlphabetFactory::instance().registerCreator(LANG_ENGLISH, create);
}

/*
 0 1 2 3 4 5 6 7 8 9 A B C D E F

 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 1
 0,1,0,1,1,1,1,0,1,1,0,0,0,1,1,1, // 2
 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 3
 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 4
 1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1, // 5
 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 6
 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0, // 7
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 8
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 9
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // a
 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0, // b
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // c
 0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0, // d
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // e
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  // f
 */

namespace CIF {

EnglishAlphabet::EnglishAlphabet() :
    DigitsAlphabet() {
    // digits initialized in parent class
    //    uliss: why '+' is missing???
    static const char * latin =
            "!#$%&()-./:;<=>?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ[]_`abcdefghijklmnopqrstuvwxyz{|}";

    for (const char * c = latin; *c; c++)
        addSymbol(*c);

    // uliss. in what code page this is????
    for (int i = 0xB0; i <= 0xBD; i++)
        addSymbol(i);


    addSymbol(0xD5);
    addSymbol(0xD7);
    addSymbol(0xD8);
}

language_t EnglishAlphabet::language() const {
    return LANG_ENGLISH;
}

}
