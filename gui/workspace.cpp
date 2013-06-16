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

#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>

#include "workspace.h"
#include "settingskeys.h"
#include "scan/scandialogtypes.h"

#ifdef Q_WS_MAC
#include "macosx/macopenfile.h"
#include "macosx/macdocktile.h"
#endif

bool Workspace::openFileWithApplication(const QString& filePath, const QString& applicationPath)
{
    if(!QFile::exists(filePath)) {
        qDebug() << Q_FUNC_INFO << "file not exits:" << filePath;
        return false;
    }

#ifdef Q_WS_MAC
    QFileInfo fi(applicationPath);
    if(!fi.exists()) {
        qDebug() << Q_FUNC_INFO << "application not exists:" << applicationPath;
        return false;
    }

    if(fi.isBundle())
        return utils::macOpenFileWithApplication(filePath, fi.bundleName());
    else
        return QProcess::startDetached(applicationPath, QStringList(filePath));

#else
    return QProcess::startDetached(applicationPath, QStringList(filePath));
#endif
}

QString Workspace::showChooseApplicationDialog()
{
    return QFileDialog::getOpenFileName(0,
                                       "Select application",
                                       QDesktopServices::storageLocation(QDesktopServices::ApplicationsLocation),
                                       "");

}

bool Workspace::launchApplication(const QString& appPath)
{
#ifdef Q_OS_MAC
    return utils::macLaunchApplication(appPath);
#else
    return QProcess::startDetached(appPath);
#endif
}

static QString platformExternalScanApp()
{
    QString res;

#ifdef Q_OS_MAC
    res = "/Applications/Image Capture.app";
#endif

    return res;
}

static void initPlatformExternalScanApp()
{
    QSettings s;
    if(!s.contains(KEY_SCAN_EXTERNAL_APP))
        s.setValue(KEY_SCAN_EXTERNAL_APP, platformExternalScanApp());
}

static int platformScanType()
{
#ifdef Q_OS_MAC
    return SCAN_DIALOG_EXTERNAL_APP;
#else
    return SCAN_DIALOG_QUNEIFORM;
#endif
}

static void initPlatformScanType()
{
    QSettings s;
    if(!s.contains(KEY_SCAN_DIALOG_TYPE))
        s.setValue(KEY_SCAN_DIALOG_TYPE, platformScanType());
}

void Workspace::initPlatformDefaultSettings()
{
    initPlatformScanType();
    initPlatformExternalScanApp();
}

void Workspace::alertFailed(const QString& msg)
{
#ifdef Q_OS_MAC
    utils::setDockBadge(msg);
#else
    qWarning() << Q_FUNC_INFO << msg;
#endif
}
