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

#include "imageview.h"
#include <qdebug.h>
#include <QScrollBar>

static const double MAX_SIZE = 4.0;
static const double MIN_SIZE = 0.1;

ImageView::ImageView(QWidget * parent) :
	QGraphicsView(parent) {
	setBackgroundRole(QPalette::Dark);
	setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageView::fitPage() {
	QRectF scene_rect = sceneRect();

	// if image is smaller then view area set it to 100% size
	if (scene_rect.height() < height() && scene_rect.width() < width()) {
		setTransform(QTransform());
	}
	else {
		fitInView(scene_rect, Qt::KeepAspectRatio);
	}

	emit scaled(transform().m11());
}

void ImageView::fitWidth() {
	QTransform transform;
	static const int CORRECTION = 6;
	int view_wd = width() - verticalScrollBar()->width() - CORRECTION;
	int scene_wd = sceneRect().width();

	if(view_wd < scene_wd) {
		qreal s = qreal(view_wd) / scene_wd;
		transform.scale(s, s);
	}

    setTransform(transform);
    ensureVisible(QRectF());

	emit scaled(transform.m11());
}

void ImageView::zoomIn() {
	if(transform().m11() < MAX_SIZE) {
		scale(1.25, 1.25);
		emit scaled(transform().m11());
	}
}

void ImageView::zoomOut() {
	if(transform().m11() > MIN_SIZE) {
		scale(0.8, 0.8);
		emit scaled(transform().m11());
	}
}
