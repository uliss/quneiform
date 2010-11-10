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
#include <QDataStream>

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

int Page::angle() const {
    QPointF pt = transform_.map(QPointF(0, 1));
    if(pt.y() == 0)
        return pt.x() > 0 ? 270 : 90;
    else if(pt.x() == 0)
        return pt.y() > 0 ? 0 : 180;
    else
        return 0;
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

bool Page::isFormatConvertionNeeded(int format) const {
    if(format == QImage::Format_Mono)
        return false;
    return true;
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

const QRectF& Page::pageArea() const {
    return page_area_;
}

void Page::recognize() {
    using namespace cf;

    try {
        QtImageLoader loader;
        QImage img(image_path_);

        // convert to 24-bit
        if(isFormatConvertionNeeded(img.format()))
            img = img.convertToFormat(QImage::Format_RGB888);

        // select page area
        if(pageArea().isValid()) {
            img = img.copy(page_area_.x(),
                           page_area_.y(),
                           page_area_.width(),
                           page_area_.height());
        }

        // rotate
        if(angle() != 0) {
            QTransform t;
            t.rotate(angle());
            img = img.transformed(t);
        }

        ImagePtr image = loader.load(&img);
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
        emit changed();
    }
    catch(Exception& e) {
        QMessageBox::critical(NULL, tr("Quneiform OCR"),
                              tr("Error while recognizing \"%1\":\n%2").arg(imagePath()).arg(e.what()));
    }
}

void Page::resetScale() {
    QTransform t;
    t.rotate(angle());
    transform_ = t;
    emit changed();
    emit transformed();
}

void Page::rotate(int angle) {
    transform_.rotate(angle);
    emit changed();
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
    emit changed();
    emit transformed();
}

void Page::setLanguage(const QString& code) {
    if(cf::Language::byCode2(code.toStdString()).isValid()) {
        language_ = code;
        qDebug() << "[Page::setLanguage]" << code;
        emit changed();
    }
    else {
        qDebug() << "Invalid language code: " << code;
    }
}

void Page::setNumber(unsigned int number) {
    if(number_ == number)
        return;

    number_ = number;
    emit changed();
}

void Page::setPageArea(const QRectF& area) {
    if(page_area_ == area)
        return;

    page_area_ = area;
    emit changed();
}

void Page::setSelected(bool value) {
    if(is_selected_ == value)
        return;

    is_selected_ = value;
    emit changed();
}

void Page::setTransform(const QTransform& t) {
    if(transform_ == t)
        return;

    transform_ = t;
    emit changed();
    emit transformed();
}

void Page::setViewScroll(const QPoint& pt) {
    view_scroll_ = pt;
}

QTransform Page::transform() const {
    return  transform_;
}

QPoint Page::viewScroll() const {
    return view_scroll_;
}

QDataStream& operator<<(QDataStream& os, const Page& page) {
    os << page.image_path_
            << page.image_size_
            << page.ocr_text_
            << page.number_
            << page.is_recognized_
            << page.is_saved_
            << page.is_selected_
            << page.r_page_
            << page.r_fragment_
            << page.page_area_
            << page.language_
            << page.transform_
            << page.is_null_
            << page.view_scroll_;
    return os;
}

QDataStream& operator>>(QDataStream& is, Page& page) {
    is >> page.image_path_
            >> page.image_size_
            >> page.ocr_text_
            >> page.number_
            >> page.is_recognized_
            >> page.is_saved_
            >> page.is_selected_
            >> page.r_page_
            >> page.r_fragment_
            >> page.page_area_
            >> page.language_
            >> page.transform_
            >> page.is_null_
            >> page.view_scroll_;

    if(page.is_selected_)
        page.setSelected(true);

    return is;
}
