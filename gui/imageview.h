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

#include <QGraphicsView>

class QGraphicsScene;
class QGestureEvent;
class QPinchGesture;
class QMenu;
class QRubberBand;
class Page;
class Selection;
class SelectionShadow;

class ImageView : public QGraphicsView {
    Q_OBJECT
public:
    ImageView(QWidget * parent = 0);
    void clearScene();
    void fitPage();
    void fitWidth();
    void originalSize();
    void showPage(Page * page);
    void zoom(qreal factor);
signals:
    void pageDeleted();
protected:
    void contextMenuEvent(QContextMenuEvent * event);
    bool event(QEvent * event);
    bool gestureEvent(QGestureEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void pinchTriggered(QPinchGesture * gesture);
    void wheelEvent(QWheelEvent * event);
private slots:
    void changeSelectionCursor(int type);
    void deletePage();
    void deletePageSelection();
    void movePageSelection(const QPointF& delta);
    void savePageSelection();
    void selectPageArea();
    void updateTransform();
private:
    void activate(bool value);
    void connectPageSignals(Page * page);
    void createContextMenu();
    void createRubberBand();
    void createPageSelection(const QRect& rect);
    void disconnectPageSignals(Page * page);
    void drawPageSelectionShadow();
    void finishPageSelection(const QRect& rect);
    void finishSelection(const QPoint& pos);
    bool isTooBig() const;
    bool isTooSmall() const;
    void resizeSelection(const QPoint& pos);
    void restorePageSelection();
    void savePageViewScroll();
    void savePageTransform();
    void setPageSelection(const QRect& rect);
    void setupScene();
    void showImage(const QString& path);
    void startSelection(const QPoint& pos);
    void restorePageScroll();
    void updateSelectionCursor();
private:
    enum select_mode_t {
        NORMAL = 0,
        SELECT_PAGE,
        SELECT_TEXT,
        SELECT_IMAGE,
        SELECT_TABLE
    };

    QGraphicsScene * scene_;
    Page * page_;
    QMenu * context_menu_;
    QRubberBand * rubber_band_;
    Selection * page_selection_;
    SelectionShadow * page_shadow_;
    QPoint selection_start_;
    select_mode_t select_mode_;
};

#endif
