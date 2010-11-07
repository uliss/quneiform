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
#include <QVBoxLayout>
#include <QWheelEvent>

//#include <QtOpenGL/QGLWidget>

#include "imagewidget.h"
#include "imageview.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent), layout_(NULL), view_(NULL) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setupLayout();
    setupView();
}

void ImageWidget::fitPage() {
    Q_CHECK_PTR(view_);
    view_->fitPage();
}

void ImageWidget::fitWidth() {
    Q_CHECK_PTR(view_);
    view_->fitWidth();
}

void ImageWidget::originalSize() {
    Q_CHECK_PTR(view_);
    view_->originalSize();
}

void ImageWidget::setupLayout() {
    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(0, 0, 0, 0);
    layout_->setMargin(0);
    layout_->setSpacing(0);
    setLayout(layout_);
}

void ImageWidget::setupView() {
    view_ = new ImageView(this);
    connect(view_, SIGNAL(pageDeleted()), SIGNAL(pageDeleted()));
//    view_->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));

    layout_->addWidget(view_);
}

void ImageWidget::showPage(Page * p) {
    if(!view_) {
        qDebug() << Q_FUNC_INFO << "no view";
        return;
    }

    view_->showPage(p);
}

QSize ImageWidget::sizeHint () const {
    return QSize(800, 600);
}

void ImageWidget::wheelEvent(QWheelEvent * event) {
    Q_CHECK_PTR(view_);

    if(!(event->modifiers() & Qt::ControlModifier))
        return QWidget::wheelEvent(event);

    if(event->delta() > 0)
        view_->zoomIn(1.03);
    else
        view_->zoomOut(0.93);

    event->accept();
}

void ImageWidget::zoomIn() {
    Q_CHECK_PTR(view_);
    view_->zoomIn(1.25);
}

void ImageWidget::zoomOut() {
    Q_CHECK_PTR(view_);
    view_->zoomOut(0.8);
}
