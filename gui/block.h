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

#ifndef BLOCK_H
#define BLOCK_H

#include <QRect>

#include "blocktype.h"

class QDataStream;

class Block
{
public:
    Block();
    Block(BlockType type, const QRect& rect);

    bool isEditable() const;
    bool isUser() const;
    void setUser(bool value);

    int number() const;
    void setNumber(int num);

    QRect rect() const;
    void setRect(const QRect& r);

    BlockType type() const;
    void setType(BlockType type);
public:
    friend QDataStream& operator<<(QDataStream& stream, const Block& block);
    friend QDataStream& operator>>(QDataStream& stream, Block& block);
private:
    QRect rect_;
    BlockType type_;
    qint16 number_;
    quint16 is_user_;
};

QDataStream& operator<<(QDataStream& stream, const Block& block);
QDataStream& operator>>(QDataStream& stream, Block& block);

#endif // BLOCK_H
