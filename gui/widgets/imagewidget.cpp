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
#include <QToolBar>
#include <QAction>

#ifdef QT_OS_MAC
#include <QtOpenGL/QGLWidget>
#endif

#include "imagewidget.h"
#include "imageview.h"
#include "iconutils.h"

ImageWidget::ImageWidget(QWidget * parent) :
    QWidget(parent),
    layout_(NULL),
    view_(NULL) ,
    toolbar_(NULL),
    act_bin_(NULL)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setupLayout();
    setupToolBar();
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

void ImageWidget::handleActionBinarize(bool checked)
{
    if(checked)
        view_->showPageBinarized();
    else
        view_->showPageOriginal();
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

void ImageWidget::setupToolBar()
{
    toolbar_ = new QToolBar(this);
    toolbar_->setIconSize(QSize(12, 12));
    toolbar_->setFloatable(false);

    act_bin_ = toolbar_->addAction(iconFromTheme("binarize"), tr("Binarize"));
    connect(act_bin_, SIGNAL(toggled(bool)), SLOT(handleActionBinarize(bool)));
    act_bin_->setCheckable(true);
    act_bin_->setChecked(false);
    act_bin_->setDisabled(true);
    layout_->addWidget(toolbar_);
}

void ImageWidget::setupView() {
    view_ = new ImageView(this);
    view_->setMinScale(0.1);
    view_->setMaxScale(10);
    connect(view_, SIGNAL(pageDeleted()), SIGNAL(pageDeleted()));
    connect(view_, SIGNAL(scaled()), SIGNAL(scaled()));
    connect(view_, SIGNAL(scaleIsTooBig()), SIGNAL(scaleIsTooBig()));
    connect(view_, SIGNAL(scaleIsTooSmall()), SIGNAL(scaleIsTooSmall()));
    connect(view_, SIGNAL(gestureRotateAttempt(int)), SIGNAL(gestureRotateAttempt(int)));
    connect(view_, SIGNAL(recognize(Page*)), SIGNAL(recognize(Page*)));
    connect(view_, SIGNAL(binarize(Page*)), SIGNAL(binarize(Page*)));

#ifdef QT_OS_MAC
    view_->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif

    layout_->addWidget(view_);
}

void ImageWidget::showChar(const QRect& bbox) {
    view_->showChar(bbox);
}

void ImageWidget::showPage(Page * p) {
    if(!view_) {
        qDebug() << Q_FUNC_INFO << "no view";
        return;
    }

    view_->showPage(p);
    resetBinarizeAction();
}

void ImageWidget::showPageBinarized()
{
    if(!view_) {
        qDebug() << Q_FUNC_INFO << "no view";
        return;
    }

    view_->showPageBinarized();
}

QSize ImageWidget::sizeHint () const {
    return QSize(800, 600);
}

void ImageWidget::updateFormatLayout() {
    Q_CHECK_PTR(view_);

    view_->updatePageArea();
}

void ImageWidget::updateSettings() {
    updateFormatLayout();
}

void ImageWidget::zoom(qreal factor) {
    Q_CHECK_PTR(view_);

    view_->zoom(factor);
}

void ImageWidget::zoomIn() {
    zoom(1.25);
}

void ImageWidget::zoomOut() {
    zoom(0.8);
}

void ImageWidget::resetBinarizeAction()
{
    act_bin_->blockSignals(true);
    act_bin_->setChecked(false);
    act_bin_->blockSignals(false);
    act_bin_->setEnabled(view_->hasPage());
}
