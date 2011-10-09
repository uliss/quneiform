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

#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QGraphicsLinearLayout>
#include <QGraphicsLayout>

#include "pageindicator.h"
#include "imagecache.h"

static const QString RECOGNIZED(":/img/oxygen/22x22/dialog_ok.png");
static const QString SAVED(":/img/oxygen/22x22/document_save.png");
static const QString WARNING(":/img/oxygen/32x32/messagebox_warning.png");
static const int ICON_WIDTH = 16;

PageIndicator::PageIndicator(QGraphicsItem * parent) :
    QGraphicsObject(parent),
    recognized_(NULL),
    saved_(NULL),
    warning_(NULL)
{
    recognized_ = new QGraphicsPixmapItem(this);
    recognized_->setToolTip(tr("Page recognized"));
    recognized_->setPixmap(indicatorIcon(RECOGNIZED));

    saved_ = new QGraphicsPixmapItem(this);
    saved_->setToolTip(tr("Page saved"));
    saved_->setPixmap(indicatorIcon(SAVED));
    saved_->moveBy(recognized_->pos().x() + recognized_->boundingRect().width(), 0);

    warning_ = new QGraphicsPixmapItem(this);
    warning_->setPixmap(indicatorIcon(WARNING));
    warning_->setToolTip(tr("Recognition errors"));
    warning_->moveBy(saved_->pos().x() + saved_->boundingRect().width(), 0);
}

QPixmap PageIndicator::indicatorIcon(const QString& path) {
    QPixmap pixmap;

    if(!ImageCache::find(path, &pixmap)) {
        pixmap.load(path);
        pixmap = pixmap.scaled(ICON_WIDTH, ICON_WIDTH, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ImageCache::insert(path, pixmap);
    }

    return pixmap;
}

//void PageIndicator::mousePressEvent(QMouseEvent * event) {
//    if (event->button() == Qt::LeftButton) {
//        if(warning_->isVisible() && warning_->geometry().contains(event->pos())) {
//            emit showWarningDetails();
//            event->accept();
//        }
//    }
//}

void PageIndicator::setRecognized(bool value) {
    if(value)
        recognized_->show();
    else
        recognized_->hide();
}

void PageIndicator::setSaved(bool value) {
    if(value)
        saved_->show();
    else
        saved_->hide();
}

void PageIndicator::setWarning(bool value) {
    if(value)
        warning_->show();
    else
        warning_->hide();
}

QRectF PageIndicator::boundingRect() const {
    return QRectF(0, 0, ICON_WIDTH * 4, ICON_WIDTH);
}

void PageIndicator::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}
