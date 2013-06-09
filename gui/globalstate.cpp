/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include "globalstate.h"

void GlobalState::set(const QString& key, const QVariant& value)
{
    map().insert(key, value);
}

bool GlobalState::contains(const QString& key)
{
    return map().contains(key);
}

QVariant GlobalState::get(const QString& key)
{
    return map().value(key);
}

QMap<QString, QVariant>& GlobalState::map()
{
    static QMap<QString, QVariant> map_;
    return map_;
}

QVariant GlobalState::get(const QString& key, const QVariant& defValue)
{
    return map().value(key, defValue);
}
