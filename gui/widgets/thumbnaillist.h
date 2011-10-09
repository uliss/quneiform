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

#include <QGraphicsView>
#include <QList>

class QMenu;

class Packet;
class Page;
class Language;
class ThumbnailWidget;
class ThumbLayout;
class ThumbScene;

class ThumbnailList: public QGraphicsView
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
      * Returns selected thumbs
      */
    QList<ThumbnailWidget*> selected();

    /**
      * Sets corresponding document for thumbnail list
      */
    void setDocument(Packet * doc);

    /**
      * Sets language for selected thumbs
      * if none selected - changes language for all thumbs
      */
    void setLanguage(const Language& lang);

    /**
      * Returns thumbnail that correspondents to given page
      * @param page
      * @return NULL if nothing found
      */
    ThumbnailWidget * thumb(Page * page);

    void updateLayout();
signals:
    void openDraggedImages(const QStringList& lst);
    void save(Page * page);
    void showPageFault(Page * page);
    void thumbSelected(Page * page);
    void thumbRecognize(Page * page);
    void thumbRecognizeList(const QList<Page*>& page);
    void thumbRemovalFinished(Page * page);
public slots:
    void setupContextMenu(QMenu*);
    void selectAll();
private:
    typedef QList<ThumbnailWidget*> ThumbList;
    void highlightAll(bool value);
    bool isValidThumbDropPosition(const QPointF& scenePos);
    ThumbnailWidget * targetDropThumb(const QPointF& pos);
    void setupActions();
    void setupLayout();
    void setupScene();
    void setScrollBars();
    /** Updates thumbnail names */
    void updateThumbNames();
private slots:
    void handleThumbDrag(ThumbnailWidget * sender, const QPointF& scenePos);
    void handleThumbDrop(ThumbnailWidget * sender, const QPointF& scenePos);
    void pageAdd(Page * page);
    void pageRemove(Page * page);
    void reorder();
    void recognizeSelectedPages(Page * page);
    void removeSelectedPages(Page * page);
    void thumbClick(int modifiers);
private:
    Packet * packet_;
    ThumbLayout * layout_;
    Page * current_page_;
    QAction * select_all_;
    ThumbScene * scene_;
    bool drag_in_progress_;
};

#endif /* THUMBNAILLIST_H_ */
