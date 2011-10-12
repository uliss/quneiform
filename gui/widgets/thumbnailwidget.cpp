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

#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QApplication>

#include "thumbnailwidget.h"
#include "thumbnaillist.h"
#include "imagecache.h"
#include "page.h"
#include "pageindicator.h"
#include "dialogs/pagepropertiesdialog.h"

static const int THUMB_IMAGE_HEIGHT = 100;
static const int THUMB_IMAGE_WIDTH = 100;
static const int THUMB_IMAGE_MARGIN = 4;
static const int THUMB_WIDTH = 150;
static const int THUMB_HEIGHT = 150;
static const int SELECT_FRAME_WIDTH = 2;

class ThumbnailWidget::Pixmap : public QGraphicsPixmapItem {
public:
    Pixmap(const QPixmap& p, QGraphicsItem * parent) :
        QGraphicsPixmapItem(p, parent),
        frame_(NULL)
    {
        setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
        frame_ = new QGraphicsRectItem(this);
        highlight(false);
        updateFrame();
    }

    void highlight(bool value) {
        if (value) {
            QPalette palette;
            QPen p(palette.highlight(), SELECT_FRAME_WIDTH);
            frame_->setPen(p);
        }
        else {
            frame_->setPen(QPen(QBrush(), 0));
        }
    }

    void setPixmap(const QPixmap& pixmap)  {
        QGraphicsPixmapItem::setPixmap(pixmap);
        updateFrame();
    }
private:
    void updateFrame() {
        frame_->setRect(boundingRect().adjusted(-1,-1,1,1));
    }
private:
    QGraphicsRectItem * frame_;
};

class ThumbnailWidget::Label : public QGraphicsSimpleTextItem {
public:
    Label(QGraphicsItem * parent) : QGraphicsSimpleTextItem(parent), highlighted_(false) {}

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
    {
        if(highlighted_) {
            QBrush brush(QPalette().highlight());
            painter->setBrush(brush);
            QPen old_pen = painter->pen();
            QPen p(QBrush(), 0);
            painter->setPen(p);
            painter->drawRect(boundingRect().adjusted(-1, -1, 1, 1));
            painter->setPen(old_pen);
        }

        QGraphicsSimpleTextItem::paint(painter, option, widget);
    }

    bool isHighlighted() const {
        return highlighted_;
    }

    void highlight(bool value) {
        if(highlighted_ == value)
            return;

        highlighted_ = value;
        update();
    }

private:
    bool highlighted_;
};

ThumbnailWidget::ThumbnailWidget(Page * page) :
    page_(page),
    pixmap_(NULL),
    label_(NULL),
    indicator_(NULL),
    drag_progress_(false),
    hightlighted_(false)
{
    setFlag(QGraphicsItem::ItemClipsToShape);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setAcceptDrops(true);

    setupPixmap();
    setupLabel();
    setupToolTip();
    setupIndicator();

    connect(page, SIGNAL(rotated(int)), SLOT(handlePageRotate()));
    connect(page, SIGNAL(changed()), SLOT(updatePageIndicators()));
    connect(page, SIGNAL(exported()), SLOT(updatePageIndicators()));

    updatePageIndicators();
}

void ThumbnailWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent * event) {
    emit createContextMenu(this, event->screenPos());
    event->accept();
}

void ThumbnailWidget::showProperties() {
    Q_CHECK_PTR(page_);
    PagePropertiesDialog d(page_);
    d.exec();
}

void ThumbnailWidget::selectThumb(bool value) {
    Q_CHECK_PTR(pixmap_);
    pixmap_->highlight(value);
    label_->highlight(value);
}

void ThumbnailWidget::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    switch (event->button()) {
    case Qt::LeftButton:
        drag_start_pos_ = event->pos();
        event->accept();
        emit clicked(this, event->modifiers());
        break;
    default:
        event->ignore();
    }
}

void ThumbnailWidget::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if(hightlighted_) {
        QBrush brush(QPalette().highlight());
        painter->setBrush(brush);
        QPen old_pen = painter->pen();
        QPen p(QBrush(), 0);
        painter->setPen(p);
        painter->drawRect(boundingRect());
        painter->setPen(old_pen);
    }
}

Page * ThumbnailWidget::page() const {
    return page_;
}

void ThumbnailWidget::pageFaultForward() {
    if(page_)
        emit showPageFault(page_);
}

void ThumbnailWidget::handlePageRotate() {
    Q_CHECK_PTR(pixmap_);
    Q_CHECK_PTR(page_);

    if(!page_->thumb())
        return;

    pixmap_->setPixmap(*page_->thumb());
    updatePixmapPos();
}

void ThumbnailWidget::setName(const QString& name) {
    Q_CHECK_PTR(label_);
    label_->setText(name);
    qreal x = (THUMB_WIDTH - label_->boundingRect().width()) / 2;
    qreal y = label_->pos().y();
    label_->setPos(x, y);
}

void ThumbnailWidget::setupIndicator() {
    static const int MARGIN = 17;
    indicator_ = new PageIndicator(this);
    indicator_->setZValue(20);
    connect(indicator_, SIGNAL(showWarningDetails()), SLOT(pageFaultForward()));

    const qreal x_off = 2;
    const qreal y_off = boundingRect().height() - (indicator_->boundingRect().height() + MARGIN);
    indicator_->setPos(x_off, y_off);
}

void ThumbnailWidget::setupLabel() {
    label_ = new Label(this);
    label_->setPos(THUMB_WIDTH / 2, THUMB_HEIGHT - 30);
}

void ThumbnailWidget::setupPixmap() {
    QPixmap thumb(THUMB_IMAGE_HEIGHT, THUMB_IMAGE_WIDTH);

    if(!page_ || !page_->thumb()) {
        thumb.fill();
    }
    else {
        thumb = *page_->thumb();
    }

    pixmap_ = new Pixmap(thumb, this);
    updatePixmapPos();
}

void ThumbnailWidget::setupToolTip() {
    Q_CHECK_PTR(page_);

    setToolTip(page_->imagePath());
}

void ThumbnailWidget::toggleSelection() { 
    selectThumb(!isThumbSelected());
    update();
}

void ThumbnailWidget::updatePageIndicators() {
    Q_CHECK_PTR(indicator_);
    indicator_->setRecognized(page_->isRecognized());
    indicator_->setSaved(page_->isExported());
    indicator_->setWarning(page_->hasFlag(Page::RECOGNITION_FAILED));
}

QRectF ThumbnailWidget::boundingRect() const
{
    return QRectF(0, 0, THUMB_WIDTH, THUMB_HEIGHT);
}

bool ThumbnailWidget::isThumbSelected() const
{
    Q_CHECK_PTR(label_);
    return label_->isHighlighted();
}

void ThumbnailWidget::updatePixmapPos()
{
    Q_CHECK_PTR(pixmap_);
    pixmap_->setPos((THUMB_WIDTH - pixmap_->boundingRect().width())/2, THUMB_IMAGE_MARGIN);
}

void ThumbnailWidget::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if(!(event->buttons() & Qt::LeftButton))
        return;

    if((event->scenePos() - drag_start_pos_).manhattanLength() < QApplication::startDragDistance())
        return;

    drag_progress_ = true;
    emit dragged(this, event->scenePos());
}

void ThumbnailWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if(!drag_progress_) {
        QGraphicsObject::mouseReleaseEvent(event);
        return;
    }

    drag_progress_ = false;
    emit dropped(this, event->scenePos());
}

void ThumbnailWidget::highlight(bool value)
{
    hightlighted_ = value;
    update();
}
