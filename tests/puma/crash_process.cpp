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

#include <string>
#include <cstdlib>

#ifndef _WIN32
#include <signal.h>
#include <sys/wait.h>
#endif

#include "puma/process_exit_codes.h"

using namespace cf;

int main(int argc, char * argv[])
{
    if(argc != 2)
        return WORKER_WRONG_ARGUMENT;

    const std::string signal(argv[1]);
    
#ifndef _WIN32
    if(signal == "segv")
        raise(SIGSEGV);
    else if(signal == "term")
        raise(SIGTERM);
    else if(signal == "ok")
        return 0;
    else
        return WORKER_UNKNOWN_ERROR;
#else
    if(signal == "segv") {
        int * ptr = NULL;
        *ptr = 0x123;
    }
    else if(signal == "term")
        abort();
    else if(signal == "ok")
        return 0;
    else
        return WORKER_UNKNOWN_ERROR;
#endif
}

