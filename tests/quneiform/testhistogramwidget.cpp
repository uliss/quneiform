/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include "testhistogramwidget.h"

#include <QTest>

#include "widgets/histogramwidget.h"

void TestHistogramWidget::testInit()
{
    std::vector<int> data;
    std::fill_n(std::back_inserter(data), 20, 20);
    data.push_back(1);
    data.push_back(2);
    data.push_back(2);
    data.push_back(45);
    data.push_back(32);
    data.push_back(1);
    data.push_back(2);
    data.push_back(1);
    data.push_back(1);
    data.push_back(1);
    data.push_back(0);
    data.push_back(0);
    data.push_back(1);

    HistogramWidget w;
    w.setFixedHeight(200);
    w.setFixedWidth(70);
    w.setData(data);
    w.setWindowModality(Qt::WindowModal);
    w.show();

    QTest::qWaitForWindowShown(&w);

    sleep(100);
}

QTEST_MAIN(TestHistogramWidget)
