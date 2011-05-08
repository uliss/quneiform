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
#include <QPushButton>
#include "testrecognitionsettingsdialog.h"

#define private public
#include "gui/recognitionsettingsdialog.h"

TestRecognitionSettingsDialog::TestRecognitionSettingsDialog(QObject *parent) :
    QObject(parent)
{
}

void TestRecognitionSettingsDialog::test() {
//    RecognitionSettings s;
//    RecognitionSettingsDialog d(s);
//    QCOMPARE(s, d.settings());

//    d.open();
//    d.accept();

//    QCOMPARE(s, d.settings());
}

QTEST_MAIN(TestRecognitionSettingsDialog);
