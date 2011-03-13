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

#ifndef LOADERS_COMMON_H
#define LOADERS_COMMON_H

#include <string>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <common/image.h>
#include <rdib/imageloader.h>
#include <cuneiform.h>

static std::string trim(const std::string& str) {
    size_t pos = str.find_last_not_of(" \n");
    return pos == str.size() ? str : str.substr(0, pos + 1);
}

#define ASSERT_BUFFER(buf, s) CPPUNIT_ASSERT_EQUAL(trim(buf.str()), std::string(s));

#define ASSERT_RECOGNIZE(loader, filename, str) {\
    ImagePtr img;\
    std::ostringstream buf;\
    CPPUNIT_ASSERT_NO_THROW(img = loader.load(std::string(LOADER_TEST_IMAGE_DIR) + filename));\
    Puma::instance().open(img);\
    Puma::instance().recognize();\
    Puma::instance().save(buf, cf::FORMAT_DEBUG);\
    Puma::instance().close();\
    ASSERT_BUFFER(buf, str);\
}

#endif // LOADERS_COMMON_H
