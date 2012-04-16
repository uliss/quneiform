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
#include <QWheelEvent>
#include <QScrollBar>
#include <QSettings>
#include <QBitmap>

// gesture support
#if QT_VERSION >= 0x040600
#include <QGesture>
#include <QGestureEvent>
#endif

#include "imageview.h"
#include "imagecache.h"
#include "page.h"
#include "pagearea.h"
#include "selection.h"
#include "selectionlist.h"

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
        min_scale_(0),
        max_scale_(100),
        pixmap_(NULL),
        scene_bbox_(NULL),
        area_(NULL),
        selections_(NULL)
{
    setupView();
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

void ImageView::clearPageLayout() {
    deletePageArea();
    selections_->clearSelections();
}

void ImageView::clearScene() {
    if(!scene()) {
        qDebug() << Q_FUNC_INFO << "no scene";
        return;
    }

    scene()->clear();
    selections_ = NULL;
    pixmap_ = NULL;
    scene_bbox_ = NULL;
    area_ = NULL;
    scene()->setSceneRect(QRect());
}

void ImageView::connectPageSignals(Page * page) {
    Q_CHECK_PTR(page);

    connect(page, SIGNAL(viewScaled()), SLOT(updateViewScale()));
    connect(page, SIGNAL(rotated(int)), SLOT(updatePageRotation()));
    connect(page, SIGNAL(recognized()), SLOT(updatePageArea()));
    connect(page, SIGNAL(destroyed()), SLOT(deletePage()));
    connect(page, SIGNAL(layoutCleared()), SLOT(clearPageLayout()));
}

void ImageView::contextMenuEvent(QContextMenuEvent * event) {
    QGraphicsView::contextMenuEvent(event);

    if(!context_menu_)
        createContextMenu();

    context_menu_->exec(event->globalPos());
}

void ImageView::createContextMenu() {
    context_menu_ = new QMenu(this);
    context_menu_->addAction(tr("Recognize"), this, SLOT(handleRecognizeRequest()));
    context_menu_->addSeparator();
    context_menu_->addAction(QIcon(":/img/oxygen/32x32/select_rectangular.png"),
                   tr("Select recognize area"),
                   this, SLOT(startPageSelection()));
    context_menu_->addSeparator();
    context_menu_->addAction(tr("Fit to width"), this, SLOT(fitWidth()));
    context_menu_->addAction(tr("Fit to page"), this, SLOT(fitPage()));
    context_menu_->addAction(tr("Original size"), this, SLOT(originalSize()));
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

void ImageView::deletePageArea() {
    delete area_;
    area_ = NULL;
}

void ImageView::disconnectPageSignals(Page * page) {
    HAS_PAGE()

    disconnect(page, SIGNAL(viewScaled()), this, SLOT(updateViewScale()));
    disconnect(page, SIGNAL(rotated(int)), this, SLOT(updatePageRotation()));
    disconnect(page, SIGNAL(destroyed()), this, SLOT(deletePage()));
    disconnect(page, SIGNAL(layoutCleared()), this, SLOT(clearPageLayout()));
} 

bool ImageView::event(QEvent * event) {
#if QT_VERSION >= 0x040600
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
#endif
    return QGraphicsView::event(event);
}

void ImageView::fitPage() {
    HAS_PAGE()

    if (isSceneSizeSmaller())
        originalSize();
    else
        fitInView(sceneRect(), Qt::KeepAspectRatio);

    savePageViewScale();
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

    savePageViewScale();
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
    if(area_)
        area_->hideLayout();
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

void ImageView::originalSize() {
    HAS_PAGE()
    page_->resetViewScale();
    emit scaled();
}

void ImageView::pinchTriggered(QPinchGesture * gesture) {
#if QT_VERSION >= 0x040600

    HAS_PAGE()
    if (gesture->state() == Qt::GestureFinished) {
        qreal rot_angle = gesture->rotationAngle();
        if(rot_angle > ROTATE_THRESHOLD)
            emit gestureRotateAttempt(90);
        else if (rot_angle < - ROTATE_THRESHOLD)
            emit gestureRotateAttempt(-90);
    }
    else if(gesture->state() == Qt::GestureUpdated) {
        qreal scale = ((gesture->lastScaleFactor() - 1) * GESTURE_SCALE_FACTOR) + 1;
        if(scale != 1)
            zoom(scale);
    }
#endif
}

void ImageView::updatePageSelection() {
    HAS_PAGE()

    if(!selections_) {
        selections_ = new SelectionList(sceneRect());
        connect(selections_, SIGNAL(changed()), SLOT(savePageSelections()));
        scene()->addItem(selections_);
    }

    selections_->blockSignals(true);

    selections_->clearSelections();
    selections_->setZValue(10);

    foreach(QRect r, page_->readAreas()) {
        selections_->addSelection(r);
    }

    selections_->blockSignals(false);
}

void ImageView::updatePageScroll() {
    HAS_PAGE()

    QPoint scroll = page_->viewScroll();
    horizontalScrollBar()->setValue(scroll.x());
    verticalScrollBar()->setValue(scroll.y());
}

void ImageView::rotatePixmap(int angle) {
    if(!pixmap_) {
        qDebug() << Q_FUNC_INFO << "NULL pixmap pointer given";
        return;
    }

    Q_CHECK_PTR(scene());

#if QT_VERSION >= 0x040600
    pixmap_->setRotation(angle);

    if(selections_)
        selections_->setRotation(angle);
#else
    pixmap_->setTransform(QTransform().rotate(angle), true);
    selections_->setTransform(QTransform().rotate(angle), true);
#endif

    QRectF pixmap_bbox = pixmap_->sceneBoundingRect();
    QTransform matrix;
    matrix.translate(- pixmap_bbox.left(), - pixmap_bbox.top());
    pixmap_->setTransform(matrix);

    if(selections_)
        selections_->setTransform(matrix);
    scene()->setSceneRect(pixmap_->sceneBoundingRect());

    if(!scene_bbox_) {
        scene_bbox_ = scene()->addRect(sceneRect(), QPen(Qt::white));
    }
    else
        scene_bbox_->setRect(sceneRect());
}

void ImageView::savePageViewScale() {
    HAS_PAGE()

    page_->setViewScale(transform().m11());
}

void ImageView::savePageViewScroll() {
    HAS_PAGE()

    page_->setViewScroll(QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()));
}

void ImageView::startPageSelection()
{
    if(selections_) {
        selections_->setSelectionType(SelectionList::SELECT_AREA);
        selections_->setSelectionMode(SelectionList::MODE_REPLACE);
    }
}

void ImageView::setMinScale(qreal factor) {
    Q_ASSERT(factor >= 0);
    min_scale_ = factor;
}

void ImageView::setMaxScale(qreal factor) {
    Q_ASSERT(factor >= 0);
    max_scale_ = factor;
}

void ImageView::setupScene() {
    scene_ = new QGraphicsScene;
    setScene(scene_);
}

void ImageView::setupView()
{
    activate(false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setBackgroundRole(QPalette::Dark);
    setViewportUpdateMode(FullViewportUpdate);
}

void ImageView::showChar(const QRect& bbox) {
    if(!area_)
        return;

    area_->showChar(bbox);
    centerOn(bbox.center());
}

void ImageView::showFormatLayout() {
    if(area_)
         area_->showLayout();
}

void ImageView::showImage() {
    Q_CHECK_PTR(page_);

    QPixmap image;
    if(!ImageCache::load(page_->imagePath(), &image)) {
        qDebug() << "[Error]" << Q_FUNC_INFO << "can't load image";
        return;
    }

    if(pixmap_)
        scene()->removeItem(pixmap_);

    pixmap_ = scene()->addPixmap(image);
    pixmap_->setZValue(0);
    pixmap_->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);

    rotatePixmap(page_->angle());
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
    updateViewScale();
    showImage();
    updatePageArea();
    updatePageScroll();
    updatePageSelection();
    activate(true);
}

void ImageView::updatePageArea() {
    Q_CHECK_PTR(scene());

    if(!page_)
        return;

    if(!area_)
        area_ = new PageArea();

    area_->setParentItem(scene_bbox_);
    area_->show(page_);
}

void ImageView::updatePageRotation() {
    HAS_PAGE()

    rotatePixmap(page_->angle());
}

void ImageView::updateViewScale() {
    HAS_PAGE()

    QTransform t;
    float vs = page_->viewScale();
    t.scale(vs, vs);
    setTransform(t);
}

void ImageView::savePageSelections()
{
    HAS_PAGE()

    if(!selections_) {
        page_->clearReadAreas();
        return;
    }

    page_->setReadAreas(selections_->selectionRects());
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
        page_->scaleView(factor);
        emit scaled();
    }
}

void ImageView::handleRecognizeRequest()
{
    emit recognize(page_);
}
