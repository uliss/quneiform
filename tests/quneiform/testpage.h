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


#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QObject>

class TestPage : public QObject
{
    Q_OBJECT
public:
    explicit TestPage(QObject *parent = 0);
public slots:
    void callTimerSlot(const char * member, int msec = 500);
    void sendDialogAccept();
    void sendDialogCancel();
    void handlePageChange();
    void handlePageRotate();
    void handlePageThumbChange();
private slots:
    void initTestCase();
    void testAngle();
    void testSetAngle();
    void testConstruct();
    void testPageName();
    void testFlags();
    void testRotate();
    void testScale();
    void testSetCEDPage();
    void testSetFlag();
    void testSetFlags();
    void testSetRecognitionSettings();
    void testSetBlocks();
    void testSetViewScroll();
    void testUnsetFlag();
    void testReadWrite();
    void testMultiThreadFuzzing();
    void testReadBoundingRect();
    void testMapPointFromBackend();
    void testMapPointFromBackendWithReadArea();
    void testMapFromBackend();
    void testMapToBackend();
    void testRects();
};

#endif // TESTPAGE_H
