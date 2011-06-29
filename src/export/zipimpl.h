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

#ifndef ZIPIMPL_H
#define ZIPIMPL_H

#include <cstddef>
#include <string>

namespace cf {

class ZipImpl {
public:
    virtual ~ZipImpl() {}
    virtual void addFile(const std::string& fname) = 0;
    virtual size_t fileCount() const = 0;
    virtual bool hasFile(const std::string& fname) const = 0;
    virtual void removeFile(const std::string& fname) = 0;
    virtual bool save(const std::string& fname) = 0;
    virtual void setCompression(const std::string& fname, int level) = 0;
    virtual void setContent(const std::string& fname, const std::string& content) = 0;
    virtual void setSource(const std::string& fname, const std::string& sourceFile) = 0;
};

}

#endif // ZIPIMPL_H
