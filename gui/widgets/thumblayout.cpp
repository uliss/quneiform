/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include "thumblayout.h"
#include "thumbnailwidget.h"

typedef QPair<ThumbnailWidget*,ThumbnailWidget*> SelectRange;

static SelectRange findSelectedRange(const QList<ThumbnailWidget*>& thumbs) {
    SelectRange res(NULL, NULL);

    foreach(ThumbnailWidget * t, thumbs) {
        if(t->isThumbSelected()) {
            if(res.first == NULL) {
                res.first = t;
            }

            res.second = t;
        }
    }

    return res;
}

ThumbLayout::ThumbLayout()
{
}

void ThumbLayout::append(ThumbnailWidget * thumb)
{
    if(!thumb) {
        qDebug() << Q_FUNC_INFO << " NULL thumb given";
        return;
    }

    if(thumbs_.contains(thumb)) {
        qDebug() << Q_FUNC_INFO << " thumb already in layout. skipping...";
        return;
    }

    qreal y_pos = 0;

    if(!thumbs_.isEmpty()) {
        y_pos = thumbs_.last()->pos().y() + thumbs_.last()->boundingRect().height();
    }

    thumbs_.append(thumb);

    if(y_pos)
        thumb->setPos(0, y_pos);

    updateThumbNames();
}

void ThumbLayout::clearSelection() {
    foreach(ThumbnailWidget * t, thumbs_) {
        t->selectThumb(false);
    }
}

int ThumbLayout::count() const
{
    return thumbs_.count();
}

ThumbnailWidget * ThumbLayout::findByPage(Page *p)
{
    foreach(ThumbnailWidget * t, thumbs_) {
        if(t->page() == p)
            return t;
    }

    return NULL;
}

void ThumbLayout::multiSelect(ThumbnailWidget * thumb)
{
    foreach(ThumbnailWidget * t, thumbs_) {
        if(t == thumb) {
            t->toggleSelection();
            return;
        }
    }
}

void ThumbLayout::remove(ThumbnailWidget * thumb)
{
    if(!thumb)
        return;

    int idx = thumbs_.indexOf(thumb);
    if(idx == -1)
        return;

    qreal y_offset = thumb->boundingRect().height();
    thumbs_.removeAt(idx);
    for(int i = idx; i < thumbs_.count(); i++) {
        thumbs_[i]->moveBy(0, - y_offset);
    }

    updateThumbNames();
}

void ThumbLayout::select(ThumbnailWidget * thumb)
{
    foreach(ThumbnailWidget * t, thumbs_) {
        t->selectThumb(thumb == t);
    }
}

void ThumbLayout::selectAll()
{
    foreach(ThumbnailWidget * t, thumbs_) {
        t->selectThumb(true);
    }
}

QList<ThumbnailWidget *> ThumbLayout::selected()
{
    QList<ThumbnailWidget*> res;

    foreach(ThumbnailWidget * t, thumbs_) {
        if(t->isThumbSelected())
            res.append(t);
    }

    return res;
}

void ThumbLayout::selectRange(int begin, int end) {
    for(int i = begin; i < thumbs_.count() && i < end; i++)
        thumbs_[i]->selectThumb(true);
}

void ThumbLayout::selectRange(ThumbnailWidget * thumb)
{
    Q_CHECK_PTR(thumb);

    int thumb_idx = thumbs_.indexOf(thumb);

    if(thumb_idx == -1)
        return;

    SelectRange range = findSelectedRange(thumbs_);

    if(!range.first) {
        thumb->selectThumb(true);
        return;
    }

    int begin = thumbs_.indexOf(range.first);

    clearSelection();

    if(thumb_idx > begin)
        selectRange(begin, thumb_idx + 1);
    else if(thumb_idx < begin)
        selectRange(thumb_idx, begin + 1);
}

QSizeF ThumbLayout::size() const
{
    if(thumbs_.isEmpty())
        return QSizeF();

    return QSizeF(thumbs_.first()->boundingRect().width(),
                  thumbs_.last()->pos().y() + thumbs_.last()->boundingRect().height());
}

void ThumbLayout::update()
{
    static const int SPACING = 1;
    qreal y_pos = 0;
    foreach(ThumbnailWidget * t, thumbs_) {
        t->setPos(0, y_pos);
        y_pos += SPACING;
        y_pos += t->boundingRect().height();
    }
}

void ThumbLayout::sortByPages(const QList<Page*>& pages)
{
    QList<ThumbnailWidget*> thumbs;

    foreach(Page * p, pages) {
        ThumbnailWidget * th = findByPage(p);
        if(!p)
            continue;

        thumbs.append(th);
    }

    thumbs_ = thumbs;
    update();
    updateThumbNames();
}

void ThumbLayout::highlightAll(bool value)
{
    foreach(ThumbnailWidget * t, thumbs_) {
        t->highlight(value);
    }
}

void ThumbLayout::updateThumbNames()
{
    for (int i = 0; i < thumbs_.count(); i++)
        thumbs_[i]->setName(QString("%1").arg(i + 1));
}

