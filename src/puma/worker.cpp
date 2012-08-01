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

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <string>
#include <exception>
#include <iostream>
#include <csignal>

#include "localrecognitionserver.h"
#include "process_exit_codes.h"
#include "common/console_messages.h"
#include "common/log.h"
#include "shmem/memorydata.h"
#include "shmem/sharedmemoryholder.h"
#include "puma/pumaimpl.h"
#include "build_number.h"

#ifndef NDEBUG
#define WORKER_PREFIX cf::console::message("[Process worker:" CF_BUILD_NUMBER "] ", cf::console::YELLOW)
#else
#define WORKER_PREFIX cf::console::message("[Process worker] ", cf::console::YELLOW)
#endif

#define CF_ERROR(msg) std::cerr << WORKER_PREFIX << msg << std::endl;

static const int SHMEM_SIZE_MAX = 100 * 1024 * 1024;

static cf::CEDPagePtr recognize(cf::ImagePtr img,
                                const cf::RecognizeOptions& ropts,
                                const cf::FormatOptions& fopts)
{
    cf::LocalRecognitionServer r;
    return r.recognizeImage(img, cf::BinarizeOptions(), ropts, fopts);
}

static cf::CEDPagePtr recognize(const cf::ImageURL& url,
                                const cf::RecognizeOptions& ropts,
                                const cf::FormatOptions& fopts)
{
    cf::LocalRecognitionServer r;
    return r.recognizeImage(url, cf::BinarizeOptions(), ropts, fopts);
}

static void worker_terminate() {
    CF_ERROR("recognition process crashed");
    exit(cf::WORKER_TERMINATE_ERROR);
}

static void signal_callback_handler(int signum) {
    switch(signum) {
    case SIGSEGV:
        cfFatal(cf::MODULE_PUMA) << "recognition process segmentation fault";
        exit(cf::WORKER_SEGFAULT_ERROR);
        break;
    case SIGABRT:
        cfFatal(cf::MODULE_PUMA) << "recognition process aborted";
        exit(cf::WORKER_ABORT_ERROR);
        break;
    case SIGTERM:
        cfFatal(cf::MODULE_PUMA) << "recognition process killed by timeout";
        exit(cf::WORKER_TIMEOUT_ERROR);
        break;
    default:
        exit(cf::WORKER_UNKNOWN_ERROR);
    }
}

int main(int argc, char ** argv)
{
    cf::Logger::config().enableRuntimeConfig(cf::MODULES_ALL);

    if(argc != 2 && argc != 3) {
        CF_ERROR("Usage: " << argv[0] << " KEY [SIZE]");
        return cf::WORKER_WRONG_ARGUMENT;
    }

    const bool use_shared_image = (argc == 3);
    const std::string shmem_key = argv[1];

    int arg_sz = 0;

    // if have size argument
    if(use_shared_image) {
        cfDebug(cf::MODULE_PUMA) << "using shared image";

        arg_sz = atoi(argv[2]);
        // case negative or too big
        if(arg_sz <= 0) {
            CF_ERROR("invalid memory size argument: " << arg_sz);
            arg_sz = 0;
        }

        if(arg_sz > SHMEM_SIZE_MAX) {
            CF_ERROR("memory size is too big: " << arg_sz);
            arg_sz = 0;
        }
    }

    const size_t shmem_size = (arg_sz == 0) ? cf::MemoryData::minBufferSize() : (size_t) arg_sz;

    cfDebug(cf::MODULE_PUMA) << "using memory size:" << shmem_size;

    std::set_terminate(worker_terminate);
    signal(SIGABRT, signal_callback_handler);
    signal(SIGSEGV, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    using namespace cf;

    try {
        SharedMemoryHolder memory;
        memory.attach(shmem_key, shmem_size);
        MemoryData data(memory.get(), shmem_size);

        FormatOptions fopts = data.formatOptions();
        RecognizeOptions ropts = data.recognizeOptions();

        CEDPagePtr page;

        try {
            if(use_shared_image)
                page = recognize(data.image(), ropts, fopts);
            else
                page = recognize(data.imageURL(), ropts, fopts);

            if(!page)
                return WORKER_RECOGNITION_ERROR;

            data.setPage(page);
        }
        catch(PumaException& e) {
            CF_ERROR(e.what());
            data.setMessage(e.what());
            return WORKER_RECOGNITION_ERROR;
        }
        catch(AbstractRecognitionServer::RecognitionException& e) {
            CF_ERROR(e.what());
            data.setMessage(e.what());

            switch(e.reason()) {
            case AbstractRecognitionServer::FILE_NOT_FOUND:
                return WORKER_FILE_NOT_FOUND;
            case AbstractRecognitionServer::IMAGE_OPEN_ERROR:
                return WORKER_LOAD_ERROR;
            default:
                return WORKER_UNKNOWN_ERROR;
            }
        }
    }
    catch(SharedMemoryHolder::Exception& e) {
        CF_ERROR(e.what());
        return WORKER_SHMEM_ERROR;
    }
    catch(std::exception& e) {
        CF_ERROR(e.what());
        return WORKER_UNKNOWN_ERROR;
    }

    return EXIT_SUCCESS;
}
