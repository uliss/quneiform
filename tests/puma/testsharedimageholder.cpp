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

#ifdef __OpenBSD__
#include <sys/param.h> // for MAXLOGNAME
#endif

#include <boost/interprocess/managed_shared_memory.hpp>

#include "testsharedimageholder.h"
#include "puma/sharedimageholder.h"
#include "puma/sharedimage.h"
#include "puma/startprocess.h"
#include "puma/sharedmemoryremover.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSharedImageHolder);

#ifndef TEST_EXE_PATH
#define TEST_EXE_PATH
#endif

using namespace cf;

static const char * SHARED_MEMORY_KEY = "test_puma";

void TestSharedImageHolder::testConstruct()
{
    SharedMemoryRemover remover(SHARED_MEMORY_KEY);

    using namespace boost::interprocess;
    //Construct managed shared memory
    managed_shared_memory segment(create_only, SHARED_MEMORY_KEY, sizeof(SharedImage) * 2);

    SharedImageHolder holder(&segment);
    CPPUNIT_ASSERT(holder.image() != NULL);
    CPPUNIT_ASSERT(holder.image()->dataSize() == 0);
    CPPUNIT_ASSERT(holder.image()->name().empty());
    CPPUNIT_ASSERT(holder.image()->dimensions() == Size());

    uchar data[100];
    data[0] = 0;
    data[1] = 1;
    data[2] = 2;

    ImagePtr pimg(new Image(data, sizeof(data), Image::AllocatorNone));
    pimg->setFileName("sample.jpg");
    pimg->setSize(Size(100, 200));
    holder.image()->set(pimg);

    CPPUNIT_ASSERT_EQUAL(0, startProcess(TEST_EXE_PATH "/service_sharedimageholder", StringList()));
}
