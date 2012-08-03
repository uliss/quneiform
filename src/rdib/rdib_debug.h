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

#ifndef RDIB_DEBUG_H
#define RDIB_DEBUG_H

#include <boost/current_function.hpp>

#include "common/helper.h"
#include "common/tostring.h"
#include "common/log.h"

#define RDIB_ERROR_FUNC cfError(cf::MODULE_RDIB) << METHOD_SIGNATURE()
#define RDIB_DEBUG_FUNC cfDebug(cf::MODULE_RDIB) << METHOD_SIGNATURE()
#define RDIB_TRACE_FUNC cfTrace(cf::MODULE_RDIB) << METHOD_SIGNATURE()
#define RDIB_WARNING_FUNC cfWarning(cf::MODULE_RDIB) << METHOD_SIGNATURE()

#endif // RDIB_DEBUG_H
