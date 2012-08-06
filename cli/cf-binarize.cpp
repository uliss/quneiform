/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "rdib/imageloaderfactory.h"
#include "cimage/cticontrol.h"
#include "rimage/cricontrol.h"
#include "common/imageurl.h"
#include "common/log.h"
#include "common/binarizeoptions.h"

int main(int argc, char ** argv)
{
    cf::Logger::config().disableColorize(cf::MODULES_ALL);
    cf::Logger::config().disablePrefix(cf::MODULES_ALL);

    if(argc < 3) {
        cfError() << "Usage: " << argv[0] << " INPUT_IMAGE OUTPUT_IMAGE [-b BINARIZATOR]";
        return EXIT_FAILURE;
    }

    std::string binarizator("kronrod");
    if(argc == 5 && strcmp(argv[3], "-b") == 0)
        binarizator = argv[4];

    cf::BinarizeOptions bopts;
    bopts.setBinarizator(cf::BINARIZATOR_KRONROD);
    if(binarizator == "threshold")
        bopts.setBinarizator(cf::BINARIZATOR_THRESHOLD);
    else if(binarizator == "kronrod")
        bopts.setBinarizator(cf::BINARIZATOR_KRONROD);
    else if(binarizator == "deza")
        bopts.setBinarizator(cf::BINARIZATOR_DEZA);
    else if(binarizator == "otsu")
        bopts.setBinarizator(cf::BINARIZATOR_OTSU);
    else
        cfWarning() << "unknown binarizator:" << binarizator;

    if(!binarizator.empty())
        cfInfo() << "using binarizator:" << binarizator;

    try {
        cf::ImagePtr input_img = cf::ImageLoaderFactory::instance().load(cf::ImageURL(argv[1]));

        if(!input_img) {
            cfError() << "Can't open image: " << argv[1];
            return EXIT_FAILURE;
        }

        if(!cf::CImage::instance().addImage("input", (cf::BitmapPtr) input_img->data())) {
            cfError() << "Can't add image to storage: " << argv[1];
            return EXIT_FAILURE;
        }

        if(!cf::RImage::instance().binarise("input", "bin", bopts)) {
            cfError() << "Can't binarize image";
            return EXIT_FAILURE;
        }

        if(!cf::CImage::instance().dumpImage("bin", argv[2])) {
            cfError() << "Can't save binarized image: \"" << argv[2] << "\"";
            return EXIT_FAILURE;
        }
    }
    catch(std::exception& e) {
        cfError() << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
