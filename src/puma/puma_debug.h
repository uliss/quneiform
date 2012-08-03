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

#ifndef PUMA_DEBUG_H
#define PUMA_DEBUG_H

#include <boost/current_function.hpp>

#include "common/log.h"
#include "common/tostring.h"
#include "common/helper.h"

#define PUMA_ERROR_FUNC() cfError(cf::MODULE_PUMA) << METHOD_SIGNATURE()
#define PUMA_DEBUG_FUNC() cfDebug(cf::MODULE_PUMA) << METHOD_SIGNATURE()
#define PUMA_INFO_FUNC()  cfInfo(cf::MODULE_PUMA) << METHOD_SIGNATURE()
#define PUMA_TRACE_FUNC() cfTrace(cf::MODULE_PUMA) << METHOD_SIGNATURE()
#define PUMA_WARNING_FUNC()  cfWarning(cf::MODULE_PUMA) << METHOD_SIGNATURE()

#endif // PUMA_DEBUG_H
