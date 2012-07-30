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

#include <cassert>

#include "log.h"
#include "console_messages.h"

namespace cf
{

static inline std::string modulePrefix(module_t m, message_t msgType)
{
    if(m == MODULES_ALL) {
        switch(msgType) {
        case MSG_TRACE:
            return "[Trace]   ";
        case MSG_DEBUG:
            return "[Debug]   ";
        case MSG_INFO:
            return "[Info]    ";
        case MSG_WARNING:
            return "[Warning] ";
        case MSG_ERROR:
            return "[Error]   ";
        case MSG_FATAL:
            return "[Fatal]   ";
        default:
            return "[...]   ";
        }
    }
    return std::string(1, '[') + moduleToString(m) + "] ";
}

static bool isModuleLogging(module_t m)
{
    switch(m) {
    default:
        return true;
    }
}

console::color_t inline messageColor(message_t t)
{
    switch(t) {
    case MSG_TRACE:
        return console::WHITE;
    case MSG_DEBUG:
        return console::BLUE;
    case MSG_INFO:
        return console::GREEN;
    case MSG_WARNING:
        return console::YELLOW;
    case MSG_ERROR:
        return console::RED;
    case MSG_FATAL:
        return console::PURPLE;
    default:
        return console::NORMAL;
    }
}

static void defaultMessageHandler(module_t module, message_t msgType, const char * message)
{
    if(!isModuleLogging(module))
        return;

    console::color_t c = messageColor(msgType);

    std::cerr << console::message(modulePrefix(module, msgType), c) << message;
    std::cerr.flush();
}

Logger::MessageHandlerFuncPtr Logger::handler_ = &defaultMessageHandler;

Logger::Logger(module_t module, message_t msgType) :
    module_(module),
    msg_type_(msgType)
{

}

Logger::Logger(const Logger& l) :
    module_(l.module_),
    msg_type_(l.msg_type_)
{
}

Logger::~Logger()
{
    std::string msg = buffer_;
    msg += '\n';

    if(!handler_)
        setMessageHandler(0);

    assert(handler_);

    handler_(module_, msg_type_, msg.c_str());
}

void Logger::setMessageHandler(Logger::MessageHandlerFuncPtr func)
{
    if(func == 0)
        handler_ = &defaultMessageHandler;
    else
        handler_ = func;
}

}

cf::Logger cfTrace(cf::module_t module)
{
    return cf::Logger(module, cf::MSG_TRACE);
}

cf::Logger cfDebug(cf::module_t module)
{
    return cf::Logger(module, cf::MSG_DEBUG);
}

cf::Logger cfInfo(cf::module_t module)
{
    return cf::Logger(module, cf::MSG_INFO);
}

cf::Logger cfWarning(cf::module_t module)
{
    return cf::Logger(module, cf::MSG_WARNING);
}

cf::Logger cfError(cf::module_t module)
{
    return cf::Logger(module, cf::MSG_ERROR);
}

cf::Logger cfFatal(cf::module_t module)
{
    return cf::Logger(module, cf::MSG_FATAL);
}
