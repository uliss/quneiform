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

#include <QGraphicsSceneEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QKeyEvent>
#include <QSettings>

#include "selectionlist.h"
#include "selection.h"
#include "page.h"
#include "settingskeys.h"

#include "resources/cursor_selection_area.xpm"
#include "resources/cursor_selection_area_add.xpm"
#include "resources/cursor_selection_text_add.xpm"
#include "resources/cursor_selection_image_add.xpm"

static const QColor SHADOW(0, 0, 0, 30);
static const int MIN_SELECTION_WIDTH = 8;
static const int MIN_SELECTION_HEIGHT = 8;

static QRect toQRect(const QRectF& r)
{
    return QRect(qRound(r.left()), qRound(r.top()), qRound(r.width()), qRound(r.height()));
}

SelectionList::SelectionList(QGraphicsItem * parent) :
    QGraphicsRectItem(QRectF(), parent),
    rubber_band_(NULL),
    type_(SELECT_NONE),
    mode_(MODE_NONE),
    page_(NULL)
{
    setActive(true);
    setPen(QPen(Qt::NoPen));
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void SelectionList::populateFromPage(const Page * p)
{
    if(!p) {
        qWarning() << Q_FUNC_INFO << "NULL page pointer given";
        return;
    }

    page_ = p;

    populateChars();
    populateLines();
    populateParagraphs();
    populateColumns();
    populateSections();
    populatePictures();

    foreach(Block block, page_->blocks(BLOCK_LAYOUT_TEXT)) {
        addLayoutBlock(block);
    }

    foreach(Block block, page_->blocks(BLOCK_LAYOUT_IMAGE)) {
        addLayoutBlock(block);
    }

    foreach(Block block, page_->blocks(BLOCK_LAYOUT_TABLE)) {
        addLayoutBlock(block);
    }
}

QRect SelectionList::selectionBoundingRect() const
{
    QRectF res;

    foreach(Selection * s, selections_) {
        if(res.isNull())
            res = s->normalRect();

        res.unite(s->normalRect());
    }

    return QRect((int) res.left(), (int) res.top(), (int) res.width(), (int) res.height());
}

bool SelectionList::isBlocksVisible(BlockType t) const
{
    foreach(Selection * s, selections_) {
        if(s->selectionType() == t)
            return s->isVisible();
    }

    return false;
}

void SelectionList::hideBlocks(BlockType t)
{
    foreach(Selection * s, selections_) {
        if(s->selectionType() == t)
            s->hide();
    }
}

void SelectionList::showBlocks(BlockType t)
{
    foreach(Selection * s, selections_) {
        if(s->selectionType() == t)
            s->show();
    }
}

QList<Block> SelectionList::imageBlocks() const
{
    QList<Block> res;

    foreach(Selection * s, selections_) {
        if(s->selectionType() != BLOCK_LAYOUT_IMAGE)
            continue;

        res.append(Block(s->selectionType(), toQRect(s->normalRect())));
    }

    return res;
}

QList<Block> SelectionList::textBlocks() const
{
    QList<Block> res;

    foreach(Selection * s, selections_) {
        if(s->selectionType() != BLOCK_LAYOUT_TEXT)
            continue;

        res.append(Block(s->selectionType(), toQRect(s->normalRect())));
    }

    return res;
}

QList<QRect> SelectionList::readAreas() const
{
    QList<QRect> res;

    foreach(Selection * s, selections_) {
        if(s->selectionType() != BLOCK_LAYOUT_AREA)
            continue;

        res.append(toQRect(s->normalRect()));
    }

    return res;
}

void SelectionList::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    // resize only with left button
    if(event->button() & Qt::LeftButton)
        return;

    if(type_ != SELECT_NONE) {
        resizeSelection(event);
        event->accept();
    }
    else {
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}

void SelectionList::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // selection only with left button
    if(event->button() != Qt::LeftButton)
        return;

    if(type_ != SELECT_NONE) {
        startSelection(event);
        event->accept();
    }
    else {
        QGraphicsRectItem::mousePressEvent(event);
    }
}

void SelectionList::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if(event->button() != Qt::LeftButton)
        return;

    if(type_ != SELECT_NONE) {
        finishSelection(event);
        event->accept();
    }
    else {
        QGraphicsRectItem::mouseReleaseEvent(event);
    }
}

void SelectionList::addLayoutBlock(const Block& block)
{
    if(!page_)
        return;

    Selection * s = new Selection(this, page_->mapFromBackend(block.rect()));
    s->setSelectionType(block.type());
    selections_.append(s);
}

void SelectionList::addSelection(const QRectF& r)
{
    foreach(Selection * s, selections_) {
        if(s->rect().intersects(r)) {
            qDebug() << Q_FUNC_INFO << "intersected selection";
        }
    }

    selections_.append(makeSelection(r));

    emit changed();
}

void SelectionList::clearSelections()
{
    foreach(Selection * s, selections_) {
        scene()->removeItem(s);
    }

    selections_.clear();

    emit changed();
}

bool SelectionList::isTurned() const
{
    if(!page_)
        return false;

    return page_->angle() % 180 != 0;
}

void SelectionList::set(SelectionList::selection_t type, SelectionList::selection_mode_t mode)
{
    type_ = type;
    mode_ = mode;

    switch(type_) {
    case SELECT_AREA:
        switch(mode_) {
        case MODE_ADD:
            setCursor(QCursor(QPixmap(cursor_selection_area_add_xpm)));
            return;
        case MODE_REPLACE:
            setCursor(QCursor(QPixmap(cursor_selection_area_xpm)));
            return;
        default:
            break;
        }
    case SELECT_IMAGE:
        switch(mode_) {
        case MODE_ADD:
        case MODE_REPLACE:
            setCursor(QCursor(QPixmap(cursor_selection_image_add_xpm)));
            return;
        default:
            break;
        }
        break;
    case SELECT_TEXT:
        switch(mode_) {
        case MODE_ADD:
        case MODE_REPLACE:
            setCursor(QCursor(QPixmap(cursor_selection_text_add_xpm)));
            return;
        default:
            break;
        }
        break;
    default:
        break; // go to unset
    }

    unsetCursor();
}

void SelectionList::setSelection(const QRectF& r)
{
    clearSelections();
    selections_.append(makeSelection(r));

    emit changed();
}

void SelectionList::handleSelectionDelete(Selection * s)
{
    if(!selections_.contains(s))
        return;

    delete s;
    selections_.removeAll(s);
    updateSelections();
}

Selection * SelectionList::makeSelection(const QRectF& r)
{
    Selection * s = new Selection(this, r);
    s->setAcceptHoverEvents(true);
    s->setZValue(1);

    switch(type_) {
    case SELECT_AREA:
        s->setSelectionType(BLOCK_LAYOUT_AREA);
        qDebug() << Q_FUNC_INFO << "area selection";
        break;
    case SELECT_TEXT:
        s->setSelectionType(BLOCK_LAYOUT_TEXT);
        qDebug() << Q_FUNC_INFO << "text selection";
        break;
    case SELECT_IMAGE:
        s->setSelectionType(BLOCK_LAYOUT_IMAGE);
        qDebug() << Q_FUNC_INFO << "image selection";
        break;
    case SELECT_TABLE:
        s->setSelectionType(BLOCK_LAYOUT_TABLE);
        break;
    default:
        qDebug() << Q_FUNC_INFO << "default selection";
        s->setSelectionType(BLOCK_LAYOUT_AREA);
        break;
    }

    return s;
}

Selection * SelectionList::selectionAt(int i)
{
    if(i < selections_.size())
        return selections_.at(i);
    return NULL;
}

void SelectionList::updateSelections()
{
    emit changed();
}

void SelectionList::finishSelection(QGraphicsSceneMouseEvent * event)
{
    removeRubberBand();

    QRectF rect(selection_start_, event->pos());
    rect = rect.normalized();
    rect = scene()->sceneRect().intersected(rect);

    if(rect.width() < MIN_SELECTION_WIDTH)
        rect.adjust(-MIN_SELECTION_WIDTH, 0, MIN_SELECTION_WIDTH, 0);
    if(rect.height() < MIN_SELECTION_HEIGHT)
        rect.adjust(0, -MIN_SELECTION_HEIGHT, 0, MIN_SELECTION_HEIGHT);
    rect = scene()->sceneRect().intersected(rect);

    switch(mode_) {
    case MODE_REPLACE:
        setSelection(rect);
        break;
    case MODE_ADD:
        addSelection(rect);
        break;
    default:
        break;
    }

    type_ = SELECT_NONE;
    mode_ = MODE_NONE;
    unsetCursor();
}

void SelectionList::startSelection(QGraphicsSceneMouseEvent * event)
{
    selection_start_ = event->pos();

    // remove previous
    removeRubberBand();
    addRubberBand();
}

void SelectionList::addRubberBand()
{
    rubber_band_ = new QGraphicsRectItem(QRectF(), this);
    rubber_band_->setPen(QPen(Qt::DotLine));
}

void SelectionList::removeRubberBand()
{
    if(rubber_band_) {
        scene()->removeItem(rubber_band_);
        delete rubber_band_;
        rubber_band_ = NULL;
    }
}

void SelectionList::populateChars()
{
    if(!page_)
        return;

    if(!QSettings().value(KEY_DEBUG_CHARACTERS_BBOX, false).toBool())
        return;

    foreach(Block block, page_->blocks(BLOCK_CHAR)) {
        addLayoutBlock(block);
    }
}

void SelectionList::populateLines()
{
    if(!page_)
        return;

    if(!QSettings().value(KEY_DEBUG_LINES_BBOX, false).toBool())
        return;

    foreach(Block block, page_->blocks(BLOCK_LINE)) {
        addLayoutBlock(block);
    }
}

void SelectionList::populateParagraphs()
{
    if(!page_)
        return;

    if(!QSettings().value(KEY_DEBUG_PARAGRAPHS_BBOX, false).toBool())
        return;

    foreach(Block block, page_->blocks(BLOCK_PARAGRAPH)) {
        addLayoutBlock(block);
    }
}

void SelectionList::populateColumns()
{
    if(!page_)
        return;

    if(!QSettings().value(KEY_DEBUG_COLUMNS_BBOX, false).toBool())
        return;

    foreach(Block block, page_->blocks(BLOCK_COLUMN)) {
        addLayoutBlock(block);
    }
}

void SelectionList::populateSections()
{
    if(!page_)
        return;

    if(!QSettings().value(KEY_DEBUG_SECTIONS_BBOX, false).toBool())
        return;

    foreach(Block block, page_->blocks(BLOCK_SECTION)) {
        addLayoutBlock(block);
    }
}

void SelectionList::populatePictures()
{
    if(!page_)
        return;

    if(!QSettings().value(KEY_DEBUG_PICTURES_BBOX, false).toBool())
        return;

    foreach(Block block, page_->blocks(BLOCK_PICTURE)) {
        addLayoutBlock(block);
    }
}

void SelectionList::resizeSelection(QGraphicsSceneMouseEvent * event)
{
    Q_CHECK_PTR(rubber_band_);

    rubber_band_->setRect(QRectF(selection_start_, event->pos()));
}

void SelectionList::keyPressEvent(QKeyEvent * event)
{
    QGraphicsRectItem::keyPressEvent(event);

    if(type_ == SELECT_NONE | event->key() == Qt::Key_Escape) {
        set(SELECT_NONE, MODE_NONE);
        return;
    }

    if(event->key() == Qt::Key_Control)
        set(type_, MODE_ADD);
}

void SelectionList::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsRectItem::keyReleaseEvent(event);

    if(type_ == SELECT_NONE) {
        set(SELECT_NONE, MODE_NONE);
        return;
    }

    if(event->key() == Qt::Key_Control)
        set(type_, MODE_REPLACE);
}
