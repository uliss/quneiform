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

#include "rdib/qtimageloader.h"
#include "cuneiform.h"
#include "rfrmt/rfrmtoptions.h"
#include "rfrmt/crtfchar.h"
#include "rfrmt/crtfpage.h"
#include "page.h"


QColor Page::format_page_color_(0, 255, 0, 200);

Page::Page(unsigned int number, const QString& image_path) :
    image_path_(image_path), number_(number), is_recognized_(false), is_saved_(false),
            is_selected_(false), language_("en") {
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

    //    QtImageLoader loader;
    ImagePtr image = ImageLoaderFactory::instance().load(image_path_.toStdString());
    if (!image)
        throw Exception("[Page::recognize] can't open image");

    // updates image size
    setImageSize(QSize(image->width(), image->height()));

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

void Page::rotate(int angle) {
	transform_.rotate(angle);
	emit rotated(angle);
}

void Page::save(const QString& /*file*/) {
    if (!isRecognized())
        throw Exception("[Page::save] page is not recognized");

    is_saved_ = true;
}

void Page::scale(qreal factor) {
	transform_.scale(factor, factor);
	emit transformed();
}

void Page::setImageSize(const QSize& size) {
    image_size_ = size;
}

void Page::setLanguage(const QString& code) {
	if(cf::Language::byCode2(code.toStdString()).isValid())
		language_ = code;
	else
		qDebug() << "Invalid language code: " << code;
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

