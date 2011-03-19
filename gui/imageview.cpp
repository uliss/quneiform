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
#include <QMenu>
#include <QRubberBand>
#include <QWheelEvent>
#include <QScrollBar>

// gesture support
#if QT_VERSION >= 0x040600
#include <QGesture>
#include <QGestureEvent>
#endif

#include "imageview.h"
#include "imagecache.h"
#include "page.h"
#include "pagelayout.h"
#include "selection.h"
#include "selectionshadow.h"

#define HAS_PAGE() {\
    if(!page_) {\
        qDebug() << "[Error]" << Q_FUNC_INFO << "no current page";\
        return;\
    }\
}

static const double GESTURE_SCALE_FACTOR = 1 / 30.0;
static const int ROTATE_THRESHOLD = 3;

ImageView::ImageView(QWidget * parent) :
        QGraphicsView(parent),
        scene_(NULL),
        page_(NULL),
        context_menu_(NULL),
        rubber_band_(NULL),
        page_selection_(NULL),
        page_shadow_(NULL),
        select_mode_(NORMAL),
        layout_(NULL),
        min_scale_(0),
        max_scale_(100) {
    activate(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setBackgroundRole(QPalette::Dark);
    setViewportUpdateMode(FullViewportUpdate);
    setupScene();
}

ImageView::~ImageView() {    
    delete scene_;
}

void ImageView::activate(bool value) {
    setEnabled(value);
    setInteractive(value);
#if QT_VERSION >= 0x040600
    value ? grabGesture(Qt::PinchGesture) : ungrabGesture(Qt::PinchGesture);
#endif
}

void ImageView::changeSelectionCursor(int type) {
    page_selection_->setCursorType(static_cast<Selection::cursor_t>(type),
                                   transform().isRotating());
}

void ImageView::clearScene() {
    if(!scene_) {
        qDebug() << Q_FUNC_INFO << "no scene";
        return;
    }

    scene_->clear();
    layout_ = NULL;
    page_selection_ = NULL;
    scene_->setSceneRect(QRect());
}

void ImageView::connectPageSignals(Page * page) {
    Q_CHECK_PTR(page);

    connect(page, SIGNAL(transformed()), SLOT(updateTransform()));
    connect(page, SIGNAL(rotated(int)), SLOT(updateTransform()));
    connect(page, SIGNAL(recognized()), SLOT(updateFormatLayout()));
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
    scene_->removeItem(page_selection_);
    delete page_selection_;
    page_selection_ = NULL;

    HAS_PAGE()
    page_->setPageArea(QRect());
}

void ImageView::disconnectPageSignals(Page * page) {
    HAS_PAGE()

    disconnect(page, SIGNAL(transformed()), this, SLOT(updateTransform()));
    disconnect(page, SIGNAL(rotated(int)), this, SLOT(updateTransform()));
    disconnect(page, SIGNAL(destroyed()), this, SLOT(deletePage()));
} 

bool ImageView::event(QEvent * event) {
#if QT_VERSION >= 0x040600
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
#endif
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

    if (isSceneSizeSmaller())
        originalSize();
    else
        fitInView(sceneRect(), Qt::KeepAspectRatio);

    savePageTransform();
    emit scaled();
}

void ImageView::fitWidth() {
    HAS_PAGE()

    QRectF scene_rect = sceneRect();

    if (isSceneWidthSmaller()) {
        originalSize();
    } else {
        if (transform().isRotating())
            scene_rect.setWidth(30);
        else
            scene_rect.setHeight(30);

        fitInView(scene_rect, Qt::KeepAspectRatio);
    }

    savePageTransform();
    emit scaled();
}

bool ImageView::gestureEvent(QGestureEvent * event) {
#if QT_VERSION >= 0x040600
    if (QGesture * pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
#endif
}

void ImageView::hideFormatLayout() {
    if(layout_)
        layout_->hide();
}

bool ImageView::isSceneSizeSmaller(){
    if(!transform().isRotating())
        return sceneRect().height() < height() && sceneRect().width() < width();
    else
        return sceneRect().height() < width() && sceneRect().width() < height();
}

bool ImageView::isSceneWidthSmaller() {
    if(!transform().isRotating())
        return sceneRect().width() < width();
    else
        return sceneRect().width() < height();
}

bool ImageView::isTooBig() const {
    qreal x = 0, y = 0;
    transform().map(1.0, 1.0, &x, &y);
    return qMax(qAbs(x), qAbs(y)) > max_scale_;
}

bool ImageView::isTooSmall() const {
    qreal x = 0, y = 0;
    transform().map(1.0, 1.0, &x, &y);
    return qMax(qAbs(x), qAbs(y)) < min_scale_;
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
    event->accept();
}

void ImageView::mouseReleaseEvent(QMouseEvent * event) {
    QGraphicsView::mouseReleaseEvent(event);

    if(event->button() != Qt::LeftButton)
        return;

    finishSelection(event->pos());
    event->accept();
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
    emit scaled();
}

void ImageView::pinchTriggered(QPinchGesture * gesture) {
#if QT_VERSION >= 0x040600

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
            zoom(scale);
    }
#endif
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
    if(page_selection_)
        page_->setPageArea(page_selection_->normalRect());
    else
        page_->setPageArea(QRect());
}

void ImageView::savePageViewScroll() {
    HAS_PAGE()

    page_->setViewScroll(QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()));
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
    scene_ = new QGraphicsScene;
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

void ImageView::showFormatLayout() {
    if(layout_)
        layout_->show();
}

void ImageView::showPage(Page * page) {
    if(!page) {
        qDebug() << Q_FUNC_INFO << "attempt to show NULL page";
        return;
    }

    // attempt to show current page
    if(page_ == page)
        return;

    if(page_) {
        // save old page view scroll
        savePageViewScroll();
        // disconnect previous
        disconnectPageSignals(page_);
    }

    // set current page
    page_ = page;
    // connect new
    connectPageSignals(page_);

    clearScene();
    updateTransform();
    showImage(page_->imagePath());
    updateFormatLayout();
    restorePageScroll();
    restorePageSelection();
    activate(true);
}

void ImageView::selectPageArea() {
    HAS_PAGE()

    select_mode_ = SELECT_PAGE;
    updateSelectionCursor();
}

void ImageView::setMinScale(qreal factor) {
    Q_ASSERT(factor >= 0);
    min_scale_ = factor;
}

void ImageView::setMaxScale(qreal factor) {
    Q_ASSERT(factor >= 0);
    max_scale_ = factor;
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

void ImageView::restorePageScroll() {
    HAS_PAGE()

    QPoint scroll = page_->viewScroll();
    horizontalScrollBar()->setValue(scroll.x());
    verticalScrollBar()->setValue(scroll.y());
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

void ImageView::updateFormatLayout() {
    Q_CHECK_PTR(scene_);

    if(!page_)
        return;

    if(!layout_) {
        layout_ = new PageLayout;
        scene_->addItem(layout_);
    }

    if(layout_->scene() != scene_)
        scene_->addItem(layout_);

    layout_->populate(*page_);
}

void ImageView::updateTransform() {
    HAS_PAGE()

    if(transform() != page_->transform())
        setTransform(page_->transform());
}

void ImageView::wheelEvent(QWheelEvent * event) {
    if(!(event->modifiers() & Qt::ControlModifier))
        return QGraphicsView::wheelEvent(event);

    if(event->delta() > 0)
        zoom(1.03);
    else
        zoom(0.93);

    event->accept();
}

void ImageView::zoom(qreal factor) {
    HAS_PAGE()

    if(factor < 1.0 && isTooSmall()) {
        emit scaleIsTooSmall();
        return;
    }
    else if(factor > 1.0 && isTooBig()) {
        emit scaleIsTooBig();
        return;
    }
    else {
        page_->scale(factor);
        emit scaled();
    }
}
