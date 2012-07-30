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
#include <iomanip>

#include "log.h"
#include "console_messages.h"
#include "singleton.h"
#include "configoptions.h"

namespace cf
{

static inline std::string pad(const std::string& str)
{
    static const size_t PADDING = 10;
    std::string res = str;
    if(res.length() < PADDING)
        return res.insert(res.length() - 1, PADDING - res.length(), ' ');
    return res;
}

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
            return "[...]     ";
        }
    }
    return pad(std::string(1, '[') + moduleToString(m) + "] ");
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

static void outputToStream(std::ostream& os, module_t module, message_t msgType, const char * message)
{
    if(Logger::config().isPrefixEnabled(module)) {
        if(Logger::config().isColorizeEnabled(module))
            os << console::message(modulePrefix(module, msgType), messageColor(msgType));
        else
            os << modulePrefix(module, msgType);
    }

    os << message << std::flush;
}

static void defaultMessageHandler(module_t module, message_t msgType, const char * message)
{
    if(Logger::config().isRuntimeConfigEnabled(module)) {
        std::string module_name = moduleToString(module);
        bool module_enabled = ConfigOptions::getBool("debug.module." + module_name, true);
        if(!module_enabled)
            return;
    }

    outputToStream(std::cerr, module, msgType, message);
}

Logger::MessageHandlerFuncPtr Logger::handler_ = &defaultMessageHandler;
LoggerConfig Logger::config_;

Logger::Logger(module_t module, message_t msgType) :
    module_(module),
    msg_type_(msgType)
{}

Logger::Logger(const Logger& l) :
    module_(l.module_),
    msg_type_(l.msg_type_)
{}

Logger::~Logger()
{
    if(!config_.isEnabled(module_, msg_type_))
        return;

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

LoggerConfig& Logger::config()
{
    return config_;
}

LoggerConfig::LoggerConfig() :
#ifndef NDEBUG
    trace_(MODULES_ALL),
    debug_(MODULES_ALL),
#else
    trace_(0),
    debug_(0),
#endif
    info_(MODULES_ALL),
    warning_(MODULES_ALL),
    error_(MODULES_ALL),
    fatal_(MODULES_ALL),
    colorize_(MODULES_ALL),
    split_(MODULES_ALL),
    runtime_config_(0),
    prefix_(MODULES_ALL)
{
}

bool LoggerConfig::isColorizeEnabled(module_t m) const
{
    return colorize_ & m;
}

bool LoggerConfig::isEnabled(module_t m, message_t t) const
{
    switch(t) {
    case MSG_TRACE:
        return trace_ & m;
    case MSG_DEBUG:
        return debug_ & m;
    case MSG_INFO:
        return info_ & m;
    case MSG_WARNING:
        return warning_ & m;
    case MSG_ERROR:
        return error_ & m;
    case MSG_FATAL:
        return fatal_ & m;
    default:
        return false;
    }
}

bool LoggerConfig::isSplitEnabled(module_t m) const
{
    return split_ & m;
}

bool LoggerConfig::isPrefixEnabled(module_t m) const
{
    return prefix_ & m;
}

bool LoggerConfig::isRuntimeConfigEnabled(module_t m) const
{
    return runtime_config_ & m;
}

void LoggerConfig::enableLog(module_t m, message_t t)
{
    switch(t) {
    case MSG_TRACE:
        trace_ |= m;
        break;
    case MSG_DEBUG:
        debug_ |= m;
        break;
    case MSG_INFO:
        info_ |= m;
        break;
    case MSG_WARNING:
        warning_ |= m;
        break;
    case MSG_ERROR:
        error_ |= m;
        break;
    case MSG_FATAL:
        fatal_ |= m;
        break;
    }
}

void LoggerConfig::disableLog(module_t m, message_t t)
{
    switch(t) {
    case MSG_TRACE:
        trace_ &= (~m);
        break;
    case MSG_DEBUG:
        debug_ &= (~m);
        break;
    case MSG_INFO:
        info_ &= (~m);
        break;
    case MSG_WARNING:
        warning_ &= (~m);
        break;
    case MSG_ERROR:
        error_ &= (~m);
        break;
    case MSG_FATAL:
        fatal_ &= (~m);
        break;
    }
}

void LoggerConfig::enableColorize(module_t m)
{
    colorize_ |= m;
}

void LoggerConfig::disableColorize(module_t m)
{
    colorize_ &= (~m);
}

void LoggerConfig::enableSplit(module_t m)
{
    split_ |= m;
}

void LoggerConfig::disableSplit(module_t m)
{
    split_ &= (~m);
}

void LoggerConfig::enablePrefix(module_t m)
{
    prefix_ |= m;
}

void LoggerConfig::disablePrefix(module_t m)
{
    prefix_ &= (~m);
}

void LoggerConfig::enableRuntimeConfig(module_t m)
{
    runtime_config_ |= m;
}

void LoggerConfig::disableRuntimeConfig(module_t m)
{
    runtime_config_ &= (~m);
}

class FileLog
{
public:
    FileLog() {}

    bool isOpened() const
    {
        return (bool) file_;
    }

    void messageHandler(module_t module, message_t msgType, const char * message);

    bool open(const std::string& path)
    {
        file_.open(path.c_str());

        if(!file_)
            std::cerr << "[Error] can't open log file for writing: " << path << std::endl;

        return file_;
    }
private:
    std::ofstream file_;
};

void FileLog::messageHandler(module_t module, message_t msgType, const char * message)
{
    if(Logger::config().isRuntimeConfigEnabled(module)) {
        const std::string mod_name = moduleToString(module);
        if(!ConfigOptions::getBool("debug.module." + mod_name, true))
            return;
    }

    if(file_) {
        std::string prefix = modulePrefix(module, msgType);
        file_ << prefix <<  message << std::flush;
    }

    if(Logger::config().isSplitEnabled(module))
        outputToStream(std::cerr, module, msgType, message);
}

typedef Singleton<FileLog, CreateUsingStatic> PersistantLog;

void fileMessageHandler(module_t m, message_t t, const char * message)
{
    static const char * LOG_PATH;
#ifdef _WIN32
    LOG_PATH = "cuneiform.log";
#else
    LOG_PATH = "/tmp/cuneiform.log";
#endif
    if(!PersistantLog::instance().isOpened())
        PersistantLog::instance().open(LOG_PATH);

    PersistantLog::instance().messageHandler(m, t, message);
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
