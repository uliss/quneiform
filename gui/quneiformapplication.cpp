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

#include <QFileOpenEvent>
#include <QFile>
#include <QtPlugin>

#include "metatyperegistrator.h"
#include "translationloader.h"
#include "quneiformapplication.h"
#include "config-version.h"
#include "common/singleton.h"
#include "plugins/dibimageioplugin.h"

Q_IMPORT_PLUGIN(dib_imageplugin)

class Log {
public:
    Log(const QString& path) : f_(path)
    {
        f_.open(QFile::WriteOnly | QFile::Truncate);
    }

    virtual ~Log()
    {
        f_.close();
    }

    void  write(const char * msg)
    {
        if(!f_.isOpen())
            return;

        f_.write("Debug: ");
        f_.write(msg);
        f_.write("\n");
        f_.flush();
    }
private:
    QFile f_;
};

class DebugLog : public Log {
public:
    DebugLog() : Log("/tmp/Quneform.debug.log") {}
};

typedef cf::Singleton<DebugLog, cf::CreateUsingStatic> DebugLogger;

#ifdef Q_WS_MAC
static void cfMessageOutput(QtMsgType type, const char * msg)
 {
     switch (type) {
     case QtDebugMsg:
         DebugLogger::instance().write(msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
 }
#endif

QuneiformApplication::QuneiformApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
#ifdef Q_WS_MAC
    qInstallMsgHandler(cfMessageOutput);
#endif

    setOrganizationName("openocr.org");
    setApplicationName("Quneiform OCR");
    setApplicationVersion(CF_VERSION);

#ifdef Q_WS_MAC
    setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    MetaTypeRegistrator registrator;
    TranslationLoader loader;
    loader.load();
    installTranslator(loader.systemTranslator());
    installTranslator(loader.applicationTranslator());
}

bool QuneiformApplication::event(QEvent * ev)
{
    bool processed = false;
    switch (ev->type()) {
    case QEvent::FileOpen: {
        QStringList files;
        files << static_cast<QFileOpenEvent*>(ev)->file();
        ev->accept();
        emit openFiles(files);
        return true;
    }
//        case QEvent::Close: {
//  }
    default:
        processed = QApplication::event(ev);
        break;
    }
    return processed;
}
