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
#include <QSignalSpy>

#include "testlanguageselect.h"
#include "gui/languageselect.h"
#include "common/lang_def.h"

TestLanguageSelect::TestLanguageSelect(QObject *parent) :
    QObject(parent)
{
}

void TestLanguageSelect::testConstruct() {
    LanguageSelect s;

    QVERIFY(s.currentLanguage().isValid());
    QCOMPARE(s.currentLanguage().name(), QString("English"));
    QCOMPARE(s.currentText(), QString("English"));
}

void TestLanguageSelect::testEmitSelected() {
    LanguageSelect s;

    QSignalSpy selected(&s, SIGNAL(languageSelected(Language)));
    QVERIFY(selected.isEmpty());

    s.setCurrentIndex(1);
    QCOMPARE(selected.count(), 1);
    QCOMPARE(selected.at(0).at(0).value<Language>(), Language(::LANGUAGE_CROATIAN));
}

void TestLanguageSelect::testSelect() {
    LanguageSelect s;
    QSignalSpy lang_changed(&s, SIGNAL(languageSelected(Language)));
    s.select(Language(::LANGUAGE_DANISH));
    QVERIFY(lang_changed.isEmpty());
    QCOMPARE(s.currentLanguage().name(), QString("Danish"));
    QCOMPARE(s.currentText(), QString("Danish"));

    s.select(Language::english());
    QVERIFY(s.currentLanguage().isValid());
    QCOMPARE(s.currentLanguage().name(), QString("English"));

    s.select(Language());
    QCOMPARE(s.currentLanguage().name(), QString("English"));
}

QTEST_MAIN(TestLanguageSelect)
