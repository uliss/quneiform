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
#include <QDomDocument>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QCoreApplication>

#include "imagecapture.h"
#include "macbundle.h"

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

static bool isImageCapturePluginInstalled()
{
    return QFile::exists(pluginDestFile());
}

static QString applicationExePath()
{
    QString result = utils::applicationBundle();
    if(!result.endsWith(".app"))
        result = QCoreApplication::applicationFilePath();

    return result;
}

static bool isImageCapturePluginSameDestination()
{
    QString x_query = "<a>{doc($file)//key[text()=\"ActionParameters\"]/following-sibling::dict[1]/string/text()}</a>";
    QXmlQuery query;
    query.bindVariable("file", QVariant(pluginDestFile()));
    query.setQuery(x_query);

    QString plugin_app;

    if(query.isValid()) {
        QString res;
        query.evaluateTo(&res);
        res = res.trimmed();
        res.chop(4);
        res.remove(0, 3);
        plugin_app = res;

        QString this_app = applicationExePath();

        if(plugin_app == this_app)
            return true;
        else
            return false;
     }
    else
        return false;
}

static bool copyImageCapturePlugin()
{
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

    QFile src_file(PLUGIN_SRC);
    if(!src_file.open(QIODevice::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "can't open plugin source:" << PLUGIN_SRC;
        return false;
    }

    QString path = applicationExePath();

    QByteArray src_data = src_file.readAll().replace("${quneiform_bundle}", path.toAscii());

    QFile dest_file(pluginDestFile());
    if(!dest_file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        qWarning() << Q_FUNC_INFO << "can't open plugin destination:" << pluginDestFile();
        return false;
    }

    if(dest_file.write(src_data) == -1) {
        qWarning() << Q_FUNC_INFO << "plugin copy failed";
        return false;
    }

    return true;
}

bool openImageCapture()
{
    if(!isImageCapturePluginInstalled()) {
        qDebug() << Q_FUNC_INFO << "installing plugin";
        copyImageCapturePlugin();
    }
    else if(!isImageCapturePluginSameDestination()) {
        qDebug() << Q_FUNC_INFO << "overwriting plugin";
        copyImageCapturePlugin();
    }

    bool rc = QProcess::startDetached("open", QStringList(IMAGE_CAPTURE_APP));

    if(!rc)
        qCritical() << Q_FUNC_INFO << "can't start aplication:" << IMAGE_CAPTURE_APP;

    return rc;
}

}
