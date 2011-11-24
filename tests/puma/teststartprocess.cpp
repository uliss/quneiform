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

#include "teststartprocess.h"
#include "puma/startprocess.h"
#include "puma/process_exit_codes.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestStartProcess);

using namespace cf;

void TestStartProcess::test()
{
    int status = 0;

    // non exists
    status = startProcess("non-exists", StringList());
    CPPUNIT_ASSERT_EQUAL((int) WORKER_UNKNOWN_ERROR, status);

#ifndef _WIN32
    status = startProcess("/bin/ls", StringList());
    CPPUNIT_ASSERT_EQUAL(0, status);

    StringList params;
    params.push_back("-$");
    status = startProcess("/bin/ls", params);
    CPPUNIT_ASSERT(status != 0);
#endif
}

