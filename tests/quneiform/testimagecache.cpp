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
#include <QFuture>
#include <QtConcurrentRun>
#include <iostream>

#include "testimagecache.h"
#include "gui/imagecache.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

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
    QVERIFY(!ImageCache::load("none", (QPixmap *) NULL));
    // non existant
    QVERIFY(!ImageCache::load("none2", &p));
    // exist
    QVERIFY(ImageCache::load(CF_IMAGE_DIR "/english.png", &p));
}

void TestImageCache::testFindImage()
{
    QImage img;
    QVERIFY(img.isNull());
    QVERIFY(!ImageCache::find("none", &img));

    QVERIFY(ImageCache::insert("key", QImage(10, 10, QImage::Format_RGB32)));
    QVERIFY(ImageCache::find("key", (QImage*) NULL));
    QVERIFY(ImageCache::find("key", &img));
    QVERIFY(!img.isNull());
    QCOMPARE(img.size(), QSize(10, 10));
}

void TestImageCache::testInsertImage()
{
    ImageCache::clear();
    QCOMPARE(ImageCache::cacheLimit(), (size_t) 10240 * 1024);

    // null image
    QImage img1;
    QVERIFY(!ImageCache::insert("key3", img1));

    QImage img2(10, 10, QImage::Format_RGB32);

    // null path
    QVERIFY(!ImageCache::insert("", img2));
    // first insert
    QVERIFY(ImageCache::insert("key3", img2));
    // replace with same size
    QVERIFY(ImageCache::insert("key3", img2));
    // replace with less size
    QVERIFY(ImageCache::insert("key3", QImage(2, 2, QImage::Format_RGB32)));
    // replace with bigger size
    QVERIFY(ImageCache::insert("key3", QImage(12, 12, QImage::Format_RGB32)));

    // set low limit - no size
    ImageCache::setCacheLimit(10);
    QVERIFY(!ImageCache::insert("key3", QImage(20, 20, QImage::Format_RGB32)));

    {
        ImageCache::clear();
        QImage img(10, 10, QImage::Format_RGB32);
        int img_sz = img.byteCount();
        ImageCache::setCacheLimit(img_sz * 3);
        QVERIFY(ImageCache::insert("1", img));
        QVERIFY(ImageCache::insert("2", img));
        QVERIFY(ImageCache::insert("3", img));

        // no space - remove old: "1"
        QVERIFY(ImageCache::insert("4", img));
        QVERIFY(!ImageCache::find("1", &img));

        // insert bigger object, remove "2" and "3"
        QVERIFY(ImageCache::insert("5", QImage(20, 10, QImage::Format_RGB32)));
        QVERIFY(!ImageCache::find("2", &img));
        QVERIFY(!ImageCache::find("3", &img));
        QVERIFY(ImageCache::find("4", &img));

        // insert entire cache
        QVERIFY(ImageCache::insert("6", QImage(30, 10, QImage::Format_RGB32)));
        QVERIFY(!ImageCache::find("4", &img));
        QVERIFY(!ImageCache::find("5", &img));

        // replace 6
        QVERIFY(ImageCache::insert("7", QImage(30, 10, QImage::Format_RGB32)));
        // can't insert
        QVERIFY(!ImageCache::insert("8", QImage(40, 10, QImage::Format_RGB32)));
        // but old cache exists
        QVERIFY(ImageCache::find("7", &img));
    }

    {   // fuzzing
        ImageCache::clear();
        ImageCache::setCacheLimit(102400);

        for(int i = 0; i < 1024; i++) {
            int rand = qrand();
            int w = rand % 20 + 1;
            int h = rand % 20 + 1;
            QString key = QString("key_%1").arg(rand);
            QVERIFY(ImageCache::insert(key, QImage(w, h, QImage::Format_RGB32)));
        }
    }
}

void TestImageCache::testLoadImage()
{
    ImageCache::clear();
    ImageCache::setCacheLimit(1024000);

    QImage img;
    QVERIFY(img.isNull());
    QVERIFY(!ImageCache::find("none2", &img));
    // empty path
    QVERIFY(!ImageCache::load("", &img));
    // NULL pointer
    QVERIFY(!ImageCache::load("none", (QImage*) NULL));
    // non existant
    QVERIFY(!ImageCache::load("none2", &img));
    // exist
    QVERIFY(img.isNull());
    QVERIFY(ImageCache::load(CF_IMAGE_DIR "/english.png", &img));
    QVERIFY(!img.isNull());
    QVERIFY(ImageCache::find(CF_IMAGE_DIR "/english.png", (QImage*) NULL));

}

static void threadLoad()
{
    QImage img;
    for(int i = 0; i < 10000; i++) {
        QVERIFY(ImageCache::load(CF_IMAGE_DIR "/english.png", &img));
    }
}

static void threadInsert()
{
    QImage img;
    for(int i = 0; i < 10000; i++) {
        int rand = qrand();
        int w = rand % 20 + 1;
        int h = rand % 20 + 1;
        QString key = QString("key_%1").arg(rand);
        QVERIFY(ImageCache::insert(key, QImage(w, h, QImage::Format_RGB32)));
        ImageCache::find(key, &img);
    }
}

void TestImageCache::testMultiThred()
{
    ImageCache::clear();
    ImageCache::setCacheLimit(1024 * 1024);

    QFuture<void> futureLoad = QtConcurrent::run(&threadLoad);
    QFuture<void> futureInsert = QtConcurrent::run(&threadInsert);

    futureLoad.waitForFinished();
    futureInsert.waitForFinished();
}

QTEST_MAIN(TestImageCache)
