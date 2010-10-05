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

#include <QtGui/QScrollBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMenu>

#include "document.h"
#include "page.h"
#include "thumbnaillist.h"
#include "thumbnailwidget.h"

static const int LIST_WIDTH = 180;

ThumbnailList::ThumbnailList(QWidget *parent) :
    QScrollArea(parent), document_(NULL), layout_(NULL) {
    setupLayout();
    setScrollBars();
}

void ThumbnailList::append(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(thumb);

    thumbs_.append(thumb);
    layout_->addWidget(thumb, 0, Qt::AlignHCenter);
    connect(thumb, SIGNAL(clicked()), SLOT(thumbClick()));
    connect(thumb, SIGNAL(recognized()), SLOT(thumbRecognize()));
    connect(thumb, SIGNAL(removed(Page*)), document_, SLOT(remove(Page*)));

    updateThumbNames();
}

void ThumbnailList::setupContextMenu(QMenu * menu) {
    Q_CHECK_PTR(menu);

    menu->addAction(tr("Select All"), this, SLOT(selectAll()), QKeySequence::SelectAll);
    menu->addAction(tr("Revert selected"), this, SLOT(revertSelection()));
    menu->addAction(tr("Delete selected"), this, SLOT(removeSelectedPages()));
}

int ThumbnailList::count() const {
    return thumbs_.count();
}

void ThumbnailList::highlight(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(thumb);

    for (int i = 0; i < thumbs_.count(); i++) {
        if (thumbs_.at(i) == thumb)
            thumbs_.at(i)->highlight(true);
        else
            thumbs_.at(i)->highlight(false);
    }
}

void ThumbnailList::pageAdd(Page * page) {
    Q_CHECK_PTR(page);

    ThumbnailWidget * thumb = new ThumbnailWidget(page, this);
    append(thumb);
}

void ThumbnailList::pageRemove(Page * page) {
    ThumbnailWidget * th = thumb(page);
    if (th)
        thumbRemove(th);
}

void ThumbnailList::removeSelectedPages() {
    document_->removeSelected();
}

void ThumbnailList::thumbRemove(ThumbnailWidget * thumb) {
    Q_CHECK_PTR(layout_);
    Q_CHECK_PTR(thumb);
    Page * page = thumb->page();
    layout_->removeWidget(thumb);
    thumbs_.removeAll(thumb);
    updateThumbNames();
    emit thumbRemovalFinished(page);
}

void ThumbnailList::revertSelection() {
    foreach(ThumbnailWidget * thumb, thumbs_)
        {
            thumb->toggleSelection();
        }
}

void ThumbnailList::selectAll() {
    foreach(ThumbnailWidget * thumb, thumbs_)
        {
            thumb->setChecked(true);
        }
}

void ThumbnailList::setDocument(Document * doc) {
    Q_CHECK_PTR(doc);

    document_ = doc;
    connect(document_, SIGNAL(pageAdded(Page*)), this, SLOT(pageAdd(Page*)));
    connect(document_, SIGNAL(pageRemoved(Page*)), this, SLOT(pageRemove(Page*)));
}

void ThumbnailList::setupLayout() {
    setWidgetResizable(true);
    setFixedWidth(LIST_WIDTH);

    QWidget * widget = new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setWidget(widget);
    layout_ = new QVBoxLayout(widget);
    layout_->setSpacing(2);
    layout_->setMargin(0);
}

void ThumbnailList::setScrollBars() {
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //verticalScrollBar()->setEnabled(false);
    setAttribute(Qt::WA_StaticContents);
}

ThumbnailWidget * ThumbnailList::thumb(Page * page) {
    for (int i = 0; i < thumbs_.count(); i++) {
        if (thumbs_.at(i)->page() == page)
            return thumbs_.at(i);
    }
    return NULL;
}

void ThumbnailList::thumbClick() {
    ThumbnailWidget * th = dynamic_cast<ThumbnailWidget*> (QObject::sender());
    if (!th)
        return;

    highlight(th);
    emit thumbSelected(th->page());
}

void ThumbnailList::thumbRecognize() {
    ThumbnailWidget * th = dynamic_cast<ThumbnailWidget*> (QObject::sender());
    if (!th)
        return;

    Q_CHECK_PTR(th->page());
    th->page()->recognize();
}

void ThumbnailList::updateThumbNames() {
    for (int i = 0; i < thumbs_.count(); i++)
        thumbs_.at(i)->setName(QString("%1").arg(i + 1));
}