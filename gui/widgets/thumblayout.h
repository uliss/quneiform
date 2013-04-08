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
     * Returns thumb at given index position
     */
    ThumbnailWidget * at(int pos);

    /**
      * Returns thumbs count in layout
      */
    int count() const;

    /**
     * Checks if layout empty
     */
    bool isEmpty() const;

    /**
      * Returns thumb by given page
      */
    ThumbnailWidget * findByPage(Page * p);

    /**
      * Sets highlight for all thumbs
      */
    void highlightAll(bool value);

    /**
     * Returns thumb position index
     */
    int indexOf(ThumbnailWidget * thumb);
    int indexOf(Page * page);

    /**
      * Selects thumb, other selected thumbs remains unchanged
      * @see select(), selectRange()
      */
    void multiSelect(ThumbnailWidget * thumb);

    /**
      * Removes thumb from layout
      */
    void remove(ThumbnailWidget * thumb);

    /**
      * Sorts layout thumb by given pages order
      */
    void sortByPages(const QList<Page*>& pages);

    /**
      * Selectes given thumb
      * @see multiSelect(), selectRange(), selected()
      */
    void select(ThumbnailWidget * thumb);

    /**
      * Returns list of selected thumbs
      * @see select(), multiSelect(), selectAll()
      */
    QList<ThumbnailWidget*> selected();

    /**
      * Selects all thumbs
      * @see selected(), select(), multiSelect(), selectRange()
      */
    void selectAll();

    /**
      * Selects range between already selected thumbs and given one
      */
    void selectRange(ThumbnailWidget * thumb);

    /**
      * Returns layout size
      */
    QSizeF size() const;

    /**
      * Updates thumb names
      */
    void updateThumbNames();
private:
    void clearSelection();
    void selectRange(int begin, int end);
    void update();
private:
    QList<ThumbnailWidget*> thumbs_;
};

#endif // THUMBLAYOUT_H
