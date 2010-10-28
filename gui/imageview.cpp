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

#include <QDebug>
#include <QScrollBar>
#include <QPixmap>
#include <QLabel>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QToolBar>
#include <QIcon>
#include <QGesture>
#include <QEvent>
#include <QPinchGesture>
#include <QGestureEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>

#include "imageview.h"
#include "imagegraphicsview.h"
#include "imagecache.h"
#include "page.h"
#include "widgetbar.h"
#include "selection.h"

static const float GESTURE_SCALE_FACTOR = 1 / 30.0;
static const int ROTATE_THRESHOLD = 3;

ImageView::ImageView(QWidget * parent) : QWidget(parent),
page_(NULL), page_area_(NULL) {
    view_ = new ImageGraphicsView;
    view_->setScene(&scene_);
    connect(view_, SIGNAL(pageAreaSelected(QRectF)), SLOT(selectPageArea(QRectF)));

    grabGesture(Qt::PinchGesture);

    layout_ = new QVBoxLayout;
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setMargin(0);
    layout_->setSpacing(0);
    layout_->addWidget(view_);

    setLayout(layout_);
}

void ImageView::changeSelectionCursor(int type) {
    if(!page_area_)
        return;

    static const Qt::CursorShape cursors[][2]  = {
        {Qt::ArrowCursor, Qt::ArrowCursor}, // Selection::NORMAL (0)
        {Qt::SizeHorCursor, Qt::SizeVerCursor}, // Selection::HORIZONTAL (1)
        {Qt::SizeVerCursor, Qt::SizeHorCursor},// Selection::VERTICAL (2)
        {Qt::SizeFDiagCursor, Qt::SizeBDiagCursor}, // Selection::DIAGONAL_LEFT (3)
        {Qt::SizeBDiagCursor, Qt::SizeFDiagCursor}
    };

//    qDebug() << type;

    static const int cursors_num = sizeof(cursors) / sizeof(cursors[0]);

    if(type < 0 || type >= cursors_num) {
        qDebug() << "[ImageView::changeSelectionCursor] invalid cursor type" << type;
        return;
    }

    if(view_->transform().isRotating())
        page_area_->setCursor(cursors[type][1]);
    else
        page_area_->setCursor(cursors[type][0]);
}

void ImageView::clear() {
    qDebug() << "[ImageView::clear]";
    foreach(QGraphicsItem * item, scene_.items()) {
        scene_.removeItem(item);
    }
}

void ImageView::connectPage() {
    Q_CHECK_PTR(page_);

    connect(page_, SIGNAL(transformed()), SLOT(updatePage()));
    connect(page_, SIGNAL(rotated(int)), SLOT(updatePage()));
    connect(page_, SIGNAL(destroyed()), SLOT(deletePage()));
}

void ImageView::deletePage() {
    if(sender() == page_) {
        qDebug() << "[ImageView::deletePage] " << page_;
        page_ = NULL;
        clear();
    }
}

void ImageView::disconnectPage() {
    if(!page_)
        return;

    disconnect(page_, SIGNAL(transformed()), this, SLOT(updatePage()));
    disconnect(page_, SIGNAL(rotated(int)), this, SLOT(updatePage()));
    disconnect(page_, SIGNAL(destroyed()), this, SLOT(deletePage()));
}

bool ImageView::event(QEvent * event) {
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

void ImageView::fitPage() {
    if (!page_) {
        qDebug() << "[ImageView::fitWidth] no current page";
        return;
    }

    QRectF scene_rect = view_->sceneRect();

    // if image is smaller then view area set it to 100% size
    if (scene_rect.height() < height() && scene_rect.width() < width())
        originalSize();
    else
        view_->fitInView(scene_rect, Qt::KeepAspectRatio);

    saveTransform();
}

void ImageView::fitWidth() {
    if (!page_) {
        qDebug() << "[ImageView::fitWidth] no current page";
        return;
    }

    QRectF scene_rect = view_->sceneRect();

    // if image is smaller then view area set it to 100% size
    if (scene_rect.height() < height() && scene_rect.width() < width()) {
        originalSize();
    } else {
        if (view_->transform().isRotating())
            scene_rect.setWidth(30);
        else
            scene_rect.setHeight(30);

        view_->fitInView(scene_rect, Qt::KeepAspectRatio);
    }

    saveTransform();
}

bool ImageView::gestureEvent(QGestureEvent * event) {
    if (QGesture * swipe = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(swipe));
    return true;
}

void ImageView::originalSize() {
    if(!page_)
        return;

    page_->resetScale();
}

void ImageView::pinchTriggered(QPinchGesture * gesture) {
    if(!page_)
        return;

    if (gesture->state() == Qt::GestureFinished) {
        qreal rot_angle = gesture->rotationAngle();
        if(rot_angle > ROTATE_THRESHOLD)
            page_->rotate(90);
        else if (rot_angle < - ROTATE_THRESHOLD)
            page_->rotate(-90);
    }
    else if(gesture->state() == Qt::GestureUpdated) {
        qreal scale = ((gesture->lastScaleFactor() - 1) * GESTURE_SCALE_FACTOR) + 1;
        if(scale != 1)
            page_->scale(scale);
    }
}

void ImageView::saveTransform() {
    Q_CHECK_PTR(page_);
    page_->setTransform(view_->transform());
}

void ImageView::selectPageArea(const QRectF& area) {
    if(!page_)
        return;

    if(area.isValid()) {
        if(!page_area_) {
            page_area_ = new Selection(area);
            connect(page_area_, SIGNAL(cursorChange(int)), SLOT(changeSelectionCursor(int)));
            scene_.addItem(page_area_);
        }
        else {
            page_area_->setRect(area);
        }
    }
    else {
        page_area_ = NULL;
    }

    page_->setPageArea(area);
}

void ImageView::setPage(Page * page) {
    Q_CHECK_PTR(page);
    disconnectPage();
    page_ = page;
    connectPage();
    updatePage();
}

void ImageView::showPage(Page * page) {
    clear();

    QPixmap image;

    if(!ImageCache::load(page->imagePath(), &image))
        return;

    QGraphicsPixmapItem * pixmap = scene_.addPixmap(image);
    pixmap->setZValue(0);
    scene_.setSceneRect(image.rect());
    setPage(page);
    selectPageArea(page->pageArea());
}

void ImageView::updatePage() {
    Q_CHECK_PTR(page_);

    if(view_->transform() != page_->transform())
        view_->setTransform(page_->transform());
}

void ImageView::zoomIn() {
    if (!page_) {
        qDebug() << "[ImageView::zoomIn] no current page";
        return;
    }

    page_->scale(1.25);
}

void ImageView::zoomOut() {
    if (!page_) {
        qDebug() << "[ImageView::zoomOut] no current page";
        return;
    }

    page_->scale(0.8);
}
