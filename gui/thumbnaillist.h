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

#ifndef THUMBNAILLIST_H_
#define THUMBNAILLIST_H_

#include <QtGui/QScrollArea>
#include <QtCore/QList>

class QVBoxLayout;
class QMenu;

class Document;
class Page;
class ThumbnailWidget;

class ThumbnailList: public QScrollArea
{
    Q_OBJECT
    public:
        ThumbnailList(QWidget * parent);

        /**
         * Appends thumbnail widget to the end of the list
         * @param thumb
         */
        void append(ThumbnailWidget * thumb);

        /**
         * Returns current selected page
         * if no page selected returns NULL
         */
        Page * currentPage();

        /**
         * Returns number of thumbs in list
         */
        int count() const;

        /**
         * Removes thumbnail
         */
        void thumbRemove(ThumbnailWidget * thumb);

        /**
         * Sets corresponding document for thumbnail list
         */
        void setDocument(Document * doc);

        /**
         * Returns thumbnail that correspondents to given page
         * @param page
         * @return NULL if nothing found
         */
        ThumbnailWidget * thumb(Page * page);
    signals:
        void thumbSelected(Page * page);
        void thumbRecognize(Page * page);
        void thumbRemovalFinished(Page * page);
    public slots:
        void handleInvalidImage(const QString& path);
        void setupContextMenu(QMenu*);
        void selectAll();
    private:
        typedef QList<ThumbnailWidget*> ThumbList;

        /**
         * Highlights given thumb and removes highlight of others
         * @param thumb
         */
        void highlightThumb(ThumbnailWidget * thumb);
        void setupLayout();
        void setScrollBars();
        /** Updates thumbnail names */
        void updateThumbNames();
    private slots:
        void pageAdd(Page * page);
        void pageRemove(Page * page);
        void removeSelectedPages();
        void revertSelection();
        void thumbClick();
    private:
        Document * document_;
        QVBoxLayout * layout_;
        ThumbList thumbs_;
        Page * current_page_;
};

#endif /* THUMBNAILLIST_H_ */
