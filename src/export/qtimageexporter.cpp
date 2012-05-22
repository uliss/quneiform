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

#include <QImageWriter>
#include <QBuffer>
#include <QImage>
#include <sstream>

#include "qtimageexporter.h"
#include "bmpimageexporter.h"
#include "imageexporterfactory.h"
#include "common/debug.h"
#include "common/imagerawdata.h"
#include "common/helper.h"

namespace {

cf::ImageExporterPtr create() {
    return cf::ImageExporterPtr(new cf::QtImageExporter());
}

bool registerCreator() {
    cf::ImageExporterFactory::instance().registerCreator(cf::FORMAT_PNG, &create, 50);
    cf::ImageExporterFactory::instance().registerCreator(cf::FORMAT_JPEG, &create, 50);
    return true;
}

bool registered = registerCreator();

}

namespace cf {

QtImageExporter::QtImageExporter(image_format_t format)
{
    setFormat(format);
}

std::string QtImageExporter::mime() const {
    switch (format()) {
    case FORMAT_BMP:
        return "image/x-ms-bmp";
    case FORMAT_PNG:
        return "image/png";
    case FORMAT_GIF:
        return "image/gif";
    case FORMAT_JPEG:
        return "image/jpeg";
    default:
        return "";
    }
}

ImageExporter::FormatList QtImageExporter::supportedFormats() const
{
    FormatList formats;

    formats.push_back(FORMAT_BMP);

    foreach(const QByteArray& b, QImageWriter::supportedImageFormats()) {
        if(b.toUpper() == "JPG" || b.toUpper() == "JPEG")
            formats.push_back(FORMAT_JPEG);

        if(b.toUpper() == "PNG")
            formats.push_back(FORMAT_PNG);
    }

    return formats;
}

void QtImageExporter::saveToStream(const ImageRawData& image, std::ostream& os)
{
    if (image.isNull())
        throw Exception("[QtImageExporter::save] null image given");

    if (os.fail())
        throw Exception("[QtImageExporter::save] invalid stream");

    try {
        BmpImageExporter bmp_exporter;
        QImage tmp;

        if(format() == FORMAT_BMP) {
            bmp_exporter.save(image, os);
            return;
        } else {
            std::stringstream buf;
            bmp_exporter.save(image, buf);

            const int buf_size = (int) streamSize(buf);
            char * chbuf = new char[buf_size];
            buf.read(chbuf, buf_size);

            if(!tmp.loadFromData((uchar*) chbuf, buf_size)) {
                Debug() << "[QtImageExporter::save] image load error\n";
                throw Exception("[QtImageExporter::save] can't load image");
            }

            delete [] chbuf;
        }

        QImageWriter writer;

        switch(format()) {
        case FORMAT_PNG:
            writer.setFormat("PNG");
            break;
        case FORMAT_JPEG:
            writer.setFormat("JPEG");
            break;
        default:
            Debug() << format() << "\n";
            throw Exception("Unsupported format");
        }

        QBuffer buffer;
        if(!buffer.open(QBuffer::ReadWrite))
            throw Exception("[QtImageExporter::save] can't open buffer");

        writer.setDevice(&buffer);
        writer.setText("", "Generator: cuneiform");

        if(!writer.write(tmp))
            throw Exception("[QtImageExporter::save] write error");

        os.write(buffer.data(), buffer.size());
    }
    catch (ImageExporter::Exception& e) {
        Debug() << e.what() << std::endl;
        throw e;
    }
}

}
