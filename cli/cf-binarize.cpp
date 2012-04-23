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
#include <iostream>
#include <stdexcept>

#include "rdib/imageloaderfactory.h"
#include "cimage/cticontrol.h"
#include "rimage/cricontrol.h"
#include "cfio/cfio.h"

int main(int argc, char ** argv)
{
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " INPUT_IMAGE OUTPUT_IMAGE\n";
        return EXIT_FAILURE;
    }

    CFIO_Init(0, 0);

    try {
        cf::ImagePtr input_img = cf::ImageLoaderFactory::instance().load(argv[1]);

        if(!input_img) {
            std::cerr << "Can't open image: " << argv[1] << "\n";
            return EXIT_FAILURE;
        }

        if(!cf::CImage::instance().addImage("input", (BitmapHandle) input_img->data())) {
            std::cerr << "Can't add image to storage: " << argv[1] << "\n";
            return EXIT_FAILURE;
        }

        if(!cf::RImage::instance().binarise("input", "bin", cf::BINARIZATOR_KRONROD)) {
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

    CFIO_Done();

    return EXIT_SUCCESS;
}
