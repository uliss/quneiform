/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

#include "singleton.h"

namespace cf
{

class DebugImpl
{
    public:
        DebugImpl() :
                null_(0) {
#ifndef NDEBUG
            os_ = &std::cerr;
#else
            os_ = &null_;
#endif
        }

        ~DebugImpl() {
            os_->flush();
        }

        std::ostream& null() {
            return null_;
        }

        template<class T>
        std::ostream& operator<<(const T& val) {
            (*os_) << val;
            return *os_;
        }

        void setOutput(std::ostream& os) {
            os_->flush();
            os_ = &os;
        }
    private:
        std::ostream * os_;
        std::ostream null_;
};

inline DebugImpl& Debug()
{
    return Singleton<DebugImpl>::instance();
}

}

#endif /* DEBUG_H_ */
