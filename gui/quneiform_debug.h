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

#ifndef QUNEIFORM_DEBUG_H
#define QUNEIFORM_DEBUG_H

#include <QDebug>

#define CF_ERROR(msg) { \
    qDebug() << "[Error]" << Q_FUNC_INFO << "=>" << msg;\
}

#define CF_WARNING(msg) { \
    qDebug() << "[Warning]" << Q_FUNC_INFO << "=>" << msg;\
}

#define CF_INFO(msg) {\
    qDebug() << "[Info]" << Q_FUNC_INFO << "=>" << msg;\
}

#endif // QUNEIFORM_DEBUG_H
