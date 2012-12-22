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

PageStorage::PageStorage()
{
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

Handle PageStorage::append(BackupPage& p)
{
    pages().push_back(new BackupPage(p));
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

int PageStorage::find(Handle page) const
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

BackupPage& PageStorage::pageAt(size_t pos)
{
    return page(pageHandleAt(pos));
}

Handle PageStorage::pageHandleAt(size_t pos)
{
    if(pos < size())
        return pages().at(pos);
    return NULL;
}

Handle PageStorage::pageType(Handle p)
{
    return page(p).type();
}

size_t PageStorage::pagePosition(Handle p)
{
    return instance().find(p);
}

size_t PageStorage::size()
{
    return pages().size();
}

void PageStorage::remove(Handle p)
{
    instance().removePage((BackupPage*) p);
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
}

void PageStorage::removePage(BackupPage * p)
{
    std::remove(pages_.begin(), pages_.end(), p);
    delete p;
}

}
}
