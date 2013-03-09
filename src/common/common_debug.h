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

#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#include <boost/current_function.hpp>

#include "tostring.h"
#include "log.h"
#include "helper.h"

#define COMMON_ERROR_FUNC cfError(MODULE_COMMON) << METHOD_SIGNATURE()
#define COMMON_DEBUG_FUNC cfDebug(MODULE_COMMON) << METHOD_SIGNATURE()
#define COMMON_TRACE_FUNC cfTrace(MODULE_COMMON) << METHOD_SIGNATURE()
#define COMMON_WARNING_FUNC cfWarning(MODULE_COMMON) << METHOD_SIGNATURE()

#endif // COMMON_DEBUG_H
