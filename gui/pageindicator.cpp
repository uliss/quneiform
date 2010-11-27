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

#include "pageindicator.h"
#include "imagecache.h"

static const QString RECOGNIZED(":/img/oxygen/22x22/dialog_ok.png");
static const QString SAVED(":/img/oxygen/22x22/document_save.png");
static const int ICON_WIDTH = 16;

PageIndicator::PageIndicator(QWidget * parent) :
    QWidget(parent), recognized_(NULL), saved_(NULL)
{
    recognized_ = new QLabel(this);
    recognized_->setFixedSize(ICON_WIDTH, ICON_WIDTH);
    saved_ = new QLabel(this);
    saved_->move(ICON_WIDTH + 5, 0);
    saved_->setFixedSize(ICON_WIDTH, ICON_WIDTH);
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

void PageIndicator::setRecognized(bool value) {
    if(value)
        recognized_->setPixmap(indicatorIcon(RECOGNIZED));
    else
        recognized_->setPixmap(QPixmap());
}

void PageIndicator::setSaved(bool value) {
    if(value)
        saved_->setPixmap(indicatorIcon(SAVED));
    else
        saved_->setPixmap(QPixmap());
}

QSize PageIndicator::sizeHint() const {
    return QSize(ICON_WIDTH * 3, ICON_WIDTH);
}
