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

#include <algorithm>

#include "selection.h"

static const int THRESHOLD = 5;
static const int MIN_WIDTH = 20;
static const int MIN_HEIGHT = 20;
static const int MOVE_STEP = 3;
static const int MOVE_FAST_FACTOR = 6;

Selection::Selection(const QRectF& area) : QGraphicsRectItem(area), resize_(NONE)
{
    QColor c(250, 0, 40, 50);
    QPen pen(c);
    pen.setWidth(2);
    setPen(pen);
    setRect(area);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void Selection::hoverEnterEvent(QGraphicsSceneHoverEvent * event) {
    setResizeCursor(event->pos());
}

template<class T> size_t array_size(const T& t) {
    return sizeof(t) / sizeof(t[0]);
}

enum border_t {
    // don't change sequential order!
    LEFT = 0,
    RIGHT = 1,
    TOP = 2,
    BOTTOM = 3
};

inline qreal borderDistance(const QRectF& r, const QPointF& pt, border_t border) {
    switch(border) {
    case LEFT:
        return qAbs(r.left() - pt.x());
    case RIGHT:
        return qAbs(r.right() - pt.x());
    case TOP:
        return qAbs(r.top() - pt.y());
    case BOTTOM:
        return qAbs(r.bottom() - pt.y());
    }
}

inline Selection::resize_t border2resize(border_t border) {
    static const Selection::resize_t tbl[] = {
        Selection::LEFT,
        Selection::RIGHT,
        Selection::UP,
        Selection::DOWN
    };

    Q_ASSERT(0 <= border && border < array_size(tbl));

    return tbl[border];
}

border_t nearestBorder(const QRectF& r, const QPointF& pt) {
    qreal v[4];
    const size_t sz = array_size(v);
    v[LEFT] = borderDistance(r, pt, LEFT);
    v[RIGHT] =  borderDistance(r, pt, RIGHT);
    v[TOP] =  borderDistance(r, pt, TOP);
    v[BOTTOM] =  borderDistance(r, pt, BOTTOM);

    return static_cast<border_t>(std::distance(v, std::min_element(v, v + sz)));
}

qreal nearestBorderDistance(const QRectF& r, const QPointF& pt) {
    return qMin(borderDistance(r, pt, LEFT),
                qMin(borderDistance(r, pt, RIGHT),
                     qMin(borderDistance(r, pt, TOP), borderDistance(r, pt, BOTTOM))));
}

enum corner_t {
    LEFT_TOP = 0,
    RIGHT_TOP = 1,
    RIGHT_BOTTOM = 2,
    LEFT_BOTTOM = 3
};

inline int corner2resize(corner_t corner) {
    static const int tbl[] = {
        Selection::LEFT | Selection::UP,
        Selection::RIGHT | Selection::UP,
        Selection::RIGHT | Selection::DOWN,
        Selection::LEFT | Selection::DOWN
    };

    Q_ASSERT(0 <= corner && corner < array_size(tbl));

    return tbl[corner];
}

corner_t nearestCorner(const QRectF& r, const QPointF& pt) {
    QPointF rel = pt - r.center();
    if(rel.x() > 0)
        return rel.y() < 0 ? RIGHT_TOP : RIGHT_BOTTOM;
    else
        return rel.y() < 0 ? LEFT_TOP : LEFT_BOTTOM;
}

bool isCloseToCorner(const QRectF& r, const QPointF& pt, corner_t corner) {
    static const qreal DELTA = 10.0;

    switch(corner) {
    case LEFT_TOP:
        return qAbs(borderDistance(r, pt, LEFT) - borderDistance(r, pt, TOP)) < DELTA;
    case LEFT_BOTTOM:
        return qAbs(borderDistance(r, pt, LEFT) - borderDistance(r, pt, BOTTOM)) < DELTA;
    case RIGHT_TOP:
        return qAbs(borderDistance(r, pt, RIGHT) - borderDistance(r, pt, TOP)) < DELTA;
    case RIGHT_BOTTOM:
        return qAbs(borderDistance(r, pt, RIGHT) - borderDistance(r, pt, BOTTOM)) < DELTA;
    }
}

bool isCloseToBorder(const QRectF& r, const QPointF& pt, border_t border) {
    static const qreal DELTA = 5;

    switch(border) {
    case LEFT:
        return borderDistance(r, pt, LEFT) < DELTA;
    case RIGHT:
        return borderDistance(r, pt, RIGHT) < DELTA;
    case TOP:
        return borderDistance(r, pt, TOP) < DELTA;
    case BOTTOM:
        return borderDistance(r, pt, BOTTOM) < DELTA;
    default:
        return false;
    }
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

bool Selection::isResizing() const {
    return resize_ != NONE;
}

void Selection::keyPressEvent(QKeyEvent * event) {
    qreal step = MOVE_STEP;

    if(event->modifiers() & Qt::ControlModifier)
        step *= MOVE_FAST_FACTOR;

    switch(event->key()) {
    case Qt::Key_Up:
        moveBy(QPointF(0, -step));
        break;
    case Qt::Key_Down:
        moveBy(QPointF(0, step));
        break;
    case Qt::Key_Left:
        moveBy(QPointF(-step, 0));
        break;
    case Qt::Key_Right:
        moveBy(QPointF(step, 0));
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

    emit resized();
}

void Selection::mousePressEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsRectItem::mousePressEvent(event);
    resize_ = resizeMode(event->pos());

    if(!resize_)
        setCursor(Qt::SizeAllCursor);
}

void Selection::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    QGraphicsRectItem::mouseReleaseEvent(event);
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

    if(resize_ & UP) {
        new_r.setTop(rect().top() + delta.y());
        if(new_r.top() < 0)
            new_r.setTop(0);
        if(new_r.height() < MIN_HEIGHT)
            return;
    }

    if(resize_ & DOWN) {
        new_r.setBottom(rect().bottom() + delta.y());
        if(new_r.bottom() > sceneRect().bottom())
            new_r.setBottom(sceneRect().bottom());
        if(new_r.height() < MIN_HEIGHT)
            return;
    }

    setRect(new_r.normalized());
}

int Selection::resizeMode(const QPointF& pos) const {
    corner_t corner = nearestCorner(rect(), pos);
    border_t border = nearestBorder(rect(), pos);

    if(isCloseToCorner(rect(), pos, corner))
        return corner2resize(corner);
    else if(isCloseToBorder(rect(), pos, border))
        return border2resize(border);
    else
        return NONE;
}

inline bool isVertical(int mode) {
    return mode & Selection::UP || mode & Selection::DOWN;
}

QRectF Selection::sceneRect() const {
    return scene()->sceneRect();
}

void Selection::setResizeCursor(const QPointF& pos) {
    int mode = resizeMode(pos);

    if(!mode) {
        emit cursorChange(NORMAL);
        return;
    }

    if(isVertical(mode)) {
        if(mode & LEFT)
            emit cursorChange(mode & UP ? DIAGONAL_LEFT : DIAGONAL_RIGHT);
        else if(mode & RIGHT)
            emit cursorChange(mode & DOWN ? DIAGONAL_LEFT : DIAGONAL_RIGHT);
        else
            emit cursorChange(VERTICAL);
    }
    else {
        emit cursorChange(HORIZONTAL);
    }
}
