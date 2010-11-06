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
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QEvent>
#include <QGesture>
#include <QGestureEvent>
#include <QMenu>
#include <QRubberBand>

#include "imageview.h"
#include "imagecache.h"
#include "page.h"
#include "selection.h"
#include "selectionshadow.h"

#define HAS_PAGE() {\
    if(!page_) {\
        qDebug() << "[Error]" << Q_FUNC_INFO << "no current page";\
        return;\
    }\
}

static const float GESTURE_SCALE_FACTOR = 1 / 30.0;
static const int ROTATE_THRESHOLD = 3;

ImageView::ImageView(QWidget * parent) :
        QGraphicsView(parent), scene_(NULL), page_(NULL), context_menu_(NULL),
        rubber_band_(NULL), page_selection_(NULL), page_shadow_(NULL), select_mode_(NORMAL) {
    activate(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setBackgroundRole(QPalette::Dark);
    setViewportUpdateMode(FullViewportUpdate);
    setupScene();
}

void ImageView::activate(bool value) {
    setEnabled(value);
    setInteractive(value);
    value ? grabGesture(Qt::PinchGesture) : ungrabGesture(Qt::PinchGesture);
}

void ImageView::changeSelectionCursor(int type) {
    static const Qt::CursorShape cursors[][2]  = {
        {Qt::ArrowCursor, Qt::ArrowCursor}, // Selection::NORMAL (0)
        {Qt::SizeHorCursor, Qt::SizeVerCursor}, // Selection::HORIZONTAL (1)
        {Qt::SizeVerCursor, Qt::SizeHorCursor},// Selection::VERTICAL (2)
        {Qt::SizeFDiagCursor, Qt::SizeBDiagCursor}, // Selection::DIAGONAL_LEFT (3)
        {Qt::SizeBDiagCursor, Qt::SizeFDiagCursor}
    };

    static const int cursors_num = sizeof(cursors) / sizeof(cursors[0]);

    if(type < 0 || type >= cursors_num) {
        qDebug() << "[Error]" << Q_FUNC_INFO << "invalid cursor type" << type;
        return;
    }

    if(transform().isRotating())
        page_selection_->setCursor(cursors[type][1]);
    else
        page_selection_->setCursor(cursors[type][0]);
}

void ImageView::clearScene() {
    if(!scene_) {
        qDebug() << Q_FUNC_INFO << "no scene";
        return;
    }

    foreach(QGraphicsItem * item, scene_->items()) {
        scene_->removeItem(item);
    }

    scene_->setSceneRect(QRect());
}

void ImageView::connectPageSignals(Page * page) {
    Q_CHECK_PTR(page);

    connect(page, SIGNAL(transformed()), SLOT(updateTransform()));
    connect(page, SIGNAL(rotated(int)), SLOT(updateTransform()));
    connect(page, SIGNAL(destroyed()), SLOT(deletePage()));
}

void ImageView::contextMenuEvent(QContextMenuEvent * event) {
    QGraphicsView::contextMenuEvent(event);

    if(!context_menu_)
        createContextMenu();

    context_menu_->exec(event->globalPos());
}

void ImageView::createContextMenu() {
    context_menu_ = new QMenu(this);
    context_menu_->addAction(QIcon(":/img/oxygen/32x32/select_rectangular.png"),
                   tr("Select recognize area"),
                   this, SLOT(selectPageArea()));
}

void ImageView::createRubberBand() {
    rubber_band_ = new QRubberBand(QRubberBand::Rectangle, this);
}

void ImageView::createPageSelection(const QRect& rect) {
    Q_CHECK_PTR(scene_);

    page_selection_ = new Selection(mapToScene(rect).boundingRect());
    page_selection_->setZValue(1);
    connect(page_selection_, SIGNAL(cursorChange(int)), SLOT(changeSelectionCursor(int)));
    connect(page_selection_, SIGNAL(selectionDeleted()), SLOT(deletePageSelection()));
    connect(page_selection_, SIGNAL(resized()), SLOT(savePageSelection()));
    connect(page_selection_, SIGNAL(moved(QPointF)), SLOT(movePageSelection(QPointF)));
    page_shadow_ = new SelectionShadow(page_selection_);
    scene_->addItem(page_selection_);
}

void ImageView::deletePage() {
    if(sender() == page_) {
        qDebug() << "[Info]" << Q_FUNC_INFO << page_;
        page_ = NULL;
        clearScene();
        activate(false);
        emit pageDeleted();
    }
}

void ImageView::deletePageSelection() {
    delete page_selection_;
    page_selection_ = NULL;

    HAS_PAGE()
    page_->setPageArea(QRectF());
}

void ImageView::disconnectPageSignals(Page * page) {
    HAS_PAGE()

    disconnect(page, SIGNAL(transformed()), this, SLOT(updateTransform()));
    disconnect(page, SIGNAL(rotated(int)), this, SLOT(updateTransform()));
    disconnect(page, SIGNAL(destroyed()), this, SLOT(deletePage()));
} 

bool ImageView::event(QEvent * event) {
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QGraphicsView::event(event);
}

void ImageView::finishPageSelection(const QRect& rect) {
    select_mode_ = NORMAL;

    if(!rect.isValid())
        return;

    QRect new_rect(mapFromScene(sceneRect()).boundingRect().intersect(rect));
    setPageSelection(new_rect);
    savePageSelection();
}

void ImageView::finishSelection(const QPoint& pos) {
    if(!page_ || select_mode_ == NORMAL)
        return;

    setCursor(QCursor());
    rubber_band_->hide();

    switch(select_mode_) {
    case SELECT_PAGE:
        finishPageSelection(QRect(selection_start_, pos).normalized());
        break;
    default:
        qDebug() << Q_FUNC_INFO << "not implemented yet for " << select_mode_;
        break;
    }
}

void ImageView::fitPage() {
    HAS_PAGE()

    // if image is smaller then view area set it to 100% size
    if (sceneRect().height() < height() && sceneRect().width() < width())
        originalSize();
    else
        fitInView(sceneRect(), Qt::KeepAspectRatio);

    savePageTransform();
}

void ImageView::fitWidth() {
    HAS_PAGE()

    QRectF scene_rect = sceneRect();

    // if image is smaller then view area set it to 100% size
    if (scene_rect.height() < height() && scene_rect.width() < width()) {
        originalSize();
    } else {
        if (transform().isRotating())
            scene_rect.setWidth(30);
        else
            scene_rect.setHeight(30);

        fitInView(scene_rect, Qt::KeepAspectRatio);
    }

    savePageTransform();
}

bool ImageView::gestureEvent(QGestureEvent * event) {
    if (QGesture * pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}


void ImageView::mouseMoveEvent(QMouseEvent * event) {
    QGraphicsView::mouseMoveEvent(event);

    // resize only with left button
    if(event->buttons() & Qt::LeftButton)
        resizeSelection(event->pos());
}

void ImageView::mousePressEvent(QMouseEvent * event) {
    QGraphicsView::mousePressEvent(event);

    if(event->isAccepted())
        return;

    // selection only with left button
    if(event->button() != Qt::LeftButton)
        return;

    startSelection(event->pos());
}

void ImageView:: mouseReleaseEvent(QMouseEvent * event) {
    QGraphicsView::mouseReleaseEvent(event);

    finishSelection(event->pos());
}

void ImageView::movePageSelection(const QPointF& delta) {
    QTransform t = transform();
    if(t.isRotating())
        t.rotate(180);

    page_selection_->moveBy(t.map(delta));
}

void ImageView::originalSize() {
    HAS_PAGE()
    page_->resetScale();
}

void ImageView::pinchTriggered(QPinchGesture * gesture) {
    HAS_PAGE()

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

void ImageView::resizeSelection(const QPoint& pos) {
    if(!page_ || select_mode_ == NORMAL)
        return;

    if(rubber_band_)
        rubber_band_->setGeometry(QRect(selection_start_, pos).normalized());
}

void ImageView::restorePageSelection() {
    HAS_PAGE()

    if(!page_->pageArea().isValid())
        return;

    if(!page_selection_)
        createPageSelection(QRect());

    page_selection_->setPos(QPointF());
    page_selection_->setRect(page_->pageArea());
    page_shadow_->setParentItem(page_selection_);

    if(!scene_->items().contains(page_selection_))
        scene_->addItem(page_selection_);
}

void ImageView::savePageSelection() {
    HAS_PAGE()
    if(page_selection_) {
        QPointF p = page_selection_->pos() + page_selection_->rect().topLeft();
        QRectF r(p, page_selection_->rect().size());
        page_->setPageArea(r);
    }
    else
        page_->setPageArea(QRect());
}

void ImageView::savePageTransform() {
    HAS_PAGE()
    page_->setTransform(transform());
}

void ImageView::setPageSelection(const QRect& rect) {
    HAS_PAGE()

    if(!page_selection_)
        createPageSelection(rect);
    else
        page_selection_->setRect(mapToScene(rect).boundingRect());
}

void ImageView::setupScene() {
    scene_ = new QGraphicsScene(this);
    setScene(scene_);
}

void ImageView::showImage(const QString& path) {
    QPixmap image;

    if(!ImageCache::load(path, &image)) {
        qDebug() << "[Error]" << Q_FUNC_INFO << "can't load image" << path;
        return;
    }

    scene_->setSceneRect(image.rect());
    QGraphicsPixmapItem * p = scene_->addPixmap(image);
    p->setZValue(0);
    p->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

void ImageView::showPage(Page * page) {
    Q_CHECK_PTR(page);

    // attempt to show current page
    if(page_ == page)
        return;

    // disconnect previous
    disconnectPageSignals(page_);
    // set current page
    page_ = page;
    // connect new
    connectPageSignals(page_);

    clearScene();
    updateTransform();
    showImage(page_->imagePath());
    restorePageSelection();
    activate(true);
}

void ImageView::selectPageArea() {
    HAS_PAGE()

    select_mode_ = SELECT_PAGE;
    updateSelectionCursor();
}

void ImageView::startSelection(const QPoint& pos) {
    if(!page_ || select_mode_ == NORMAL)
        return;

    // delete previous selection
    deletePageSelection();

    selection_start_ = pos;

    if (!rubber_band_)
        createRubberBand();

    rubber_band_->setGeometry(QRect(selection_start_, QSize()));
    rubber_band_->show();
}

void ImageView::updateSelectionCursor() {
    HAS_PAGE()

    switch(select_mode_) {
    case NORMAL:
        setCursor(QCursor());
        break;
    case SELECT_PAGE:
        setCursor(Qt::CrossCursor);
        break;
    case SELECT_IMAGE:
    case SELECT_TEXT:
    case SELECT_TABLE:
        qDebug() << Q_FUNC_INFO << "not implemented";
        break;
    }
}

void ImageView::updateTransform() {
    HAS_PAGE()

    if(transform() != page_->transform())
        setTransform(page_->transform());
}

void ImageView::zoomIn(qreal factor) {
    HAS_PAGE()
    page_->scale(factor);
}

void ImageView::zoomOut(qreal factor) {
    HAS_PAGE()
    page_->scale(factor);
}
