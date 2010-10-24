/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef IMAGE_VIEW_H_
#define IMAGE_VIEW_H_

#include <QWidget>
#include <QGraphicsScene>

class QGraphicsView;
class QVBoxLayout;
class QGestureEvent;
class QPinchGesture;
class QGraphicsRectItem;
class ImageGraphicsView;
class Page;

class ImageView : public QWidget {
    Q_OBJECT
public:
    ImageView(QWidget * parent);
    void clear();
    bool event(QEvent * event);
    bool gestureEvent(QGestureEvent * event);
    void pinchTriggered(QPinchGesture * gesture);
    void showPage(Page * page);
    void setPage(Page * page);
public slots:
    void deletePage();
    void fitPage();
    void fitWidth();
    void originalSize();
    void zoomIn();
    void zoomOut();
private slots:
    void selectPageArea(const QRectF& area);
    void updatePage();
private:
    void connectPage();
    void disconnectPage();
    void saveTransform();
private:
    Page * page_;
    QVBoxLayout * layout_;
    QGraphicsScene scene_;
    ImageGraphicsView * view_;
    QGraphicsRectItem * page_area_;
};

#endif
