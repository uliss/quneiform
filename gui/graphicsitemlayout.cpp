/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <QGraphicsItem>

#include "graphicsitemlayout.h"

GraphicsItemLayout::GraphicsItemLayout()
{
}

void GraphicsItemLayout::addItem(QGraphicsItem * item)
{
    items_.append(item);
    update();
}

void GraphicsItemLayout::clear()
{
    items_.clear();
}

void GraphicsItemLayout::removeItem(QGraphicsItem * item)
{
    items_.removeAll(item);
    update();
}

void GraphicsItemLayout::update()
{
    if(items_.isEmpty())
        return;

    QPointF pos = items_.first()->pos();
    foreach(QGraphicsItem * item, items_) {
        if(!item->isVisible())
            continue;

        item->setPos(pos);
        pos.rx() += item->boundingRect().width();
    }
}
