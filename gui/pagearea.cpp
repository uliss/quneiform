/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <QSettings>
#include <QDebug>

#include <QPen>
#include <QGraphicsScene>

#include "pagearea.h"
#include "pagelayout.h"

PageArea::PageArea() :
        layout_(NULL),
        current_char_bbox_(NULL)
{}

void PageArea::clear() {
    clearCurrentChar();
    clearLayout();
}

void PageArea::clearCurrentChar() {
    delete current_char_bbox_;
    current_char_bbox_ = NULL;
}

void PageArea::clearLayout() {
    delete layout_;
    layout_ = NULL;
}

void PageArea::hideLayout() {
    if(layout_)
        layout_->hide();
}

void PageArea::show(Page * page) {
    updateLayout(page);
}

void PageArea::showChar(const QRect& bbox) {
    QSettings settings;
    settings.beginGroup("format");
    QPen p(settings.value("currentCharColor", Qt::red).value<QColor>());

    if(!current_char_bbox_) {
        current_char_bbox_ = new QGraphicsRectItem(bbox, this);
        current_char_bbox_->setPen(p);
    }
    else {
        current_char_bbox_->setRect(bbox);
    }
}

void PageArea::showLayout() {
    if(layout_)
        layout_->show();
}

void PageArea::updateLayout(Page * page) {
    if(!page) {
        qDebug() << "[Error]" << Q_FUNC_INFO << "null page pointer";
        return;
    }

    if(!layout_)
        layout_ = new PageLayout();
    else
        layout_->clear();

    layout_->setParentItem(this);
    layout_->populate(*page);
}
