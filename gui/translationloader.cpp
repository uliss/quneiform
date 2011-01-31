/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <locale.h>
#include <QtGlobal>
#include <QLocale>
#include <QLibraryInfo>
#include <QDebug>
#include <QApplication>

#ifdef Q_WS_MAC
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFBundle.h>
#endif

#include "config.h" // for INSTALL_DATADIR
#include "translationloader.h"

// static members init
QTranslator TranslationLoader::app_;
QTranslator TranslationLoader::system_;

TranslationLoader::TranslationLoader()
{
}

QTranslator * TranslationLoader::applicationTranslator() {
    return &app_;
}

void TranslationLoader::load() {
    loadSystemTranslation();
    loadApplicationTranslation();
}

void TranslationLoader::loadSystemTranslation() {
    qDebug() << Q_FUNC_INFO << "loading qt system translations for locale:" << locale_.name();
    bool res = system_.load("qt_" + locale_.name(),
                 QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    if(!res)
        qDebug() << Q_FUNC_INFO << "unable to load translation for locale:" << locale_.name();
}

void TranslationLoader::loadApplicationTranslation() {
    QStringList paths;
	paths << "gui";
	// assuming that application installed like that:
	//  /
	//   ---/bin
	//   ---/share
	//   ---/lib
	paths << QApplication::applicationDirPath() + "/../share/cuneiform/locale";
#if defined(Q_WS_MAC)
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                                kCFURLPOSIXPathStyle);
    const char * pathPtr = CFStringGetCStringPtr(macPath,
                                                CFStringGetSystemEncoding());
    CFRelease(appUrlRef);
    CFRelease(macPath);

    paths << QString(pathPtr) + QString("/Contents/Resources");  
#elif defined(Q_WS_X11)
    paths << INSTALL_DATADIR "/locale";
    paths << "/usr/share/cuneiform/locale";
#endif

    loadApplicationTranslations(paths);
}

bool TranslationLoader::loadApplicationTranslation(const QString& path) {
    QString tr_name = locale_.name();
    bool res = app_.load(tr_name, path);

    if(!res)
        qDebug() << Q_FUNC_INFO << "translation" << tr_name << "not found in" << path;

    return res;
}

void TranslationLoader::loadApplicationTranslations(const QStringList& paths) {
    qDebug() << Q_FUNC_INFO << "load application translations from paths:" << paths;

    foreach(QString path, paths) {
        if(loadApplicationTranslation(path))
            break;
    }
}

QTranslator * TranslationLoader::systemTranslator() {
    return &system_;
}
