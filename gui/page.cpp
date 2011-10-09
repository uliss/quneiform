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

#include "puma/puma.h"
#include "ced/cedpage.h"
#include "page.h"
#include "imagecache.h"
#include "exportsettings.h"
#include "export/cedserializer.h"
#include "export/qtextdocumentexporter.h"
#include "export/rectexporter.h"
#include "export/exporterfactory.h"

static const int THUMB_IMAGE_HEIGHT = 100;
static const int THUMB_IMAGE_WIDTH = 100;

static language_t languageToType(const Language& lang) {
    if(lang.isValid()) {
        return static_cast<language_t>(lang.code());
    }
    else {
        qDebug() << "[Warning] Language is not valid, using English instead";
        return ::LANGUAGE_ENGLISH;
    }
}

Page::Page(const QString& image_path) :
    image_path_(image_path),
    state_flags_(NONE),
    angle_(0),
    view_scale_(1.0),
    doc_(NULL),
    thumb_(NULL)
{
    initThumb();
    initRects();
    initDocument();
}

Page::~Page() {
    delete thumb_;
}

int Page::angle() const {
    return angle_;
}

void Page::appendBlock(const QRect& rect, BlockType type) {
    Q_ASSERT(type < blocks_.size());
    blocks_[type].append(rect);
}

cf::CEDPagePtr Page::cedPage() {
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

void Page::exportTo(const QString& file, const ExportSettings& s) {
    QMutexLocker lock(&mutex_);

    if (!isRecognized())
        throw Exception("[Page::exportTo] page is not recognized");

    try {
        ExporterPtr exp = ExporterFactory::instance().make(s);
        exp->exportTo(this, file);
    }
    catch(ExporterException& e) {
        std::cerr << e.what() << std::endl;
        throw Exception(e.what());
    }

    state_flags_ |= EXPORTED;
    state_flags_ &= (~EXPORT_FAILED);
    emit exported();
}

Page::PageFlags Page::flags() const {
    return state_flags_;
}

const FormatSettings& Page::formatSettings() const {
    return format_settings_;
}

bool Page::hasFlag(PageFlag flag) const {
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
    return hasFlag(RECOGNIZED);
}

bool Page::isExported() const {
    return hasFlag(EXPORTED);
}

Language Page::language() const {
    return language_;
}

QString Page::name() const {
    return QFileInfo(image_path_).fileName();
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

void Page::scaleView(qreal factor) {
    setViewScale(view_scale_ * factor);
}

void Page::setAngle(int angle) {
    QMutexLocker lock(&mutex_);

    const int new_angle = (360 + (angle % 360)) % 360;

    if(thumb_)
        setThumb(thumb_->transformed(QTransform().rotate(new_angle - angle_)));

    angle_ = new_angle;
    emit changed();
    emit rotated(angle_);
}

void Page::setCEDPage(cf::CEDPagePtr page) {
    QMutexLocker lock(&mutex_);

    if(cedpage_.get() == page.get())
        return;

    cedpage_ = page;

    updateBlocks();

    _unsetFlag(RECOGNITION_FAILED);
    emit changed();

    if(cedpage_) {
        _setFlag(RECOGNIZED);
        emit recognized();
    }

}

void Page::setChanged() {
    _setFlag(CHANGED);
    _unsetFlag(EXPORTED);
    emit changed();
}

void Page::setRecognized(bool value) {
    if(value) {
        _unsetFlag(RECOGNITION_FAILED);
        _setFlag(RECOGNIZED);
        emit recognized();
        emit changed();
    }
    else {
        _unsetFlag(RECOGNIZED);
        _unsetFlag(RECOGNITION_FAILED);
    }
}

void Page::setFlag(PageFlag flag) {
    if(!hasFlag(flag)) {
        _setFlag(flag);
        emit changed();
    }
}

void Page::setFlags(PageFlags flags) {
    state_flags_ = flags;
    emit changed();
}

void Page::setFormatSettings(const FormatSettings& s) {
//    if(format_settings_ == s)
//        return;

    format_settings_ = s;

    setChanged();
}

void Page::setLanguage(const Language& lang) {
    QMutexLocker lock(&mutex_);

    if(language_ == lang)
        return;
    else
        language_ = lang;

    setChanged();
}

void Page::setPageArea(const QRect& area) {
    QMutexLocker lock(&mutex_);

    if(page_area_ == area)
        return;

    page_area_ = area;

    setChanged();
}

void Page::setRecognitionSettings(const RecognitionSettings& opts) {
    QMutexLocker lock(&mutex_);

    if(rec_settings_ == opts)
        return;

    rec_settings_ = opts;

    setChanged();
}

void Page::setBlocks(const QList<QRect>& rects, BlockType type) {
    Q_ASSERT(type < blocks_.size());

    clearBlocks(type);

    foreach(QRect r, rects) {
        page_area_.topLeft();
        blocks_[type].append(r);
    }
}

void Page::setViewScale(float scale) {
    view_scale_ = scale;
    emit viewScaled();
}

void Page::setViewScroll(const QPoint& pt) {
    view_scroll_ = pt;
}

void Page::unsetFlag(PageFlag flag) {
    if(hasFlag(flag)) {
        _unsetFlag(flag);
        emit changed();
    }
}

void Page::updateBlocks() {
    Q_CHECK_PTR(cedpage_);

    cf::RectExporter exp(cedpage_);
    exp.collect();

    blockSignals(true);
    clearBlocks();
    setBlocks(exp.pictures(), PICTURE);
    setBlocks(exp.chars(), CHAR);
    setBlocks(exp.lines(), LINE);
    setBlocks(exp.paragraphs(), PARAGRAPH);
    setBlocks(exp.columns(), COLUMN);
    setBlocks(exp.sections(), SECTION);
    blockSignals(false);
}

void Page::updateTextDocument() {
    doc_->clear();
    cf::FormatOptions opts;
    formatSettings().exportTo(opts);
    opts.setLanguage(languageToType(language()));

    QTextDocumentExporter exp(this->cedpage_, opts);
    exp.setPage(this);
    exp.setDocument(doc_);
    exp.exportPage(*cedpage_);
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
            << page.state_flags_
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

    if(page.thumb_) {
        os << true;
        os << *(page.thumb_);
    }
    else {
        os << false;
    }

    return os;
}

QDataStream& operator>>(QDataStream& is, Page& page) {
    QMutexLocker lock(&page.mutex_);
    is >> page.image_path_
            >> page.image_size_
            >> page.state_flags_
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
    page.cedpage_ = ced.page();

    bool has_pixmap = false;
    is >> has_pixmap;

    if(has_pixmap) {
        QPixmap thumb;
        is >> thumb;
        page.setThumb(thumb);
    }

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

const QPixmap * Page::thumb() const
{
    return thumb_;
}

void Page::setThumb(const QPixmap& thumb)
{
    if(thumb_)
        *thumb_ = thumb;
    else
        thumb_ = new QPixmap(thumb);
}

void Page::initThumb()
{
    QPixmap pixmap;
    if(ImageCache::load(image_path_, &pixmap)) {
        is_null_ = false;
        image_size_ = pixmap.size();

        if(pixmap.height() > pixmap.width())
            setThumb(pixmap.scaledToHeight(THUMB_IMAGE_HEIGHT));
        else
            setThumb(pixmap.scaledToWidth(THUMB_IMAGE_WIDTH));
    }
    else {
        is_null_ = true;
    }
}

void Page::initDocument()
{
    if(doc_)
        delete doc_;

    doc_ = new QTextDocument(this);
}
