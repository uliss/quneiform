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
#include <iomanip>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/preprocessor/control.hpp>
#include <boost/version.hpp>

#include "options.h"
#include "config-verbose.h" // for cmake config options
#include "alphabets/alphabetfactory.h"
#include "common/outputformat.h"
#include "common/language.h"
#include "rdib/imageloaderfactory.h"

namespace cf {

void printOption(std::ostream& os,
                 const std::string& longOpt,
                 const std::string& shortOpt,
                 const std::string& description)
{
    static const int SHORT_FIELD_WIDTH = 5;
    static const int LONG_FIELD_WIDTH = 33;
    static const int INDENT = 4;
    static const int TOTAL_INDENT = SHORT_FIELD_WIDTH + LONG_FIELD_WIDTH + INDENT;
    os << std::string(INDENT, ' ') << std::left << std::setw(SHORT_FIELD_WIDTH) << shortOpt;
    os << std::left << std::setw(LONG_FIELD_WIDTH) << longOpt;

    typedef std::vector<std::string> StringList;
    StringList lines;
    boost::algorithm::split(lines,
                            description,
                            boost::algorithm::is_any_of("\n"),
                            boost::algorithm::token_compress_on);

    if(lines.size() == 1) {
        os << description << "\n";
    }
    else {
        for(size_t i = 0; i < lines.size(); i++) {
            if(i == 0)
                os << lines[i] << "\n";
            else
                os << std::string(TOTAL_INDENT, ' ') << lines[i] << "\n";
        }
    }
}

void printSupportedFormats(std::ostream& os) {
    static const std::string INDENT(4, ' ');
    static const int FIELD_WIDTH = 12;

    os << "Supported formats:\n";
    OutputFormatList formats = OutputFormat::formats();
    for (OutputFormatList::iterator it = formats.begin(), end = formats.end(); it != end; ++it) {
        os << INDENT << std::left << std::setw(FIELD_WIDTH);
        os << OutputFormat::name(*it) << ' ' << OutputFormat::description(*it) << "\n";
    }
}

void printSupportedInputImageFormats(std::ostream& os) {
    static const std::string INDENT(4, ' ');

    os << "Supported image input formats:\n";
    ImageFormatList formats = ImageLoaderFactory::instance().supportedFormats();
    for(size_t i = 0; i < formats.size(); i++)
        os << INDENT << imageFormatToString(formats[i]) << "\n";
}

void printLanguages(std::ostream& os) {
    static const std::string INDENT(4, ' ');

    os << "Supported languages:\n";

    LanguageList langs = AlphabetFactory::instance().supportedLanguages();
    Language::sortByName(langs);
    for (LanguageList::iterator it = langs.begin(), end = langs.end(); it != end; ++it) {
        os << INDENT << std::left << std::setw(12) << Language::isoCode3(*it);
        os << std::setw(12) << Language::isoCode2(*it) << Language::isoName(*it) << "\n";
    }
}

void printVersion(std::ostream& os) {
    os << "Cuneiform " << CF_VERSION << "; build: " << CF_BUILD_NUMBER << "\n"
       << "system:    " << CMAKE_SYSTEM << "\n"
       << "build:     " << CMAKE_BUILD_TYPE << "\n"
       << "cpu:       " << CMAKE_SYSTEM_PROCESSOR << "\n"
       << "cpu arch:  " << CF_PLATFORM_BIT << "-bit\n"
       << "compiler:  " << CMAKE_CXX_COMPILER << "\n"
       << "compiler version:  " << CF_COMPILER_VERSION << "\n"
       << "cxx flags: " << CF_CXX_FLAGS << "\n";

#ifdef CMAKE_OSX_SYSROOT
    os << "osx root:  " << CMAKE_OSX_SYSROOT << "\n";
#endif

    os << "boost version: " << BOOST_VERSION << "\n";
}


}
