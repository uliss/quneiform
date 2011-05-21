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
#include "testexportsettings.h"
#include "gui/exportsettings.h"

TestExportSettings::TestExportSettings(QObject * parent) :
    QObject(parent)
{
}

void TestExportSettings::testExtension() {
    ExportSettings s;

#define CHECK_EXT(settings, format) QCOMPARE(settings.extension(), QString(format));

    CHECK_EXT(s, "txt")

    s.setFormat(ExportSettings::SMARTTEXT);
    CHECK_EXT(s, "txt")

    s.setFormat(ExportSettings::HTML);
    CHECK_EXT(s, "html")

    s.setFormat(ExportSettings::ODF);
    CHECK_EXT(s, "odt");

    s.setFormat(ExportSettings::DJVUTXT);
    CHECK_EXT(s, "txt");

    s.setFormat(ExportSettings::DJVUXML);
    CHECK_EXT(s, "xml");
}

QTEST_MAIN(TestExportSettings)
