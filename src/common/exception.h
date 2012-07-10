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

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <stdexcept>
#include <sstream>

#include "globus.h"

namespace cf
{

template<class T>
class CLA_EXPO RuntimeExceptionImpl: public std::runtime_error
{
public:
    RuntimeExceptionImpl(const std::string& msg = "", int code = 0);
    virtual ~RuntimeExceptionImpl() throw() {}

    int code() const
    {
        return code_;
    }

    template<typename Value>
    RuntimeExceptionImpl& operator<<(const Value& v)
    {
        std::ostringstream buf;
        buf << v;
        if(!msg_.empty())
            msg_ += ' ';

        msg_ += buf.str();
        return *this;
    }

    virtual const char * what() const throw()
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
    int code_;
};

template<class T>
RuntimeExceptionImpl<T>::RuntimeExceptionImpl(const std::string& msg, int code) :
    std::runtime_error(""),
    code_(code)
{
    msg_ = msg;
}

}

#endif /* EXCEPTION_H_ */
