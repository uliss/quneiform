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

#include <QMutexLocker>

#include "pagerecognitionqueue.h"
#include "pagerecognizer.h"
#include "page.h"
#include "document.h"
#include "common/lang_def.h" // for language_t

PageRecognitionQueue::PageRecognitionQueue(QObject *parent) :
    QThread(parent), language_(LANGUAGE_ENGLISH)
{
}

void PageRecognitionQueue::add(Document * doc) {
    Q_CHECK_PTR(doc);

    for(int i = 0; i < doc->pageCount(); i++)
        add(doc->page(i));
}

void PageRecognitionQueue::add(Page * p) {
    Q_CHECK_PTR(p);

    QMutexLocker lock(&mutex_);
    if(!pages_.contains(p))
        pages_.enqueue(p);
}

bool PageRecognitionQueue::isEmpty() const {
    return pages_.isEmpty();
}

void PageRecognitionQueue::run() {
    QMutexLocker lock(&mutex_);

    PageRecognizer r(NULL);
    r.setLanguage(language_);
    while(!pages_.empty()) {
        Page * p = pages_.dequeue();
        r.setPage(p);
        r.start();
        r.wait();
    }
}

void PageRecognitionQueue::setLanguage(int language) {
    language_ = language;
}
