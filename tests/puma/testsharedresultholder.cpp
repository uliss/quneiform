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

#ifdef __OpenBSD__
#include <sys/param.h> // for MAXLOGNAME
#endif

#include <boost/interprocess/managed_shared_memory.hpp>

#include "testsharedresultholder.h"
#include "puma/sharedresultholder.h"
#include "puma/sharedresult.h"
#include "puma/sharedmemoryremover.h"
#include "puma/startprocess.h"
#include "ced/cedpage.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSharedResultHolder);

#ifndef TEST_EXE_PATH
#define TEST_EXE_PATH
#endif

using namespace cf;

static const char * SHARED_MEMORY_KEY = "test_puma";

void TestSharedResultHolder::testConstruct()
{
    SharedMemoryRemover remover(SHARED_MEMORY_KEY);

    using namespace boost::interprocess;
    managed_shared_memory segment(create_only, SHARED_MEMORY_KEY, sizeof(SharedResult) * 2);

    SharedResultHolder sholder(&segment);
    CEDPagePtr p(new CEDPage);
    p->setImageName("sample");
    p->setImageSize(Size(300, 400));
    sholder.sharedResult()->store(p);

    CPPUNIT_ASSERT_EQUAL(0, startProcess(TEST_EXE_PATH "/service_sharedresultholder", StringList()));
}
