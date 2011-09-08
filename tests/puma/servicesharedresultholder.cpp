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

#ifdef __OpenBSD__
#include <sys/param.h> // for MAXLOGNAME
#endif

#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>

#include "puma/sharedresultholder.h"
#include "puma/sharedresult.h"

using namespace boost::interprocess;
using namespace cf;

int main() {
    try {
        managed_shared_memory segment(open_only, "test_puma");

        SharedResult * sh_res = SharedResultHolder::find(&segment);
        if(!sh_res)
            return 3;

        CEDPagePtr p = sh_res->get();

        if(!p.get())
            return 4;

        if(p->imageName() != "sample")
            return 5;

        if(p->imageSize() != Size(300, 400))
            return 6;
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
