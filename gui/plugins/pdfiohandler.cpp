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

#include <QImage>
#include <QDebug>
#include <poppler-qt4.h>

#include "pdfiohandler.h"

PdfIOHandler::PdfIOHandler() :
    doc_(NULL),
    current_page_(0),
    quality_(72)
{
}

PdfIOHandler::~PdfIOHandler()
{
    delete doc_;
}

bool PdfIOHandler::canRead() const
{
    if(!device()->reset())
        return false;

    return device()->peek(4) == QByteArray("%PDF");
}

bool PdfIOHandler::read(QImage * image)
{
    if(!loadPDF())
        return false;

    Poppler::Page * page = doc_->page(currentImageNumber());
    if(!page)
        return false;

    double q = quality_;
    if(quality_ == -1)
        q = 72;

    *image = page->renderToImage(q, q);
    delete page;
    return true;
}

bool PdfIOHandler::write(const QImage& image)
{
    return false;
}

int PdfIOHandler::imageCount() const
{
    if(!loadPDF())
        return 0;

    return doc_->numPages();
}

QVariant PdfIOHandler::option(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case Size: {
        if(!loadPDF())
            return QSize();

        Poppler::Page * page = doc_->page(currentImageNumber());
        if(!page)
            return QVariant();

        QSize size = page->pageSize();
        size *= double(quality_) / 72.0;
        delete page;
        return size;
    } break;
    case BackgroundColor:
        if(!loadPDF())
            return QVariant();

        return doc_->paperColor();
    case Description: {
        if(!loadPDF())
            return QVariant();

        QStringList desc;
        foreach(const QString &key, doc_->infoKeys())
            desc << QString("%1:%2").arg(key).arg(doc_->info(key));

        return desc.join("\n\n");
    }
    case Name:
        if(!loadPDF())
            return QVariant();

        return doc_->info("Title");
    case Quality:
        return quality_;
    default:
        return QVariant();
    }
}

bool PdfIOHandler::loadPDF() const
{
    if(!doc_){
        doc_ = Poppler::Document::loadFromData(device()->readAll());

        if(!doc_ || doc_->isLocked()) {
            delete doc_;
            doc_ = 0;
            return false;
        } else {
            doc_->setRenderHint(Poppler::Document::Antialiasing, true);
            doc_->setRenderHint(Poppler::Document::TextAntialiasing, true);
        }
    }
    return true;
}

int PdfIOHandler::currentImageNumber() const
{
    return current_page_;
}

bool PdfIOHandler::jumpToImage(int imageNumber)
{
    if(!loadPDF())
        return false;

    if(imageNumber >= doc_->numPages())
        return false;

    current_page_ = imageNumber;
    return true;
}

bool PdfIOHandler::jumpToNextImage()
{
    if(!loadPDF())
        return false;

    if(current_page_ < doc_->numPages() - 1) {
        current_page_++;
        return true;
    }

    return false;
}


bool PdfIOHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case ScaledSize:
    case Size:
    case Description:
    case BackgroundColor:
    case Quality:
    case Name:
        return true;
    default: return false;
    }
}

void PdfIOHandler::setOption(QImageIOHandler::ImageOption option, const QVariant& value)
{
    switch(option) {
    case BackgroundColor:
        if(!loadPDF())
            return;
        doc_->setPaperColor(qvariant_cast<QColor>(value));
        break;
   case Quality:
        quality_ = value.toInt();
        break;
    default:
        break;
    }
}
