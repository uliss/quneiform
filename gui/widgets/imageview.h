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
class Page;
class PageArea;
class Selection;
class SelectionList;

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
      * Hides format layout
      * @see showFormatLayout()
      */
    void hideFormatLayout();

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
      * Zooms page to original size
      * @see fitPage(), fitWidth(), zoom()
      */
    void originalSize();

    void updatePageArea();
signals:
    /**
      * Emmitted after gesture rotate attempt
      */
    void gestureRotateAttempt(int angle);

    /**
      * Emitted after connected page deletion
      */
    void pageDeleted();

    /**
      * Emitted on recognize request
      */
    void recognize(Page*);

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
    void pinchTriggered(QPinchGesture * gesture);
    void wheelEvent(QWheelEvent * event);
private slots:
    void clearPageLayout();
    void deletePage();
    void handleRecognizeRequest();
    void savePageSelections();
    void startPageSelection();
    void updateViewScale();
    void updatePageRotation();
private:
    void activate(bool value);
    void connectPageSignals(Page * page);
    void createContextMenu();
    void deletePageArea();
    void disconnectPageSignals(Page * page);
    bool isSceneSizeSmaller();
    bool isSceneWidthSmaller();
    bool isTooBig() const;
    bool isTooSmall() const;
    void rotatePixmap(int angle);
    void savePageViewScale();
    void savePageViewScroll();
    void setupScene();
    void setupView();
    void showImage();
    void updatePageScroll();
    void updatePageSelection();
private:
    QGraphicsScene * scene_;
    Page * page_;
    QMenu * context_menu_;
    qreal min_scale_;
    qreal max_scale_;
    QGraphicsPixmapItem * pixmap_;
    QGraphicsRectItem * scene_bbox_;
    PageArea * area_;
    SelectionList * selections_;
};

#endif
