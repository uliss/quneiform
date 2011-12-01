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

#include <cstring>

#include "testsharedmemoryholder.h"
#include "shmem/sharedmemoryholder.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSharedMemoryHolder);

void TestSharedMemoryHolder::testInit()
{
    cf::SharedMemoryHolder sh;
    CPPUNIT_ASSERT(!sh.isAttached());
    CPPUNIT_ASSERT(sh.get() == NULL);
    CPPUNIT_ASSERT(sh.size() == 0);
}

void TestSharedMemoryHolder::testCreate()
{
    cf::SharedMemoryHolder sh;
    CPPUNIT_ASSERT_NO_THROW(sh.create("test", 1024));
    CPPUNIT_ASSERT(sh.isAttached());
    CPPUNIT_ASSERT(sh.size() == 1024);
    CPPUNIT_ASSERT(sh.get() != NULL);
    CPPUNIT_ASSERT_NO_THROW(sh.remove());
}

void TestSharedMemoryHolder::testRemove()
{
    cf::SharedMemoryHolder sh;

    CPPUNIT_ASSERT_NO_THROW(sh.create("test", 1024));

    CPPUNIT_ASSERT(sh.isAttached());
    CPPUNIT_ASSERT(sh.size() == 1024);
    CPPUNIT_ASSERT(sh.get() != NULL);

    CPPUNIT_ASSERT_NO_THROW(sh.remove());

    CPPUNIT_ASSERT(!sh.isAttached());
    CPPUNIT_ASSERT(sh.size() == 0);
    CPPUNIT_ASSERT(sh.get() == NULL);
}

class Message {
public:
    Message() : size_(0) {}

    std::string get() const {
        return std::string(msg_, 0, size_);
    }

    void set(const std::string& msg) {
        size_ = msg.length();
        strncpy(msg_, msg.c_str(), size_);
    }
private:
    size_t size_;
    char msg_[100];
};

void TestSharedMemoryHolder::testAttach()
{
    {
        cf::SharedMemoryHolder sh;
        CPPUNIT_ASSERT_NO_THROW(sh.create("test", 1024));
        Message * msg = sh.toObject<Message>(0);
        msg->set("Test message");
        CPPUNIT_ASSERT_NO_THROW(sh.detach());
    }

    {
        cf::SharedMemoryHolder sh;
        CPPUNIT_ASSERT_NO_THROW(sh.attach("test", 1024));
        CPPUNIT_ASSERT(sh.size() == 1024);
        CPPUNIT_ASSERT(sh.isAttached());
        CPPUNIT_ASSERT(sh.get() != NULL);

        Message * msg = sh.toObject<Message>(0);

        CPPUNIT_ASSERT_EQUAL(std::string("Test message"), msg->get());

        CPPUNIT_ASSERT_NO_THROW(sh.detach());
        CPPUNIT_ASSERT_NO_THROW(sh.remove());
    }
}
