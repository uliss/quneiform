/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef UTF8_H
#define UTF8_H

namespace cf {

enum unicode_t {
    CYRILLIC_CAPITAL_LETTER_I = 0x0418	,
    CYRILLIC_CAPITAL_LETTER_SHCHA = 0x0429,
    CYRILLIC_SMALL_LETTER_I = 0x0438,
    CYRILLIC_SMALL_LETTER_SHCHA = 0x0449
};

template<unicode_t> struct Utf8 {};

#define MAKE_UTF8_CP1251(utf8, cp1251) \
    template<> struct Utf8<utf8> {\
        const static unsigned char CP1251 = cp1251;\
};

MAKE_UTF8_CP1251(CYRILLIC_CAPITAL_LETTER_I, 0xC8)
MAKE_UTF8_CP1251(CYRILLIC_SMALL_LETTER_I, 0xE8)
MAKE_UTF8_CP1251(CYRILLIC_SMALL_LETTER_SHCHA, 0xF9)
MAKE_UTF8_CP1251(CYRILLIC_CAPITAL_LETTER_SHCHA, 0xD9)

}

#endif // UTF8_H
