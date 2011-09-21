/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef STARTPROCESS_H
#define STARTPROCESS_H

#include <vector>
#include <string>

namespace cf {

typedef std::vector<std::string> StringList;

/**
  * Starts program in different process
  * @param program - program path
  * @param params - list of arguments
  * @param timeout - after what time (in seconds) started programm will be killed, if hangs
  *                  pass 0 to wait forever
  * @return exit code - @see WorkerExitValues
  */
int startProcess(const std::string& program, const StringList& params, int timeout = 20);

}

#endif // STARTPROCESS_H
