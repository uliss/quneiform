/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include <QMutexLocker>
#include <sstream>

#include "pagerecognizer.h"
#include "page.h"
#include "cuneiform.h"
#include "rdib/qtimageloader.h"

inline QRect toRect(const QRectF& r) {
    return QRect(r.x(), r.y(), r.width(), r.height());
}

PageRecognizer::PageRecognizer(Page *p, QObject * parent)
    : QThread(parent), page_(p) {
}

void PageRecognizer::doRecognize() {
    qDebug() << Q_FUNC_INFO;
    cf::Puma::instance().recognize();
}

void PageRecognizer::formatResult() {
    qDebug() << Q_FUNC_INFO;
    cf::Puma::instance().formatResult();
}

bool PageRecognizer::isFormatConvertionNeeded(int format) const {
    // on mono image segfault sometimes occures
    return true;
//    return format != QImage::Format_Mono;
}

QImage PageRecognizer::loadImage() const {
    Q_CHECK_PTR(page_);

    qDebug() << Q_FUNC_INFO;
    QImage img(page_->imagePath());

    // select page area
    if(page_->pageArea().isValid())
        img = img.copy(toRect(page_->pageArea()));

    // rotate
    if(page_->angle() != 0) {
        QTransform t;
        t.rotate(page_->angle());
        img = img.transformed(t);
    }

    // convert to 24-bit
    if(isFormatConvertionNeeded(img.format()))
        img = img.convertToFormat(QImage::Format_RGB888);

    return img;
}

void PageRecognizer::openImage() {
    cf::QtImageLoader loader;
    QImage img = loadImage();

    cf::ImagePtr image = loader.load(&img);
    if (!image)
        throw Page::Exception("[PageRecognizer::openImage] can't open image");

    cf::Puma::instance().open(image);
}

void PageRecognizer::run() {
    recognize();
}

void PageRecognizer::recognize() {
    try {
        {
//            QMutexLocker lock(page_->mutex());
            setRecognizeOptions();
            openImage();
            emit finished(20);
            doRecognize();
            emit finished(90);
            formatResult();
        }

        saveOcrText();
        emit finished(100);
    }
    catch(std::exception& e) {
        emit failed(e.what());
    }
}

void PageRecognizer::saveOcrText() {
    Q_CHECK_PTR(page_);

    qDebug() << Q_FUNC_INFO;
    std::ostringstream buf;
    cf::Puma::instance().save(buf, cf::FORMAT_TEXT);
    page_->setOcrText(QString::fromUtf8(buf.str().c_str()));
}

void PageRecognizer::setLanguage(int language) {
    language_ = language;
}

void PageRecognizer::setPage(Page * p) {
    Q_CHECK_PTR(p);

    page_ = p;
}

void PageRecognizer::setRecognizeOptions() {
    cf::RecognizeOptions recognize_options;
    recognize_options.setLanguage(static_cast<language_t>(language_));
    cf::Puma::instance().setRecognizeOptions(recognize_options);
    cf::Config::instance().setDebug(false);
}
