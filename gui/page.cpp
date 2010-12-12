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

#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QMutexLocker>

#include "page.h"
#include "imagecache.h"

Page::Page(const QString& image_path) :
        image_path_(image_path), number_(0), state_flags_(NONE),
        is_selected_(false) {

    QPixmap pixmap;
    if(ImageCache::load(image_path_, &pixmap)) {
        is_null_ = false;
        image_size_ = pixmap.size();
    }
    else {
        is_null_ = true;
    }

    initRects();
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

Page::PageFlags Page::flags() const {
    return state_flags_;
}

bool Page::hasFlag(PageFlag flag) {
    return state_flags_ & flag;
}

QString Page::imagePath() const {
    return image_path_;
}

QSize Page::imageSize() const {
    return image_size_;
}

void Page::initRects() {
    rects_.clear();
    // pictures
    rects_ << Rectangles();
    // chars
    rects_ << Rectangles();
}

bool Page::isNull() const {
    return is_null_;
}

bool Page::isRecognized() const {
    return state_flags_ & RECOGNIZED;
}

bool Page::isSaved() const {
    return state_flags_ & SAVED;
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

const QRect& Page::pageArea() const {
    return page_area_;
}

const RecognitionSettings& Page::recognitionSettings() const {
    return rec_settings_;
}

const Page::Rectangles& Page::rects(RectType t) const {
    Q_ASSERT(t < rects_.size());

    return rects_.at(t);
}

void Page::resetScale() {
    QMutexLocker lock(&mutex_);

    QTransform t;
    t.rotate(angle());
    transform_ = t;
    emit changed();
    emit transformed();
}

void Page::rotate(int angle) {
    QMutexLocker lock(&mutex_);

    transform_.rotate(angle);
    emit changed();
    emit rotated(angle);
}

void Page::save(const QString& file) {
    QMutexLocker lock(&mutex_);

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
        setFlag(SAVING_FAILED);
        return;
    }

    output.write(ocr_text_.toLocal8Bit());
    output.close();

    qDebug() << "[Page::save] saved" << file;

    setFlag(SAVED);
    unsetFlag(SAVING_FAILED);
    emit saved();
}

void Page::scale(qreal factor) {
    QMutexLocker lock(&mutex_);

    transform_.scale(factor, factor);
    emit changed();
    emit transformed();
}

void Page::setFlag(PageFlag flag) {
    state_flags_ |= flag;
    emit changed();
}

void Page::setFlags(PageFlags flags) {
    state_flags_ = flags;
    emit changed();
}

void Page::setNumber(unsigned int number) {
    QMutexLocker lock(&mutex_);

    if(number_ == number)
        return;

    number_ = number;
    emit changed();
}

void Page::setOcrText(const QString& text) {
    QMutexLocker lock(&mutex_);

    ocr_text_ = text;

    setFlag(RECOGNIZED);
    unsetFlag(SAVED);

    emit changed();
    emit recognized();
}

void Page::setPageArea(const QRect& area) {
    QMutexLocker lock(&mutex_);

    if(page_area_ == area)
        return;

    page_area_ = area;
    emit changed();
}

void Page::setRecognizeOptions(const RecognitionSettings& opts) {
    QMutexLocker lock(&mutex_);

    if(rec_settings_ == opts)
        return;

    rec_settings_ = opts;
    unsetFlag(SAVED);
    emit changed();
}

void Page::setRects(const QList<QRect>& rects, RectType type) {
    Q_ASSERT(type < rects_.size());

    rects_[type] = rects;
}

void Page::setSelected(bool value) {
    if(is_selected_ == value)
        return;

    is_selected_ = value;
    emit changed();
}

void Page::setTransform(const QTransform& t) {
    QMutexLocker lock(&mutex_);

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

void Page::unsetFlag(PageFlag flag) {
    state_flags_ &= (~flag);
}

QPoint Page::viewScroll() const {
    return view_scroll_;
}

QDataStream& operator<<(QDataStream& os, const Page& page) {
    QMutexLocker lock(&page.mutex_);
    os << page.image_path_
            << page.image_size_
            << page.ocr_text_
            << page.number_
            << page.state_flags_
            << page.is_selected_
            << page.page_area_
            << page.transform_
            << page.is_null_
            << page.view_scroll_
            << page.rec_settings_;
    return os;
}

QDataStream& operator>>(QDataStream& is, Page& page) {
    QMutexLocker lock(&page.mutex_);
    is >> page.image_path_
            >> page.image_size_
            >> page.ocr_text_
            >> page.number_
            >> page.state_flags_
            >> page.is_selected_
            >> page.page_area_
            >> page.transform_
            >> page.is_null_
            >> page.view_scroll_
            >> page.rec_settings_;

    if(page.is_selected_)
        page.setSelected(true);

    return is;
}

QDataStream& operator<<(QDataStream& os, const Page::PageFlags& flags) {
    os << int(flags);
    return os;
}

QDataStream& operator>>(QDataStream& is, Page::PageFlags& flags) {
    int i = 0;
    is >> i;
    flags &= 0;
    flags |= static_cast<Page::PageFlag>(i);
    return is;
}
