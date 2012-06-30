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

#ifndef PDFIOHANDLER_H
#define PDFIOHANDLER_H

#include <QImageIOHandler>
#include <QRect>
#include <QSize>

namespace Poppler {
class Document;
}

class PdfIOHandler : public QImageIOHandler
{
public:
    PdfIOHandler();
    ~PdfIOHandler();
    bool canRead () const;
    bool read(QImage * image);
    bool write(const QImage & image);

    int imageCount() const;
    int currentImageNumber() const;
    bool jumpToImage(int imageNumber);
    bool jumpToNextImage();

    QVariant option(ImageOption option) const;
    bool supportsOption(ImageOption option) const;
    void setOption(ImageOption option, const QVariant& value);
private:
    bool loadPDF() const;
private:
    mutable Poppler::Document * doc_;
    int current_page_;
    int quality_;
};

#endif // PDFIOHANDLER_H
