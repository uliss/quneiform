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

#ifndef RECOGNITIONSTATE_H
#define RECOGNITIONSTATE_H

#include <boost/function.hpp>

#include "globus.h"

namespace cf {

class CLA_EXPO RecognitionState
{
public:
    typedef boost::function<void(int)> Callback;
public:
    enum state_t {
        NONE       = 0,
        OPENED     = 0x1,
        BINARIZED  = 0x2,
        ANALYZED   = 0x4,
        RECOGNIZED = 0x8,
        FORMATTED  = 0x10,
        FAILED     = 0x1000
    };
public:
    RecognitionState();
    state_t get() const { return state_; }
    void setCallback(Callback cb);
    template<class T>
    void setCallback(T * obj, void (T::*callback)(int));
    void reset() { state_ = NONE; }
    void set(state_t st);
private:
    Callback cb_;
    state_t state_;
};

template<class T>
inline void RecognitionState::setCallback(T * obj, void (T::*callback)(int)) {
    cb_ = std::bind1st(std::mem_fun(callback), obj);
}

}

#endif // RECOGNITIONSTATE_H
