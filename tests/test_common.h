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
void writeToXmlArchive(const std::string& filename, const std::string& obj_name, const T& object) {
    std::ofstream xml(filename.c_str());
    assert(xml);
    CIF::CEDXmlOutputArchive ar(xml);
    ar << boost::serialization::make_nvp(obj_name.c_str(), object);
}

template<class T>
void writeToTextArchive(const std::string& filename, const T& object) {
    std::ofstream txt(filename.c_str());
    assert(txt);
    CIF::CEDOutputArchive ar(txt);
    ar << object;
}

template<class T>
void readFromXmlArchive(const std::string& filename, const std::string& obj_name, T& object) {
    std::ifstream xml(filename.c_str());
    CIF::CEDXmlInputArchive ar(xml);
    ar >> boost::serialization::make_nvp(obj_name.c_str(), object);
}

template<class T>
void readFromTextArchive(const std::string& filename, T& object) {
    std::ifstream xml(filename.c_str());
    CIF::CEDInputArchive ar(xml);
    ar >> object;
}

#endif /* TEST_COMMON_H_ */
