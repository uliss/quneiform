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

#include <boost/scoped_ptr.hpp>

#include "testsharedresult.h"
#include "ced/cedpage.h"
#include "puma/sharedresult.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSharedResult);

using namespace cf;

typedef boost::scoped_ptr<SharedResult> PSRes;

void TestSharedResult::testConstruct()
{
    PSRes shres(new SharedResult);
    CPPUNIT_ASSERT(!shres->get().get());
}

void TestSharedResult::testStore()
{
    PSRes shres(new SharedResult);
    CEDPagePtr p(new CEDPage);
    p->setImageName("sample");
    p->setImageSize(Size(500, 600));

    shres->store(p);

    {
        CEDPagePtr res = shres->get();
        CPPUNIT_ASSERT_EQUAL(p->imageName(), res->imageName());
        CPPUNIT_ASSERT_EQUAL(p->imageSize(), res->imageSize());
    }
}
