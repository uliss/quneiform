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
class QGraphicsRectItem;
class QGraphicsPixmapItem;
class QGestureEvent;
class QPinchGesture;
class QMenu;
class QRubberBand;
class Page;
class PageLayout;
class Selection;
class SelectionShadow;

class ImageView : public QGraphicsView {
    Q_OBJECT
public:
    ImageView(QWidget * parent = 0);
    ~ImageView();

    /**
      * Clears view and scene
      */
    void clearScene();

    /**
      * Fits page into viewpoint area
      * if page is smaller than viewport size
      * page scaled to it's original size
      * @see fitWidth(), originalSize(), zoom()
      */
    void fitPage();

    /**
      * Fits page into viewport area by width
      * if page is smaller than viewport size
      * page scaled to it's original size
      * @see fitPage(), originalSize(), zoom()
      */
    void fitWidth();

    /**
      * Hides format layout
      * @see showFormatLayout()
      */
    void hideFormatLayout();

    /**
      * Zooms page to original size
      * @see fitPage(), fitWidth(), zoom()
      */
    void originalSize();

    /**
      * Show char bbox
      */
    void showChar(const QRect& bbox);

    /**
      * Shows cuneiform page formatting
      * @see hideFormatLayout()
      */
    void showFormatLayout();

    /**
      * Shows page. Previous items are cleared
      */
    void showPage(Page * page);

    /**
      * Sets view minimal scale
      */
    void setMinScale(qreal factor);

    /**
      * Sets view maximum scale
      */
    void setMaxScale(qreal factor);

    /**
      * Zooms page
      * @see fitPage(), fitWidth(), originalSize()
      */
    void zoom(qreal factor);
public slots:
    void updateFormatLayout();
signals:
    void pageDeleted();

    /**
      * Emitted after view scaling
      */
    void scaled();

    /**
      * Emitted after scale attempt if scale is too big
      */
    void scaleIsTooBig();

    /**
      * Emitted after scale attempt if scale is too small
      */
    void scaleIsTooSmall();
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
    void updatePageRotation();
    void updateViewScale();
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
    bool isSceneSizeSmaller();
    bool isSceneWidthSmaller();
    bool isTooBig() const;
    bool isTooSmall() const;
    void resizeSelection(const QPoint& pos);
    void restorePageSelection();
    void savePageViewScale();
    void savePageViewScroll();
    void setPageSelection(const QRect& rect);
    void setupScene();
    void showImage(const QString& path);
    void startSelection(const QPoint& pos);
    void restorePageScroll();
    void rotatePixmap(int angle);
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
    PageLayout * layout_;
    qreal min_scale_;
    qreal max_scale_;
    QGraphicsRectItem * current_char_bbox_;
    QGraphicsPixmapItem * pixmap_;
};

#endif
