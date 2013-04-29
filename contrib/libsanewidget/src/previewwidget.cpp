/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include <QImage>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "previewwidget.h"
#include "preview_area.h"

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent),
    preview_(NULL),
    preview_image_(NULL),
    zoom_in_btn_(NULL),
    zoom_out_btn_(NULL),
    zoom_sel_btn_(NULL),
    zoom_fit_btn_(NULL),
    preview_btn_(NULL),
    scan_btn_(NULL)
{
    preview_ = new PreviewArea(this);
    connect(preview_, SIGNAL(newSelection(float, float, float, float)),
             this, SLOT(handleSelection(float, float, float, float)));

    preview_image_ = preview_->getImage();

    zoom_in_btn_ = new QPushButton(tr("Zoom In"));
    zoom_out_btn_ = new QPushButton(tr("Zoom Out"));
    zoom_sel_btn_ = new QPushButton(tr("Zoom to Selection"));
    zoom_fit_btn_ = new QPushButton(tr("Zoom to Fit"));
    preview_btn_ = new QPushButton(tr("Preview"));
    scan_btn_ = new QPushButton(tr("Final Scan"));

    connect(zoom_in_btn_, SIGNAL(clicked(void)), preview_, SLOT(zoomIn(void)));
    connect(zoom_out_btn_, SIGNAL(clicked(void)), preview_, SLOT(zoomOut(void)));
    connect(zoom_sel_btn_, SIGNAL(clicked(void)), preview_, SLOT(zoomSel(void)));
    connect(zoom_fit_btn_, SIGNAL(clicked(void)), preview_, SLOT(zoom2Fit(void)));
    connect(scan_btn_, SIGNAL(clicked(void)), this, SIGNAL(scan()));
    connect(preview_btn_, SIGNAL(clicked(void)), this, SIGNAL(preview()));

    QVBoxLayout * layout = new QVBoxLayout;
    setLayout(layout);

    QHBoxLayout * zoom_layout = new QHBoxLayout;
    zoom_layout->setMargin(0);
    zoom_layout->setSpacing(5);

    layout->addWidget(preview_, 100);
    layout->addLayout(zoom_layout, 0);

    zoom_layout->addWidget(zoom_in_btn_);
    zoom_layout->addWidget(zoom_out_btn_);
    zoom_layout->addWidget(zoom_sel_btn_);
    zoom_layout->addWidget(zoom_fit_btn_);
    zoom_layout->addStretch();
    zoom_layout->addWidget(preview_btn_);
    zoom_layout->addWidget(scan_btn_);
}

QImage * PreviewWidget::previewImage()
{
    return preview_image_;
}

void PreviewWidget::handleSelection(float tl_x, float tl_y, float br_x, float br_y)
{
    if ((preview_image_->width() == 0) || (preview_image_->height() == 0))
        return;

    emit newSelection(QRect(QPoint(tl_x, tl_y), QPoint(br_x, br_y)));
}

void PreviewWidget::setIcon(const QIcon& icon, IconType type)
{
    switch(type) {
    case ICON_ZOOM_IN:
        zoom_in_btn_->setIcon(icon);
        zoom_in_btn_->setToolTip(zoom_in_btn_->text());
        zoom_in_btn_->setText("");
        break;
    case ICON_ZOOM_OUT:
        zoom_out_btn_->setIcon(icon);
        zoom_out_btn_->setToolTip(zoom_out_btn_->text());
        zoom_out_btn_->setText("");
        break;
    case ICON_ZOOM_SEL:
        zoom_sel_btn_->setIcon(icon);
        zoom_sel_btn_->setToolTip(zoom_sel_btn_->text());
        zoom_sel_btn_->setText("");
        break;
    case ICON_ZOOM_FIT:
        zoom_fit_btn_->setIcon(icon);
        zoom_fit_btn_->setToolTip(zoom_fit_btn_->text());
        zoom_fit_btn_->setText("");
        break;
    case ICON_PREVIEW:
        preview_btn_->setIcon(icon);
        break;
    case ICON_SCAN:
        scan_btn_->setIcon(icon);
        break;
    default:
        qWarning() << Q_FUNC_INFO << "unknown icon type:" << icon;
    }
}

void PreviewWidget::setTLX(float percent)
{
    preview_->setTLX(percent);
}

void PreviewWidget::setTLY(float percent)
{
    preview_->setTLY(percent);
}

void PreviewWidget::setBRX(float percent)
{
    preview_->setBRX(percent);
}

void PreviewWidget::setBRY(float percent)
{
    preview_->setBRY(percent);
}

QSize PreviewWidget::sizeHint() const
{
    return QSize(300, 700);
}

void PreviewWidget::updatePreviewSize(int width, int height)
{
    updatePreviewSize(QSize(width, height));
}

void PreviewWidget::updatePreviewSize(const QSize& size)
{
    if(preview_image_->size() != size) {
        *preview_image_ = QImage(size, QImage::Format_RGB32);
    }

    preview_image_->fill(Qt::white);
    // update the size of the preview widget.
    preview_->zoom2Fit();
}

void PreviewWidget::updateScanSize(const QSize& size)
{
    if (preview_image_->size() != size) {
        *preview_image_ = QImage(size, QImage::Format_RGB32);
        preview_image_->fill(Qt::white);

        preview_->clearSelection();
        // update the size of the preview widget.
        preview_->updateScaledImg();
    }
}

void PreviewWidget::updateScanSize(int width, int height)
{
    updateScanSize(QSize(width, height));
}

void PreviewWidget::updateScaledImg()
{
    preview_->updateScaledImg();
}
