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

#include <QApplication>
#include <QStringList>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "mainwindow.h"
#include "config.h" // for INSTALL_DATADIR

#ifdef Q_WS_MAC
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFBundle.h>
#endif

int main(int argc, char * argv[]) {
#ifdef Q_WS_X11
    QApplication::setGraphicsSystem("raster");
#endif

    QApplication app(argc, argv);

    // load system translation
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                 QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // load application translation
    QTranslator translator;
    QString tr_path;
#ifdef Q_WS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                                kCFURLPOSIXPathStyle);
    const char * pathPtr = CFStringGetCStringPtr(macPath,
                                                CFStringGetSystemEncoding());
    CFRelease(appUrlRef);
    CFRelease(macPath);

    tr_path = pathPtr;
    tr_path += "/Contents/Resources";
#else
    tr_path = INSTALL_DATADIR;
#endif

    translator.load(QLocale::system().name(), tr_path);
    app.installTranslator(&translator);

    app.setOrganizationName("openocr.org");
    app.setApplicationName("Quneiform OCR");
    app.setApplicationVersion("0.0.1");
    MainWindow w;
    w.show();

    if(argc > 1) {
        QStringList files;
        for(int i = 1; i < argc; i++)
            files << QString::fromLocal8Bit(argv[i]);

        w.openImages(files);
    }

    return app.exec();
}
