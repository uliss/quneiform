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

#include <sys/time.h>
#include <signal.h>
#include <iostream>
#include <cstdio> // for perror
#include <boost/current_function.hpp>

#include "processtimeoutkiller.h"
#include "common/singleton.h"
#include "common/debug.h"
#include "common/console_messages.h"

#define CF_INFO cf::Debug() << cf::console::info << BOOST_CURRENT_FUNCTION

namespace cf {

template<class T>
struct LocalCreate {
    static T * create() {
        static T obj(0, 0);
        return &obj;
    }
};

typedef Singleton<ProcessTimeoutKiller, LocalCreate> Static;

static void signal_handler(int signum) {
    Static::instance().signalHandler(signum);
}

ProcessTimeoutKiller::ProcessTimeoutKiller(pid_t pid, int sec) :
    pid_(pid), signal_(SIGTERM), timeout_(sec)
{}

void ProcessTimeoutKiller::registerTimer() {
    Static::instance() = *this;
    ::signal(SIGALRM, &signal_handler);
}

void ProcessTimeoutKiller::setupTimer() {
    struct itimerval t;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec = timeout_;
    t.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &t, 0);
    CF_INFO << " timeout set to " << timeout_ << " seconds for pid: " << pid_ << std::endl;
}

void ProcessTimeoutKiller::signalHandler(int signum) {
    CF_INFO << " signal received: " << signum << "\n";
    Debug() << "    Killing pid: " << pid_ << "\n";
    int status = kill(pid_, signal_);
    if(status != 0)
        perror(BOOST_CURRENT_FUNCTION);
}

void ProcessTimeoutKiller::start() {
    if(timeout_ <= 0)
        return;

    setupTimer();
    registerTimer();
}

}
