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

class Page;
class PageIndicator;

/**
  * @brief ThumbnailWidget represents a single thumbnail in the ThumbnailList
  */
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
      * Highlights thumbnail background
      */
    void highlight(bool value);

    /**
      * Checks if thumb is selected
      */
    bool isThumbSelected() const;

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
      */
    void setName(const QString& name);

    /**
      * Shows format settings dialog
      */
    void showFormatSettings();

    /**
      * Shows recognize settings dialog
      */
    void showRecognizeSettings();

    /**
      * Shows thumb page properties dialog
      */
    void showProperties();

    /**
      * Toggles thumb selection
      */
    void toggleSelection();
signals:
    void clicked(ThumbnailWidget * sender, int modifier);
    void createContextMenu(ThumbnailWidget * sender, const QPoint& pos);
    void dragged(ThumbnailWidget * sender, QPointF);
    void dropped(ThumbnailWidget * sender, QPointF);
    void removed(Page*);
    void showPageFault(Page*);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
private:
    void setupIndicator();
    void setupLabel();
    void setupPixmap();
    void setupToolTip();
    void updatePixmapPos();
private slots:
    void pageFaultForward();
    void handlePageRotate();
    void updatePageIndicators();
private:
    Page * page_;
    Pixmap * pixmap_;
    Label * label_;
    PageIndicator * indicator_;
    QPointF drag_start_pos_;
    bool drag_progress_;
    bool hightlighted_;
};

#endif /* THUMBNAILWIDGET_H_ */
