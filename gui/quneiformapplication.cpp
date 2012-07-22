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

#include "metatyperegistrator.h"
#include "translationloader.h"
#include "quneiformapplication.h"
#include "config-version.h"
#include "guilog.h"
#include "iconutils.h"

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
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
    qInstallMsgHandler(guiMessageLogger);
#endif


    Q_INIT_RESOURCE(theme_faenza);
    Q_INIT_RESOURCE(theme_mac);
    Q_INIT_RESOURCE(theme_gnome);
    Q_INIT_RESOURCE(theme_oxygen);
    Q_INIT_RESOURCE(theme_human);
    Q_INIT_RESOURCE(theme_snowish);

    setOrganizationName("openocr.org");
    setApplicationName("Quneiform OCR");
    setApplicationVersion(CF_VERSION);
    iconThemeSetup();

#ifdef Q_WS_MAC
//    setAttribute(Qt::AA_DontShowIconsInMenus);
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
