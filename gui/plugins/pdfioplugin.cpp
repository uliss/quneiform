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

#include "pdfioplugin.h"
#include "pdfiohandler.h"

PdfIOPlugin::PdfIOPlugin(QObject * parent) :
    QImageIOPlugin(parent)
{
}

QImageIOPlugin::Capabilities PdfIOPlugin::capabilities(QIODevice * device, const QByteArray& format) const
{
    if(format.toLower() == "pdf")
        return CanRead;

    if(!format.isEmpty())
        return 0;

    if(device->isOpen() &&
            device->isReadable() &&
            device->peek(4) == "PDF")
        return CanRead;

    return 0;
}

QImageIOHandler * PdfIOPlugin::create(QIODevice * device, const QByteArray& format) const
{
    QImageIOHandler * res = new PdfIOHandler;
    res->setDevice(device);
    res->setFormat(format);
    return res;
}

QStringList PdfIOPlugin::keys() const
{
    return QStringList("pdf");
}

Q_EXPORT_PLUGIN2(pdf_imageplugin, PdfIOPlugin)

