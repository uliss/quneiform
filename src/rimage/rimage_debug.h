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

#ifndef RIMAGE_DEBUG_H
#define RIMAGE_DEBUG_H

#include <boost/current_function.hpp>

#include "common/log.h"
#include "common/helper.h"

#define RIMAGE_ERROR cfError(cf::MODULE_RIMAGE) << METHOD_SIGNATURE()
#define RIMAGE_TRACE_FUNC() cfTrace(cf::MODULE_RIMAGE) << METHOD_SIGNATURE()
#define RIMAGE_DEBUG_FUNC() cfDebug(cf::MODULE_RIMAGE) << METHOD_SIGNATURE()

#endif // RIMAGE_DEBUG_H
