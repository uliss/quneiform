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

#include <boost/current_function.hpp>
#include <boost/scoped_array.hpp>

#include <algorithm>
#include <cstdio>
#include <cstring>

#include "common/console_messages.h"
#include "common/debug.h"
#include "common/filesystem.h"
#include "shmem_data.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#include "processtimeoutkiller.h"
#endif

#include "startprocess.h"

#define CF_INFO cf::Debug() << cf::console::info << BOOST_CURRENT_FUNCTION
#define CF_ERROR std::cerr << cf::console::error << BOOST_CURRENT_FUNCTION

namespace cf {

#ifdef _WIN32
int startProcess(const std::string& program, const StringList& params) {
    std::string cmd = program;
    for(size_t i = 0; i < params.size(); i++)
        cmd += " " + params[i];

    Debug() << "Command: " << cmd << std::endl;

    // Set up members of the PROCESS_INFORMATION structure.
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure.
    STARTUPINFO siStartInfo;
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);

    LPSTR str = _strdup(cmd.c_str());

    // Create the child process.
    BOOL bSuccess = CreateProcess(NULL,
                                  str,           // command line
                                  NULL,          // process security attributes
                                  NULL,          // primary thread security attributes
                                  TRUE,          // handles are inherited
                                  0,             // creation flags
                                  NULL,          // use parent's environment
                                  NULL,          // use parent's current directory
                                  &siStartInfo,  // STARTUPINFO pointer
                                  &piProcInfo);  // receives PROCESS_INFORMATION

    free(str);

    // If an error occurs, exit the application.
    if (!bSuccess) {
        Debug() << BOOST_CURRENT_FUNCTION << ": can't start worker\n";
        return WORKER_UNKNOWN_ERROR;
    }
    else {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example.
        static const int TIMEOUT = 10 * 1000;

        DWORD status = WaitForSingleObject(piProcInfo.hProcess, TIMEOUT);

//        CloseHandle(piProcInfo.hProcess);
//        CloseHandle(piProcInfo.hThread);
    }

    return 0;
}
#else
int startProcess(const std::string& program, const StringList& params)
{
    if(!fs::fileExists(program)) {
        CF_ERROR << " program not exists: " << program << "\n";
        return WORKER_UNKNOWN_ERROR;
    }

    pid_t child_pid = fork();

    if(child_pid == -1) {
        perror(BOOST_CURRENT_FUNCTION);
        return WORKER_UNKNOWN_ERROR;
    }

    if(child_pid != 0) {
        CF_INFO << " child process started: '"
                << program << "' with pid = " << child_pid << " and params: ";

        for(size_t i = 0; i < params.size(); i++)
            Debug() << params[i] << ' ';

        Debug() << "\n";

        // parent
        static const int TIMEOUT = 10; //seconds
        ProcessTimeoutKiller watchdog(child_pid, TIMEOUT);
        watchdog.start();

        int status;
        int res = waitpid(child_pid, &status, 0);
        if(res == -1)
            perror(BOOST_CURRENT_FUNCTION);

        return WEXITSTATUS(status);
    }
    else {
        // child
        size_t argc = params.size() + 2;
        char ** argv = new char*[argc];
        argv[0] = (char*) program.c_str();

        for(size_t i = 0; i < params.size(); i++) {
            argv[i+1] = (char*) params[i].c_str();
        }

        argv[argc - 1] = NULL;

        int status = execv(program.c_str(), argv);
        if(status != 0) {
            perror(BOOST_CURRENT_FUNCTION);
            return WORKER_UNKNOWN_ERROR;
        }
        return 0;
    }
}
#endif

}

