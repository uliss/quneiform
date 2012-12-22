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

#ifndef PAGESTORAGE_H
#define PAGESTORAGE_H

#include <vector>

#include "globus.h"

namespace cf {
namespace cpage {

class BackupPage;
typedef std::vector<BackupPage*> PageList;

class CLA_EXPO PageStorage 
{
    PageStorage();
public:
    int find(Handle page) const;
public:
    static PageStorage& instance();
    static PageList& pages();
    static Handle append(BackupPage& p);
    static Handle backupPage(Handle p);
    static void clear();
    static void clearPage(Handle p);
    static BackupPage& page(Handle p);
    static BackupPage& pageAt(size_t pos);
    static Handle pageHandleAt(size_t pos);
    static Handle pageType(Handle p);
    static size_t pagePosition(Handle p);
    static size_t size();
    static void remove(Handle p);
    static bool undo(Handle p, Handle num);
private:
    void clearPages();
    void removePage(BackupPage * p);
private:
    PageList pages_;
};

}
}

#endif // PAGESTORAGE_H
