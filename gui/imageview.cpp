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

#include <qdebug.h>
#include <QScrollBar>

#include "imageview.h"
#include "page.h"

ImageView::ImageView(QWidget * parent) :
	QGraphicsView(parent), page_(NULL) {
	setBackgroundRole(QPalette::Dark);
}

void ImageView::fitPage() {
	Q_CHECK_PTR(page_);
	QRectF scene_rect = sceneRect();

	// if image is smaller then view area set it to 100% size
	if (scene_rect.height() < height() && scene_rect.width() < width())
		originalSize();
	else
		fitInView(scene_rect, Qt::KeepAspectRatio);

	saveTransform();
}

void ImageView::fitWidth() {
	Q_CHECK_PTR( page_);

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

	saveTransform();
}

void ImageView::deletePage() {
    if(sender() == page_) {
        qDebug() << "[ImageView::deletePage] " << page_;
        page_ = NULL;
    }
}

void ImageView::originalSize() {
	Q_CHECK_PTR(page_);
}

void ImageView::saveTransform() {
	Q_CHECK_PTR(page_);
	page_->setTransform(transform());
}

void ImageView::setPage(Page * page) {
	Q_CHECK_PTR(page);
        if(page_) {
                disconnect(page_, SIGNAL(transformed()), this, SLOT(updatePage()));
                disconnect(page_, SIGNAL(rotated(int)), this, SLOT(updatePage()));
        }

	page_ = page;

	connect(page_, SIGNAL(transformed()), this, SLOT(updatePage()));
	connect(page_, SIGNAL(rotated(int)), this, SLOT(updatePage()));
        connect(page, SIGNAL(destroyed()), this, SLOT(deletePage()));

	updatePage();
}

void ImageView::updatePage() {
	Q_CHECK_PTR(page_);

	if(transform() != page_->transform())
		setTransform(page_->transform());
}

void ImageView::zoomIn() {
	Q_CHECK_PTR( page_);
	page_->scale(1.25);
}

void ImageView::zoomOut() {
	Q_CHECK_PTR(page_);
	page_->scale(0.8);
}
