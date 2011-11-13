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
#include <string>
#include <exception>
#include <iostream>
#include <csignal>

#include "localrecognitionserver.h"
#include "export/exporterfactory.h"
#include "rdib/imageloaderfactory.h"
#include "shmem/memorydata.h"
#include "shmem/sharedmemoryholder.h"
#include "shmem_data.h"
#include "common/console_messages.h"
#include "common/debug.h"

#define WORKER_PREFIX cf::console::message("[Process worker] ", cf::console::YELLOW)
#define CF_ERROR(msg) std::cerr << WORKER_PREFIX << msg << std::endl;

static cf::CEDPagePtr recognize(cf::ImagePtr img,
                         const cf::RecognizeOptions& ropts,
                         const cf::FormatOptions& fopts)
{
    cf::LocalRecognitionServer r;
    return r.recognize(img, ropts, fopts);
}

static cf::CEDPagePtr recognize(const std::string& path,
                         const cf::RecognizeOptions& ropts,
                         const cf::FormatOptions& fopts)
{
    cf::ImagePtr img = cf::ImageLoaderFactory::instance().load(path);
    return recognize(img, ropts, fopts);
}

static void worker_terminate() {
    CF_ERROR("recognition process crashed");
    exit(cf::WORKER_TERMINATE_ERROR);
}

static void signal_callback_handler(int signum) {
    switch(signum) {
    case SIGSEGV:
        CF_ERROR("recognition process segmentation fault");
        exit(cf::WORKER_SEGFAULT_ERROR);
        break;
    case SIGABRT:
        CF_ERROR("recognition process aborted");
        exit(cf::WORKER_ABORT_ERROR);
        break;
    case SIGTERM:
        CF_ERROR("recognition process killed by timeout");
        exit(cf::WORKER_TIMEOUT_ERROR);
        break;
    default:
        exit(cf::WORKER_UNKNOWN_ERROR);
    }
}

int main(int argc, char ** argv) {
    if(argc != 2) {
        CF_ERROR("Usage: worker KEY");
        return cf::WORKER_WRONG_ARGUMENT;
    }

    const std::string shmem_key = argv[1];

    std::set_terminate(worker_terminate);
    signal(SIGABRT, signal_callback_handler);
    signal(SIGSEGV, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    using namespace cf;

    try {
        const size_t shm_size = MemoryData::minBufferSize();
        SharedMemoryHolder memory;
        memory.attach(shmem_key, shm_size);
        MemoryData data(memory.get(), shm_size);

        FormatOptions fopts = data.formatOptions();
        RecognizeOptions ropts = data.recognizeOptions();
        std::string image_path = data.imagePath();

        CEDPagePtr page = recognize(image_path, ropts, fopts);

        if(!page)
            return WORKER_RECOGNITION_ERROR;

        data.setPage(page);
    }
    catch(SharedMemoryHolder::Exception& e) {
        std::cerr << WORKER_PREFIX << e.what() << std::endl;
        return WORKER_SHMEM_ERROR;
    }
    catch(std::exception& e) {
        CF_ERROR(e.what());
        return WORKER_UNKNOWN_ERROR;
    }

    return EXIT_SUCCESS;
}
