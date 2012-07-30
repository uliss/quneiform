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

#ifndef LOG_H
#define LOG_H

#include <iostream>

#include "stringbuffer.h"
#include "modules.h"
#include "globus.h"

namespace cf
{

enum message_t {
    MSG_TRACE   = 0x1,
    MSG_DEBUG   = 0x2,
    MSG_INFO    = 0x4,
    MSG_WARNING = 0x8,
    MSG_ERROR   = 0x10,
    MSG_FATAL   = 0x20
};

class CLA_EXPO Logger
{
    void operator=(const Logger&);
public:
    typedef void (*MessageHandlerFuncPtr)(module_t module, message_t msgType, const char * message);
public:
    Logger(module_t module, message_t msgType);
    Logger(const Logger& l);
    ~Logger();

    template<class T>
    Logger& operator<<(const T& v)
    {
#ifdef NDEBUG
        if(msg_type_ & (MSG_TRACE | MSG_DEBUG))
            return *this;
#endif
        buffer_ << v;
        return *this;
    }

    template<class T>
    Logger& write(const T& v)
    {
#ifdef NDEBUG
        if(msg_type_ & (MSG_TRACE | MSG_DEBUG))
            return *this;
#endif
        buffer_.write(v);
        return *this;
    }
public:
    static void setMessageHandler(MessageHandlerFuncPtr func);
private:
    module_t module_;
    message_t msg_type_;
    StringBuffer buffer_;
private:
    static MessageHandlerFuncPtr handler_;
};

}

FUN_EXPO__ cf::Logger cfTrace(cf::module_t module = cf::MODULES_ALL);
FUN_EXPO__ cf::Logger cfDebug(cf::module_t module = cf::MODULES_ALL);
FUN_EXPO__ cf::Logger cfInfo(cf::module_t module = cf::MODULES_ALL);
FUN_EXPO__ cf::Logger cfWarning(cf::module_t module = cf::MODULES_ALL);
FUN_EXPO__ cf::Logger cfError(cf::module_t module = cf::MODULES_ALL);
FUN_EXPO__ cf::Logger cfFatal(cf::module_t module = cf::MODULES_ALL);

#endif // LOG_H
