/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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
#include <QBrush>
#include <QPainter>
#include <QPalette>

#include "graphicsroundedrectitem.h"

GraphicsRoundedRectItem::GraphicsRoundedRectItem(QGraphicsItem * parent) :
    QGraphicsRectItem(parent),
    border_radius_(0)
{
    setPen(QPen(QBrush(), 0));
}

qreal GraphicsRoundedRectItem::borderRadius() const
{
    return border_radius_;
}

void GraphicsRoundedRectItem::setBorderRadius(qreal r)
{
    border_radius_ = r;
}

void GraphicsRoundedRectItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRoundedRect(boundingRect().adjusted(1, 1, -1, -1), border_radius_, border_radius_);
    painter->restore();
}
