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

#include <sstream>
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QMessageBox>
#include <QFile>

#include "rdib/qtimageloader.h"
#include "cuneiform.h"
#include "rfrmt/rfrmtoptions.h"
#include "rfrmt/crtfchar.h"
#include "rfrmt/crtfpage.h"
#include "page.h"
#include "imagecache.h"


QColor Page::format_page_color_(0, 255, 0, 200);

Page::Page(const QString& image_path) :
        image_path_(image_path), number_(0), is_recognized_(false), is_saved_(false),
        is_selected_(false), language_("en") {

    QPixmap pixmap;
    if(ImageCache::load(image_path_, &pixmap)) {
        is_null_ = false;
        image_size_ = pixmap.size();
    }
    else {
        is_null_ = true;
    }
}

void Page::drawFormatLayout(QGraphicsScene * scene) const {
    Q_CHECK_PTR(scene);
    drawFormatPageLayout(scene);
}

void Page::drawFormatPageLayout(QGraphicsScene * scene) const {
    QPen pen(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor(format_page_color_);

    for (int i = 0; i < r_page_.count(); i++) {
        QGraphicsItem * r = scene->addRect(r_page_[i], pen);
        r->setZValue(i + 1);
        r ->setToolTip(QString("Page layout: %1x%2").arg(r_page_[i].width()).arg(
                r_page_[i].height()));
    }
}

void Page::fillFormatLayout(const cf::CRtfPage * page) {
    Q_CHECK_PTR(page);

    r_page_.append(QRect(QPoint(page->m_rect.left, page->m_rect.top), QPoint(page->m_rect.right,
                                                                             page->m_rect.bottom)));
}

QString Page::imagePath() const {
    return image_path_;
}

QSize Page::imageSize() const {
    return image_size_;
}

bool Page::isNull() const {
    return is_null_;
}

bool Page::isRecognized() const {
    return is_recognized_;
}

bool Page::isSaved() const {
    return is_saved_;
}

bool Page::isSelected() const {
    return is_selected_;
}

unsigned int Page::number() const {
    return number_;
}

QString Page::ocrText() const {
    return ocr_text_;
}

void Page::recognize() {
    using namespace cf;

    try {
    	//    QtImageLoader loader;
    	ImagePtr image = ImageLoaderFactory::instance().load(image_path_.toLocal8Bit().data());
    	if (!image)
            throw Exception("[Page::recognize] can't open image");

    	RecognizeOptions recognize_options;
    	recognize_options.setLanguage(Language::byCode2(language_.toStdString()).get());
    	//	recognize_options.setOneColumn(do_singlecolumn);
    	//	recognize_options.setFax(do_fax);
    	//	recognize_options.setDotMatrix(do_dotmatrix);
    	//	recognize_options.setSpellCorrection(do_speller);
    	//	recognize_options.setAutoRotate(do_autorotate);
    	//	recognize_options.setPictureSearch(do_pictures);
    	//  Puma::instance().setOptionTables(puma_table_t mode);
    	Puma::instance().setRecognizeOptions(recognize_options);

    	Puma::instance().open(image);
    	Puma::instance().recognize();
    	Puma::instance().formatResult();

    	//fillFormatLayout(Puma::instance().formatPage());

    	std::ostringstream buf;
    	Puma::instance().save(buf, FORMAT_HTML);

    	ocr_text_ = QString::fromUtf8(buf.str().c_str());
    	is_recognized_ = true;
    }
    catch(Exception& e) {
        QMessageBox::critical(NULL, tr("Quneiform OCR"),
                              tr("Error while recognizing \"%1\":\n%2").arg(imagePath()).arg(e.what()));
    }
}

void Page::rotate(int angle) {
    transform_.rotate(angle);
    emit rotated(angle);
}

void Page::save(const QString& file) {
    if (!isRecognized())
        throw Exception("[Page::save] page is not recognized");

    QFile output(file);

    if(output.exists()) {
        int ret = QMessageBox::question(NULL,
                                        tr("Quneiform OCR"),
                                        tr("File already exists. Overwrite?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::Yes);
        switch(ret) {
        case QMessageBox::No:
            qDebug() << "[Page::save] canceled for" << file;
            return;
        case QMessageBox::Yes:
        default:
            qDebug() << "[Page::save] overwriting" << file;
            break;
        }
    }

    if(!output.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(NULL,
                              tr("Quneiform OCR"),
                              tr("Saved failed. Can't open file \"%1\" for writing.").arg(file));
    }

    output.write(ocr_text_.toLocal8Bit());
    output.close();

    qDebug() << "[Page::save] saved" << file;

    is_saved_ = true;
}

void Page::scale(qreal factor) {
    transform_.scale(factor, factor);
    emit transformed();
}

void Page::setLanguage(const QString& code) {
    if(cf::Language::byCode2(code.toStdString()).isValid()) {
        language_ = code;
        qDebug() << "[Page::setLanguage]" << code;
    }
    else {
        qDebug() << "Invalid language code: " << code;
    }
}

void Page::setNumber(unsigned int number) {
    number_ = number;
}

void Page::setSelected(bool value) {
    is_selected_ = value;
}

void Page::setTransform(const QTransform& t) {
    if(transform_ != t) {
        transform_ = t;
        emit transformed();
    }
}

QTransform Page::transform() const {
    return  transform_;
}

