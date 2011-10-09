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

#ifndef THUMBNAILWIDGET_H_
#define THUMBNAILWIDGET _H_

#include <QString>
#include <QGraphicsObject>

class QMenu;
class QAction;

class Page;
class PageIndicator;

// ThumbnailWidget represents a single thumbnail in the ThumbnailList
class ThumbnailWidget: public QGraphicsObject
{
    Q_OBJECT
public:
    class Pixmap;
    class Label;
public:
    ThumbnailWidget(Page * page);

    /**
      * Returns thumb bounding rect
      */
    QRectF boundingRect() const;

    /**
      * Highlights wthumbnail background
      */
    void highlight(bool value);

    /**
      * Checks if thumb is selected
      */
    bool isThumbSelected() const;

    /**
      * Returns thumb name
      * @see setName()
      */
    QString name() const;

    /**
      * Returns pointer to thumb page
      */
    Page * page() const;

    /**
      * Selected thumbnail
      */
    void selectThumb(bool value);

    /**
      * Sets thumbnail name
      * @see name()
      */
    void setName(const QString& name);

    /**
      * Toggles thumb selection
      */
    void toggleSelection();
signals:
    void clicked(int modifier);
    void contextMenuCreated(QMenu*);
    void dragged(ThumbnailWidget * sender, QPointF);
    void dropped(ThumbnailWidget * sender, QPointF);
    void recognize(Page*);
    void removed(Page*);
    void save(Page*);
    void showPageFault(Page*);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
private:
    void setupActions();
    void setupIndicator();
    void setupLabel();
    void setupPixmap();
    void setupToolTip();
    void updatePixmapPos();
private slots:
    void pageFaultForward();
    void recognizeThumb();
    void removePage();
    void handlePageRotate();
    void savePage();
    void showProperties();
    void showFormatSettings();
    void showRecognizeSettings();
    void updatePageIndicators();
private:
    Page * page_;
    Pixmap * pixmap_;
    Label * label_;
    QAction * act_recognize_;
    QAction * act_save_as_;
    QAction * act_properties_;
    QAction * act_delete_;
    PageIndicator * indicator_;
    QPointF drag_start_pos_;
    bool drag_progress_;
    bool hightlighted_;
};

#endif /* THUMBNAILWIDGET_H_ */
