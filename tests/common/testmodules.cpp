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

#include "testmodules.h"
#include "common/modules.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestModules);

using namespace cf;

void TestModules::testModuleToString()
{
#define ASSERT_MODULE(mod, name) CPPUNIT_ASSERT_EQUAL(std::string(name), moduleToString(mod));

    ASSERT_MODULE(MODULE_NONE,   "NONE");
    ASSERT_MODULE(MODULE_PUMA,   "PUMA");
    ASSERT_MODULE(MODULE_RDIB,   "RDIB");
    ASSERT_MODULE(MODULE_RFRMT,  "RFRMT");
    ASSERT_MODULE(MODULE_RIMAGE, "RIMAGE");
    ASSERT_MODULE(MODULE_STD,    "STD");
    ASSERT_MODULE(MODULE_USAGE,  "USAGE");
    ASSERT_MODULE(MODULES_ALL,   "ALL");
    ASSERT_MODULE(0x8000000000000L, "INVALID");

#undef ASSERT_MODULE

}
