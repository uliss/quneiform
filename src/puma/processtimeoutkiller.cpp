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
#include "puma_debug.h"

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

void ProcessTimeoutKiller::cancel() {
    pid_ = -1;
    Static::instance().pid_ = -1;
}

void ProcessTimeoutKiller::registerTimer() {
    Static::instance() = *this;
    ::signal(SIGALRM, &signal_handler);
}

void ProcessTimeoutKiller::setSignal(int s) {
    signal_ = s;
}

void ProcessTimeoutKiller::setupTimer() {
    struct itimerval t;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 0;
    t.it_value.tv_sec = timeout_;
    t.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &t, 0);
    PUMA_INFO_FUNC() << "timeout set to" << timeout_
                     << "seconds for pid:" << pid_;
}

void ProcessTimeoutKiller::signalHandler(int signum) {
    if(pid_ <= 0)
        return;

    PUMA_INFO_FUNC() << "signal received:" << signum;
    PUMA_TRACE_FUNC() << "    Killing pid:" << pid_;

    int status = kill(pid_, signal_);
    if(status != 0)
        perror(BOOST_CURRENT_FUNCTION);
}

int ProcessTimeoutKiller::signal() const {
    return signal_;
}

void ProcessTimeoutKiller::start() {
    if(timeout_ <= 0)
        return;

    setupTimer();
    registerTimer();
}

}
