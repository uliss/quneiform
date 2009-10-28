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

    void import(const char * tbl) {
        initFromTable(tbl);
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
    t->addSymbol('A');
    CPPUNIT_ASSERT(t->isCode('A'));

    t->removeSymbol('A');
    CPPUNIT_ASSERT(!t->isCode('A'));

    delete t;
}

void TestAlphabet::testExport() {
    Alphabet * t = new AAlphabet;
    t->addSymbol('A');
    t->addSymbol(255);

    char * table = new char[t->size()];

    t->exportToTable(table);
    CPPUNIT_ASSERT(table[1] == 0);
    CPPUNIT_ASSERT(table[255] == 1);
    CPPUNIT_ASSERT(table[(int)'A'] == 1);

    delete t;
    delete[] table;
}

void TestAlphabet::testImport() {
    static const char alph_digital[256]={
    //  0 1 2 3 4 5 6 7 8 9 A B C D E F
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 0
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 1
        0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,    // 2
        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,    // 3
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 4
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 5
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 6
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 7
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 8
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // 9
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // a
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // b
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // c
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // d
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    // e
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0     // f
     };

    AAlphabet * t = new AAlphabet;

    t->import(alph_digital);
    CPPUNIT_ASSERT_EQUAL(t->toString(), std::string("#0123456789"));

    delete t;
}

void TestAlphabet::testDigits() {
    Alphabet * t = new DigitsAlphabet;
    CPPUNIT_ASSERT_EQUAL(t->toString(), std::string("#0123456789"));
    delete t;
}
