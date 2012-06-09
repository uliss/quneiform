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

#include <QDir>
#include <QFile>
#include <QDateTime>
#include <cstdio>

#include "common/singleton.h"
#include "guilog.h"

class LogFile {
public:
    LogFile(const QString& path, const QString& prefix, bool dateTime = true) :
        f_(path),
        prefix_(prefix),
        print_datetime_(dateTime)
    {
        f_.open(QFile::WriteOnly | QFile::Truncate);
    }

    ~LogFile()
    {
        f_.close();
    }

    void write(const char * msg)
    {
        if(!f_.isOpen())
            return;

        f_.write(prefix_.toAscii());

        if(print_datetime_) {
            QString datetime = QDateTime::currentDateTime().toString(Qt::ISODate);
            f_.write("(");
            f_.write(datetime.toAscii());
            f_.write(") ");
        }

        f_.write(msg);
        f_.write("\n");
        f_.flush();
    }
private:
    QString prefix_;
    QFile f_;
    bool print_datetime_;
};

QString guiDebugLogPath()
{
    return QDir::tempPath() + "/Quneiform.debug.log";
}

QString guiWarningLogPath()
{
    return QDir::tempPath() + "/Quneiform.warning.log";
}

QString guiCriticalLogPath()
{
    return QDir::tempPath() + "/Quneiform.critical.log";
}

QString guiFatalLogPath()
{
    return QDir::tempPath() + "/Quneiform.fatal.log";
}

class DebugLog : public LogFile {
public:
    DebugLog() : LogFile(guiDebugLogPath(), "DEBUG ", false) {}
};

class WarningLog : public LogFile {
public:
    WarningLog() : LogFile(guiWarningLogPath(), "WARNING ") {}
};

class CriticalLog : public LogFile {
public:
    CriticalLog() : LogFile(guiCriticalLogPath(), "CRITICAL ") {}
};

class FatalLog : public LogFile {
public:
    FatalLog() : LogFile(guiFatalLogPath(), "FATAL ") {}
};

typedef cf::Singleton<DebugLog, cf::CreateUsingStatic> DebugLogger;
typedef cf::Singleton<WarningLog, cf::CreateUsingStatic> WarningLogger;
typedef cf::Singleton<CriticalLog, cf::CreateUsingStatic> CriticalLogger;
typedef cf::Singleton<FatalLog, cf::CreateUsingStatic> FatalLogger;

void guiMessageLogger(QtMsgType type, const char * msg)
{
    switch(type) {
    case QtDebugMsg:
        DebugLogger::instance().write(msg);
        break;
    case QtWarningMsg:
        DebugLogger::instance().write(msg);
        WarningLogger::instance().write(msg);
        break;
    case QtCriticalMsg:
        DebugLogger::instance().write(msg);
        CriticalLogger::instance().write(msg);
        break;
    case QtFatalMsg:
        DebugLogger::instance().write(msg);
        FatalLogger::instance().write(msg);
        break;
    default:
        DebugLogger::instance().write(msg);
        break;
    }
}

