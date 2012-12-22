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
typedef BackupPage * PageHandle;
typedef std::vector<PageHandle> PageList;

class CLA_EXPO PageStorage 
{
    PageStorage();
public:
    ~PageStorage();
public:
    /**
     * Appends page to storage
     * @return pointer to added page
     * @see pageAt()
     */
    static PageHandle append(const BackupPage& p);

    /**
     * Removes all pages from storage
     * @see remove()
     */
    static void clear();

    /**
     * Returns current page handle
     * @see currentPageNumber()
     */
    static PageHandle currentPage();

    /**
     * Returns current page number or -1 if not found
     * @see currentPage()
     */
    static int currentPageNumber();

    /**
     * Search for given page handle in storage
     * @return handle position or -1 if not found
     */
    static int findPage(PageHandle p);

    /**
     * Returns page handle at given position
     * @return NULL if not found
     */
    static PageHandle pageAt(size_t pos);

    /**
     * Returns page count
     */
    static size_t pageCount();

    /**
     * Removes page from storage
     * @param page - page handle
     * @see clear()
     */
    static void remove(PageHandle page);

    /**
     * Sets current page
     * @param pos - page position in storage
     * @return true on success
     */
    static bool setCurrentPage(size_t pos);
private:
    static PageStorage& instance();
    static PageList& pages();
    void clearPages();
    int find(PageHandle page) const;
    void removePage(PageHandle p);
private:
    PageList pages_;
    PageHandle current_;
};

}
}

#endif // PAGESTORAGE_H
