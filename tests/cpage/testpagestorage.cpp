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

#include <string.h>

#include "testpagestorage.h"
#include "cpage/backup.h"
#include "cpage/pagestorage.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPageStorage);

using namespace cf::cpage;

void TestPageStorage::testInit()
{
    CPPUNIT_ASSERT(!PageStorage::pageCount());
    CPPUNIT_ASSERT(!PageStorage::currentPage());
    CPPUNIT_ASSERT_EQUAL(int(-1), PageStorage::currentPageNumber());
    CPPUNIT_ASSERT_EQUAL(int(-1), PageStorage::findPage(NULL));
    CPPUNIT_ASSERT_EQUAL(PageHandle(0), PageStorage::pageAt(0));
    CPPUNIT_ASSERT(!PageStorage::setCurrentPage(0));
}

void TestPageStorage::testAppend()
{
    BackupPage p;
    uchar data[10];
    memset(data, 0xff, sizeof(data));
    p.setData(0, data, sizeof(data));
    PageHandle new_p = PageStorage::append(p);
    CPPUNIT_ASSERT_EQUAL(size_t(1), PageStorage::pageCount());
    CPPUNIT_ASSERT(new_p);
    CPPUNIT_ASSERT(new_p->dataPtr());
    CPPUNIT_ASSERT(new_p->dataSize() == sizeof(data));
    CPPUNIT_ASSERT(new_p->dataPtr()[0] == 0xff);
    CPPUNIT_ASSERT_EQUAL(PageStorage::currentPage(), new_p);
    CPPUNIT_ASSERT_EQUAL(PageStorage::pageAt(0), new_p);
    CPPUNIT_ASSERT_EQUAL(int(0), PageStorage::currentPageNumber());
    PageStorage::clear();
}

void TestPageStorage::testRemove()
{
    PageStorage::remove((PageHandle) 0xffff);
    CPPUNIT_ASSERT_EQUAL(size_t(0), PageStorage::pageCount());

    PageStorage::append(BackupPage());
    PageStorage::append(BackupPage());
    CPPUNIT_ASSERT_EQUAL(size_t(2), PageStorage::pageCount());

    PageStorage::remove(NULL);
    CPPUNIT_ASSERT_EQUAL(size_t(2), PageStorage::pageCount());
    CPPUNIT_ASSERT_EQUAL(PageStorage::pageAt(1), PageStorage::currentPage());
    PageStorage::remove(PageStorage::pageAt(1));
    CPPUNIT_ASSERT_EQUAL(size_t(1), PageStorage::pageCount());
    CPPUNIT_ASSERT_EQUAL(PageHandle(0), PageStorage::currentPage());
    CPPUNIT_ASSERT(PageStorage::setCurrentPage(0));
    PageStorage::remove(PageStorage::pageAt(0));
    CPPUNIT_ASSERT_EQUAL(size_t(0), PageStorage::pageCount());
}
