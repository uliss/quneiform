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

PageHandle PageStorage::append(const BackupPage& p)
{
    pages().push_back(new BackupPage(p));
    instance().current_ = pages().back();
    return pages().back();
}

Handle PageStorage::backupPage(Handle p)
{
    return page(p).BackUp();
}

void PageStorage::clear()
{
    instance().clearPages();
}

void PageStorage::clearPage(Handle p)
{
    page(p).Clear();
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

int PageStorage::findPage(PageHandle p)
{
    return instance().find(p);
}

int PageStorage::find(PageHandle page) const
{
    for(size_t i = 0; i < pages_.size(); i++) {
        if(pages_[i] == page)
            return i;
    }

    return -1;
}

BackupPage& PageStorage::page(Handle p)
{
    BackupPage * res = (BackupPage*) (p);
    return *res;
}

PageHandle PageStorage::pageAt(size_t pos)
{
    if(pos < pageCount())
        return pages().at(pos);

    return NULL;
}

Handle PageStorage::pageType(Handle p)
{
    return page(p).type();
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

bool PageStorage::undo(Handle p, Handle num)
{
    return page(p).Undo(num);
}

void PageStorage::clearPages()
{
    for(size_t i = 0; i < pages_.size(); i++)
        delete pages_[i];

    pages_.clear();
    current_ = NULL;
}

void PageStorage::removePage(PageHandle p)
{
    std::remove(pages_.begin(), pages_.end(), p);
    delete p;

    if(current_ == p)
        current_ = NULL;
}

}
}
