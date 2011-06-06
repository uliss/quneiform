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

#ifndef CEDSERIALIZER_H
#define CEDSERIALIZER_H

class QDataStream;

namespace cf {
class CEDPage;
}

class CEDSerializer
{
public:
    CEDSerializer(cf::CEDPage * page = 0);
    cf::CEDPage * page() {
        return page_;
    }
public:
    friend QDataStream& operator<<(QDataStream& stream, const CEDSerializer& ced);
    friend QDataStream& operator>>(QDataStream& stream, CEDSerializer& ced);
private:
    cf::CEDPage * page_;
};

QDataStream& operator<<(QDataStream& stream, const CEDSerializer& ced);
QDataStream& operator>>(QDataStream& stream, CEDSerializer& ced);

#endif // CEDSERIALIZER_H
