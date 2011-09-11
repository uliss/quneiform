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

#include "testcedserializer.h"
#include "gui/export/cedserializer.h"
#include "ced/cedpage.h"

#define REMOVE(name) { QFile f(name); f.remove(); }

#define OPEN_IN(name) QFile file(name);\
    file.open(QIODevice::ReadOnly);\
    QDataStream in(&file);

#define OPEN_OUT(name) QFile file(name);\
    file.open(QIODevice::WriteOnly);\
    QDataStream out(&file);

TestCEDSerializer::TestCEDSerializer(QObject * parent) :
    QObject(parent)
{
}

void TestCEDSerializer::testConstruct() {
    CEDSerializer ced1;
    QCOMPARE(ced1.page(), cf::CEDPagePtr());

    cf::CEDPagePtr p(new cf::CEDPage);
    CEDSerializer ced2(p);
    QCOMPARE(ced2.page(), p);
}

void TestCEDSerializer::testReadWrite() {
    QString fname("cedserializer.tmp");
    cf::CEDPagePtr p(new cf::CEDPage);
    p->setImageName("CED Image");
    CEDSerializer ced1(p);

    {
        OPEN_OUT(fname);
        out << ced1;
    }

    {
        CEDSerializer ced2;
        OPEN_IN(fname);
        in >> ced2;

        QVERIFY(ced2.page());
        QCOMPARE(ced1.page()->imageName(), ced2.page()->imageName());
    }

    REMOVE(fname)
}

void TestCEDSerializer::testReadWriteEmpty() {
    QString fname("cedserializer.tmp");
    CEDSerializer ced1;

    {
        OPEN_OUT(fname);
        out << ced1;
    }

    {
        CEDSerializer ced2;
        OPEN_IN(fname);
        in >> ced2;

        QVERIFY(!ced2.page());
    }

    REMOVE(fname)
}

QTEST_MAIN(TestCEDSerializer)
