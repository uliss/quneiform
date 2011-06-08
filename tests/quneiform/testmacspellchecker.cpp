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
#include <QDebug>

#include "testmacspellchecker.h"
#include "gui/spell/macspellchecker.h"
#include "common/lang_def.h"

void TestMacSpellChecker::testConstruct() {
    MacSpellChecker spell(Language::english());
    QCOMPARE(spell.language(), Language(LANGUAGE_ENGLISH));

    qDebug() << spell.supportedLanguages();
    QVERIFY(!spell.supportedLanguages().isEmpty());
}

void TestMacSpellChecker::testHasErrors() {
    MacSpellChecker spell(Language::english());

    QVERIFY(spell.hasErrors("errour"));
    QVERIFY(!spell.hasErrors("no errors"));
}

void TestMacSpellChecker::testSetLanguage() {
    MacSpellChecker spell(Language::english());
    QVERIFY(spell.setLanguage(Language(LANGUAGE_ENGLISH)));
    QVERIFY(!spell.setLanguage(Language(LANGUAGE_UNKNOWN)));
}

void TestMacSpellChecker::testSpellErrors() {
    MacSpellChecker spell(Language::english());
    QVERIFY(spell.spellErrors("test").isEmpty());

    MacSpellChecker::SpellList error_list = spell.spellErrors("errar string");
    QVERIFY(!error_list.isEmpty());
    QCOMPARE(error_list.count(), 1);
    QCOMPARE(error_list.first(), Range(0, 5));

    error_list = spell.spellErrors("error streng");
    QCOMPARE(error_list.first(), Range(6, 6));
}

void TestMacSpellChecker::testSuggest() {
    MacSpellChecker spell(Language::english());

    QStringList words = spell.suggest("errour");
    qDebug() << words;
    QVERIFY(!words.isEmpty());
    QCOMPARE(words.first(), QString("error"));
}

QTEST_MAIN(TestMacSpellChecker)
