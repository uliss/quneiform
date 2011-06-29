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

#ifndef MINIZIPIMPL_H
#define MINIZIPIMPL_H

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "zipimpl.h"
#include "minizip/zip.h"

namespace cf {

class MiniZipFile {
public:
    MiniZipFile(const std::string& zip_fname);
    int compressionLevel() const;
    int compressionMethod() const;
    const std::string& content() const;
    void setCompressionLevel(int level);
    void setContent(const std::string& content);
    void setSource(const std::string& source);
    const std::string& source() const { return source_; }
    const std::string& zipName() const { return zip_fname_; }
private:
    std::string zip_fname_;
    std::string content_;
    std::string source_;
    int level_;
};

typedef boost::shared_ptr<MiniZipFile> FilePtr;

class MiniZipImpl : public ZipImpl
{
public:
    MiniZipImpl();
    void addFile(const std::string& fname);
    size_t fileCount() const;
    bool hasFile(const std::string& fname) const;
    void removeFile(const std::string& fname);
    bool save(const std::string& fname);
    void setCompression(const std::string& fname, int level);
    void setContent(const std::string& fname, const std::string& content);
    void setSource(const std::string& fname, const std::string& sourceFile);
private:
    typedef std::vector<FilePtr> FileList;
    typedef FileList::iterator iterator;
    typedef FileList::const_iterator const_iterator;
private:
    const_iterator find(const std::string& fname) const;
    iterator find(const std::string& fname);
    void saveFile(FilePtr f);
    void writeFile(FilePtr f, const char * data, size_t length);
    void writeFile(FilePtr f, const std::string& filename);
private:
    zipFile zf;
    FileList files_;
};

}

#endif // MINIZIPIMPL_H
