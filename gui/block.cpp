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

#include <QDataStream>

#include "block.h"

Block::Block() :
    type_(BLOCK_CHAR),
    number_(0),
    is_user_(0)
{
}

Block::Block(BlockType type, const QRect& rect) :
    rect_(rect),
    type_(type),
    number_(0),
    is_user_(0)
{
}

bool Block::isEditable() const
{
    switch(type_) {
    case BLOCK_CHAR:
    case BLOCK_LINE:
    case BLOCK_PARAGRAPH:
    case BLOCK_COLUMN:
    case BLOCK_SECTION:
    case BLOCK_PICTURE:
        return false;
    default:
        return true;
    }
}

bool Block::isUser() const
{
    return is_user_ == 1;
}

void Block::setUser(bool value)
{
    is_user_ = value ? 1 : 0;
}

int Block::number() const
{
    return number_;
}

void Block::setNumber(int num)
{
    number_ = static_cast<qint16>(num);
}

QRect Block::rect() const
{
    return rect_;
}

void Block::setRect(const QRect& r)
{
    rect_ = r;
}

BlockType Block::type() const
{
    return type_;
}

void Block::setType(BlockType type)
{
    type_ = type;
}

QDataStream& operator<<(QDataStream& stream, const Block& block)
{
    stream << block.rect_
           << block.type_
           << block.number_;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, Block& block)
{
    stream >> block.rect_;
    int type;
    stream >> type;
    block.type_ = static_cast<BlockType>(type);
    stream >> block.number_;
    return stream;
}
