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

#ifndef SELECTIONLIST_H
#define SELECTIONLIST_H

#include <QGraphicsRectItem>
#include <QObject>

#include "block.h"

class Selection;
class Page;

class SelectionList : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum selection_t {
        SELECT_NONE = 0,
        SELECT_AREA,
        SELECT_TEXT,
        SELECT_IMAGE,
        SELECT_TABLE
    };

    enum selection_mode_t {
        MODE_NONE = 0,
        MODE_REPLACE,
        MODE_ADD
    };

public:
    SelectionList(QGraphicsItem * parent = NULL);
    void addSelection(const QRectF &r);
    void clearSelections();
    bool isTurned() const;
    void set(selection_t type, selection_mode_t mode);
    void populateFromPage(const Page * p);
    QRect selectionBoundingRect() const;

    bool isBlocksVisible(BlockType t) const;
    void hideBlocks(BlockType t);
    void showBlocks(BlockType t);

    QList<Block> imageBlocks() const;
    QList<Block> textBlocks() const;
    QList<QRect> readAreas() const;
signals:
    void changed();
protected:
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
private:
    friend class Selection;
private:
    void addLayoutBlock(const Block& block);
    void handleSelectionDelete(Selection * s);

    Selection * makeSelection(const QRectF &r);
    Selection * selectionAt(int i);
    void setSelection(const QRectF& r);
    void updateSelections();

    void finishSelection(QGraphicsSceneMouseEvent * event);
    void resizeSelection(QGraphicsSceneMouseEvent * event);
    void startSelection(QGraphicsSceneMouseEvent * event);

    void addRubberBand();
    void removeRubberBand();

    void populateChars();
    void populateLines();
    void populateParagraphs();
    void populateColumns();
    void populateSections();
    void populatePictures();
private:
    QPointF selection_start_;
    QList<Selection*> selections_;
    QGraphicsRectItem * rubber_band_;
    selection_t type_;
    selection_mode_t mode_;
    const Page * page_;
};

#endif // SELECTIONLIST_H
