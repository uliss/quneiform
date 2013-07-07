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
#include "process_exit_codes.h"
#include "shmem/sharedmemoryholder.h"
#include "shmem/memorydata.h"
#include "common/envpaths.h"
#include "common/helper.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"
#include "common/console_messages.h"
#include "common/imageurl.h"
#include "puma_debug.h"
#include "config-paths.h" // for INSTALL_PREFIX

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

namespace cf {

ProcessRecognitionServer::ProcessRecognitionServer()
    : worker_timeout_(20)
{
    PUMA_TRACE_FUNC();
}

ProcessRecognitionServer::~ProcessRecognitionServer()
{
    PUMA_TRACE_FUNC();
}

void ProcessRecognitionServer::addImageBlock(const Rect& r)
{
    PUMA_WARNING_FUNC() << r << "not implemeneted";
}

void ProcessRecognitionServer::addTableBlock(const Rect& r)
{
    PUMA_WARNING_FUNC() << r << "not implemeneted";
}

void ProcessRecognitionServer::addTextBlock(const Rect& r)
{
    PUMA_WARNING_FUNC() << r << "not implemeneted";
}

bool ProcessRecognitionServer::binarize()
{
    stateSet(RecognitionState::BINARIZED);
    counterAdd(10);
    return true;
}

CEDPagePtr ProcessRecognitionServer::format()
{
    stateSet(RecognitionState::FORMATTED);
    counterAdd(10);
    return page_;
}

bool ProcessRecognitionServer::manualLayout()
{
    return false;
}

bool ProcessRecognitionServer::analyze()
{
    stateSet(RecognitionState::ANALYZED);
    counterAdd(10);
    return true;
}

bool ProcessRecognitionServer::open(const ImageURL& url)
{
    counterReset();

    if(url.empty()) {
        PUMA_ERROR_FUNC() << "empty image url";
        setFailedState();
        return false;
    }

    if(!url.exists()) {
        PUMA_ERROR_FUNC() << "can't open file" << url;
        setFailedState();
        return false;
    }

    url_ = url;
    src_ = SOURCE_URL;
    stateSet(RecognitionState::OPENED);
    counterAdd(10);
    return true;
}

bool ProcessRecognitionServer::open(ImagePtr img)
{
    counterReset();

    if(!img) {
        PUMA_ERROR_FUNC() << "null image pointer";
        setFailedState();
        return false;
    }

    if(img->dataSize() == 0) {
        PUMA_ERROR_FUNC() << "empty image pointer";
        setFailedState();
        return false;
    }

    image_ = img;
    src_ = SOURCE_IMAGE;
    stateSet(RecognitionState::OPENED);
    counterAdd(10);
    return true;
}

bool ProcessRecognitionServer::recognize()
{
    key_ = makeKey();

    try {
        size_t SHMEM_SIZE = MemoryData::minBufferSize();
        if(src_ == SOURCE_IMAGE)
            SHMEM_SIZE += image_->dataSize();

        SharedMemoryHolder memory(true);
        memory.create(key_, SHMEM_SIZE);

        data_.setMemory(memory.get());
        data_.setSize(SHMEM_SIZE);
        data_.setFormatOptions(formatOptions());
        data_.setRecognizeOptions(recognizeOptions());
        data_.setBinarizeOptions(binarizeOptions());

        bool rc = true;
        switch(src_) {
        case SOURCE_IMAGE:
            rc = recognizeImagePtr();
            break;
        case SOURCE_URL:
            rc = recognizeUrl();
            break;
        default:
            PUMA_ERROR_FUNC() << "unknown source type:" << src_;
            return false;
        }

        if(!rc) {
            PUMA_ERROR_FUNC() << "failed";
            setFailedState();
            return false;
        }

        stateSet(RecognitionState::RECOGNIZED);
        counterAdd(60);

        page_ = data_.page();
        if(!page_) {
            PUMA_ERROR_FUNC() << "format failed";
            return false;
        }

        data_.reset();

        return true;
    }
    catch(SharedMemoryHolder::LowSharedMemoryException& e) {
        handleMemoryLimits(e);
        throw e;
    }
    catch(std::exception& e) {
        handleOtherErrors(e);
        throw e;
    }
}

void ProcessRecognitionServer::setWorkerTimeout(int sec)
{
    worker_timeout_ = sec;
}

void ProcessRecognitionServer::startWorker(const std::string& key, size_t size)
{
    StringList params;
    params.push_back(key);

    if(size)
        params.push_back(toString(size));

    std::string exe_path = workerPath();

    if(exe_path.empty())
        throw RecognitionException("can't find process worker", WORKER_NOT_FOUND);

    int code = startProcess(exe_path, params, worker_timeout_);

    handleWorkerExitCode(code);
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
    file_names.push_back("cuneiform-worker.exe");
    // for testing purposes
    file_names.push_back("../../cuneiform-worker.exe");
#else
    file_names.push_back("cuneiform-worker");
    file_names.push_back("../lib/cuneiform/cuneiform-worker");
    file_names.push_back("../lib64/cuneiform/cuneiform-worker");
    // for testing purposes
    file_names.push_back("../../cuneiform-worker");
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

void ProcessRecognitionServer::handleMemoryLimits(std::exception& e)
{
    setFailedState();

    typedef SharedMemoryHolder::LowSharedMemoryException Exception;
    Exception& excpt = dynamic_cast<Exception&>(e);

    std::ostringstream buf;
    buf << excpt.what() << ".\n\n";
    buf << "Change your system shared memory limits.\n";
    buf << "Current system shared limit is: " << excpt.current() / 1024 << " kbytes.\n";
    buf << "But requested memory size was: " << excpt.required() / 1024 << " kbytes.\n";

    PUMA_ERROR_FUNC() << buf.str();

    throw RecognitionException(buf.str());
}

void ProcessRecognitionServer::handleOtherErrors(std::exception &e)
{
    setFailedState();
    PUMA_ERROR_FUNC() << e.what();
    throw RecognitionException(e.what());
}

void ProcessRecognitionServer::handleWorkerExitCode(int code)
{
    switch(code) {
    case EXIT_SUCCESS:
        return;
    case WORKER_UNKNOWN_ERROR:
        throw RecognitionException("unknown error");
    case WORKER_SEGMENT_NOT_FOUND:
        throw RecognitionException("shared memory segment not found", SHMEM_ERROR);
    case WORKER_SAVE_ERROR:
        throw RecognitionException("error while saving into shared memory segment", SHMEM_ERROR);
    case WORKER_RECOGNITION_ERROR:
        throw RecognitionException("recognition error", RECOGNITION_ERROR);
    case WORKER_SHMEM_ERROR:
        throw RecognitionException("shared memory error", SHMEM_ERROR);
    case WORKER_TERMINATE_ERROR:
        throw RecognitionException("recognition process terminated", WORKER_CRASH);
    case WORKER_ABORT_ERROR:
        throw RecognitionException("recognition process aborted", WORKER_CRASH);
    case WORKER_SEGFAULT_ERROR:
        throw RecognitionException("recognition process segmentation fault", WORKER_CRASH);
    case WORKER_TIMEOUT_ERROR:
        throw RecognitionException("recognition process killed by timeout", WORKER_TIMEOUT);
    default:
        throw RecognitionException("unknown worker return code");
    }
}

bool ProcessRecognitionServer::recognizeImagePtr()
{
    data_.setImage(image_);

    try {
        startWorker(key_, data_.size());
    }
    catch(RecognitionException& e) {
        if(!data_.message().empty()) {
            setFailedState();
            PUMA_ERROR_FUNC() << data_.message();
            throw RecognitionException(data_.message());
        }

        handleOtherErrors(e);
        throw e;
    }

    return true;
}

void ProcessRecognitionServer::setFailedState()
{
    stateSet(RecognitionState::FAILED);
}

bool ProcessRecognitionServer::recognizeUrl()
{
    data_.setImageURL(url_);

    try {
        startWorker(key_, 0);
    }
    catch(RecognitionException& e) {
        if(!data_.message().empty()) {
            setFailedState();
            PUMA_ERROR_FUNC() << data_.message();
            throw RecognitionException(data_.message());
        }

        handleOtherErrors(e);
        throw e;
    }

    return true;
}

}
