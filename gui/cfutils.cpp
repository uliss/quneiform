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

#include "cfutils.h"

QRect toQRect(const cf::Rect& r)
{
    return QRect(r.left(), r.top() - 1, r.width(), r.height());
}

cf::Rect toRect(const QRect& r)
{
    return cf::Rect(r.left(), r.top(), r.width(), r.height());
}

QColor toQColor(const cf::Color& c)
{
   return QColor(c.red(), c.green(), c.blue());
}
