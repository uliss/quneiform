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

#ifndef PROCESSRECOGNITIONSERVER_H
#define PROCESSRECOGNITIONSERVER_H

#include <string>

#include "globus.h"
#include "abstractrecognitionserver.h"
#include "common/imageurl.h"
#include "common/image.h"
#include "shmem/memorydata.h"

namespace cf {

class CLA_EXPO ProcessRecognitionServer : public AbstractRecognitionServer
{
public:
    ProcessRecognitionServer();
    ~ProcessRecognitionServer();

    void addImageBlock(const Rect& r);
    void addTableBlock(const Rect& r);
    void addTextBlock(const Rect& r);
    bool analyze();
    bool binarize();
    CEDPagePtr format();
    bool manualLayout();
    bool open(const ImageURL& url);
    bool open(ImagePtr img);
    bool recognize();

    /**
      * Sets worker timeout in seconds. After timeout expire, worker
      * process will be killed
      * @param sec <= 0 means no timeout
      * @see workerTimeout()
      */
    void setWorkerTimeout(int sec);

    /**
      * Returns worker process timeout
      * @see setWorkerTimeout()
      */
    int workerTimeout() const;
private:
    void handleMemoryLimits(std::exception& e);
    void handleOtherErrors(std::exception& e);
    void handleWorkerExitCode(int code);
    bool recognizeImagePtr();
    bool recognizeUrl();
    void setFailedState();
    void startWorker(const std::string& key, size_t size);
    std::string workerPath() const;

    enum source_t {
        SOURCE_URL,
        SOURCE_IMAGE
    };
private:
    MemoryData data_;
    std::string key_;
    ImageURL url_;
    ImagePtr image_;
    source_t src_;
    CEDPagePtr page_;
    int worker_timeout_;
};

}

#endif // PROCESSRECOGNITIONSERVER_H
