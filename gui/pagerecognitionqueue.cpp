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
#include <QMessageBox>

#include "pagerecognitionqueue.h"
#include "pagerecognizer.h"
#include "page.h"
#include "document.h"
#include "recognitionprogressdialog.h"
#include "quneiform_debug.h"

PageRecognitionQueue::PageRecognitionQueue(QObject * parent) :
    QObject(parent)
{
    setupProgressDialog();
    setupPageRecognizer();
}

void PageRecognitionQueue::add(Document * doc) {
    Q_CHECK_PTR(doc);

    for(int i = 0; i < doc->pageCount(); i++)
        add(doc->page(i));
}

void PageRecognitionQueue::add(Page * p) {
    Q_CHECK_PTR(p);

    if(!pages_.contains(p))
        pages_.enqueue(p);
}

void PageRecognitionQueue::pageFault(const QString& msg) {
    qDebug() << Q_FUNC_INFO << recognizer_->page()->imagePath() << msg;
}

void PageRecognitionQueue::start() {
    emit started();
    const int total = pages_.count();
    int done = 0;
    progress_->reset();
    while(!pages_.empty()) {
        if(progress_->wasCanceled())
            break;

        Page * p = pages_.dequeue();

        progress_->setCurrentPage(p);

        recognizer_->setPage(p);
        recognizer_->start();
        recognizer_->wait();

        progress_->setValue((++done * 100) / total);
    }
    emit finished();
}

void PageRecognitionQueue::setLanguage(int lang) {
    recognizer_->setLanguage(lang);
}

void PageRecognitionQueue::setupProgressDialog() {
    progress_ = new RecognitionProgressDialog(NULL);
    connect(this, SIGNAL(started()), progress_, SLOT(show()));
    connect(this, SIGNAL(started()), progress_, SLOT(reset()));
    connect(this, SIGNAL(finished()), progress_, SLOT(hide()));
}

void PageRecognitionQueue::setupPageRecognizer() {
    recognizer_ = new PageRecognizer(NULL, this);
    connect(recognizer_, SIGNAL(failed(QString)), SLOT(pageFault(QString)));
    connect(recognizer_, SIGNAL(opened()), SLOT(pageOpened()));
    connect(progress_, SIGNAL(canceled()), recognizer_, SLOT(abort()));
}
