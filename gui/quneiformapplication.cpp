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
#include <QDebug>
#include <QtPlugin>
#include <QIcon>
#include <QDir>

#include "metatyperegistrator.h"
#include "translationloader.h"
#include "quneiformapplication.h"
#include "config-version.h"
#include "guilog.h"
#include "iconutils.h"
#include "theme-config.h"
#include "workspace.h"

Q_IMPORT_PLUGIN(dib_imageplugin)

#ifdef WITH_TIFF
Q_IMPORT_PLUGIN(multitiff_imageplugin)
#endif

#ifdef WITH_PDF
Q_IMPORT_PLUGIN(pdf_imageplugin)
#endif

QuneiformApplication::QuneiformApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
    TranslationLoader loader;
    loader.load();
    installTranslator(loader.systemTranslator());
    installTranslator(loader.applicationTranslator());

    platformInit();
    resourcesInit();
    stylesheetInit();

    setOrganizationName("openocr.org");
    setApplicationName("Quneiform OCR");
    setApplicationVersion(CF_VERSION);
    iconThemeSetup();
    Workspace::initPlatformDefaultSettings();

    MetaTypeRegistrator registrator;
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
    default:
        processed = QApplication::event(ev);
        break;
    }
    return processed;
}

void QuneiformApplication::addBundlePluginPath()
{
    QDir dir(applicationDirPath());
    dir.cdUp();
    dir.cd("PlugIns");
    addLibraryPath(dir.absolutePath());
}

void QuneiformApplication::platformInit()
{
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
    qInstallMsgHandler(guiMessageLogger);
#endif

#ifdef Q_WS_MAC
    setAttribute(Qt::AA_DontShowIconsInMenus);
    addBundlePluginPath();
#endif
}

void QuneiformApplication::resourcesInit()
{
    Q_INIT_RESOURCE(theme_oxygen);

#ifdef WITH_THEME_FAENZA
    Q_INIT_RESOURCE(theme_faenza);
#endif

#ifdef WITH_THEME_MAC
    Q_INIT_RESOURCE(theme_mac);
#endif

#ifdef WITH_THEME_GNOME
    Q_INIT_RESOURCE(theme_gnome);
#endif

#ifdef WITH_THEME_HUMAN
    Q_INIT_RESOURCE(theme_human);
#endif

#ifdef WITH_THEME_SNOWISH
    Q_INIT_RESOURCE(theme_snowish);
#endif
}

void QuneiformApplication::stylesheetInit()
{
    QString filename;
#ifdef Q_OS_MAC
    filename = ":/macosx/style.css";
#endif

#ifdef Q_OS_LINUX
    filename = ":/linux/style.css";
#endif

    QFile f(filename);
    if(!f.open(QFile::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "Can't open stylesheet file";
        return;
    }

    qApp->setStyleSheet(f.readAll());
}
