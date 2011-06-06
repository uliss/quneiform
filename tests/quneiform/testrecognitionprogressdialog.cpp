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
#include <QTimer>
#include <QSignalSpy>
#include "testrecognitionprogressdialog.h"
#include "gui/dialogs/recognitionprogressdialog.h"
#include "gui/pagerecognitionqueue.h"
#include "gui/page.h"
#include "gui/pagerecognizer.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR ""
#endif

TestRecognitionProgressDialog::TestRecognitionProgressDialog(QObject * parent) :
    QObject(parent)
{
}

void TestRecognitionProgressDialog::testConstruct() {
    RecognitionProgressDialog d;
}

void TestRecognitionProgressDialog::testDialog() {
    RecognitionProgressDialog d;
    PageRecognitionQueue q;
    q.recognizer()->setStageSleep(PageRecognizer::OPEN, 80);
    q.recognizer()->setStageSleep(PageRecognizer::RECOGNIZE, 80);
    q.recognizer()->setStageSleep(PageRecognizer::FORMAT, 80);

    d.connectToQueue(&q);

    Page p1(CF_IMAGE_DIR "/english.png");
    Page p2(CF_IMAGE_DIR "/russian.png");

    q.add(&p1);
    q.add(&p2);

    q.start();
}

void TestRecognitionProgressDialog::testCancel() {
    RecognitionProgressDialog d;
    PageRecognitionQueue q;
    d.connectToQueue(&q);

    q.recognizer()->setStageSleep(PageRecognizer::OPEN, 10);
    q.recognizer()->setStageSleep(PageRecognizer::RECOGNIZE, 10);
    q.recognizer()->setStageSleep(PageRecognizer::FORMAT, 10);

    Page p1(CF_IMAGE_DIR "/english.png");
    Page p2(CF_IMAGE_DIR "/russian.png");

    q.add(&p1);
    q.add(&p2);

    QSignalSpy finished(&q, SIGNAL(finished(int)));
    QTimer::singleShot(15, &q, SLOT(abort()));
    q.start();
    QVERIFY(q.isEmpty());

    QCOMPARE(finished.count(), 1);
    QCOMPARE(finished.at(0).at(0).toInt(), 0);
}

QTEST_MAIN(TestRecognitionProgressDialog);
