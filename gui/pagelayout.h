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
    QGraphicsItemGroup * charBlocks();
    void clear();
    QGraphicsItemGroup * columnBlocks();
    QGraphicsItemGroup * lineBlocks();
    QGraphicsItemGroup * paragraphBlocks();
    QGraphicsItemGroup * pictureBlocks();
    void populate(const Page& page);
    QGraphicsItemGroup * sectionBlocks();
    typedef QGraphicsItemGroup * PageLayout::*GroupMember;
private:
    QColor blockColor(int type) const;
    void clearGroupBlocks(GroupMember ptr);
    QGraphicsRectItem * createBlock(const QRect& r, const QColor& color);
    void populateGroup(QGraphicsItemGroup * group, int group_type);
private:
    const Page * page_;
    QGraphicsItemGroup * chars_;
    QGraphicsItemGroup * columns_;
    QGraphicsItemGroup * lines_;
    QGraphicsItemGroup * paragraphs_;
    QGraphicsItemGroup * pictures_;
    QGraphicsItemGroup * sections_;
};

#endif // PAGELAYOUT_H
