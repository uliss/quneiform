/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef PAGEAREA_H
#define PAGEAREA_H

#include <QGraphicsRectItem>
#include <QObject>

#include "blocktype.h"

class Page;
class SelectionList;

class PageArea : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    PageArea();
    ~PageArea();

    void clear();
    void show(Page * page);
    QRect showChar(const QRect& bbox);

    bool isBlockVisible(BlockType t) const;
    void hideBlocks(BlockType t);
    void showBlocks(BlockType t);
    void startImageBlockSelection();
    void startPageAreaSelection();
    void startTextBlockSelection();
public slots:
    void saveSelections();
private:
    void clearCurrentChar();
    void updateLayout();
    QRect mapFromPage(const QRect& r) const;
private:
    Page * page_;
    QGraphicsRectItem * current_char_bbox_;
    SelectionList * selections_;
};

#endif // PAGEAREA_H
