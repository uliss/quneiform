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

class SelectionShadow;

class Selection : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Selection(const QRectF& = QRectF());

    enum border_t {
        NONE = 0,
        LEFT = 1,
        RIGHT = 2,
        TOP = 4,
        BOTTOM = 8
    };

    enum corner_t {
        LEFT_TOP = Selection::LEFT | Selection::TOP,
        RIGHT_TOP = Selection::RIGHT | Selection::TOP,
        RIGHT_BOTTOM = Selection::RIGHT | Selection::BOTTOM,
        LEFT_BOTTOM = Selection::LEFT | Selection::BOTTOM
    };

    enum cursor_t {
        NORMAL = 0,
        HORIZONTAL = 1,
        VERTICAL = 2,
        DIAGONAL_LEFT = 3,
        DIAGONAL_RIGHT = 4
    };

    /**
      * Moves selection by given offset
      * @note that if selection moving outside of graphics scene
      * is not allowed
      */
    void moveBy(const QPointF& delta);

    /**
      * Returns normalized selection rectange
      * taking into account pos() and rect() values
      */
    QRect normalRect() const;

    /**
      * Sets selection cursor when it's over selection borders
      * @param cursor - selection cursor
      * @param vertical - true if selection rotated for 90 or 270 degrees
      */
    void setCursorType(cursor_t cursor, bool vertical);
signals:
    /**
      * Emitted if cursor is above selection borders
      */
    void cursorChange(int type);

    /**
      * Emitted when user press arrow keys to move selection
      * @param delta move offset
      */
    void moved(const QPointF& delta);

    /**
      * Emitted when user press Delete key on selection
      */
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
    qreal borderDistance(const QPointF& pt, Selection::border_t border) const;
    int bordersResized(const QPointF& pos) const;
    bool isCloseToBorder(const QPointF& pt, Selection::border_t border) const;
    bool isCloseToCorner(const QPointF& pt, corner_t corner) const;
    bool isResizing() const;
    border_t nearestBorder(const QPointF& pt) const;
    qreal nearestBorderDistance(const QPointF& pt) const;
    corner_t nearestCorner(const QPointF& pt) const;
    void resizeBy(const QPointF& delta);
    QRectF sceneRect() const;
    void setResizeCursor(const QPointF& pos);
private:
    SelectionShadow * shadow_;
    char resize_;
};

#endif // SELECTION_H
