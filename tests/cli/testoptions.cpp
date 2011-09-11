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

#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "testoptions.h"
#include "../../cli/options.h"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TestOptions);

void TestOptions::testPrintLanguages()
{
    stringstream buf;
    cf::printLanguages(buf);

    vector<string> res;
    string src = buf.str();
    boost::algorithm::split(res, src,
                            boost::algorithm::is_any_of("\n"),
                            boost::algorithm::token_compress_on);
    CPPUNIT_ASSERT(!res.empty());
    CPPUNIT_ASSERT_EQUAL((size_t) 28, res.size());
}

void TestOptions::testPrintSupportedFormats()
{
    stringstream buf;
    cf::printSupportedFormats(buf);

    vector<string> res;
    string src = buf.str();
    boost::algorithm::split(res, src,
                            boost::algorithm::is_any_of("\n"),
                            boost::algorithm::token_compress_on);
    CPPUNIT_ASSERT(!res.empty());
    CPPUNIT_ASSERT_EQUAL((size_t) 14, res.size());
}




