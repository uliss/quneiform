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

#ifndef ASSERT_RECOGNIZE_H
#define ASSERT_RECOGNIZE_H

#include <boost/algorithm/string/trim.hpp>

#include "common/image.h"
#include "ced/cedpageptr.h"
#include "export/debugexporter.h"

#define ASSERT_LOCAL_RECOGNIZE_RESULT(filename, ropts, fopts, result) {\
    cf::LocalRecognitionServer server;\
    server.setTextDebug(true);\
    fopts.writeBom(false);\
    cf::ImagePtr img = cf::ImageLoaderFactory::instance().load(filename);\
    server.recognize(img, ropts, fopts);\
    cf::DebugExporter exp(fopts);\
    std::ostringstream buf;\
    exp.exportTo(buf);\
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string(result));\
    }

#define ASSERT_PROCESS_RECOGNIZE_RESULT(filename, ropts, fopts, result) {\
    cf::ProcessRecognitionServer server;\
    fopts.writeBom(false);\
    cf::ImagePtr img = cf::ImageLoaderFactory::instance().load(filename);\
    cf::CEDPagePtr p = server.recognize(img, ropts, fopts);\
    CPPUNIT_ASSERT(p.get());\
    CPPUNIT_ASSERT(!p->empty());\
    cf::TextExporter exp(p, fopts);\
    std::ostringstream buf;\
    exp.exportTo(buf);\
    CPPUNIT_ASSERT_EQUAL(boost::algorithm::trim_copy(buf.str()), std::string(result));\
    }

#endif // ASSERT_RECOGNIZE_H
