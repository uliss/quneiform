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

#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <boost/current_function.hpp>
#include <boost/scoped_array.hpp>

#include "common/debug.h"
#include "minizipimpl.h"

#define ERROR_BUFFER(buf) std::ostringstream buf; buf << BOOST_CURRENT_FUNCTION << " "
#define MINIZIP_THROW(buf) throw std::runtime_error(buf.str().c_str())

namespace cf {

static const size_t BUFFER_SIZE = 16384;

MiniZipFile::MiniZipFile(const std::string& zip_fname) :
        zip_fname_(zip_fname),
        level_(Z_BEST_COMPRESSION) {
}

int MiniZipFile::compressionLevel() const {
    return level_;
}

int MiniZipFile::compressionMethod() const {
    return level_ != 0 ? Z_DEFLATED : 0;
}

const std::string& MiniZipFile::content() const {
    return content_;
}

void MiniZipFile::setCompressionLevel(int level) {
    level_ = level;
}

void MiniZipFile::setContent(const std::string& content) {
    content_ = content;
    source_.clear();
}

void MiniZipFile::setSource(const std::string& source) {
    source_ = source;
    content_.clear();
}

MiniZipImpl::MiniZipImpl()
{}

void MiniZipImpl::addFile(const std::string& fname) {
    if (hasFile(fname)) {
        Debug() << BOOST_CURRENT_FUNCTION << ": file " << fname << " already exists in zip archive" << std::endl;
        return;
    }

    FilePtr f(new MiniZipFile(fname));
    files_.push_back(f);
}

size_t MiniZipImpl::fileCount() const {
    return files_.size();
}

static bool hasZipName(FilePtr f, const std::string& name) {
    return f->zipName() == name;
}

MiniZipImpl::const_iterator MiniZipImpl::find(const std::string& fname) const {
    return std::find_if(
            files_.begin(),
            files_.end(),
            std::bind2nd(std::ptr_fun(&hasZipName), fname));
}

MiniZipImpl::iterator MiniZipImpl::find(const std::string& fname) {
    return std::find_if(files_.begin(),
                        files_.end(),
                        std::bind2nd(std::ptr_fun(hasZipName), fname));
}

bool MiniZipImpl::hasFile(const std::string& fname) const {
    return  find(fname) != files_.end();
}

void MiniZipImpl::removeFile(const std::string& fname) {
    iterator last = std::remove_if(files_.begin(),
                                   files_.end(),
                                   std::bind2nd(std::ptr_fun(hasZipName), fname));

    files_.erase(last, files_.end());
}

bool MiniZipImpl::save(const std::string& fname) {
    zf = zipOpen(fname.c_str(), APPEND_STATUS_CREATE);

    if(zf == NULL) {
        Debug() << BOOST_CURRENT_FUNCTION << ": error opening " << fname << std::endl;
        return false;
    }
    else
        Debug() << BOOST_CURRENT_FUNCTION << ": creating " << fname << std::endl;

    try {
        for(iterator it = files_.begin(), end = files_.end(); it != end; ++it)
            saveFile(*it);
    }
    catch(std::runtime_error& e) {
        Debug() << BOOST_CURRENT_FUNCTION << ": Exception occured while saving file " << fname << std::endl;
        Debug() << e.what();
        return false;
    }

    if(zipClose(zf, NULL) != ZIP_OK) {
        Debug() << BOOST_CURRENT_FUNCTION << ": error in closing " << fname << std::endl;
        return false;
    }

    return true;
}

void MiniZipImpl::saveFile(FilePtr f) {
    assert(zf);

    zip_fileinfo zi;

    zi.tmz_date.tm_sec =
            zi.tmz_date.tm_min =
            zi.tmz_date.tm_hour =
            zi.tmz_date.tm_mday =
            zi.tmz_date.tm_mon =
            zi.tmz_date.tm_year = 0;

    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;

    int err = zipOpenNewFileInZip2(zf,
                                   f->zipName().c_str(),
                                   &zi,
                                   NULL, // extra field local
                                   0,    // extra field local size
                                   NULL, // extra field global
                                   0,    // extra field global size
                                   NULL, // comment
                                   f->compressionMethod(), // method
                                   f->compressionLevel(), // level
                                   0);

    if (err != ZIP_OK) {
        ERROR_BUFFER(buf) << "error in opening " << f->zipName() << "in zipfile" << std::endl;
        MINIZIP_THROW(buf);
    }

    if(!f->content().empty())
        writeFile(f, f->content().c_str(), f->content().size());
    else
        writeFile(f, f->source());



    if (zipCloseFileInZip(zf) != ZIP_OK) {
        ERROR_BUFFER(buf) << "error in closing " << f->zipName() << " in the zipfile" << std::endl;
        MINIZIP_THROW(buf);
    }
}

void MiniZipImpl::setCompression(const std::string& fname, int level) {
    iterator it = find(fname);
    if(it == files_.end())
        return;

    (*it)->setCompressionLevel(level);
}

void MiniZipImpl::setContent(const std::string& fname, const std::string& content) {
    iterator it = find(fname);
    if(it == files_.end()) {
        Debug() << BOOST_CURRENT_FUNCTION << ": invalid filename" << std::endl;
        return;
    }

    (*it)->setContent(content);
}

void MiniZipImpl::setSource(const std::string& fname, const std::string& sourceFile) {
    iterator it = find(fname);
    if(it == files_.end()) {
        Debug() << BOOST_CURRENT_FUNCTION << ": invalid filename" << std::endl;
        return;
    }

    (*it)->setSource(sourceFile);
}

void MiniZipImpl::writeFile(FilePtr f, const char * data, size_t len) {
    if (zipWriteInFileInZip(zf, data, len) != ZIP_OK) {
        ERROR_BUFFER(buf) << "error in writing " << f->zipName() << " in the zipfile" << std::endl;
        MINIZIP_THROW(buf);
    }
}

void MiniZipImpl::writeFile(FilePtr f, const std::string& filename) {
    std::ifstream in(filename.c_str());

    if (!in) {
        ERROR_BUFFER(buf) << "can't open file " << filename << std::endl;
        MINIZIP_THROW(buf);
    }

    boost::scoped_array<char> buffer;

    in.seekg (0, std::ios::end);
    size_t length = in.tellg();
    in.seekg (0, std::ios::beg);

    if(length == 0) {
        writeFile(f, "", 0);
        return;
    }

    buffer.reset(new char[length]);
    in.read(buffer.get(), length);
    writeFile(f, buffer.get(), length);
}

}
