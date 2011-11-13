/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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


#include <cstring>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h> // for GetCurrentProcessId()
#endif

#include "processrecognitionserver.h"
#include "startprocess.h"
#include "shmem_data.h"
#include "shmem/sharedmemoryholder.h"
#include "shmem/memorydata.h"
#include "common/debug.h"
#include "common/envpaths.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"
#include "common/console_messages.h"
#include "config.h"

static const std::string makeKey() {
    std::ostringstream buf;
    static int count = 0;
    buf << "cfshmem";

#ifdef _WIN32
    buf << GetCurrentProcessId();
#else
    buf << getpid();
#endif

    buf << ++count;
    return buf.str();
}

#define CF_ERROR(msg) std::cerr << cf::console::error << \
    BOOST_CURRENT_FUNCTION << ": " << msg << std::endl;

#define CF_INFO(msg) std::cerr << cf::console::info << \
    BOOST_CURRENT_FUNCTION << ": " << msg << std::endl;

namespace cf {

ProcessRecognitionServer::ProcessRecognitionServer()
    : worker_timeout_(20)
{
    CF_INFO("constructed");
}

CEDPagePtr ProcessRecognitionServer::recognize(const std::string& imagePath,
                                               const RecognizeOptions& ropts,
                                               const FormatOptions& fopts)
{
    const std::string SHMEM_KEY = makeKey();

    try {
        if(imagePath.empty())
            throw RecognitionException("empty image path given");

        const size_t SHMEM_SIZE = MemoryData::minBufferSize();
        SharedMemoryHolder memory;
        memory.create(SHMEM_KEY, SHMEM_SIZE);
        MemoryData data(memory.get(), SHMEM_SIZE);
        data.setFormatOptions(fopts);
        data.setRecognizeOptions(ropts);
        data.setImagePath(imagePath);

        startWorker(SHMEM_KEY);

        CEDPagePtr res = data.page();

        if(!res.get())
            throw RecognitionException("Recognition error");

        return res;
    }
    catch(std::exception& e) {
        if(state_)
            state_->set(RecognitionState::FAILED);

        CF_ERROR(e.what());
        throw RecognitionException(e.what());
    }
}

CEDPagePtr ProcessRecognitionServer::recognize(ImagePtr image,
                                               const RecognizeOptions& ropts,
                                               const FormatOptions& fopts)
{
    const std::string SHMEM_KEY = makeKey();

    try {
        if(!image.get())
            throw RecognitionException("NULL image given");

        if(image->dataSize() == 0)
            throw RecognitionException("empty image given");

        const size_t SHMEM_SIZE = MemoryData::minBufferSize();
        SharedMemoryHolder memory;
        memory.create(SHMEM_KEY, SHMEM_SIZE);
        MemoryData data(memory.get(), SHMEM_SIZE);
        data.setFormatOptions(fopts);
        data.setRecognizeOptions(ropts);
        data.setImage(image);
        startWorker(SHMEM_KEY);

        CEDPagePtr res = data.page();

        if(!res.get())
            throw RecognitionException("Recognition error");

        return res;
    }
    catch(std::exception& e) {
        if(state_)
            state_->set(RecognitionState::FAILED);

        CF_ERROR(e.what());
        throw RecognitionException(e.what());
    }
}

void ProcessRecognitionServer::setWorkerTimeout(int sec)
{
    worker_timeout_ = sec;
}

void ProcessRecognitionServer::startWorker(const std::string& key) {
    if(counter_)
        counter_->reset();

    StringList params;
    params.push_back(key);

    std::string exe_path = workerPath();

    if(exe_path.empty())
        throw RecognitionException("can't find process worker");

    int code = startProcess(exe_path, params, worker_timeout_);

    if(counter_) {
        counter_->add(20);
        counter_->add(10);
        counter_->add(70);
    }

    if(state_) {
        state_->set(RecognitionState::LOADED);
        state_->set(RecognitionState::OPENED);
        state_->set(RecognitionState::ANALYZED);
        state_->set(RecognitionState::RECOGNIZED);
        state_->set(RecognitionState::FORMATTED);
    }

    switch(code) {
    case EXIT_SUCCESS:
        return;
    case WORKER_UNKNOWN_ERROR:
        throw RecognitionException("unknown error");
    case WORKER_SEGMENT_NOT_FOUND:
        throw RecognitionException("shared memory segment not found");
    case WORKER_SAVE_ERROR:
        throw RecognitionException("error while saving into shared memory segment");
    case WORKER_RECOGNITION_ERROR:
        throw RecognitionException("recognition error");
    case WORKER_SHMEM_ERROR:
        throw RecognitionException("shared memory error");
    case WORKER_TERMINATE_ERROR:
        throw RecognitionException("recognition process terminated");
    case WORKER_ABORT_ERROR:
        throw RecognitionException("recognition process aborted");
    case WORKER_SEGFAULT_ERROR:
        throw RecognitionException("recognition process segmentation fault");
    case WORKER_TIMEOUT_ERROR:
        throw RecognitionException("recognition process killed by timeout");
    default:
        return;
    }
}

std::string ProcessRecognitionServer::workerPath() const {
    PathList search_paths;
    search_paths.push_back(".");

#ifdef _WIN32
    char szAppPath[MAX_PATH]      = "";
    char szAppDirectory[MAX_PATH] = "";

    ::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1);
    strncpy(szAppDirectory, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
    szAppDirectory[strlen(szAppDirectory)] = '\0';

    search_paths.push_back(szAppDirectory);
#else
    search_paths.push_back(INSTALL_PREFIX "/lib/cuneiform");
    search_paths.push_back(INSTALL_PREFIX "/lib64/cuneiform");
#endif

    PathList env_paths = envPaths();
    std::copy(env_paths.begin(), env_paths.end(), std::back_inserter(search_paths));
    StringList file_names;

#ifdef _WIN32
    file_names.push_back("new-cuneiform-worker.exe");
    // for testing purposes
    file_names.push_back("../../new-cuneiform-worker.exe");
#else
    file_names.push_back("new-cuneiform-worker");
    file_names.push_back("../lib/cuneiform/new-cuneiform-worker");
    file_names.push_back("../lib64/cuneiform/new-cuneiform-worker");
    // for testing purposes
    file_names.push_back("../../new-cuneiform-worker");
#endif

    std::string result_path;
    if(!findInPaths(file_names, search_paths, &result_path)) {
        typedef std::ostream_iterator<StringList::value_type> str_it;
        std::cerr << BOOST_CURRENT_FUNCTION << " files: '";
        std::copy(file_names.begin(), file_names.end(), str_it(std::cerr, " "));
        std::cerr << "' not found in following paths:\n";
        std::copy(search_paths.begin(), search_paths.end(), str_it(std::cerr, "\n"));

        return std::string();
    }
    else {
        return result_path;
    }
}

int ProcessRecognitionServer::workerTimeout() const {
    return worker_timeout_;
}

}
