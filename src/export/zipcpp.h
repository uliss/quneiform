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

#ifndef ZIPCPP_H
#define ZIPCPP_H

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

namespace cf {

class ZipImpl;

class ZipCpp : public boost::noncopyable
{
public:
    ZipCpp();

    enum compression_t {
        NONE = 0,
        FAST = 1,
        BEST = 9
    };

    void addFile(const std::string& zipName);
    bool empty() const;
    size_t fileCount() const;
    bool hasFile(const std::string& zipName);
    void removeFile(const std::string& zipName);
    bool save(const std::string& fname);
    void setCompression(const std::string& zipName, compression_t c);
    void setContent(const std::string& zipName, const std::string& content);
    void setSource(const std::string& zipName, const std::string& sourceFile);
private:
    std::auto_ptr<ZipImpl> impl_;
};

}

#endif // ZIPCPP_H
