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

#include "shared_memory_type.h"
#include "localrecognitionserver.h"
#include "export/exporterfactory.h"
#include "rdib/imageloaderfactory.h"
#include "common/formatoptions.h"
#include "common/recognizeoptions.h"
#include "common/console_messages.h"
#include "common/debug.h"
#include "shmem_data.h"
#include "sharedimage.h"
#include "sharedimageholder.h"
#include "sharedresult.h"
#include "sharedresultholder.h"
#include "sharedoptions.h"
#include "sharedoptionsholder.h"

#define WORKER_PREFIX cf::console::message("[Process worker] ", cf::console::YELLOW)
#define CF_ERROR(msg) std::cerr << WORKER_PREFIX << msg << std::endl;

static cf::CEDPagePtr recognize(cf::ImagePtr img,
                         const cf::RecognizeOptions& ropts,
                         const cf::FormatOptions& fopts)
{
    cf::LocalRecognitionServer r;
    return r.recognize(img, ropts, fopts);
}

static cf::CEDPagePtr recognize(cf::SharedImage * sh_image,
                         const cf::RecognizeOptions& ropts,
                         const cf::FormatOptions& fopts)
{
    if(!sh_image)
        return cf::CEDPagePtr();

    return recognize(sh_image->get(), ropts, fopts);
}

static cf::CEDPagePtr recognize(const std::string& path,
                         const cf::RecognizeOptions& ropts,
                         const cf::FormatOptions& fopts)
{
    cf::ImagePtr img = cf::ImageLoaderFactory::instance().load(path);
    return recognize(img, ropts, fopts);
}

static bool savePage(cf::CEDPagePtr p, cf::SharedResult * output) {
    bool ok = output->store(p);
    if(!ok)
        CF_ERROR("error while store in shared memory");
    return ok;
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
    bool use_shared_image = false;
    std::string file_name;

    if(argc < 2) {
        CF_ERROR("Usage: worker KEY [filepath]");
        return cf::WORKER_UNKNOWN_ERROR;
    }

    const char * SHARED_MEMORY_KEY = argv[1];

    if(argc == 2)
        use_shared_image = true;
    else
        file_name = argv[2];

    std::set_terminate(worker_terminate);
    signal(SIGABRT, signal_callback_handler);
    signal(SIGSEGV, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    using namespace boost::interprocess;
    using namespace cf;

    try {
        //Open managed shared memory
        SharedMemory segment(open_only, SHARED_MEMORY_KEY);

        // find shared result
        SharedResult * sh_result = SharedResultHolder::find(&segment);
        if(!sh_result) {
            CF_ERROR("Can't open shared result");
            return WORKER_SEGMENT_NOT_FOUND;
        }

        // find shared image
        SharedImage * sh_image = NULL;

        if(use_shared_image) {
            sh_image = SharedImageHolder::find(&segment);
            if(!sh_image) {
                CF_ERROR("Can't open shared image");
                return WORKER_SEGMENT_NOT_FOUND;
            }
        }

        // find shared options
        FormatOptions fopts;
        RecognizeOptions ropts;
        SharedOptions * sh_options = SharedOptionsHolder::find(&segment);

        if(!sh_options) {
            CF_ERROR("can't find shared options. Using default.");
        }
        else {
            fopts = sh_options->formatOptions();
            ropts = sh_options->recognizeOptions();
        }

        CEDPagePtr page;

        if(use_shared_image)
            page = recognize(sh_image, ropts, fopts);
        else
            page = recognize(file_name, ropts, fopts);

        if(!page)
            return WORKER_RECOGNITION_ERROR;

        if(!savePage(page, sh_result))
            return WORKER_SAVE_ERROR;
    }
    catch(interprocess_exception& e){
        std::cerr << WORKER_PREFIX << "can't open shared memory: '"
                  << SHARED_MEMORY_KEY << "'. " << e.what() << std::endl;
        return WORKER_SHMEM_ERROR;
    }
    catch(std::exception& e) {
        CF_ERROR(e.what());
        return WORKER_UNKNOWN_ERROR;
    }

    return EXIT_SUCCESS;
}

