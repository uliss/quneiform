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

#ifndef PDFIOPLUGIN_H
#define PDFIOPLUGIN_H

#include <QImageIOPlugin>

class PdfIOPlugin : public QImageIOPlugin
{
public:
    PdfIOPlugin(QObject * parent = 0);
    Capabilities capabilities(QIODevice * device, const QByteArray& format) const;
    QImageIOHandler * create(QIODevice * device, const QByteArray& format = QByteArray()) const;
    QStringList keys() const;
};

#endif // PDFIOPLUGIN_H
