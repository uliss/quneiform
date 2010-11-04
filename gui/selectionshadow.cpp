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
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPainter>

#include "selectionshadow.h"
#include "selection.h"

SelectionShadow::SelectionShadow(Selection * parent) :
    QGraphicsItem(parent)
{
    setFlags(0);
}

QRectF SelectionShadow::boundingRect() const {
    if(scene())
        return scene()->sceneRect();
    else
        return QRectF();
}

void SelectionShadow::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/) {
    Q_CHECK_PTR(scene());

    QPainterPath p;
    p.addRect(scene()->sceneRect());
    p.addRect(static_cast<Selection*>(parentItem())->rect());

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 30));
    painter->drawPath(p);
    painter->restore();
}
