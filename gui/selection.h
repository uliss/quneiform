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


#ifndef SELECTION_H
#define SELECTION_H

#include <QGraphicsRectItem>
#include <QObject>

class Selection : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Selection(const QRectF& = QRectF());

    enum resize_t {
        NONE = 0,
        LEFT = 1,
        RIGHT = 2,
        UP = 4,
        DOWN = 8
    };

    enum cursor_t {
        NORMAL = 0,
        HORIZONTAL = 1,
        VERTICAL = 2,
        DIAGONAL_LEFT = 3,
        DIAGONAL_RIGHT = 4
    };

 signals:
    void cursorChange(int type);
    void selectionDeleted();
    void resized();
protected:
    void hoverEnterEvent (QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
    void keyPressEvent(QKeyEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
private:
    bool isValidPoint(const QPointF& pos) const;
    bool isValidRect(const QRectF& rect) const;
    int resizeMode(const QPointF& pos) const;
    void setResizeCursor(const QPointF& pos);
private:
    char resize_;
};

#endif // SELECTION_H
