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

#include <cassert>
#include "alphabet.h"

namespace CIF {

Alphabet::Alphabet() {
}

Alphabet::~Alphabet() {
}

void Alphabet::exportToTable(char * table) {
    for (size_t i = 0; i < chars_.size(); i++)
        table[i] = chars_.test(i) ? 1 : 0;
}

void Alphabet::initFromTable(const char * bits) {
    for (size_t i = 0; i < chars_.size(); i++) {
        if (bits[i])
            chars_.set(i);
    }
}

bool Alphabet::isCode(size_t code) {
    assert(code < chars_.size());
    return chars_.test(code);
}

void Alphabet::set(size_t pos) {
    assert(pos < chars_.size());
    chars_.set(pos);
}

void Alphabet::unset(size_t pos) {
    assert(pos < chars_.size());
    chars_.set(pos, 0);
}

std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet) {
    os << "Alphabet\n";
    os << "string: " << alphabet.chars_.to_string() << "\n";
    os << "long:   " << alphabet.chars_.to_ulong() << "\n";
    for (size_t i = 0; i < alphabet.chars_.size(); i++) {
        if (alphabet.chars_.test(i))
            os << " " << (char) i;
        if (i && !i % 16)
            os << "\n";
    }
    return os;
}
}

