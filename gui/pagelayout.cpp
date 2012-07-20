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
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPen>
#include <QColor>
#include <QSettings>

#include "pagelayout.h"
#include "page.h"
#include "settingskeys.h"

PageLayout::PageLayout()
    : page_(NULL)
{
    chars_ = new QGraphicsItemGroup();
    addToGroup(chars_);

    columns_ = new QGraphicsItemGroup();
    addToGroup(columns_);

    lines_ = new QGraphicsItemGroup();
    addToGroup(lines_);

    paragraphs_ = new QGraphicsItemGroup();
    addToGroup(paragraphs_);

    pictures_ = new QGraphicsItemGroup();
    addToGroup(pictures_);

    sections_ = new QGraphicsItemGroup();
    addToGroup(sections_);
}

QColor PageLayout::blockColor(int type) const {
    switch(type) {
    case Page::CHAR:
        return QColor(Qt::cyan);
    case Page::COLUMN:
        return QColor(Qt::red);
    case Page::LINE:
        return QColor(Qt::green);
    case Page::PARAGRAPH:
        return QColor(Qt::yellow);
    case Page::PICTURE:
        return QColor(Qt::blue);
    case Page::SECTION:
        return QColor(Qt::gray);
    default:
        return QColor(Qt::black);
    }
}

void PageLayout::clear() {
    clearGroupBlocks(&PageLayout::chars_);
    clearGroupBlocks(&PageLayout::columns_);
    clearGroupBlocks(&PageLayout::lines_);
    clearGroupBlocks(&PageLayout::paragraphs_);
    clearGroupBlocks(&PageLayout::pictures_);
    clearGroupBlocks(&PageLayout::sections_);
}

void PageLayout::clearGroupBlocks(GroupMember ptr) {
    if(!(this->*ptr))
        return;

    foreach(QGraphicsItem * item, (this->*ptr)->childItems()) {
        (this->*ptr)->removeFromGroup(item);
        if(item->scene())
            item->scene()->removeItem(item);
        delete item;
    }
}

QGraphicsItemGroup * PageLayout::columnBlocks() {
    return columns_;
}

QGraphicsRectItem * PageLayout::createBlock(const QRect& r, const QColor& c) {
    Q_ASSERT(page_);
    QGraphicsRectItem * block = new QGraphicsRectItem(r);
    QPen pen(c, 0, Qt::SolidLine);
    block->setPen(pen);
    return block;
}

QGraphicsItemGroup * PageLayout::charBlocks() {
    return chars_;
}

QGraphicsItemGroup * PageLayout::lineBlocks() {
    return lines_;
}

QGraphicsItemGroup * PageLayout::paragraphBlocks() {
    return paragraphs_;
}

QGraphicsItemGroup * PageLayout::pictureBlocks() {
    return pictures_;
}

void PageLayout::populate(const Page& page) {
    clear();

    QSettings settings;

    page_ = &page;

    if(settings.value(KEY_DEBUG_CHARACTERS_BBOX, false).toBool())
        populateGroup(chars_, Page::CHAR);

    if(settings.value(KEY_DEBUG_COLUMNS_BBOX, false).toBool())
        populateGroup(columns_, Page::COLUMN);

    if(settings.value(KEY_DEBUG_LINES_BBOX, false).toBool())
        populateGroup(lines_, Page::LINE);

    if(settings.value(KEY_DEBUG_PARAGRAPHS_BBOX, false).toBool())
        populateGroup(paragraphs_, Page::PARAGRAPH);

    if(settings.value(KEY_DEBUG_PICTURES_BBOX, false).toBool())
        populateGroup(pictures_, Page::PICTURE);

    if(settings.value(KEY_DEBUG_SECTIONS_BBOX, false).toBool())
        populateGroup(sections_, Page::SECTION);
}

void PageLayout::populateGroup(QGraphicsItemGroup * group, int group_type) {
    Q_ASSERT(group);
    Q_ASSERT(page_);

    Page::BlockType block_t = static_cast<Page::BlockType>(group_type);

    foreach(QRect r, page_->blocks(block_t)) {
        group->addToGroup(createBlock(mapFromPage(r), blockColor(block_t)));
    }

    addToGroup(group);
}

QGraphicsItemGroup * PageLayout::sectionBlocks() {
    return sections_;
}

QRect PageLayout::mapFromPage(const QRect& r) const
{
    if(!page_)
        return r;

    return page_->mapFromPage(r);
}

