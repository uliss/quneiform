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

#include <cassert>
#include <boost/current_function.hpp>

#include "sharedoptionsholder.h"
#include "sharedoptions.h"

namespace cf
{

static const char * SHARED_OPTIONS_KEY = "options";

SharedOptionsHolder::SharedOptionsHolder(SharedMemory * segment) :
    segment_(segment)
{
    assert(segment);
    options_ = segment_->construct<SharedOptions>(SHARED_OPTIONS_KEY)();

    if(!options_) {
        std::cerr << BOOST_CURRENT_FUNCTION << ": key already used => "
                  << "'" << SHARED_OPTIONS_KEY << "'" << std::endl;
    }
}

SharedOptionsHolder::~SharedOptionsHolder() {
    if(!segment_)
        return;

    if(segment_->find<SharedOptions>(SHARED_OPTIONS_KEY).first)
        segment_->destroy<SharedOptions>(SHARED_OPTIONS_KEY);
}

SharedOptions * SharedOptionsHolder::find(SharedMemory * segment) {
    if(!segment)
        return NULL;

    return segment->find<SharedOptions>(SHARED_OPTIONS_KEY).first;
}

SharedOptions * SharedOptionsHolder::options() {
    return options_;
}

}
