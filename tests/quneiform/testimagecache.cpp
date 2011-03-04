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

#include <QtTest>
#include "testimagecache.h"
#include "gui/imagecache.h"

TestImageCache::TestImageCache(QObject *parent) :
    QObject(parent)
{
}

void TestImageCache::testFind() {
    QPixmap p;
    QVERIFY(ImageCache::find("none", &p) == false);
}

void TestImageCache::testInsert() {
    QPixmap p;
    QVERIFY(!ImageCache::find("none", &p));
    ImageCache::insert("none", p);

    QVERIFY(ImageCache::find("none", &p));
}

void TestImageCache::testLoad() {
    QPixmap p;
    QVERIFY(!ImageCache::find("none2", &p));
    // empty path
    QVERIFY(!ImageCache::load("", &p));
    // NULL pointer
    QVERIFY(!ImageCache::load("none", NULL));
    // non existant
    QVERIFY(!ImageCache::load("none2", &p));
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    TestImageCache tc;
    return QTest::qExec(&tc, argc, argv);
}
