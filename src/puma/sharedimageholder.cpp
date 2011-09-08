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

#include <cassert>
#include <iostream>
#include <boost/current_function.hpp>

#include "sharedimageholder.h"
#include "sharedimage.h"
#include "common/console_messages.h"

#define CF_ERROR std::cerr << cf::console::error << BOOST_CURRENT_FUNCTION

namespace cf {

static const char * SHARED_IMAGE_KEY = "image";

SharedImageHolder::SharedImageHolder(bi::managed_shared_memory * segment) :
    segment_(segment),
    image_(NULL)
{
    assert(segment);
    image_ = segment_->construct<SharedImage>(SHARED_IMAGE_KEY)();

    if(!image_) {
        CF_ERROR << ": key already used => "
                 << "'" << SHARED_IMAGE_KEY << "'" << std::endl;
    }
}

SharedImageHolder::~SharedImageHolder() {
    if(!segment_)
        return;

    if(segment_->find<SharedImage>(SHARED_IMAGE_KEY).first)
        segment_->destroy<SharedImage>(SHARED_IMAGE_KEY);
}

SharedImage * SharedImageHolder::find(bi::managed_shared_memory * segment) {
    if(!segment)
        return NULL;

    return segment->find<SharedImage>(SHARED_IMAGE_KEY).first;
}

SharedImage *  SharedImageHolder::image() {
    return image_;
}

}
