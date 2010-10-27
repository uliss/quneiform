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

#include <QPen>
#include <QDebug>
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>

#include "selection.h"

static const int THRESHOLD = 5;
static const int MIN_WIDTH = 20;
static const int MIN_HEIGHT = 20;

Selection::Selection(const QRectF& area) : QGraphicsRectItem(area), resize_mode_(NONE)
{
    QColor c(250, 0, 40, 50);
    QPen pen(c);
    pen.setWidth(2);
    setPen(pen);
    setRect(area);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void Selection::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
    setResizeCursor(event->pos());
}

void Selection::hoverMoveEvent(QGraphicsSceneHoverEvent * event) {
    if(rect().adjusted(THRESHOLD, THRESHOLD, -THRESHOLD, -THRESHOLD).contains(event->pos()))
        setCursor(QCursor());
    else
        setResizeCursor(event->pos());
}

void Selection::hoverLeaveEvent(QGraphicsSceneHoverEvent * event) {
   setCursor(QCursor());
}

void Selection::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    switch(resize_mode_) {
    case LEFT: {
            QRectF r = rect();
            if(r.width() < MIN_WIDTH && (r.left() < event->pos().x()))
                break;

            r.setLeft(event->pos().x());
            setRect(r.normalized());
        }
        break;
    case RIGHT:
        {
            QRectF r = rect();
            if(r.width() < MIN_WIDTH && (event->pos().x() < r.right()))
                break;

            r.setRight(event->pos().x());
            setRect(r.normalized());
        }
        break;
    case UP:
        {
            QRectF r = rect();
            if(r.height() < MIN_HEIGHT && (r.top() < event->pos().y()))
                break;

            r.setTop(event->pos().y());
            setRect(r.normalized());
        }
        break;
    case DOWN:
        {
            QRectF r = rect();
            if(r.height() < MIN_HEIGHT && (event->pos().y() < r.bottom()))
                break;

            r.setBottom(event->pos().y());
            setRect(r.normalized());
        }
        break;
    default:
        break;
    }
}

void Selection::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsRectItem::mousePressEvent(event);

    resize_mode_ = resizeMode(event->pos());
}

void Selection::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsRectItem::mouseReleaseEvent(event);
    setSelected(false);
}

void Selection::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    QGraphicsRectItem::paint(painter, option, widget);
//    QPainterPath path(rect().bottomRight());
//    path.moveTo(0, 10);
//    path.moveTo(-10, 10);

//    painter->save();
//    QPen pen(Qt::blue);
//    pen.setWidth(10);
//    painter->setPen(pen);
//    painter->drawPath(path);
//    painter->restore();
}

Selection::resize_t Selection::resizeMode(const QPointF& pos) const {
    qreal v_dist = qMin(qAbs(rect().top() - pos.y()), qAbs(rect().bottom() - pos.y()));
    qreal h_dist = qMin(qAbs(rect().left() - pos.x()), qAbs(rect().right() - pos.x()));

    if(v_dist < h_dist) {
        if(THRESHOLD < v_dist)
            return NONE;
        else
            return qAbs(rect().top() - pos.y()) < qAbs(rect().bottom() - pos.y()) ? UP : DOWN;
    }
    else {
        if(THRESHOLD < h_dist)
            return NONE;
        else
            return qAbs(rect().left() - pos.x()) < qAbs(rect().right() - pos.x()) ? LEFT : RIGHT;
    }
}

void Selection::setResizeCursor(const QPointF& pos) {
    qreal v_dist = qMin(qAbs(rect().top() - pos.y()), qAbs(rect().bottom() - pos.y()));
    qreal h_dist = qMin(qAbs(rect().left() - pos.x()), qAbs(rect().right() - pos.x()));

    setCursor(v_dist < h_dist ? Qt::SizeVerCursor : Qt::SizeHorCursor);
}
