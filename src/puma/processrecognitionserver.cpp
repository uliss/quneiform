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

#ifdef __OpenBSD__
#include <sys/param.h> // for MAXLOGNAME
#endif

#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "processrecognitionserver.h"
#include "startprocess.h"
#include "shmem_data.h"
#include "sharedimage.h"
#include "sharedimageholder.h"
#include "sharedresult.h"
#include "sharedresultholder.h"
#include "sharedoptions.h"
#include "sharedoptionsholder.h"
#include "common/debug.h"
#include "common/envpaths.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"
#include "common/console_messages.h"
#include "sharedmemoryremover.h"
#include "config.h"

namespace bi = boost::interprocess;

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
{
    CF_INFO("constructed");
}

CEDPagePtr ProcessRecognitionServer::recognize(ImagePtr image,
                                               const RecognizeOptions& ropts,
                                               const FormatOptions& fopts)
{
    using namespace boost::interprocess;

    const std::string SHMEM_KEY = makeKey();
    SharedMemoryRemover remover(SHMEM_KEY);

    try {
        if(!image.get())
            throw RecognitionException("NULL image given");

        if(image->dataSize() == 0 && image->fileName().empty())
            throw RecognitionException("empty image given");

        static const size_t SHMEM_SIZE = sizeof(SharedResult) * 2 + sizeof(SharedImage) + sizeof(SharedOptions);

        //Construct managed shared memory
        managed_shared_memory segment(create_only, SHMEM_KEY.c_str(), SHMEM_SIZE);
        CF_INFO("Shared segment '"<< SHMEM_KEY << "' created with size: " << SHMEM_SIZE << " bytes.");

        // create shared image
        SharedImageHolder sh_image_holder(&segment);
        SharedImage * sh_image = sh_image_holder.find(&segment);

        if(!sh_image)
            throw RecognitionException("Can't open shared image");

        if(image->dataSize() != 0) {
            sh_image->set(image);
        }

        // create shared result
        SharedResultHolder result_holder(&segment);

        // create shared options
        SharedOptionsHolder sh_opt_holder(&segment);
        sh_opt_holder.options()->store(ropts);
        sh_opt_holder.options()->store(fopts);

        bool ok = startWorker(image, SHMEM_KEY);

        if(!ok && state_)
            state_->set(RecognitionState::FAILED);

        CEDPagePtr res = result_holder.page();

        if(!res.get())
            throw RecognitionException("Recognition error");

        return res;
    }
    catch(interprocess_exception& e) {
        if(state_)
            state_->set(RecognitionState::FAILED);

        CF_ERROR(e.what());
        throw RecognitionException(e.what());
    }
    catch(std::exception& e) {
        if(state_)
            state_->set(RecognitionState::FAILED);

        CF_ERROR(e.what());
        throw RecognitionException(e.what());
    }
}

bool ProcessRecognitionServer::processWorkerReturnCode(int code)
{
#define ERROR(msg) CF_ERROR(msg); return false;
    switch(code) {
    case EXIT_SUCCESS:
        return true;
    case WORKER_UNKNOWN_ERROR:
        ERROR("unknown error");
    case WORKER_SEGMENT_NOT_FOUND:
        ERROR("shared memory segment not found");
    case WORKER_SAVE_ERROR:
        ERROR("error while saving into shared memory segment");
    case WORKER_RECOGNITION_ERROR:
        ERROR("recognition error");
    case WORKER_SHMEM_ERROR:
        ERROR("shared memory error");
    case WORKER_TERMINATE_ERROR:
        ERROR("recognition process terminated");
    case WORKER_ABORT_ERROR:
        ERROR("recognition process aborted");
    case WORKER_SEGFAULT_ERROR:
        ERROR("recognition process segmentation fault");
    case WORKER_TIMEOUT_ERROR:
        ERROR("recognition process killed by timeout");
    }

    return true;
}

bool ProcessRecognitionServer::startWorker(ImagePtr image, const std::string& key) {
    if(counter_)
        counter_->reset();

    StringList params;
    params.push_back(key);

    if(!image->fileName().empty())
        params.push_back(image->fileName());

    std::string exe_path = workerPath();
    if(exe_path.empty()) {
        CF_ERROR("can't find process worker");
        return false;
    }

    int code = startProcess(exe_path, params);

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

    return processWorkerReturnCode(code);
}

std::string ProcessRecognitionServer::workerPath() const {
    PathList search_paths;
    search_paths.push_back(".");
    search_paths.push_back(INSTALL_PREFIX "/lib/cuneiform");
    search_paths.push_back(INSTALL_PREFIX "/lib64/cuneiform");
    PathList env_paths = envPaths();
    std::copy(env_paths.begin(), env_paths.end(), std::back_inserter(search_paths));
    StringList file_names;

#ifdef _WIN32
    file_names.push_back("cuneiform-worker.exe");
    file_names.push_back("../lib/cuneiform/cuneiform-worker.exe");
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

}
