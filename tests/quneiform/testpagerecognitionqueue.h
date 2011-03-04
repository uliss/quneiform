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

#ifndef TESTPAGERECOGNITIONQUEUE_H
#define TESTPAGERECOGNITIONQUEUE_H

#include <QObject>

class TestPageRecognitionQueue : public QObject
{
Q_OBJECT
public:
    explicit TestPageRecognitionQueue(QObject *parent = 0);
private slots:
    void testConstruct();
    void testAddPacket();
    void testAddPage();
    void testStart();
    void testSetLanguage();
    void testEmitStep();
    void testIsFailed();
    void testAbort();
    void testPercentDone();
};

#endif // TESTPAGERECOGNITIONQUEUE_H
