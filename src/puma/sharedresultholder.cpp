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
#include <iostream>
#include <boost/current_function.hpp>

#include "sharedresultholder.h"
#include "sharedresult.h"
#include "common/console_messages.h"

#define CF_ERROR std::cerr << cf::console::error << BOOST_CURRENT_FUNCTION << ": "

namespace cf
{

static const char * SHARED_RESULT_KEY = "result";

SharedResultHolder::SharedResultHolder(bi::managed_shared_memory * segment) :
    segment_(segment) {
    assert(segment);
    shm_result_ = segment_->construct<SharedResult>(SHARED_RESULT_KEY)();

    if(!shm_result_) {
        CF_ERROR << "key already used => " << "'" << SHARED_RESULT_KEY << "'" << std::endl;
    }
}

SharedResultHolder::~SharedResultHolder() {
    if(!segment_)
        return;

    if(segment_->find<SharedResult>(SHARED_RESULT_KEY).first)
        segment_->destroy<SharedResult>(SHARED_RESULT_KEY);
}

SharedResult * SharedResultHolder::find(bi::managed_shared_memory * segment) {
    if(!segment)
        return NULL;
    return segment->find<SharedResult>(SHARED_RESULT_KEY).first;
}

CEDPagePtr SharedResultHolder::page() {
    return shm_result_ ? shm_result_->get() : CEDPagePtr();
}

SharedResult * SharedResultHolder::sharedResult() {
    return shm_result_;
}

}
