/* ============================================================
*
* This file is part of the KDE project
*
* Date        : 2008-11-15
* Description :Selection QGraphicsItem for the image viewer.
*
* Copyright (C) 2008 by Kare Sars <kare dot sars at iki dot fi>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) version 3, or any
* later version accepted by the membership of KDE e.V. (or its
* successor approved by the membership of KDE e.V.), which shall
* act as a proxy defined in Section 6 of version 3 of the license.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* ============================================================ */

#include "selectionitem.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QCursor>
#include <QList>

namespace KSaneIface
{


static const qreal selMargin = 4.0;
static const QPointF boundMargin(selMargin,selMargin);
static const qreal addRemMargin = 8.0;
static const QPointF addRemMarginPoint(addRemMargin, addRemMargin);

struct SelectionItem::Private
{
    QPen       penDark;
    QPen       penLight;
    QPen       penAddRemFg;
    QRectF     rect;
    qreal      maxX;
    qreal      maxY;
    bool       hasMaxX;
    bool       hasMaxY;
    bool       hasMax;
    bool       isSaved;
    bool       showAddRem;
    qreal      invZoom;
    qreal      selMargin;
    QRectF     addRemRect;
};


SelectionItem::SelectionItem(QRectF rect) : QGraphicsItem(), d(new Private)
{
    d->hasMaxX = false;
    d->hasMaxY = false;
    d->hasMax = false;
    setRect(rect);

    d->penDark.setColor(Qt::black);
    d->penDark.setStyle(Qt::SolidLine);
    d->penLight.setColor(Qt::white);
    d->penLight.setStyle(Qt::DashLine);

    // FIXME We should probably use some standard KDE color here and not hard code it
    d->penAddRemFg.setColor(Qt::darkGreen);
    d->penAddRemFg.setStyle(Qt::SolidLine);
    d->penAddRemFg.setWidth(3);

    d->isSaved = false;
    d->showAddRem = false;
    d->invZoom = 1;
    d->selMargin = selMargin;
    
    d->addRemRect = QRectF(0,0,0,0);
}

SelectionItem::~SelectionItem()
{
    delete d;
}

void SelectionItem::saveZoom(qreal zoom)
{
    if (zoom < 0.00001) zoom = 0.00001;
    d->invZoom = 1/zoom;

    d->selMargin = selMargin * d->invZoom;

    qreal margin = addRemMargin * d->invZoom;
    QPointF pMargin = addRemMarginPoint * d->invZoom;
    d->addRemRect = QRectF(d->rect.center()-pMargin, QSizeF(margin*2.0, margin*2.0));
    d->penAddRemFg.setWidthF(3.0 * d->invZoom);
}

void SelectionItem::setSaved(bool isSaved)
{
    if (isSaved) {
        d->penDark.setColor(Qt::darkBlue);
        d->penLight.setColor(Qt::red);
        d->penAddRemFg.setColor(Qt::darkRed);
        d->isSaved = true;
    }
    else {
        d->penDark.setColor(Qt::black);
        d->penLight.setColor(Qt::white);
        d->penAddRemFg.setColor(Qt::darkGreen);
        d->isSaved = false;
    }
}

void SelectionItem::setMaxRight(qreal maxX)
{
    d->maxX = maxX;
    d->hasMaxX = true;
    if (d->hasMaxY) d->hasMax = true;
}

void SelectionItem::setMaxBottom(qreal maxY)
{
    d->maxY = maxY;
    d->hasMaxY = true;
    if (d->hasMaxX) d->hasMax = true;
}

SelectionItem::Intersects SelectionItem::intersects(QPointF point)
{
    bool oldState = d->showAddRem;
    d->showAddRem = false;
    
    if ((point.x() < (d->rect.left()-d->selMargin)) ||
        (point.x() > (d->rect.right()+d->selMargin)) ||
        (point.y() < (d->rect.top()-d->selMargin)) ||
        (point.y() > (d->rect.bottom()+d->selMargin)))
    {
        if (oldState != d->showAddRem) update();
        return None;
    }

    if (point.x() < (d->rect.left()+d->selMargin)) {
        if (oldState != d->showAddRem) update();
        if (point.y() < (d->rect.top()+d->selMargin))    return TopLeft;
        if (point.y() > (d->rect.bottom()-d->selMargin)) return BottomLeft;
        return Left;
    }

    if (point.x() > (d->rect.right()-d->selMargin)) {
        if (oldState != d->showAddRem) update();
        if (point.y() < (d->rect.top()+d->selMargin))    return TopRight;
        if (point.y() > (d->rect.bottom()-d->selMargin)) return BottomRight;
        return Right;
    }

    if (point.y() < (d->rect.top()+d->selMargin)) {
        if (oldState != d->showAddRem) update();
        return Top;
    }
    if (point.y() > (d->rect.bottom()-d->selMargin)) {
        if (oldState != d->showAddRem) update();
        return Bottom;
    }
    
    d->showAddRem = true;
    if (oldState != d->showAddRem) update();

    if ((point.x() > d->addRemRect.left()) &&
        (point.x() < d->addRemRect.right()) &&
        (point.y() > d->addRemRect.top()) &&
        (point.y() < d->addRemRect.bottom()))
    {
        return AddRemove;
    }
    return Move;
}

void SelectionItem::setRect(QRectF rect)
{
    prepareGeometryChange();
    d->rect = rect;
    d->rect = d->rect.normalized();
    if (d->hasMax) {
        if (d->rect.top() < 0) d->rect.setTop(0);
        if (d->rect.left() < 0) d->rect.setLeft(0);
        if (d->rect.right() > d->maxX) d->rect.setRight(d->maxX);
        if (d->rect.bottom() > d->maxY) d->rect.setBottom(d->maxY);
    }

    // calculate the add/remove rectangle
    qreal margin = addRemMargin * d->invZoom;
    QPointF pMargin = addRemMarginPoint * d->invZoom;
    d->addRemRect = QRectF(d->rect.center()-pMargin, QSizeF(margin*2, margin*2));
}

QPointF SelectionItem::fixTranslation(QPointF dp)
{
    if ((d->rect.left()   + dp.x()) < 0) dp.setX(-d->rect.left());
    if ((d->rect.top()    + dp.y()) < 0) dp.setY(-d->rect.top());
    if ((d->rect.right()  + dp.x()) > d->maxX) dp.setX(d->maxX - d->rect.right());
    if ((d->rect.bottom() + dp.y()) > d->maxY) dp.setY(d->maxY - d->rect.bottom());
    return dp;
}

QRectF SelectionItem::rect()
{
    return d->rect;
}

QRectF SelectionItem::boundingRect() const
{
    QRectF tmp(d->rect.topLeft()-boundMargin, d->rect.bottomRight()+boundMargin);
    if (tmp.top() > d->addRemRect.top()) {
        tmp.setTop(d->addRemRect.top());
    }
    if (tmp.left() > d->addRemRect.left()) {
        tmp.setLeft(d->addRemRect.left());
    }
    
    if (tmp.bottom() < d->addRemRect.bottom()) {
        tmp.setBottom(d->addRemRect.bottom());
    }
    
    if (tmp.right() < d->addRemRect.right()) {
        tmp.setRight(d->addRemRect.right());
    }
    
    return tmp;
}

void SelectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(d->penDark);
    painter->drawRect(d->rect);

    painter->setPen(d->penLight);
    painter->drawRect(d->rect);

    if (d->showAddRem) {
        painter->fillRect(d->addRemRect, QBrush(Qt::white));
        QLineF minus(d->addRemRect.left()+3*d->invZoom, d->rect.center().y(),
                      d->addRemRect.right()-3*d->invZoom, d->rect.center().y());
        painter->setPen(d->penAddRemFg);

        painter->drawLine(minus);

        if (!d->isSaved) {
            QLineF plus(d->rect.center().x(), d->addRemRect.top()+3*d->invZoom,
                         d->rect.center().x(), d->addRemRect.bottom()-3*d->invZoom);
            painter->drawLine(plus);
        }
    }
}

}  // NameSpace KSaneIface
