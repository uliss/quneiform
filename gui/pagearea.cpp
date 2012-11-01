/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <QSettings>
#include <QDebug>
#include <QPen>
#include <QGraphicsScene>

#include "page.h"
#include "pagearea.h"
#include "selectionlist.h"
#include "settingskeys.h"

PageArea::PageArea() :
    page_(NULL),
    current_char_bbox_(NULL),
    selections_(NULL)
{}

PageArea::~PageArea()
{
}

void PageArea::clear()
{
    clearCurrentChar();
}

void PageArea::clearCurrentChar() {
    delete current_char_bbox_;
    current_char_bbox_ = NULL;
}

void PageArea::saveSelections()
{
    if(!page_) {
        qWarning() << Q_FUNC_INFO << "NULL page";
        return;
    }

    if(!selections_) {
        page_->clearReadAreas();
        return;
    }

    page_->setReadAreas(selections_->readAreas());
    page_->setImageBlocks(selections_->imageBlocks());
    page_->setTextBlocks(selections_->textBlocks());
}

void PageArea::show(Page * page)
{
    page_ = page;
    updateLayout();
}

QRect PageArea::showChar(const QRect& bbox)
{
    QPen pen(QSettings().value(KEY_CURRENT_CHAR_COLOR, Qt::red).value<QColor>());

    QRect view_bbox = mapFromPage(bbox);

    if(!current_char_bbox_) {
        current_char_bbox_ = new QGraphicsRectItem(view_bbox, this);
        current_char_bbox_->setPen(pen);
        QColor background = pen.color();
        background.setAlpha(20);
        current_char_bbox_->setBrush(background);
    }
    else {
        current_char_bbox_->setRect(view_bbox);
    }

    return view_bbox;
}

bool PageArea::isBlockVisible(BlockType t) const
{
    if(!selections_)
        return false;

    return selections_->isBlocksVisible(t);
}

void PageArea::hideBlocks(BlockType t)
{
    if(!selections_)
        return;

    selections_->hideBlocks(t);
}

void PageArea::showBlocks(BlockType t)
{
    if(!selections_)
        return;

    selections_->showBlocks(t);
}

void PageArea::startImageBlockSelection()
{
    if(selections_)
        selections_->set(SelectionList::SELECT_IMAGE, SelectionList::MODE_ADD);
}

void PageArea::startPageAreaSelection()
{
    if(selections_)
        selections_->set(SelectionList::SELECT_AREA, SelectionList::MODE_REPLACE);
}

void PageArea::startTextBlockSelection()
{
    if(selections_)
        selections_->set(SelectionList::SELECT_TEXT, SelectionList::MODE_ADD);
}

void PageArea::updateLayout()
{
    if(!page_) {
        qDebug() << "[Error]" << Q_FUNC_INFO << "null page pointer";
        return;
    }

    if(!selections_) {
        selections_ = new SelectionList(this);
        connect(selections_, SIGNAL(changed()), SLOT(saveSelections()));
    }

    selections_->blockSignals(true);
    selections_->clearSelections();
    selections_->setRect(scene()->sceneRect());
    selections_->setZValue(10);
    selections_->populateFromPage(page_);

    foreach(QRect r, page_->readAreas()) {
        selections_->addSelection(r);
    }

    selections_->blockSignals(false);
}

QRect PageArea::mapFromPage(const QRect& r) const
{
    if(!page_)
        return r.normalized();

    return page_->mapFromBackend(r).normalized();
}
