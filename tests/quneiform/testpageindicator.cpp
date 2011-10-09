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
#include <QLabel>
#include <QSignalSpy>
#include "testpageindicator.h"

#define private public
#include "gui/widgets/pageindicator.h"

TestPageIndicator::TestPageIndicator(QObject *parent) :
    QObject(parent)
{
}

void TestPageIndicator::testConstruct() {
    PageIndicator pi;
    pi.show();
    QVERIFY(pi.saved_->isVisible());
    QVERIFY(pi.recognized_->isVisible());
    QVERIFY(pi.warning_->isVisible());
}

void TestPageIndicator::testSave() {
    PageIndicator pi;
    pi.show();
    QVERIFY(pi.saved_->isVisible());

    pi.setSaved(false);
    QVERIFY(!pi.saved_->isVisible());

    pi.setSaved(true);
    QVERIFY(pi.saved_->isVisible());
}

void TestPageIndicator::testRecognized() {
    PageIndicator pi;
    pi.show();
    QVERIFY(pi.recognized_->isVisible());

    pi.setRecognized(false);
    QVERIFY(!pi.recognized_->isVisible());

    pi.setRecognized(true);
    QVERIFY(pi.recognized_->isVisible());
}

void TestPageIndicator::testWarning() {
    PageIndicator pi;
    pi.show();
    QVERIFY(pi.warning_->isVisible());

    pi.setWarning(false);
    QVERIFY(!pi.warning_->isVisible());

    pi.setWarning(true);
    QVERIFY(pi.warning_->isVisible());
}

QTEST_MAIN(TestPageIndicator);
