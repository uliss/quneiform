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
#include "testalphabet.h"
#include "alphabets/alphabet.h"
#include "alphabets/digits.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestAlphabet);

using namespace CIF;
using namespace std;

class AAlphabet: public Alphabet
{
public:
    language_t language() const {
        return LANG_TOTAL;
    }
};

void TestAlphabet::testInit() {
    Alphabet * t = new AAlphabet;

    CPPUNIT_ASSERT_EQUAL(t->language(), LANG_TOTAL);
    for (size_t i = 0; i < t->size(); i++) {
        CPPUNIT_ASSERT(!t->isCode(i));
    }

    delete t;
}

void TestAlphabet::testSet() {
    Alphabet * t = new AAlphabet;

    CPPUNIT_ASSERT(!t->isCode('A'));
    t->set('A');
    CPPUNIT_ASSERT(t->isCode('A'));

    t->unset('A');
    CPPUNIT_ASSERT(!t->isCode('A'));

    delete t;
}

void TestAlphabet::testExport() {
    Alphabet * t = new AAlphabet;
    t->set('A');
    t->set(255);

    char * table = new char[t->size()];

    t->exportToTable(table);
    CPPUNIT_ASSERT(table[1] == 0);
    CPPUNIT_ASSERT(table[255] == 1);
    CPPUNIT_ASSERT(table[(int)'A'] == 1);

    delete t;
    delete[] table;
}
