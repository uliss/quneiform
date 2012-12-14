/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <cstdio>
#include <string.h>

#include "testbackuppage.h"
#include "cpage/backup.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBackupPage);

using namespace cf;

void TestBackupPage::testInit()
{
    BackupPage p;
    CPPUNIT_ASSERT(p.empty());
    p.Clear();
}

void TestBackupPage::testSave()
{
    const char * data = "some data";
    BackupPage p;
    p.setType(CPAGE_GetInternalType("page"));
    p.setData(CPAGE_GetInternalType("data"), (void*) data, strlen(data));
    FILE * f = fopen("test.cpage", "w");
    CPPUNIT_ASSERT(f);
    p.Save(f);
    fclose(f);
}

void TestBackupPage::testRestore()
{
    BackupPage p;
    FILE * f = fopen("test.cpage", "r");
    CPPUNIT_ASSERT(f);
    p.Restore(f);
    fclose(f);

    char data[10];
    memset(&data, 0, sizeof(data));
    p.getData(CPAGE_GetInternalType("data"), data, 9);
    CPPUNIT_ASSERT_EQUAL(std::string("some data"), std::string(data));
}
