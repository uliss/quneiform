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
#include <QMenu>
#include <QContextMenuEvent>
#include <QRubberBand>
#include <QGraphicsRectItem>

#include "imagegraphicsview.h"

ImageGraphicsView::ImageGraphicsView(QWidget * parent) :
    QGraphicsView(parent), select_mode_(NORMAL), rubber_band_(NULL)
{
    setBackgroundRole(QPalette::Dark);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setInteractive(true);
}

void ImageGraphicsView::contextMenuEvent(QContextMenuEvent * event) {
    Q_CHECK_PTR(event);

    QGraphicsView::contextMenuEvent(event);

    QMenu menu;
    QAction * s_recognize = menu.addAction(QIcon(":/img/oxygen/22x22/.png"),
                   tr("Select recognize area"),
                   this, SLOT(selectPageArea()));
    s_recognize->setCheckable(true);

    QAction * s_text = menu.addAction(QIcon(":/img/oxygen/22x22/.png"),
                   tr("Select text areas"),
                   this, SLOT(selectPageArea()));
    s_text->setCheckable(true);

    QAction * s_image = menu.addAction(QIcon(":/img/oxygen/22x22/.png"),
                   tr("Select images"),
                   this, SLOT(selectPageArea()));
    s_image->setCheckable(true);

    menu.exec(event->globalPos());
}

void ImageGraphicsView::mouseMoveEvent(QMouseEvent * event) {
    QGraphicsView::mouseMoveEvent(event);

    if(select_mode_ == NORMAL)
        return;

    if(event->buttons() & Qt::LeftButton) {
        if(rubber_band_)
            rubber_band_->setGeometry(QRect(selection_origin_, event->pos()).normalized());
    }


}

void ImageGraphicsView::mousePressEvent(QMouseEvent * event) {
    QGraphicsView::mousePressEvent(event);

    if(select_mode_ == NORMAL)
        return;

    if(event->button() != Qt::LeftButton)
        return;

    selection_origin_ = event->pos();
    if (!rubber_band_)
        rubber_band_ = new QRubberBand(QRubberBand::Rectangle, this);

    rubber_band_->setGeometry(QRect(selection_origin_, QSize()));
    rubber_band_->show();
}

void ImageGraphicsView:: mouseReleaseEvent(QMouseEvent * event) {
    QGraphicsView::mouseReleaseEvent(event);

    if(select_mode_ == NORMAL)
        return;

    QPolygonF r = mapToScene(QRect(selection_origin_, event->pos()).normalized());

    switch(select_mode_) {
    case SELECT_PAGE:
        rubber_band_->hide();
        emit pageAreaSelected(r.boundingRect());
        select_mode_ = NORMAL;
        break;
    default:
        break;
    }

    setCursor(QCursor());
}

void ImageGraphicsView::selectPageArea() {
    select_mode_ = SELECT_PAGE;
    updateCursor();
}

void ImageGraphicsView::updateCursor() {
    switch(select_mode_) {
    case NORMAL:
        setCursor(QCursor());
        break;
    case SELECT_PAGE:
        setCursor(Qt::CrossCursor);
        break;
    case SELECT_IMAGE:
    case SELECT_TEXT:
    case SELECT_TABLE:
        setDragMode(QGraphicsView::RubberBandDrag);
        break;
    }
}
