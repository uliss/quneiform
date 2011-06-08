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

#include <QTest>

#include "testaspellchecker.h"
#include "gui/spell/aspellchecker.h"
#include "common/lang_def.h"

void TestASpellChecker::testConstruct() {
    ASpellChecker c(Language::english());
    QCOMPARE(c.language(), Language::english());
}

void TestASpellChecker::testCheckWord() {
    ASpellChecker c(Language::english());

    QVERIFY(c.checkWord("error"));
    QVERIFY(!c.checkWord("errour"));

    c.setLanguage(Language(LANGUAGE_RUSSIAN));
    QVERIFY(c.checkWord(QString::fromUtf8("правильно")));
}

void TestASpellChecker::testSpellErrors() {
    ASpellChecker c(Language::english());

    QVERIFY(c.spellErrors("no error string").isEmpty());
    ISpellChecker::SpellList errors = c.spellErrors("errour string");

    QCOMPARE(errors.count(), 1);
    QCOMPARE(errors.first(), Range(0, 6));

    if(c.supportedLanguages().contains(Language(LANGUAGE_RUSSIAN))) {
        c.setLanguage(Language(LANGUAGE_RUSSIAN));
        errors = c.spellErrors(QString::fromUtf8("строка с ошипкой"));
        QCOMPARE(errors.count(), 1);
        QCOMPARE(errors.first(), Range(9, 7));
    }
}

void TestASpellChecker::testSpellSuggest() {
    ASpellChecker c(Language::english());

    QCOMPARE(c.suggest("errror").first(), QString("error"));

    if(c.supportedLanguages().contains(Language(LANGUAGE_RUSSIAN))) {
        c.setLanguage(Language(LANGUAGE_RUSSIAN));
        QCOMPARE(c.suggest(QString::fromUtf8("ошыбка")).first(), QString::fromUtf8("ошибка"));
    }
}

QTEST_MAIN(TestASpellChecker)
