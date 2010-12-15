/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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


#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <QGraphicsItemGroup>

class Page;

class PageLayout : public QGraphicsItemGroup
{
public:
    PageLayout();
    void clear();
    void populate(const Page& page);
private:
    void populateChars(const Page& page);
    void populateLines(const Page& page);
    void populateParagraphs(const Page& page);
    void populatePictures(const Page& page);
};

#endif // PAGELAYOUT_H
