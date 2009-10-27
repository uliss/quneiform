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

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include <bitset>
#include <iostream>
#include "common/exception.h"
#include "lang_def.h"

namespace CIF {

class Alphabet
{
public:
    Alphabet();
    virtual ~Alphabet();
    // table[0-255]
    void exportToTable(char * table);
    bool isCode(size_t code);
    virtual language_t language() const = 0;
    void set(size_t pos);
    size_t size() const;
    void unset(size_t pos);
protected:
    // table[0-255]
    void initFromTable(const char * bits);
private:
    std::bitset<256> chars_;
    friend std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet);
};

std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet);
typedef RuntimeExceptionImpl<Alphabet> AlphabetException;

}

#endif /* ALPHABET_H_ */
