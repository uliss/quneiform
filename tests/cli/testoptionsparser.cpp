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
#include <cstdlib>

#include "testoptionsparser.h"
#include "../../cli/optionsparser.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestOptionsParser);

using namespace cf;

#define MAKE_ARGS(...) \
    const char * argv[] = {"cuneiform-test", __VA_ARGS__, NULL};\
    const int argc = sizeof(argv) / sizeof(argv[0]) - 1;

#define PARSE(parser) parser.parse(argc, (char**) argv)

#define PARSE_ARGS(parse, ...) {\
    MAKE_ARGS(__VA_ARGS__);\
    PARSE(parse);\
    }

#define CHECK_EXIT(parser, ...) {\
    MAKE_ARGS(__VA_ARGS__);\
    CPPUNIT_ASSERT_THROW(PARSE(parser), OptionsParser::ExitException);\
    }

#define CHECK_EXIT_CODE(parser, code, ...) {\
    MAKE_ARGS(__VA_ARGS__);\
        try {\
            PARSE(parser);\
        }\
        catch(OptionsParser::ExitException& e) {\
            CPPUNIT_ASSERT_EQUAL(code, e.exitCode());\
        }\
    }


struct NullCout {
    NullCout() { std::cout.flush(); std::cout.setstate(std::ios::failbit); }
    ~NullCout() { std::cout.clear(); }
};

struct NullCerr {
    NullCerr() { std::cerr.flush(); std::cerr.setstate(std::ios::failbit); }
    ~NullCerr() { std::cerr.clear(); }
};

void TestOptionsParser::testHelp()
{
    NullCout cout;
    OptionsParser p;

    CHECK_EXIT(p, "--help");
    CHECK_EXIT(p, "-h");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "--help");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "-h");
}

void TestOptionsParser::testVersion()
{
    NullCout cout;
    OptionsParser p;

    CHECK_EXIT(p, "--version");
    CHECK_EXIT(p, "-V");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "--version");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "-V");
}

void TestOptionsParser::testUnknown()
{
    NullCout cout;
    NullCerr cerr;
    OptionsParser p;

    CHECK_EXIT_CODE(p, EXIT_FAILURE, "--unknown");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "-U");
}

void TestOptionsParser::testNoInput()
{
    NullCout cout;
    NullCerr cerr;
    OptionsParser p;

    CHECK_EXIT_CODE(p, EXIT_FAILURE, "--bom");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "-b");
}

void TestOptionsParser::testFormat()
{
    NullCout cout;
    NullCerr cerr;
    OptionsParser p;

    CHECK_EXIT(p, "--format");
    CHECK_EXIT(p, "-f");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "--format");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "-f");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "--format", "help");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "-f", "help");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "--format", "unknown");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "-f", "unknown");

    PARSE_ARGS(p, "--format", "fb2", "output");
    CPPUNIT_ASSERT_EQUAL(FORMAT_FB2, p.cliOptions().outputFormat());
    PARSE_ARGS(p, "--format", "odf", "output");
    CPPUNIT_ASSERT_EQUAL(FORMAT_ODF, p.cliOptions().outputFormat());
}

void TestOptionsParser::testLanguages()
{
    NullCout cout;
    NullCerr cerr;
    OptionsParser p;

    CHECK_EXIT(p, "--language");
    CHECK_EXIT(p, "-l");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "--language");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "-l");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "--language", "help");
    CHECK_EXIT_CODE(p, EXIT_SUCCESS, "-l", "help");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "--language", "unknown");
    CHECK_EXIT_CODE(p, EXIT_FAILURE, "-l", "unknown");

    PARSE_ARGS(p, "--language", "it", "output");
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, p.formatOptions().language());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, p.recognizeOptions().language());
    PARSE_ARGS(p, "--language", "ru", "output");
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUSSIAN, p.formatOptions().language());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUSSIAN, p.recognizeOptions().language());
}

