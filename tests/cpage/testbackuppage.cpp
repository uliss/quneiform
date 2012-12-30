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

#include <fstream>
#include <string.h>

#include "testbackuppage.h"
#include "cpage/backup.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBackupPage);

using namespace cf::cpage;

void TestBackupPage::testInit()
{
    BackupPage p;
    CPPUNIT_ASSERT(p.empty());
    CPPUNIT_ASSERT(!p.backupCount());
    CPPUNIT_ASSERT(!p.backupAt(0));
    CPPUNIT_ASSERT(!p.current());
    CPPUNIT_ASSERT(!p.undo());
    CPPUNIT_ASSERT(!p.redo());
    p.clearBackups();
}

void TestBackupPage::testSave()
{
    const char * data = "some data";
    BackupPage p;
    p.setType(CPAGE_GetInternalType("page"));
    p.setData(CPAGE_GetInternalType("data"), (void*) data, strlen(data));
    p.makeBackup();
    p.makeBackup();
    p.backupAt(0)->setType(CPAGE_GetInternalType("backup"));
    std::ofstream os("test.cpage");
    CPPUNIT_ASSERT(os);
    CPPUNIT_ASSERT(p.save(os));
    os.close();

    std::ofstream bad;
    bad << 0x1234;
    CPPUNIT_ASSERT(!p.save(bad));
}

void TestBackupPage::testRestore()
{
    BackupPage p;
    std::ifstream is("test.cpage");
    CPPUNIT_ASSERT(is);
    p.restore(is);
    is.close();

    char data[10];
    memset(&data, 0, sizeof(data));
    p.getData(CPAGE_GetInternalType("data"), data, 9);
    CPPUNIT_ASSERT_EQUAL(std::string("some data"), std::string(data));
    CPPUNIT_ASSERT_EQUAL(size_t(2), p.backupCount());
    CPPUNIT_ASSERT_EQUAL(std::string("backup"), std::string(CPAGE_GetNameInternalType(p.backupAt(0)->type())));
}

void TestBackupPage::testMakeBackup()
{
    BackupPage p;
    Page * bak1 = p.makeBackup();
    CPPUNIT_ASSERT(bak1);
    CPPUNIT_ASSERT_EQUAL(bak1, p.current());
    CPPUNIT_ASSERT_EQUAL(bak1, p.backupAt(0));
    CPPUNIT_ASSERT_EQUAL(size_t(1), p.backupCount());

    p.clearBackups();
    CPPUNIT_ASSERT(!p.backupCount());
    CPPUNIT_ASSERT(!p.backupAt(0));
    CPPUNIT_ASSERT(!p.current());
}

void TestBackupPage::testUndoRedo()
{
    BackupPage p;
    p.setType(0xf);
    Page * bak1 = p.makeBackup();
    CPPUNIT_ASSERT_EQUAL(bak1, p.current());
    p.setType(0xff);
    Page * bak2 = p.makeBackup();
    CPPUNIT_ASSERT_EQUAL(bak2, p.current());
    p.setType(0xfff);
    Page * bak3 = p.makeBackup();
    CPPUNIT_ASSERT_EQUAL(0xfff, p.type());

    // 3 backups: 0xf and 0xff and 0xfff

    CPPUNIT_ASSERT_EQUAL(0xfff, bak3->type());
    CPPUNIT_ASSERT_EQUAL(0xff, bak2->type());
    CPPUNIT_ASSERT_EQUAL(0xf, bak1->type());

    CPPUNIT_ASSERT(p.undo());
    CPPUNIT_ASSERT_EQUAL(0xff, p.type());
    CPPUNIT_ASSERT(p.undo());
    CPPUNIT_ASSERT_EQUAL(0xf, p.type());
    CPPUNIT_ASSERT(p.undo());
    CPPUNIT_ASSERT(!p.undo());
    CPPUNIT_ASSERT_EQUAL(0xf, p.type());

    CPPUNIT_ASSERT(p.redo());
    CPPUNIT_ASSERT_EQUAL(0xff, p.type());
    CPPUNIT_ASSERT(p.redo());
    CPPUNIT_ASSERT_EQUAL(0xfff, p.type());
    CPPUNIT_ASSERT(!p.redo());

    CPPUNIT_ASSERT(p.undo());
    CPPUNIT_ASSERT_EQUAL(0xff, p.type());

    CPPUNIT_ASSERT(p.undo());
    CPPUNIT_ASSERT_EQUAL(0xf, p.type());
    CPPUNIT_ASSERT(p.undo());
    CPPUNIT_ASSERT_EQUAL(0xf, p.type());
    CPPUNIT_ASSERT(!p.undo());
}

void TestBackupPage::testCopy()
{
    BackupPage p1;
    p1.makeBackup();
    p1.makeBackup();

    BackupPage p2(p1);
    CPPUNIT_ASSERT_EQUAL(p1.backupCount(), p2.backupCount());
    CPPUNIT_ASSERT(p2.current() == p2.backupAt(1));

    BackupPage p3;
    p3.makeBackup();
    p3 = p1;

    CPPUNIT_ASSERT_EQUAL(p3.backupCount(), p1.backupCount());
    CPPUNIT_ASSERT(p3.current() == p3.backupAt(1));

    BackupPage p4;
    p3 = p4;
    CPPUNIT_ASSERT(p3.backupCount() == 0);
    CPPUNIT_ASSERT(p3.current() == NULL);

    BackupPage p5(p4);
    CPPUNIT_ASSERT(p5.backupCount() == 0);
    CPPUNIT_ASSERT(p5.current() == NULL);
}
