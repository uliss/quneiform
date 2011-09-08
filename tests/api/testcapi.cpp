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

#include <boost/algorithm/string/trim.hpp>

#include "testcapi.h"
#include "c/c_api.h"
#include "c/c_api_def.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCApi);

#ifndef TEST_IMG_PATH
#define TEST_IMG_PATH ""
#endif

#define TRUE 1
#define FALSE 0

#define CF_CHECK_SET_ROPTS(option) {\
    CPPUNIT_ASSERT_EQUAL(cf_recognition_set##_##option(opts, TRUE), 0);\
    CPPUNIT_ASSERT_EQUAL(cf_recognition##_##option(opts), TRUE);\
}

#define CF_CHECK_UNSET_ROPTS(option) {\
    CPPUNIT_ASSERT_EQUAL(cf_recognition_set##_##option(opts, FALSE), 0);\
    CPPUNIT_ASSERT_EQUAL(cf_recognition##_##option(opts), FALSE);\
}

#define CF_CHECK_SET_FOPTS(option) {\
    CPPUNIT_ASSERT_EQUAL(cf_format_set##_##option(opts, TRUE), 0);\
    CPPUNIT_ASSERT_EQUAL(cf_format##_##option(opts), TRUE);\
}

#define CF_CHECK_UNSET_FOPTS(option) {\
    CPPUNIT_ASSERT_EQUAL(cf_format_set##_##option(opts, FALSE), 0);\
    CPPUNIT_ASSERT_EQUAL(cf_format##_##option(opts), FALSE);\
}

void TestCApi::testRecognitionOptions() {
    cf_recognition_options opts = cf_recognition_options_create();
    // fax
    CF_CHECK_SET_ROPTS(fax)
    CF_CHECK_UNSET_ROPTS(fax)

    // dotmatrix
    CF_CHECK_SET_ROPTS(dotmatrix)
    CF_CHECK_UNSET_ROPTS(dotmatrix)

    // single column
    CF_CHECK_SET_ROPTS(single_column)
    CF_CHECK_UNSET_ROPTS(single_column)

    // search pictures
    CF_CHECK_SET_ROPTS(search_pictures)
    CF_CHECK_UNSET_ROPTS(search_pictures)

    // spell correction
    CF_CHECK_SET_ROPTS(spell_correction)
    CF_CHECK_UNSET_ROPTS(spell_correction)

    cf_recognition_options_destroy(opts);
}

void TestCApi::testFormatOptions() {
    cf_format_options opts = cf_format_options_create();
    // bom
    CF_CHECK_SET_FOPTS(write_bom)
    CF_CHECK_UNSET_FOPTS(write_bom)

    cf_format_options_destroy(opts);
}

void TestCApi::testAppend() {
    // null page
    CPPUNIT_ASSERT(cf_export_append(NULL, "", FORMAT_TEXT, NULL) != 0);

    cf_page page = cf_recognize(TEST_IMG_PATH "/english.png", NULL, NULL);
    CPPUNIT_ASSERT(page);

    // invalid format
    CPPUNIT_ASSERT(cf_export_append(page, "", -10, NULL) != 0);

    cf_page_destroy(page);
}

void TestCApi::testRecognize() {
    CPPUNIT_ASSERT(!cf_recognize(NULL, NULL, NULL));
    CPPUNIT_ASSERT(!cf_recognize("not-exists", NULL, NULL));
    CPPUNIT_ASSERT(!cf_recognize(TEST_IMG_PATH "/invalid.png", NULL, NULL));

    cf_page page = cf_recognize(TEST_IMG_PATH "/english.png", NULL, NULL);
    CPPUNIT_ASSERT(page);

#ifndef _WIN32
    CPPUNIT_ASSERT(cf_export_save(page, "/dev/stdout", FORMAT_TEXT, NULL) == 0);
#endif

    cf_page_destroy(page);
}

void TestCApi::testSaveToMemory() {
    cf_format_options opts = cf_format_options_create();
    cf_format_set_write_bom(opts, 0);
    cf_page page = cf_recognize(TEST_IMG_PATH "/english.png", NULL, opts);
    CPPUNIT_ASSERT(page);

    char small_buffer[5];
    size_t small_buffer_size = sizeof(small_buffer);
    CPPUNIT_ASSERT(cf_export_save_to_memory(page, small_buffer, &small_buffer_size, FORMAT_TEXT, NULL) == -1);

    // NULL page
    CPPUNIT_ASSERT(cf_export_save_to_memory(NULL, small_buffer, &small_buffer_size, FORMAT_TEXT, NULL) == -1);

    // NULL destination
    CPPUNIT_ASSERT(cf_export_save_to_memory(page, NULL, &small_buffer_size, FORMAT_TEXT, NULL) == -1);

    // NULL dest size
    CPPUNIT_ASSERT(cf_export_save_to_memory(page, small_buffer, NULL, FORMAT_TEXT, NULL) == -1);

    // invalid format
    CPPUNIT_ASSERT(cf_export_save_to_memory(page, small_buffer, &small_buffer_size, -10, NULL) == -1);

    char buffer[100];
    size_t buffer_size = sizeof(buffer);
    CPPUNIT_ASSERT(cf_export_save_to_memory(page, buffer, &buffer_size, FORMAT_TEXT, opts) == 0);

    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(std::string(buffer, buffer_size - 1)),
                         std::string("ENGLISH"));

    cf_format_options_destroy(opts);
    cf_page_destroy(page);
}

