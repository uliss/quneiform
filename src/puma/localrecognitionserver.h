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

#ifndef LOCALRECOGNITIONSERVER_H
#define LOCALRECOGNITIONSERVER_H

#include "abstractrecognitionserver.h"
#include "globus.h"

namespace cf {

class CLA_EXPO LocalRecognitionServer : public AbstractRecognitionServer
{
public:
    LocalRecognitionServer();
    ~LocalRecognitionServer();

    /**
     * Makes document layout
     * @return true on success
     */
    bool analyze();

    /**
     * Binarizes image
     * @param bopts - binarization options
     * @return true on success
     */
    bool binarize();

    /**
     * Formats recognition result
     * @return pointer to CEDPage
     */
    CEDPagePtr format();

    /**
     * Opens image
     * @param url - image url
     * @return true on success
     */
    bool open(const ImageURL& url);
    bool open(ImagePtr image);

    /**
     * Recognizes image
     * @returns true on success
     */
    bool recognize();
private:
    void close();
    void handleExceptionCommon(std::exception& e);
    void setOptions();

    // sets local and callback state
    void setTotalState(int state);
    void setFailed();

    bool isOpened() const;
    bool isBinarized() const;
    bool isAnalyzed() const;
    bool isRecognized() const;
private:
    ImagePtr image_;
    int local_state_;
};

}

#endif // LOCALRECOGNITIONSERVER_H
