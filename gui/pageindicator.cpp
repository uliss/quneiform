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

#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "pageindicator.h"
#include "imagecache.h"

static const QString RECOGNIZED(":/img/oxygen/22x22/dialog_ok.png");
static const QString SAVED(":/img/oxygen/22x22/document_save.png");
static const QString WARNING(":/img/oxygen/32x32/messagebox_warning.png");
static const int ICON_WIDTH = 16;

PageIndicator::PageIndicator(QWidget * parent) :
    QWidget(parent), recognized_(NULL), saved_(NULL), warning_(NULL)
{
    recognized_ = new QLabel(this);
    recognized_->setFixedSize(ICON_WIDTH, ICON_WIDTH);
    recognized_->setToolTip(tr("Page recognized"));
    recognized_->setPixmap(indicatorIcon(RECOGNIZED));

    saved_ = new QLabel(this);
    saved_->setFixedSize(ICON_WIDTH, ICON_WIDTH);
    saved_->setToolTip(tr("Page saved"));
    saved_->setPixmap(indicatorIcon(SAVED));

    warning_ = new QLabel(this);
    warning_->setFixedSize(ICON_WIDTH, ICON_WIDTH);
    warning_->setPixmap(indicatorIcon(WARNING));
    warning_->setToolTip(tr("Recognition errors"));

    QHBoxLayout * l = new QHBoxLayout;
    l->addWidget(recognized_, 0, Qt::AlignLeft);
    l->addWidget(saved_, 0, Qt::AlignLeft);
    l->addWidget(warning_, 0, Qt::AlignLeft);
    l->addStretch(10);
    l->setContentsMargins(0, 0, 0, 0);
    l->setMargin(0);
    setLayout(l);
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

void PageIndicator::mousePressEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        if(warning_->isVisible() && warning_->geometry().contains(event->pos())) {
            emit showPageFault();
            event->accept();
        }
    }
}

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

QSize PageIndicator::sizeHint() const {
    return QSize(ICON_WIDTH * 4, ICON_WIDTH);
}
