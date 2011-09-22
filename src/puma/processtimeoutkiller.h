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

#ifndef PROCESSTIMEOUTKILLER_H
#define PROCESSTIMEOUTKILLER_H

#include <unistd.h>

namespace cf {

class ProcessTimeoutKiller
{
public:
    ProcessTimeoutKiller(pid_t pid, int seconds);

    /**
      * Cancels timer
      * @see start()
      */
    void cancel();

    /**
      * Sets sended signal
      */
    void setSignal(int s);

    /**
      * Returns signal sent to process
      */
    int signal() const;

    /**
      * Starts timer
      * @see cancel()
      */
    void start();
public:
    void signalHandler(int signum);
private:
    void registerTimer();
    void setupTimer();
private:
    pid_t pid_;
    int signal_;
    int timeout_;
};
}

#endif // PROCESSTIMEOUTKILLER_H
