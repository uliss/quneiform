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
      * Returns current selected page
      * if no page selected returns NULL
      */
    Page * currentPage();

    /**
      * Sets corresponding packet for thumbnail list
      */
    void setPacket(Packet * packet);

    /**
      * Sets language for selected thumbs
      * if none selected - changes language for all thumbs
      */
    void setLanguage(const Language& lang);
signals:
    /**
      * Emitted on drag and drop attempt into thumbnail list
      * @param list file paths
      */
    void openDraggedImages(const QStringList& lst);

    /**
      * Emitted on page recognize request
      * @param page - target page
      */
    void recognizePage(Page * page);

    /**
      * Emitted on several page recognize request
      * @param pages - target pages
      */
    void recognizePages(const QList<Page*>& pages);

    /**
      * Emitted on save page request
      * @param page - target page
      */
    void savePage(Page * page);

    /**
      * Emitted on show page request
      * @param page - target page
      */
    void showPage(Page * page);

    /**
      * Emitted on page recognition error request
      */
    void showPageFault(Page * page);
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    bool isValidDropPos(const QPointF& scenePos);
    ThumbnailWidget * findThumbByPos(const QPointF& scenePos);
    void setDragCursor(bool value);
    void setupActions();
    void setupActionDelete();
    void setupActionOpenExternal();
    void setupActionProperties();
    void setupActionRecognize();
    void setupActionSaveAs();
    void setupActionSelectAll();
    void setupLayout();
    void setupScene();
    void setupScrollBars();
    void showFirst();
    void showLast();
    void showNext();
    void showPrevious();
    void thumbAppend(ThumbnailWidget * thumb);
    void thumbRemove(ThumbnailWidget * thumb);
private slots:
    void contextThumbFormatSettings();
    void contextThumbOpenExternal();
    void contextThumbProperties();
    void contextThumbRecognize();
    void contextThumbRecognizeSettings();
    void contextThumbRemove();
    void contextThumbSave();
    void handleThumbClick(ThumbnailWidget * sender, int modifiers);
    void handleThumbContextMenu(ThumbnailWidget * sender, const QPoint& pos);
    void handleThumbDrag(ThumbnailWidget * sender, const QPointF& scenePos);
    void handleThumbDrop(ThumbnailWidget * sender, const QPointF& scenePos);
    void handleThumbReorder();
    void pageAdd(Page * page);
    void pageRemove(Page * page);
    void selectAll();
    void updateLayout();
private:
    Packet * packet_;
    Page * current_page_;
    ThumbScene * scene_;
    ThumbLayout * layout_;
    ThumbnailWidget * context_thumb_;
    QAction * act_select_all_;
    QAction * act_recognize_;
    QAction * act_save_as_;
    QAction * act_properties_;
    QAction * act_delete_;
    QAction * act_open_external_;
};

#endif /* THUMBNAILLIST_H_ */
