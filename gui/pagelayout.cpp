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

PageLayout::PageLayout()
{
}

void PageLayout::clear() {
    Q_CHECK_PTR(scene());

    foreach(QGraphicsItem * item, childItems()) {
        scene()->removeItem(item);
    }
}

void PageLayout::populate(const Page& page) {
    QSettings settings;
    settings.beginGroup("debug");
    settings.beginGroup("format");

    if(settings.value("showPicturesBBox", false).toBool())
        populatePictures(page);

    if(settings.value("showCharactersBBox", false).toBool())
        populateChars(page);

    if(settings.value("showLinesBBox", false).toBool())
        populateLines(page);

    if(settings.value("showParagraphsBBox", false).toBool())
        populateParagraphs(page);

    if(settings.value("showSectionsBBox", false).toBool())
        populateSections(page);

    if(settings.value("showColumnsBBox", false).toBool())
        populateColumns(page);
}

void PageLayout::populateChars(const Page& page) {
    foreach(QRect r, page.rects(Page::CHAR)) {
        r.translate(page.pageArea().topLeft());
        QGraphicsRectItem * rect = new QGraphicsRectItem(r);
        rect->setPen(QColor(Qt::cyan));
        addToGroup(rect);
    }
}

void PageLayout::populateColumns(const Page& page) {
    foreach(QRect r, page.rects(Page::COLUMN)) {
        r.translate(page.pageArea().topLeft());
        QGraphicsRectItem * rect = new QGraphicsRectItem(r);
        rect->setPen(QColor(Qt::red));
        addToGroup(rect);
    }
}

void PageLayout::populateLines(const Page& page) {
    foreach(QRect r, page.rects(Page::LINE)) {
        r.translate(page.pageArea().topLeft());
        QGraphicsRectItem * rect = new QGraphicsRectItem(r);
        rect->setPen(QColor(Qt::green));
        addToGroup(rect);
    }
}

void PageLayout::populateParagraphs(const Page& page) {
    foreach(QRect r, page.rects(Page::PARAGRAPH)) {
        r.translate(page.pageArea().topLeft());
        QGraphicsRectItem * rect = new QGraphicsRectItem(r);
        rect->setPen(QColor(Qt::yellow));
        addToGroup(rect);
    }
}

void PageLayout::populatePictures(const Page& page) {
    foreach(QRect r, page.rects(Page::PICTURE)) {
        r.translate(page.pageArea().topLeft());
        QGraphicsRectItem * rect = new QGraphicsRectItem(r);
        rect->setPen(QColor(Qt::blue));
        rect->setToolTip(QString("[%1 %2 %3x%4]").arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height()));
        addToGroup(rect);
    }
}

void PageLayout::populateSections(const Page& page) {
    foreach(QRect r, page.rects(Page::SECTION)) {
        r.translate(page.pageArea().topLeft());
        QGraphicsRectItem * rect = new QGraphicsRectItem(r);
        rect->setPen(QColor(Qt::gray));
//        rect->setToolTip(QString("[%1 %2 %3x%4]").arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height()));
        addToGroup(rect);
    }
}
