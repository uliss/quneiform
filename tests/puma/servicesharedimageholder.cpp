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

#include "puma/shared_memory_type.h"
#include "puma/sharedimage.h"
#include "puma/sharedimageholder.h"

using namespace boost::interprocess;
using namespace cf;

int main() {
    try {
        SharedMemory segment(open_only, "test_puma");

        SharedImage * sh_image = SharedImageHolder::find(&segment);
        if(!sh_image)
            return 3;

        if(sh_image->name() != "sample.jpg")
            return 4;

        if(sh_image->dimensions() != Size(100, 200))
            return 5;

        if(sh_image->dataSize() != 100)
            return 6;

        if(static_cast<char*>(sh_image->data())[2] != 2)
            return 7;
    }
    catch(interprocess_exception& e){
        std::cerr << "can't open shared memory 'test_puma'. " << e.what() << std::endl;
        return 2;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
