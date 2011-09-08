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

#include <cassert>
#include <algorithm>
#include <sstream>
#include <exception>
#include <boost/current_function.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "sharedoptions.h"
#include "common/serialize.h"
#include "common/helper.h"

namespace cf
{

typedef boost::archive::binary_oarchive BinOutputArchive;
typedef boost::archive::binary_iarchive BinInputArchive;

SharedOptions::SharedOptions() :
    format_size_(0), recognize_size_(0)
{
    clear();
}

void SharedOptions::clear() {
    format_size_ = 0;
    recognize_size_ = 0;
    std::fill(format_data_, format_data_ + MAXSIZE, '\0');
    std::fill(recognize_data_, recognize_data_ + MAXSIZE, '\0');
}

FormatOptions SharedOptions::formatOptions() const {
    if(format_size_ == 0)
        return FormatOptions();

    try {
        std::stringstream buf;
        buf.write(format_data_, format_size_);
        BinInputArchive ar(buf);

        FormatOptions opts;
        ar >> opts;

        return opts;
    }
    catch(std::exception& e) {
        std::cerr << BOOST_CURRENT_FUNCTION << " error while reading from shread memory: "
                  << e.what() << std::endl;
        return FormatOptions();
    }
}

RecognizeOptions SharedOptions::recognizeOptions() const {
    if(recognize_size_ == 0)
        return RecognizeOptions();

    try {
        std::stringstream buf;
        buf.write(recognize_data_, recognize_size_);
        BinInputArchive ar(buf);

        RecognizeOptions opts;
        ar >> opts;

        return opts;
    }
    catch(std::exception& e) {
        std::cerr << BOOST_CURRENT_FUNCTION << " error while reading from shread memory: "
                  << e.what() << std::endl;
        return RecognizeOptions();
    }
}

void SharedOptions::store(const FormatOptions& opts) {
    try {
        std::stringstream buf;
        BinOutputArchive ar(buf);
        ar << opts;

        format_size_ = streamSize(buf);
        if(format_size_ >= MAXSIZE) {
            std::cerr << BOOST_CURRENT_FUNCTION << ": format options is too big" << std::endl;
            return;
        }

        buf.read(format_data_, format_size_);
    }
    catch(std::exception& e) {
        std::cerr << BOOST_CURRENT_FUNCTION << " error while storing format options"
                     " into shared memory: " << e.what() << std::endl;
    }
}

void SharedOptions::store(const RecognizeOptions& opts) {
    try {
        std::stringstream buf;
        BinOutputArchive ar(buf);
        ar << opts;

        recognize_size_ = streamSize(buf);
        if(recognize_size_ >= MAXSIZE) {
            std::cerr << BOOST_CURRENT_FUNCTION << ": recognize options is too big" << std::endl;
            return;
        }

        buf.read(recognize_data_, recognize_size_);
    }
    catch(std::exception& e) {
        std::cerr << BOOST_CURRENT_FUNCTION << " error while storing recognize options"
                     " into shared memory: " << e.what() << std::endl;
    }
}

}
