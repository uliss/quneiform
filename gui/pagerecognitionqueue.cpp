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
    QThread(parent), abort_(false)
{
    setupProgressDialog();
    setupPageRecognizer();
}

void PageRecognitionQueue::abort() {
    pause();
    int button = QMessageBox::question(NULL,
                                       tr("Recognition abort"),
                                       tr("Do you really want to abort recognition?"),
                                       QMessageBox::Cancel,
                                       QMessageBox::Yes);

    resume();

    if(QMessageBox::Yes == button) {
        abort_ = true;
        recognizer_->abort();
    }
    else
        progress_->show();
}

void PageRecognitionQueue::add(Document * doc) {
    Q_CHECK_PTR(doc);

    if(isRunning()) {
        CF_WARNING("PageRecognitionQueue is running. Can't add document.")
        return;
    }

    for(int i = 0; i < doc->pageCount(); i++)
        add(doc->page(i));
}

void PageRecognitionQueue::add(Page * p) {
    Q_CHECK_PTR(p);

    if(isRunning()) {
        CF_WARNING("PageRecognitionQueue is running. Can't add page.")
        return;
    }

    if(!pages_.contains(p))
        pages_.enqueue(p);
}

void PageRecognitionQueue::pause() {
    recognizer_->pause();
}

void PageRecognitionQueue::pageFault(const QString& msg) {
    qDebug() << Q_FUNC_INFO << recognizer_->page()->imagePath() << msg;
}

void PageRecognitionQueue::pageOpened() {
    progress_->setCurrentPage(recognizer_->page());
}

void PageRecognitionQueue::resume() {
    recognizer_->resume();
}

void PageRecognitionQueue::run() {
    abort_ = false;
    const int total = pages_.count();
    int done = 0;
    while(!pages_.empty() && !abort_) {
        Page * p = pages_.dequeue();
        recognizer_->setPage(p);
        recognizer_->start();
        recognizer_->wait();
        emit percentsDone((++done * 100) / total);
    }
}

void PageRecognitionQueue::setLanguage(int lang) {
    if(isRunning()) {
        CF_WARNING("PageRecognitionQueue is running. Can't change language!")
        return;
    }

    recognizer_->setLanguage(lang);
}

void PageRecognitionQueue::setupProgressDialog() {
    progress_ = new RecognitionProgressDialog(NULL);
    connect(this, SIGNAL(started()), progress_, SLOT(show()));
    connect(this, SIGNAL(started()), progress_, SLOT(reset()));
    connect(this, SIGNAL(finished()), progress_, SLOT(hide()));
    connect(this, SIGNAL(percentsDone(int)), progress_, SLOT(setTotalValue(int)));
    connect(progress_, SIGNAL(aborted()), SLOT(abort()));
    connect(progress_, SIGNAL(paused()), SLOT(pause()));
    connect(progress_, SIGNAL(resumed()), SLOT(resume()));
}

void PageRecognitionQueue::setupPageRecognizer() {
    recognizer_ = new PageRecognizer(NULL, this);
    connect(recognizer_, SIGNAL(failed(QString)), SLOT(pageFault(QString)));
    connect(recognizer_, SIGNAL(opened()), SLOT(pageOpened()));
}
