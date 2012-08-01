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

#include "testmmapsharedmemory.h"
#include "shmem/mmapsharedmemory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestMMapSharedMemory);

using namespace cf;

void TestMMapSharedMemory::testCreate()
{
    MMapSharedMemory mmap;
    void * mem = mmap.create(0xBEEF, 1024);
    CPPUNIT_ASSERT(mem);

    char * dest = (char*) mem;
    sprintf(dest, "%s", "test message");

    mmap.close(mem);
}

void TestMMapSharedMemory::testOpen()
{
    MMapSharedMemory mmap;
    void * mem = mmap.open(0xBEEF, 1024);
    CPPUNIT_ASSERT(mem);
    CPPUNIT_ASSERT_EQUAL(std::string("test message"), std::string((char*) mem));
    mmap.close(mem);
}

void TestMMapSharedMemory::testRemove()
{
    MMapSharedMemory mmap;
    CPPUNIT_ASSERT(mmap.remove(0xBEEF));
}
