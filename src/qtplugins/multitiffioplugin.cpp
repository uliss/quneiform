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

#include "multitiffioplugin.h"
#include "multitiffiohandler.h"

MultiTIFFIOPlugin::MultiTIFFIOPlugin(QObject * parent) :
    QImageIOPlugin(parent)
{
}

QImageIOPlugin::Capabilities MultiTIFFIOPlugin::capabilities(QIODevice * device, const QByteArray& format) const
{
    if(format.toLower() == "mtiff")
        return CanRead;

    if(format.isEmpty()) {
        if(device->isOpen() &&
                device->isReadable() &&
                device->peek(2) == "II*")
            return CanRead;
    }

    return 0;
}

QImageIOHandler * MultiTIFFIOPlugin::create(QIODevice * device, const QByteArray& format) const
{
    if(capabilities(device, format) != CanRead)
        return NULL;

    MultiTIFFIOHandler * res = new MultiTIFFIOHandler;
    res->setDevice(device);
    return res;
}

QStringList MultiTIFFIOPlugin::keys() const
{
    return QStringList("mtiff");
}

Q_EXPORT_PLUGIN2(multitiff_imageplugin, MultiTIFFIOPlugin)

