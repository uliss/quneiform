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
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>

#include <map>
#include <algorithm>

#include "selection.h"
#include "selectionshadow.h"

static const int THRESHOLD = 5;
static const int MIN_WIDTH = 20;
static const int MIN_HEIGHT = 20;
static const int MOVE_STEP = 3;
static const int MOVE_FAST_FACTOR = 6;

Selection::Selection(const QRectF& area) : QGraphicsRectItem(area), resize_(NONE)
{
    QColor c(0, 0, 0, 100);
    QPen pen(c);
    pen.setWidth(2);
    setPen(pen);
    setRect(area);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

qreal Selection::borderDistance(const QPointF& pt, Selection::border_t border) const {
    switch(border) {
    case Selection::LEFT:
        return qAbs(rect().left() - pt.x());
    case Selection::RIGHT:
        return qAbs(rect().right() - pt.x());
    case Selection::TOP:
        return qAbs(rect().top() - pt.y());
    case Selection::BOTTOM:
        return qAbs(rect().bottom() - pt.y());
    }
}

int Selection::bordersResized(const QPointF& pos) const {
    corner_t corner = nearestCorner(pos);
    border_t border = nearestBorder(pos);

    if(isCloseToCorner(pos, corner))
        return corner;
    else if(isCloseToBorder(pos, border))
        return border;
    else
        return NONE;
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

bool Selection::isCloseToBorder(const QPointF& pt, Selection::border_t border) const {
    static const qreal DELTA = 5;

    switch(border) {
    case Selection::LEFT:
        return borderDistance(pt, Selection::LEFT) < DELTA;
    case Selection::RIGHT:
        return borderDistance(pt, Selection::RIGHT) < DELTA;
    case Selection::TOP:
        return borderDistance(pt, Selection::TOP) < DELTA;
    case Selection::BOTTOM:
        return borderDistance(pt, Selection::BOTTOM) < DELTA;
    default:
        return false;
    }
}

static const qreal DELTA = 10.0;

inline bool isNear(qreal v0, qreal v1) {
    return qAbs(v0 - v1) < DELTA && v0 < DELTA;
}

bool Selection::isCloseToCorner(const QPointF& pt, corner_t corner) const {
    static const qreal DELTA = 10.0;

    switch(corner) {
    case LEFT_TOP:
        return isNear(borderDistance(pt, Selection::LEFT), borderDistance(pt, Selection::TOP));
    case LEFT_BOTTOM:
        return isNear(borderDistance(pt, Selection::LEFT), borderDistance(pt, Selection::BOTTOM));
    case RIGHT_TOP:
        return isNear(borderDistance(pt, Selection::RIGHT), borderDistance(pt, Selection::TOP));
    case RIGHT_BOTTOM:
        return isNear(borderDistance(pt, Selection::RIGHT), borderDistance(pt, Selection::BOTTOM));
    }
}

bool Selection::isResizing() const {
    return resize_ != NONE;
}

typedef std::map<Selection::border_t, qreal> dist_map_t;
typedef dist_map_t::value_type border_dist_t;

bool distCmp(const border_dist_t& d0, const border_dist_t& d1) {
    return d0.second < d1.second;
}

Selection::border_t Selection::nearestBorder(const QPointF& pt) const {
    dist_map_t map;

    map[Selection::LEFT] = borderDistance(pt, Selection::LEFT);
    map[Selection::RIGHT] =  borderDistance(pt, Selection::RIGHT);
    map[Selection::TOP] =  borderDistance(pt, Selection::TOP);
    map[Selection::BOTTOM] =  borderDistance(pt, Selection::BOTTOM);

    return std::min_element(map.begin(),
                            map.end(),
                            distCmp)->first;
}

qreal Selection::nearestBorderDistance(const QPointF& pt) const {
    return qMin(borderDistance(pt, Selection::LEFT),
                qMin(borderDistance(pt, Selection::RIGHT),
                     qMin(borderDistance(pt, Selection::TOP),
                          borderDistance(pt, Selection::BOTTOM))));
}

Selection::corner_t Selection::nearestCorner(const QPointF& pt) const {
    QPointF rel = pt - rect().center();
    if(rel.x() > 0)
        return rel.y() < 0 ? RIGHT_TOP : RIGHT_BOTTOM;
    else
        return rel.y() < 0 ? LEFT_TOP : LEFT_BOTTOM;
}

void Selection::keyPressEvent(QKeyEvent * event) {
    qreal step = MOVE_STEP;

    if(event->modifiers() & Qt::ControlModifier)
        step *= MOVE_FAST_FACTOR;

    switch(event->key()) {
    case Qt::Key_Up:
        emit moved(QPointF(0, -step));
        break;
    case Qt::Key_Down:
        emit moved(QPointF(0, step));
        break;
    case Qt::Key_Left:
        emit moved(QPointF(-step, 0));
        break;
    case Qt::Key_Right:
        emit moved(QPointF(step, 0));
        break;
    case Qt::Key_Delete:
        emit selectionDeleted();
    default:
        break;
    }
}

void Selection::moveBy(const QPointF& delta) {
    QRectF new_r = rect();
    new_r.moveTo(rect().topLeft() + delta);

    if(delta.x() < 0 && new_r.left() < 0)
        new_r.moveTo(0, new_r.top());

    if(delta.x() > 0 && new_r.right() > sceneRect().right())
        new_r.moveTo(sceneRect().right() - new_r.width(), new_r.top());

    if(delta.y() < 0 && new_r.top() < 0)
        new_r.moveTo(new_r.left(), 0);

    if(delta.y() > 0 && new_r.bottom() > sceneRect().bottom())
        new_r.moveTo(new_r.left(), sceneRect().bottom() - new_r.height());

    setRect(new_r);
}

void Selection::mouseMoveEvent(QGraphicsSceneMouseEvent * event) {
    QPointF move_delta = event->pos() - event->lastPos();

    if(isResizing())
        resizeBy(move_delta);
    else
        moveBy(move_delta);
}

void Selection::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsRectItem::mousePressEvent(event);
    if(event->button() != Qt::LeftButton)
        return;

    resize_ = bordersResized(event->pos());

    if(!resize_)
        setCursor(Qt::SizeAllCursor);
}

void Selection::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsRectItem::mouseReleaseEvent(event);
    emit resized();
//    setSelected(false);
}

void Selection::resizeBy(const QPointF& delta) {
    QRectF new_r = rect();

    if(resize_ & LEFT) {
        new_r.setLeft(rect().left() + delta.x());
        if(new_r.left() < 0)
            new_r.setLeft(0);
        if(new_r.width() < MIN_WIDTH)
            return;
    }

    if(resize_ & RIGHT) {
        new_r.setRight(rect().right() + delta.x());
        if(new_r.right() > sceneRect().right())
            new_r.setRight(sceneRect().right());
        if(new_r.width() < MIN_WIDTH)
            return;
    }

    if(resize_ & TOP) {
        new_r.setTop(rect().top() + delta.y());
        if(new_r.top() < 0)
            new_r.setTop(0);
        if(new_r.height() < MIN_HEIGHT)
            return;
    }

    if(resize_ & BOTTOM) {
        new_r.setBottom(rect().bottom() + delta.y());
        if(new_r.bottom() > sceneRect().bottom())
            new_r.setBottom(sceneRect().bottom());
        if(new_r.height() < MIN_HEIGHT)
            return;
    }

    setRect(new_r.normalized());
}

inline bool isVertical(int mode) {
    return mode & Selection::TOP || mode & Selection::BOTTOM;
}

QRectF Selection::sceneRect() const {
    return scene()->sceneRect();
}

void Selection::setResizeCursor(const QPointF& pos) {
    int mode = bordersResized(pos);

    if(!mode) {
        emit cursorChange(NORMAL);
        return;
    }

    if(isVertical(mode)) {
        if(mode & LEFT)
            emit cursorChange(mode & TOP ? DIAGONAL_LEFT : DIAGONAL_RIGHT);
        else if(mode & RIGHT)
            emit cursorChange(mode & BOTTOM ? DIAGONAL_LEFT : DIAGONAL_RIGHT);
        else
            emit cursorChange(VERTICAL);
    }
    else {
        emit cursorChange(HORIZONTAL);
    }
}
