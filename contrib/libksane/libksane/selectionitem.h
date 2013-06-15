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
#ifndef SELECTIONITEM_H
#define SELECTIONITEM_H

#include <QGraphicsPixmapItem>
#include <QPainter>

namespace KSaneIface
{

class SelectionItem : public QGraphicsItem
{
    public:
        typedef enum
        {
            None,
            Top,
            TopRight,
            Right,
            BottomRight,
            Bottom,
            BottomLeft,
            Left,
            TopLeft,
            Move,
            AddRemove
        } Intersects;
        
        explicit SelectionItem(QRectF rect);
        ~SelectionItem();

        void setMaxRight(qreal maxRight);
        void setMaxBottom(qreal maxBottom);
        void setSaved(bool isSaved);

        Intersects intersects(QPointF point);

        void saveZoom(qreal zoom);

        void setRect(QRectF rect);
        QPointF fixTranslation(QPointF dp);
        QRectF rect();

    public:
        // Graphics Item methods
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:
        struct Private;
        Private * const d;
};

}  // NameSpace KSaneIface

#endif

