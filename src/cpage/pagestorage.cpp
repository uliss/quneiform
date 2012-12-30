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

#include "pagestorage.h"
#include "backup.h"

namespace cf {
namespace cpage {

PageStorage::PageStorage() :
    current_(NULL)
{}

PageStorage::~PageStorage()
{
    clearPages();
    clearNameDataPrivate();
}

PageStorage& PageStorage::instance()
{
    static PageStorage s;
    return s;
}

PageList& PageStorage::pages()
{
    return instance().pages_;
}

Handle PageStorage::appendName(const char *name)
{
    namedata_.push_back(name);
    return reinterpret_cast<Handle>(namedata_.size());
}

PageHandle PageStorage::append(const BackupPage& p)
{
    pages().push_back(new BackupPage(p));
    instance().current_ = pages().back();
    return pages().back();
}

Handle PageStorage::appendNameData(const char * name)
{
    return instance().appendName(name);
}

void PageStorage::clear()
{
    instance().clearPages();
}

PageHandle PageStorage::currentPage()
{
    return instance().current_;
}

int PageStorage::currentPageNumber()
{
    if(!currentPage())
        return -1;

    return findPage(currentPage());
}

Handle PageStorage::findNameData(const char * name)
{
    return instance().findName(name);
}

int PageStorage::findPage(PageHandle p)
{
    return instance().find(p);
}

const char * PageStorage::namedata(Handle type)
{
    return instance().namedata_.at((size_t) type - 1).c_str();
}

int PageStorage::find(PageHandle page) const
{
    for(size_t i = 0; i < pages_.size(); i++) {
        if(pages_[i] == page)
            return i;
    }

    return -1;
}

Handle PageStorage::findName(const char * name)
{
    NameMap::iterator it = std::find(namedata_.begin(), namedata_.end(), name);
    if(it == namedata_.end())
        return NULL;
    return reinterpret_cast<Handle>(std::distance(namedata_.begin(), it) + 1);
}

PageHandle PageStorage::pageAt(size_t pos)
{
    if(pos < pageCount())
        return pages().at(pos);

    return NULL;
}

size_t PageStorage::pageCount()
{
    return pages().size();
}

void PageStorage::remove(PageHandle p)
{
    instance().removePage(p);
}

bool PageStorage::setCurrentPage(size_t pos)
{
    if(pos < pageCount()) {
        instance().current_ = pages().at(pos);
        return true;
    }

    return false;
}

void PageStorage::clearNameData()
{
    instance().clearNameDataPrivate();
}

void PageStorage::clearPages()
{
    for(size_t i = 0; i < pages_.size(); i++)
        delete pages_[i];

    pages_.clear();
    current_ = NULL;
}

void PageStorage::clearNameDataPrivate()
{
    namedata_.clear();
}

void PageStorage::removePage(PageHandle p)
{
    PageList::iterator it = std::find(pages_.begin(), pages_.end(), p);
    if(it != pages_.end()) {
        pages_.erase(it);
        delete p;
    }

    if(current_ == p)
        current_ = NULL;
}

}
}
