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

#include <QFile>
#include <QDebug>
#include <QTextDocument>
#include <QTextDocumentWriter>

#include "nativeexporter.h"
#include "page.h"

NativeExporter::NativeExporter(const ExportSettings& s) :
    IQfExporter(s)
{
}

void NativeExporter::exportTo(Page * p, const QString& fname) {
    Q_CHECK_PTR(p);

    if(fname.isEmpty())
        throw ExporterException("File name is empty");

    if(p->document()->isEmpty())
        throw ExporterException("Document is empty");

    QByteArray format;
    switch(settings().format()) {
    case ExportSettings::PLAINTEXT:
        format = "plaintext";
        break;
    case ExportSettings::ODF:
        format = "odf";
        break;
    case ExportSettings::HTML:
        format = "html";
        break;
    default:
        throw ExporterException("Invalid export format");
        break;
    }

    QTextDocumentWriter writer(fname, format);

    if(!writer.write(p->document()))
        throw ExporterException("Export failed");

    qDebug() << Q_FUNC_INFO << "saved to:" << fname;
}
