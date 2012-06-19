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

int main(int argc, char ** argv)
{
    if(argc < 3) {
        std::cerr << "Usage: " << argv[0] << " INPUT_IMAGE OUTPUT_IMAGE [-b BINARIZATOR]\n";
        return EXIT_FAILURE;
    }

    std::string binarizator("kronrod");
    if(argc == 5 && strcmp(argv[3], "-b") == 0)
        binarizator = argv[4];

    cf::binarizator_t binType = cf::BINARIZATOR_KRONROD;
    if(binarizator == "threshold")
        binType = cf::BINARIZATOR_THRESHOLD;

    try {
        cf::ImagePtr input_img = cf::ImageLoaderFactory::instance().load(argv[1]);

        if(!input_img) {
            std::cerr << "Can't open image: " << argv[1] << "\n";
            return EXIT_FAILURE;
        }

        if(!cf::CImage::instance().addImage("input", (cf::BitmapPtr) input_img->data())) {
            std::cerr << "Can't add image to storage: " << argv[1] << "\n";
            return EXIT_FAILURE;
        }

        if(!cf::RImage::instance().binarise("input", "bin", binType, 127)) {
            std::cerr << "Can't binarize image\n";
            return EXIT_FAILURE;
        }

        if(!cf::CImage::instance().dumpImage("bin", argv[2])) {
            std::cerr << "Can't save binarized image: \"" << argv[2] << "\"\n";
            return EXIT_FAILURE;
        }
    }
    catch(std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
