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
#include <QFile>
#include <QDataStream>

#include "testlanguage.h"
#include "gui/language.h"
#include "common/lang_def.h"

#define CHECK_FROM_ISO2(iso, lang_code)  QCOMPARE(Language::fromIsoCode2(iso).code(), (int) lang_code);
#define CHECK_ISO2(code, iso) QCOMPARE(Language(code).isoCode2(), QString(iso));

void TestLanguage::testConstruct() {
    Language l(1000);
    QVERIFY(!l.isValid());
    QCOMPARE(l.code(), (int) ::LANGUAGE_UNKNOWN);
    QVERIFY(l.name().isEmpty());

    Language l2(::LANGUAGE_ENGLISH);
    QVERIFY(l2.isValid());
    QCOMPARE(l2.name(), QString("English"));
}

void TestLanguage::testFromIsoCode2() {
    CHECK_FROM_ISO2("", LANGUAGE_UNKNOWN);
    CHECK_FROM_ISO2("ru", LANGUAGE_RUSSIAN);
    CHECK_FROM_ISO2("en", LANGUAGE_ENGLISH);
    CHECK_FROM_ISO2("en_US", LANGUAGE_ENGLISH);
}

void TestLanguage::testIsoCode2() {
    CHECK_ISO2(LANGUAGE_UNKNOWN, "");
    CHECK_ISO2(LANGUAGE_BULGARIAN, "bg");
    CHECK_ISO2(LANGUAGE_RUSSIAN, "ru");
}

void TestLanguage::testSupportedLanguages() {
    QList<Language> langs = Language::supportedLanguages();
    QCOMPARE(langs.count(), 24);
    QCOMPARE(langs.first().code(), 0);
    QCOMPARE(langs.first().name(), QString("English"));

    langs = Language::supportedLanguages(Language::BY_NAME);
    QCOMPARE(langs.count(), 24);
    QCOMPARE(langs.first().code(), (int) ::LANGUAGE_BULGARIAN);
    QCOMPARE(langs.first().name(), QString("Bulgarian"));
    QCOMPARE(langs.at(1).code(), (int) ::LANGUAGE_CROATIAN);
}

void TestLanguage::testReadWrite() {
    QString fname("language.dat");

    {
        Language lang1(::LANGUAGE_RUSSIAN);
        QVERIFY(lang1.isValid());
        QFile file(fname);
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << lang1;
    }

    {
        Language lang2(10000);
        QVERIFY(!lang2.isValid());
        QFile file(fname);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> lang2;

        QVERIFY(lang2.isValid());
        QCOMPARE(lang2.code(), (int) ::LANGUAGE_RUSSIAN);
        QCOMPARE(lang2.name(), QString("Russian"));
    }

}

QTEST_APPLESS_MAIN(TestLanguage)

