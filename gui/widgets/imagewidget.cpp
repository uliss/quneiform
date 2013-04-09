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
    act_bin_(NULL),
    act_add_area_(NULL),
    act_add_image_(NULL),
    act_add_text_(NULL),
    act_toggle_layout_(NULL),
    act_segment_(NULL)
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

void ImageWidget::handleActionAddImageBlock()
{
    Q_CHECK_PTR(view_);
    view_->startImageBlockSelection();
}

void ImageWidget::handleActionAddTextBlock()
{
    Q_CHECK_PTR(view_);
    view_->startTextBlockSelection();
}

void ImageWidget::handleActionSegment()
{
    Q_CHECK_PTR(view_);

    if(!view_->hasPage()) {
        qWarning() << Q_FUNC_INFO << "NULL page request";
        return;
    }

    emit segment(view_->page());
}

void ImageWidget::handleActionSelectArea()
{
    Q_CHECK_PTR(view_);
    view_->startPageAreaSelection();
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
    toolbar_->setProperty("imageView", true);
    toolbar_->setIconSize(QSize(12, 12));
    toolbar_->setFloatable(false);

    act_bin_ = toolbar_->addAction(iconFromTheme("binarize"), tr("Binarize"));
    act_bin_->setToolTip(tr("Binarize"));
    act_bin_->setCheckable(true);
    act_bin_->setChecked(false);
    act_bin_->setDisabled(true);
    connect(act_bin_, SIGNAL(toggled(bool)), SLOT(handleActionBinarize(bool)));

    act_add_area_ = toolbar_->addAction(iconFromTheme("select-rectangular"), tr("Select recognition area"));
    act_add_area_->setDisabled(true);
    act_add_area_->setToolTip(tr("Select recognition area"));
    connect(act_add_area_, SIGNAL(triggered()), SLOT(handleActionSelectArea()));

    act_add_text_ = toolbar_->addAction(iconFromTheme("insert-text"), tr("Add text block"));
    act_add_text_->setDisabled(true);
    act_add_text_->setToolTip(tr("Add text block"));
    connect(act_add_text_, SIGNAL(triggered()), SLOT(handleActionAddTextBlock()));

    act_add_image_ = toolbar_->addAction(iconFromTheme("insert-image"), tr("Add image block"));
    act_add_image_->setDisabled(true);
    act_add_image_->setToolTip(tr("Add image block"));
    connect(act_add_image_, SIGNAL(triggered()), SLOT(handleActionAddImageBlock()));

    toolbar_->addSeparator();

//    act_toggle_layout_ = toolbar_->addAction(tr("Show layout"));
//    connect(act_toggle_layout_, SIGNAL(triggered()), SLOT(toggleLayoutBlocks()));
//    act_toggle_layout_->setCheckable(true);

    act_segment_ = toolbar_->addAction(tr("Analyze"));
    act_segment_->setDisabled(true);
    act_segment_->setToolTip(tr("Perform page layout analysis"));
    connect(act_segment_, SIGNAL(triggered()), SLOT(handleActionSegment()));

    layout_->addWidget(toolbar_);
}

void ImageWidget::setupView() {
    view_ = new ImageView(this);
    view_->setMinScale(0.1);
    view_->setMaxScale(10);
    connect(view_, SIGNAL(pageDeleted()), SLOT(handlePageDelete()));
    connect(view_, SIGNAL(scaled()), SIGNAL(scaled()));
    connect(view_, SIGNAL(scaleIsTooBig()), SIGNAL(scaleIsTooBig()));
    connect(view_, SIGNAL(scaleIsTooSmall()), SIGNAL(scaleIsTooSmall()));
    connect(view_, SIGNAL(gestureRotateAttempt(int)), SIGNAL(gestureRotateAttempt(int)));
    connect(view_, SIGNAL(recognize(Page*)), SIGNAL(recognize(Page*)));

#ifdef QT_OS_MAC
    view_->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif

    layout_->addWidget(view_);
}

void ImageWidget::showChar(const QRect& bbox) {
    view_->showChar(bbox);
}

void ImageWidget::toggleLayoutBlocks()
{
    view_->isLayoutBlockVisible(BLOCK_LAYOUT_TEXT) ?
                view_->hideLayoutBlocks(BLOCK_LAYOUT_TEXT) :
                view_->showLayoutBlocks(BLOCK_LAYOUT_TEXT);

    view_->isLayoutBlockVisible(BLOCK_LAYOUT_IMAGE) ?
                view_->hideLayoutBlocks(BLOCK_LAYOUT_IMAGE) :
                view_->showLayoutBlocks(BLOCK_LAYOUT_IMAGE);
}

void ImageWidget::showPage(Page * p) {
    if(!view_) {
        qDebug() << Q_FUNC_INFO << "no view";
        return;
    }

    view_->showPage(p);
    updateActions();
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

void ImageWidget::updateFormatLayout()
{
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

void ImageWidget::handlePageDelete()
{
    updateActions();
    emit pageDeleted();
}

void ImageWidget::updateActions()
{
    act_bin_->blockSignals(true);
    act_bin_->setChecked(false);
    act_bin_->blockSignals(false);

    bool has_page = view_->hasPage();

    act_bin_->setEnabled(has_page);
    act_add_image_->setEnabled(has_page);
    act_add_text_->setEnabled(has_page);
    act_add_area_->setEnabled(has_page);
    act_segment_->setEnabled(has_page);

//    act_toggle_layout_->setChecked(view_->isLayoutBlockVisible(BLOCK_LAYOUT_TEXT));
}
