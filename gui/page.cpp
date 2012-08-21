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
#include <QTransform>

#include <string>
#include <sstream>
#include <limits>

#include "puma/puma.h"
#include "ced/cedpage.h"
#include "page.h"
#include "imagecache.h"
#include "exportsettings.h"
#include "export/cedserializer.h"
#include "export/qtextdocumentexporter.h"
#include "export/rectexporter.h"
#include "export/exporterfactory.h"

static const QPoint INVALID_POINT(std::numeric_limits<int>::max(), std::numeric_limits<int>::min());

static language_t languageToType(const Language& lang) {
    if(lang.isValid()) {
        return static_cast<language_t>(lang.code());
    }
    else {
        qDebug() << "[Warning] Language is not valid, using English instead";
        return ::LANGUAGE_ENGLISH;
    }
}

Page::Page(const QString& image_path, bool load) :
    image_url_(image_path, 0),
    state_flags_(NONE),
    angle_(0),
    view_scale_(1.0),
    view_scroll_(INVALID_POINT),
    doc_(NULL),
    thumb_(NULL)
{
    is_null_ = !image_url_.exists();

    if(load)
        updateImageSize();

    initRects();
    initDocument();
}

Page::Page(const ImageURL& imageUrl, bool load) :
    image_url_(imageUrl),
    state_flags_(NONE),
    angle_(0),
    view_scale_(1.0),
    view_scroll_(INVALID_POINT),
    doc_(NULL),
    thumb_(NULL)
{
    is_null_ = !image_url_.exists();

    if(load)
        updateImageSize();

    initRects();
    initDocument();
}

Page::~Page() {
    delete thumb_;
}

void Page::addReadArea(const QRect& r)
{
    QMutexLocker lock(&mutex_);

    read_areas_.append(r);

    setChanged();
}

void Page::clearReadAreas()
{
    QMutexLocker lock(&mutex_);

    read_areas_.clear();

    setChanged();
}

bool Page::hasReadAreas() const
{
    return !read_areas_.isEmpty();
}

bool Page::hasThumb() const
{
    return thumb_ && (!thumb_->isNull());
}

QList<QRect> Page::readAreas() const
{
    return read_areas_;
}

QRect Page::readBoundingRect() const
{
    QRect full_image(QPoint(), imageSize());
    if(read_areas_.isEmpty())
        return full_image;

    QRect res;
    for(int i = 0; i < read_areas_.size(); i++) {
        if(i == 0)
            res = read_areas_[i];
        else
            res |= read_areas_[i];
    }

    return res & full_image;
}

void Page::setReadAreas(const QList<QRect>& rects)
{
    QMutexLocker lock(&mutex_);

    read_areas_ = rects;

    setChanged();
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
    clearReadAreas();
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
    return image_url_.path();
}

ImageURL Page::imageURL() const
{
    return image_url_;
}

QSize Page::imageSize() const
{
    if(image_size_.isNull())
        updateImageSize();

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

QTransform Page::fromBackendMatrix() const
{
    if(image_size_.isNull())
        updateImageSize();

    int width = image_size_.width();
    int height = image_size_.height();

    QTransform  matrix;
    QRect read_area = readBoundingRect();

    switch(angle_) {
    case 0:
        matrix.translate(read_area.left(), read_area.top());
        break;
    case 90:
        matrix.rotate(-90);
        matrix.translate(-(height-1), 0);
        if(hasReadAreas())
            matrix.translate((height-1) - read_area.bottom(), read_area.left());
        break;
    case 180:
        matrix.rotate(180);
        matrix.translate(-(width-1), -(height-1));
        if(hasReadAreas())
            matrix.translate((width-1) - read_area.right(), (height-1) - read_area.bottom());
        break;
    case 270:
        matrix.rotate(-270);
        matrix.translate(0, -(width-1));
        if(hasReadAreas())
            matrix.translate(read_area.top(), (width-1) - read_area.right());
        break;
    default:
        qWarning() << Q_FUNC_INFO << "invalid turn angle:" << angle_;
        break;
    }

    return matrix;
}

QTransform Page::toBackendMatrix() const
{
    if(image_size_.isNull())
        updateImageSize();

    QTransform matrix;
    QRect read_bbox = readBoundingRect();

    switch(angle_) {
    case 90:
        matrix.rotate(90);
        matrix.translate(0, -(read_bbox.height()-1));
        break;
    case 180:
        matrix.rotate(180);
        matrix.translate(-(read_bbox.width()-1), -(read_bbox.height()-1));
        break;
    case 270:
        matrix.rotate(270);
        matrix.translate(-(read_bbox.width()-1), 0);
    default:
        break;
    }

    return matrix;
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

QString Page::name() const
{
    return QFileInfo(image_url_.path()).fileName();
}

QPoint Page::mapFromBackend(const QPoint& pt) const
{
    return fromBackendMatrix().map(pt);
}

QRect Page::mapFromBackend(const QRect& rect) const
{
    QTransform matrix = fromBackendMatrix();
    QPoint pt0 = matrix.map(rect.topLeft());
    QPoint pt1 = matrix.map(rect.bottomRight());
    return QRect(pt0, pt1).normalized();
}

QPoint Page::mapToBackend(const QPoint& pt) const
{
    QPoint offset = readBoundingRect().topLeft();
    return toBackendMatrix().map(pt - offset);
}

QRect Page::mapToBackend(const QRect& r) const
{
    QPoint pt0 = mapToBackend(r.topLeft());
    QPoint pt1 = mapToBackend(r.bottomRight());
    return QRect(pt0, pt1).normalized();
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

void Page::setAngle(int angle)
{
    const int new_angle = (360 + (angle % 360)) % 360;

    {
        QMutexLocker lock(&mutex_);
        if(thumb_)
            *thumb_ = thumb_->transformed(QTransform().rotate(new_angle - angle_));

        angle_ = new_angle;
    }

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

void Page::setImageSize(const QSize& sz)
{
    QMutexLocker lock(&mutex_);
    image_size_ = sz;
}

void Page::setLanguage(const Language& lang) {
    QMutexLocker lock(&mutex_);

    if(language_ == lang)
        return;
    else
        language_ = lang;

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

void Page::updateImageSize() const
{
    QImage image;
    if(ImageCache::load(image_url_, &image)) {
        image_size_ = image.size();
        is_null_ = false;
    }
    else {
        is_null_ = true;
        image_size_ = QSize();
    }
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

bool Page::isFirstViewScroll() const
{
    return view_scroll_ == INVALID_POINT;
}

QImage Page::binarizedImage() const
{
    if(binarized_)
        return *binarized_;

    return QImage();
}

bool Page::isBinarized() const
{
    return binarized_ && !binarized_->isNull();
}

void Page::setBinarizedImage(const QImage& image)
{
    binarized_.reset(new QImage(image));
    emit binarized();
}

QDataStream& operator<<(QDataStream& os, const Page& page) {
    QMutexLocker lock(&page.mutex_);
    os << page.image_url_
            << page.image_size_
            << page.state_flags_
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

    os << page.read_areas_;

    return os;
}

QDataStream& operator>>(QDataStream& is, Page& page) {
    QMutexLocker lock(&page.mutex_);
    is >> page.image_url_
            >> page.image_size_
            >> page.state_flags_
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

    bool has_thumb = false;
    is >> has_thumb;

    if(has_thumb) {
        QImage thumb;
        is >> thumb;

        if(page.thumb_)
            *(page.thumb_) = thumb;
        else
            page.thumb_ = new QImage(thumb);
    }

    is >> page.read_areas_;

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

QPixmap Page::thumb() const
{
    QMutexLocker lock(&mutex_);

    if(!thumb_)
        return QPixmap();

    return QPixmap::fromImage(*thumb_);
}

void Page::setThumb(const QImage& thumb)
{
    {
        QMutexLocker lock(&mutex_);

        if(thumb_)
            *thumb_ = thumb;
        else
            thumb_ = new QImage(thumb);
    }

    emit thumbChanged();
}

void Page::initDocument()
{
    if(doc_)
        delete doc_;

    doc_ = new QTextDocument(this);
}

QSize Page::maxThumbnailSize()
{
    return QSize(200, 200);
}
