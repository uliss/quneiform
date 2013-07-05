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

#include <iomanip>
#include <boost/foreach.hpp>

#include "binarizeoptions.h"

namespace cf {

BinarizeOptions::BinarizeOptions() :
    binarizator_(BINARIZATOR_DEFAULT)
{
}

BinarizeOptions::BinarizeOptions(binarizator_t t) :
    binarizator_(t)
{
}

bool BinarizeOptions::hasOption(const std::string& key) const
{
    return options_.find(key) != options_.end();
}

bool BinarizeOptions::optionBool(const std::string& key, bool fallback) const
{
    if(!hasOption(key))
        return fallback;

    try {
        return boost::get<bool>(options_.find(key)->second);
    }
    catch(const boost::bad_get&) {
        throw Exception() << "not bool value for key:" << key;
    }
}

float BinarizeOptions::optionFloat(const std::string& key, float fallback) const
{
    if(!hasOption(key))
        return fallback;

    try {
        return boost::get<float>(options_.find(key)->second);
    }
    catch(const boost::bad_get&) {
        throw Exception() << "not float value for key:" << key;
    }
}

int BinarizeOptions::optionInt(const std::string& key, int fallback) const
{
    if(!hasOption(key))
        return fallback;
    try {
        return boost::get<int>(options_.find(key)->second);
    }
    catch(const boost::bad_get&) {
        throw Exception() << "not int value for key:" << key;
    }
}

std::string BinarizeOptions::optionString(const std::string& key, const std::string& fallback) const
{
    if(!hasOption(key))
        return fallback;

    try {
        return boost::get<std::string>(options_.find(key)->second);
    }
    catch(const boost::bad_get&) {
        throw Exception() << "not string value for key:" << key << options_.find(key)->second;
    }
}

binarizator_t BinarizeOptions::binarizator() const
{
    return binarizator_;
}

void BinarizeOptions::setBinarizator(binarizator_t t)
{
    binarizator_ = t;
}

void BinarizeOptions::setOption(const std::string& key, bool value)
{
    options_[key] = value;
}

void BinarizeOptions::setOption(const std::string& key, float value)
{
    options_[key] = value;
}

void BinarizeOptions::setOption(const std::string& key, int value)
{
    options_[key] = value;
}

void BinarizeOptions::setOption(const std::string& key, const char * value)
{
    options_[key] = std::string(value);
}

void BinarizeOptions::setOption(const std::string& key, const std::string& value)
{
    options_[key] = value;
}

static std::string toString(binarizator_t t)
{
    switch(t) {
    case BINARIZATOR_OTSU:
        return "otsu";
    case BINARIZATOR_DEZA:
        return "deza";
    case BINARIZATOR_KRONROD:
        return "kronrod";
    default:
        return "unknown";
    }
}

template<class T>
static void OPT(std::ostream& os, const std::string& name, const T& value) {
    static const int FIELD_WIDTH = 25;
    static const std::string INDENT(4, ' ');
    os << INDENT << std::left << std::setw(FIELD_WIDTH) << name + ": " << value << "\n";
 }

std::ostream& operator<<(std::ostream& os, const BinarizeOptions& bopts)
{
    os << "Binarize options:\n";

    OPT(os, "Binarizator:", toString(bopts.binarizator_));

    BOOST_FOREACH(BinarizeOptions::OptionMap::value_type v, bopts.options_) {
        OPT(os, v.first, v.second);
    }

    return os;
}

}
