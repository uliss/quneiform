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

#include "zipimpl.h"
#include "zipcpp.h"
#include "minizipimpl.h"

namespace cf {

ZipCpp::ZipCpp() {
    impl_.reset(new MiniZipImpl);
}

void ZipCpp::addFile(const std::string& name) {
    assert(impl_.get());
    impl_->addFile(name);
}

bool ZipCpp::empty() const {
    assert(impl_.get());
    return impl_->fileCount() == 0;
}

size_t ZipCpp::fileCount() const {
    assert(impl_.get());
    return impl_->fileCount();
}

bool ZipCpp::hasFile(const std::string& zipName) {
    assert(impl_.get());
    return impl_->hasFile(zipName);
}

void ZipCpp::removeFile(const std::string& name) {
    assert(impl_.get());
    impl_->removeFile(name);
}

bool ZipCpp::save(const std::string& name) {
    assert(impl_.get());
    return impl_->save(name);
}

void ZipCpp::setCompression(const std::string& zipName, compression_t c) {
    assert(impl_.get());
    impl_->setCompression(zipName, c);
}

void ZipCpp::setContent(const std::string& zipName, const std::string& content) {
    assert(impl_.get());
    impl_->setContent(zipName, content);
}

void ZipCpp::setSource(const std::string& zipName, const std::string& sourceFile) {
    assert(impl_.get());
    impl_->setSource(zipName, sourceFile);
}

}
