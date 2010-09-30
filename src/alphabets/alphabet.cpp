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
#include <iomanip>
#include <sstream>
#include "alphabet.h"

namespace cf
{

Alphabet::Alphabet()
{
}

Alphabet::~Alphabet()
{
}

void Alphabet::exportToTable(char * table)
{
    for (size_t i = 0; i < chars_.size(); i++)
        table[i] = chars_.test(i) ? 1 : 0;
}

void Alphabet::initFromTable(const char * bits)
{
    for (size_t i = 0; i < chars_.size(); i++) {
        if (bits[i])
            chars_.set(i);
    }
}

bool Alphabet::isCode(size_t code)
{
    assert(code < chars_.size());
    return chars_.test(code);
}

void Alphabet::addSymbol(size_t pos)
{
    assert(pos < chars_.size());
    chars_.set(pos);
}

size_t Alphabet::size() const
{
    return chars_.size();
}

void Alphabet::removeSymbol(size_t pos)
{
    assert(pos < chars_.size());
    chars_.set(pos, 0);
}

std::string Alphabet::toString() const
{
    std::ostringstream os;

    for (size_t i = 0; i < chars_.size(); i++) {
        if (chars_.test(i))
            os << (char) i;
    }

    return os.str();
}

std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet)
{
    os << "Alphabet\n";
    os << "Characters: " << alphabet.toString() << "\n";

    for (size_t i = 0; i < 16; i++)
        os << " " << std::hex << i;

    os << "\n";
    std::string delim(32, '_');
    os << " " << delim << "\n";

    for (size_t i = 0, total = alphabet.chars_.size(); i < total; i++) {
        os << " " << (int) alphabet.chars_.test(i);

        if (i != 0 && (i + 1) % 16 == 0)
            os << "| " << std::hex << ((i - 1) / 16) << "\n";
    }

    os << "\n";
    return os;
}
}

