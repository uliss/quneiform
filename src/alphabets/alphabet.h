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

    /**
     * Adds symbol in alphabet
     * @param pos
     */
    void addSymbol(size_t code);

    /**
     * Exports alphabet to table, that have to be char array[0-alphabet_size]
     * Existed symbol exported as 1, non-existent as 0 at array position == symbol code
     */
    void exportToTable(char * table);

    /**
     * Checks if symbol with given code exists in alphabet
     * @param code symbol
     * @return true if exists
     */
    bool isCode(size_t code);

    /**
     * Returns alphabet language
     */
    virtual language_t language() const = 0;

    /**
     * Removes symbol from alphabet with given code
     * @param code
     */
    void removeSymbol(size_t code);

    /**
     * Returns size of alphabet table
     * @return
     */
    size_t size() const;

    /**
     * Returns all alphabet characters as string
     * @return
     */
    std::string toString() const;
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
