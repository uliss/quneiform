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

#ifndef LAYOUTBLOCK_H
#define LAYOUTBLOCK_H

#include <vector>
#include <iosfwd>

#include "globus.h"
#include "common/rect.h"

namespace cf
{

class CLA_EXPO LayoutBlock
{
public:
    enum Type {
        INVALID,
        TEXT,
        IMAGE,
        TABLE
    };
public:
    LayoutBlock(const Rect& r, Type t = INVALID);

    /**
     * Returns true if image block
     * @see type()
     */
    bool isImage() const;

    /**
     * Returns true if text block
     * @see type()
     */
    bool isText() const;

    /**
     * Returns block bounding rect
     * @see setRect()
     */
    Rect rect() const;

    /**
     * Sets block bounding rect
     * @see rect()
     */
    void setRect(const Rect& r);

    /**
     * Returns block type
     * @see setType()
     */
    Type type() const;

    /**
     * Sets block type
     * @see type()
     */
    void setType(Type t);
private:
    Rect rect_;
    Type type_;
};

typedef std::vector<LayoutBlock> LayoutBlockList;

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const LayoutBlock& block);

}

#endif // LAYOUTBLOCK_H
