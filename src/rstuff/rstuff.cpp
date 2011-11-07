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

#include <iostream>
#include <boost/current_function.hpp>

#include "rstuff.h"
#include "rstuff_local.h"
#include "common/recognizeoptions.h"
#include "puma/pumadef.h"

namespace cf {

static const int MAIN_BUF_SIZE = 500000;
static const int WORK_BUF_SIZE = 180000;

RStuff::RStuff() :
    image_data_(NULL),
    buffer_main_(NULL),
    buffer_work_(NULL)
{
    if(!RSTUFF_Init(PUMA_MODULE_RSTUFF, NULL))
        std::cerr << BOOST_CURRENT_FUNCTION << " failed." << std::endl;

    buffer_main_ = new uchar[MAIN_BUF_SIZE];
    buffer_work_ = new uchar[WORK_BUF_SIZE];
}

RStuff::~RStuff() {
    RSTUFF_Done();
    delete []buffer_main_;
    delete []buffer_work_;
}

void RStuff::binarise()
{
    RSTUFF_RSBinarise();
}

void RStuff::normalize()
{
    if (!RSTUFF_RSNormalise(image_data_, buffer_main_, MAIN_BUF_SIZE, buffer_work_, WORK_BUF_SIZE))
        throw Exception("RSTUFF_RSNormalise failed");
}

void RStuff::setCallbacks(RSCBProgressPoints * cb)
{
    if(!RSTUFF_SetImportData(RSTUFF_FN_SetProgresspoints, cb))
        std::cerr << BOOST_CURRENT_FUNCTION << " failed." << std::endl;
}

void RStuff::setImageData(RSPreProcessImage * imageData)
{
    image_data_ = imageData;
}

void RStuff::setRecognizeOptions(const cf::RecognizeOptions& ropts)
{
    if(!image_data_)
        return;

    image_data_->gbAutoRotate = ropts.autoRotate();
    image_data_->gnPictures = ropts.pictureSearch();
    image_data_->gnLanguage = ropts.language();
    image_data_->gbDotMatrix = ropts.dotMatrix();
    image_data_->gbFax100 = ropts.fax();
    image_data_->gnTables = ropts.tableMode();
}

}
