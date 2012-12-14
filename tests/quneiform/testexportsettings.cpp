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
#include "common/outputformat.h"

TestExportSettings::TestExportSettings(QObject * parent) :
    QObject(parent)
{
}

void TestExportSettings::testExtension() {
    ExportSettings s;

#define CHECK_EXT(settings, format) QCOMPARE(settings.extension(), QString(format));

    CHECK_EXT(s, "txt");

    s.setFormat(ExportSettings::SMARTTEXT);
    CHECK_EXT(s, "txt");

    s.setFormat(ExportSettings::HTML);
    CHECK_EXT(s, "html");

    s.setFormat(ExportSettings::HOCR);
    CHECK_EXT(s, "html");

    s.setFormat(ExportSettings::NATIVE);
    CHECK_EXT(s, "ced");

    s.setFormat(ExportSettings::NATIVEXML);
    CHECK_EXT(s, "xml");

    s.setFormat(ExportSettings::FB2);
    CHECK_EXT(s, "fb2");

    s.setFormat(ExportSettings::ODF);
    CHECK_EXT(s, "odt");

    s.setFormat(ExportSettings::DJVUTXT);
    CHECK_EXT(s, "txt");

    s.setFormat(ExportSettings::DJVUXML);
    CHECK_EXT(s, "xml");
}

void TestExportSettings::testCfFormat()
{
    ExportSettings s;

#define CHECK_FMT(settings, format) QCOMPARE(static_cast<cf::format_t>(settings.cfFormatType()), format);

    s.setFormat(ExportSettings::PLAINTEXT);
    CHECK_FMT(s, cf::FORMAT_TEXT);

    s.setFormat(ExportSettings::SMARTTEXT);
    CHECK_FMT(s, cf::FORMAT_SMARTTEXT);

    s.setFormat(ExportSettings::HTML);
    CHECK_FMT(s, cf::FORMAT_HTML);

    s.setFormat(ExportSettings::ODF);
    CHECK_FMT(s, cf::FORMAT_ODF);

    s.setFormat(ExportSettings::DJVUTXT);
    CHECK_FMT(s, cf::FORMAT_DJVUTXT);

    s.setFormat(ExportSettings::DJVUXML);
    CHECK_FMT(s, cf::FORMAT_DJVUXML);

    s.setFormat(ExportSettings::HOCR);
    CHECK_FMT(s, cf::FORMAT_HOCR);

    s.setFormat(ExportSettings::NATIVE);
    CHECK_FMT(s, cf::FORMAT_NATIVE_TXT);

    s.setFormat(ExportSettings::NATIVEXML);
    CHECK_FMT(s, cf::FORMAT_NATIVE_XML);

    s.setFormat(ExportSettings::FB2);
    CHECK_FMT(s, cf::FORMAT_FB2);

#undef CHECK_FMT
}

QTEST_MAIN(TestExportSettings)
