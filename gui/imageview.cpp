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

#include "imageview.h"
#include "imagecache.h"
#include "page.h"
#include "widgetbar.h"

ImageView::ImageView(QWidget * parent) : QWidget(parent),
page_(NULL) {
    view_ = new QGraphicsView;
    view_->setBackgroundRole(QPalette::Dark);
    view_->setScene(&scene_);

    layout_ = new QVBoxLayout;
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setMargin(0);
    layout_->setSpacing(0);
    layout_->addWidget(view_);

    setLayout(layout_);
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

void ImageView::originalSize() {
    // TODO
}

void ImageView::saveTransform() {
    Q_CHECK_PTR(page_);
    page_->setTransform(view_->transform());
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

    scene_.addPixmap(image);
    scene_.setSceneRect(image.rect());

    view_->setDragMode(QGraphicsView::ScrollHandDrag);
    setPage(page);
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
