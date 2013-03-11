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

#include <boost/current_function.hpp>

#include "recognitionfactory.h"
#include "localrecognitionserver.h"

#ifdef BUILD_PROCESS_RECOGNITION_SERVER
#include "processrecognitionserver.h"
#endif

#include "common/log.h"

namespace cf {

RecognitionPtr RecognitionFactoryImpl::make(RecognitionServerType type)
{
    switch(type) {
    case SERVER_NULL:
        return RecognitionPtr();
        break;
    case SERVER_DUMMY:
        break;
    case SERVER_LOCAL:
        ptr_.reset(new LocalRecognitionServer);
        break;
#ifdef BUILD_PROCESS_RECOGNITION_SERVER
    case SERVER_PROCESS:
        // reuse
        if(ptr_ && dynamic_cast<ProcessRecognitionServer*>(ptr_.get()))
            break;

        ptr_.reset(new ProcessRecognitionServer);
        break;
#endif
    default:
        cfDebug(MODULE_PUMA) << BOOST_CURRENT_FUNCTION << "default recognition server used.";
        ptr_.reset(new LocalRecognitionServer);
        break;
    }

    return ptr_;
}

}
