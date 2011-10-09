/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef THUMBLAYOUT_H
#define THUMBLAYOUT_H

#include <QList>
#include <QSizeF>

class ThumbnailWidget;
class Page;

class ThumbLayout
{
public:
    ThumbLayout();

    /**
      * Adds thumb at the layout end
      */
    void append(ThumbnailWidget * thumb);

    /**
      * Returns pointer to thumb at given position
      */
    ThumbnailWidget * at(int i);

    /**
      * Removes all thumbs from layout
      */
    void clear();

    /**
      * Returns thumbs count in layout
      */
    int count() const;

    ThumbnailWidget * findByPage(Page * p);
    void highlightAll(bool value);
    void insert(ThumbnailWidget * thumb, int pos);

    /**
      * Returns true if layout is empty
      */
    bool isEmpty() const;

    void multiSelect(ThumbnailWidget * thumbs);
    void remove(ThumbnailWidget * thumb);
    void sortByPages(const QList<Page*>& pages);
    void select(ThumbnailWidget * thumb, bool value);
    QList<ThumbnailWidget*> selected();
    void selectAll();
    void selectRange(ThumbnailWidget * thumb);
    void set(const QList<ThumbnailWidget*>& thumbs);
    QSizeF size() const;
    void update();
private:
    void clearSelection();
    void selectRange(int begin, int end);
private:
    QList<ThumbnailWidget*> thumbs_;
};

#endif // THUMBLAYOUT_H
