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

#ifndef SCAN_DEBUG_H
#define SCAN_DEBUG_H

#include <boost/current_function.hpp>

#include "common/debug.h"

#define SCANNER_ERROR cf::Debug() << "[Error][Scan] " << BOOST_CURRENT_FUNCTION << " "
#define SCANNER_WARNING cf::Debug() << "[Warning][Scan] " << BOOST_CURRENT_FUNCTION << " "
#define SCANNER_DEBUG cf::Debug() << "[Debug][Scan] " << BOOST_CURRENT_FUNCTION << " "

#endif // SCAN_DEBUG_H
