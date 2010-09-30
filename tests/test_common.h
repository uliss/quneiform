/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef TEST_COMMON_H_
#define TEST_COMMON_H_

#include <string>
#include <fstream>
#include <ced/cedarchive.h>

template<class T>
void writeToXml(cf::CEDXmlOutputArchive& ar, const std::string& obj_name, const T& object) {
    ar << boost::serialization::make_nvp(obj_name.c_str(), object);
}

template<class T>
void writeToXmlArchive(const std::string& filename, const std::string& obj_name, const T& object) {
    std::ofstream xml(filename.c_str());
    assert(xml);
    cf::CEDXmlOutputArchive ar(xml);
    writeToXml(ar, obj_name, object);
}

template<class T>
void writeToTextArchive(const std::string& filename, const T& object) {
    std::ofstream txt(filename.c_str());
    assert(txt);
    cf::CEDOutputArchive ar(txt);
    ar << object;
}

template<class T>
void readFromXml(cf::CEDXmlInputArchive& ar, const std::string& obj_name, T& object) {
    ar >> boost::serialization::make_nvp(obj_name.c_str(), object);
}

template<class T>
void readFromXmlArchive(const std::string& filename, const std::string& obj_name, T& object) {
    std::ifstream xml(filename.c_str());
    cf::CEDXmlInputArchive ar(xml);
    readFromXml(ar, obj_name, object);
}

template<class T>
void readFromTextArchive(const std::string& filename, T& object) {
    std::ifstream xml(filename.c_str());
    cf::CEDInputArchive ar(xml);
    ar >> object;
}

#endif /* TEST_COMMON_H_ */
