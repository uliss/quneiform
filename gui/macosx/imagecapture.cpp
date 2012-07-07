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

#include <QProcess>
#include <QDebug>
#include <QDir>

#include "imagecapture.h"

namespace utils
{

static const char * IMAGE_CAPTURE_APP = "/Applications/Image Capture.app";
static const char * PLUGIN_DEST_DIR =
        "Library/Workflows/Applications/Image Capture/Quneiform.workflow/Contents";
static const char * PLUGIN_SRC = ":/macosx/ImageCapturePlugin.wflow";

static QString pluginDestDir()
{
    return QString("%1/%2").arg(QDir::homePath()).arg(QString::fromUtf8(PLUGIN_DEST_DIR));
}

static QString pluginDestFile()
{
    return QString("%1/%2").arg(pluginDestDir()).arg("document.wflow");
}

static bool copyImageCapturePlugin()
{
    if(QFile::exists(pluginDestFile()))
        return true;

    if(!QFile::exists(PLUGIN_SRC)) {
        qWarning() << Q_FUNC_INFO << "Image Capture plugin not found in resources";
        return false;
    }

    if(!QFile::exists(pluginDestDir())) {
        if(!QDir().mkpath(pluginDestDir())) {
            qWarning() << Q_FUNC_INFO << "Can't create Image Capture plugin directory:" << pluginDestDir();
            return false;
        }

        qDebug() << Q_FUNC_INFO << "plugin folder created:" << pluginDestDir();
    }

    if(!QFile::copy(PLUGIN_SRC, pluginDestFile())) {
        qWarning() << Q_FUNC_INFO << "plugin copy failed";
        return false;
    }

    return true;
}

bool openImageCapture()
{
    if(!QFile::exists(pluginDestFile()))
        copyImageCapturePlugin();

    bool rc = QProcess::startDetached("open", QStringList(IMAGE_CAPTURE_APP));

    if(!rc)
        qCritical() << Q_FUNC_INFO << "can't start aplication:" << IMAGE_CAPTURE_APP;

    return rc;
}

}
