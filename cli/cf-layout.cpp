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

#include <cstdio>
#include <iostream>
#include <string>

#include "common/log.h"
#include "common/imageurl.h"
#include "cpage/cpage.h"
#include "rdib/imageloaderfactory.h"
#include "puma/localrecognitionserver.h"

static const char * program_name = "";

static void print_help()
{
    cfError() << "Usage:" << program_name << "[-txt] IMAGE";
}

int main(int argc, char * argv[])
{
    program_name = argv[0];

    if(argc < 2) {
        print_help();
        return EXIT_FAILURE;
    }

    std::string filename(argv[1]);
    if(filename.empty()) {
        cfError() << "Empty argument.";
        print_help();
        return EXIT_FAILURE;
    }

    bool txt_dump = false;
    if(filename.substr(0, 4) == std::string("-txt")) {
        txt_dump = true;
        if(argc != 3) {
            cfError() << "Image filename required.";
            print_help();
            return EXIT_FAILURE;
        }

        filename = argv[2];
    }

    try {
        cf::ImagePtr input_img = cf::ImageLoaderFactory::instance().load(cf::ImageURL(filename));
        cf::LocalRecognitionServer r;
        r.open(input_img);

        if(!input_img) {
            cfError() << "Can't open image:" << filename;
            return EXIT_FAILURE;
        }

        if(!r.binarize()) {
            cfError() << "Can't binarize image:" << filename;
            return EXIT_FAILURE;
        }

        if(!r.analyze()) {
            cfError() << "Can't analyze image layout:" << filename;
            return EXIT_FAILURE;
        }

        if(txt_dump) {
            cf::LayoutBlockList txt_blocks = r.textBlocks();
            size_t total = 0;

            for(size_t i = 0; i < txt_blocks.size(); i++)
                std::cout << "#" << total++ << " " << txt_blocks.at(i) << "\n";

            cf::LayoutBlockList img_blocks = r.imageBlocks();
            for(size_t i = 0; i < img_blocks.size(); i++)
                std::cout << "#" << total++ << " " << img_blocks.at(i) << "\n";

            cf::LayoutBlockList tbl_blocks = r.tableBlocks();
            for(size_t i = 0; i < tbl_blocks.size(); i++)
                std::cout << "#" << total++ << " " << tbl_blocks.at(i) << "\n";
        }
        else {
            cfInfo() << "saving to \"layout.cpage\"...";
            r.saveLayout("layout.cpage");
        }
    }
    catch(std::exception& e) {
        cfError() << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
