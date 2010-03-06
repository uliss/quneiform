/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#include "testodfexporter.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestOdfExporter);

#include "export/odfexporter.h"
using namespace CIF;

void TestOdfExporter::testInit() {
    Exporter * e = new OdfExporter(NULL);
    CPPUNIT_ASSERT_NO_THROW(e->exportTo("tmp.odt"));
    CPPUNIT_ASSERT_THROW(e->exportTo(std::cout), Exporter::Exception);
    delete e;
}


