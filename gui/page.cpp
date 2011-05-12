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

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QMutexLocker>
#include <QPixmap>
#include <QTextDocument>

#include <string>
#include <sstream>

#include "ced/cedpage.h"
#include "page.h"
#include "imagecache.h"
#include "cedserializer.h"

Page::Page(const QString& image_path) :
        image_path_(image_path),
        number_(0),
        state_flags_(NONE),
        is_selected_(false),
        angle_(0),
        view_scale_(1.0),
        cedpage_(NULL)
{
    QPixmap pixmap;
    if(ImageCache::load(image_path_, &pixmap)) {
        is_null_ = false;
        image_size_ = pixmap.size();
    }
    else {
        is_null_ = true;
    }

    initRects();

    doc_ = new QTextDocument(this);
}

Page::~Page() {
    delete cedpage_;
}

int Page::angle() const {
    return angle_;
}

void Page::appendBlock(const QRect& rect, BlockType type) {
    Q_ASSERT(type < blocks_.size());
    blocks_[type].append(rect);
}

cf::CEDPage * Page::cedPage() {
    return cedpage_;
}

void Page::clearBlocks() {
    for(int i = 0; i < blocks_.count(); i++)
        blocks_[i].clear();
}

void Page::clearBlocks(BlockType type) {
    Q_ASSERT(type < blocks_.size());
    blocks_[type].clear();
}

void Page::clearLayout() {
    blockSignals(true);
    clearBlocks();
    setPageArea(QRect());
    blockSignals(false);
    emit layoutCleared();
}

QTextDocument * Page::document() {
    return doc_;
}

Page::PageFlags Page::flags() const {
    return state_flags_;
}

const FormatSettings& Page::formatSettings() const {
    return format_settings_;
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
    blocks_.clear();
    // pictures
    blocks_ << Rectangles();
    // chars
    blocks_ << Rectangles();
    // lines
    blocks_ << Rectangles();
    // paragraphs
    blocks_ << Rectangles();
    // columns
    blocks_ << Rectangles();
    // sections
    blocks_ << Rectangles();
}

bool Page::isNull() const {
    return is_null_;
}

bool Page::isRecognized() const {
    return state_flags_ & RECOGNIZED;
}

bool Page::isExported() const {
    return state_flags_ & EXPORTED;
}

bool Page::isSelected() const {
    return is_selected_;
}

Language Page::language() const {
    return language_;
}

QString Page::name() const {
    QFileInfo inf(image_path_);
    return inf.fileName();
}

unsigned int Page::number() const {
    return number_;
}

const QRect& Page::pageArea() const {
    return page_area_;
}

const RecognitionSettings& Page::recognitionSettings() const {
    return rec_settings_;
}

const Page::Rectangles& Page::blocks(BlockType t) const {
    Q_ASSERT(t < blocks_.size());

    return blocks_.at(t);
}

int Page::blocksCount(BlockType t) const {
    Q_ASSERT(t < blocks_.size());
    return blocks_.at(t).count();
}

void Page::resetViewScale() {
    setViewScale(1.0);
}

void Page::rotate(int angle) {
    setAngle(angle_ + angle);
}

void Page::exportTo(const QString& file) {
    QMutexLocker lock(&mutex_);

//    if (!isRecognized())
//        throw Exception("[Page::save] page is not recognized");

//    QFile output(file);

//    if(!output.open(QIODevice::WriteOnly)) {
//        state_flags_ |= EXPORT_FAILED;
//        state_flags_ &= (~EXPORTED);

//        throw Exception(tr("Saved failed. Can't open file \"%1\" for writing.").arg(file));
//    }

//    output.write(ocr_text_.toLocal8Bit());
//    output.close();

//    qDebug() << "[Page::save] saved" << file;

    state_flags_ |= EXPORTED;
    state_flags_ &= (~EXPORT_FAILED);
    emit exported();
}

void Page::scaleView(qreal factor) {
    setViewScale(view_scale_ * factor);
}

void Page::setAngle(int angle) {
    QMutexLocker lock(&mutex_);

    angle_ = (360 + (angle % 360)) % 360;
    emit changed();
    emit rotated(angle_);
}

void Page::setCEDPage(cf::CEDPage * page) {
    delete cedpage_;
    cedpage_ = page;
}

void Page::setFlag(PageFlag flag) {
    state_flags_ |= flag;
    emit changed();
}

void Page::setFlags(PageFlags flags) {
    state_flags_ = flags;
    emit changed();
}

void Page::setFormatSettings(const FormatSettings& settings) {
    format_settings_ = settings;
    emit changed();
}

void Page::setLanguage(const Language& lang) {
    QMutexLocker lock(&mutex_);

    if(language_ == lang)
        return;
    else
        language_ = lang;

    emit changed();
}

void Page::setNumber(unsigned int number) {
    QMutexLocker lock(&mutex_);

    if(number_ == number)
        return;

    number_ = number;
    emit changed();
}

void Page::setPageArea(const QRect& area) {
    QMutexLocker lock(&mutex_);

    if(page_area_ == area)
        return;

    page_area_ = area;
    emit changed();
}

void Page::setRecognitionSettings(const RecognitionSettings& opts) {
    QMutexLocker lock(&mutex_);

    if(rec_settings_ == opts)
        return;

    rec_settings_ = opts;
    state_flags_ &= (~EXPORTED);
    emit changed();
}

void Page::setBlocks(const QList<QRect>& rects, BlockType type) {
    Q_ASSERT(type < blocks_.size());

    clearBlocks(type);

    foreach(QRect r, rects) {
        page_area_.topLeft();
        blocks_[type].append(r);
    }

    emit changed();
}

void Page::setSelected(bool value) {
    if(is_selected_ == value)
        return;

    is_selected_ = value;
    emit changed();
}

void Page::setViewScale(float scale) {
    view_scale_ = scale;
    emit viewScaled();
}

void Page::setViewScroll(const QPoint& pt) {
    view_scroll_ = pt;
}

void Page::unsetFlag(PageFlag flag) {
    if(state_flags_ & flag) {
        state_flags_ &= (~flag);
        emit changed();
    }
}

float Page::viewScale() const {
    return view_scale_;
}

QPoint Page::viewScroll() const {
    return view_scroll_;
}

QDataStream& operator<<(QDataStream& os, const Page& page) {
    QMutexLocker lock(&page.mutex_);
    os << page.image_path_
            << page.image_size_
            << page.number_
            << page.state_flags_
            << page.is_selected_
            << page.page_area_
            << page.angle_
            << page.view_scale_
            << page.view_scroll_
            << page.is_null_
            << page.rec_settings_
            << page.blocks_
            << page.format_settings_
            << page.language_;

    CEDSerializer ced(page.cedpage_);
    os << ced;

    return os;
}

QDataStream& operator>>(QDataStream& is, Page& page) {
    QMutexLocker lock(&page.mutex_);
    is >> page.image_path_
            >> page.image_size_
            >> page.number_
            >> page.state_flags_
            >> page.is_selected_
            >> page.page_area_
            >> page.angle_
            >> page.view_scale_
            >> page.view_scroll_
            >> page.is_null_
            >> page.rec_settings_
            >> page.blocks_
            >> page.format_settings_
            >> page.language_;

    CEDSerializer ced;
    is >> ced;
    page.setCEDPage(ced.page());

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
