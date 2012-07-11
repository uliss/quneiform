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

#include <QtPlugin>
#include <QDebug>

#include "dibimageioplugin.h"
#include "dibimageiohandler.h"

DIBImageIOPlugin::DIBImageIOPlugin(QObject * parent) :
    QImageIOPlugin(parent)
{
}

QImageIOPlugin::Capabilities DIBImageIOPlugin::capabilities(QIODevice * device,
                                                            const QByteArray& format) const
{
    if(format.toLower() == "dib")
        return CanRead;

    if(format.isEmpty()) {
        DIBImageIOHandler handler;
        handler.setDevice(device);
        if (handler.canRead())
            return CanRead;
    }

    return 0;
}

QImageIOHandler * DIBImageIOPlugin::create(QIODevice * device, const QByteArray& format) const
{
    return new DIBImageIOHandler;
}

QStringList DIBImageIOPlugin::keys() const
{
    return QStringList("dib");
}

Q_EXPORT_PLUGIN2(dib_imageplugin, DIBImageIOPlugin)
