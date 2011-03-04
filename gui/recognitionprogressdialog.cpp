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
#include <QLabel>

#include "page.h"
#include "recognitionprogressdialog.h"
#include "pagerecognitionqueue.h"

RecognitionProgressDialog::RecognitionProgressDialog(QWidget * parent) :
        QProgressDialog(parent) {
//    setModal(true);
//    setWindowModality(Qt::ApplicationModal);
    setMinimumDuration(0);
    setMinimum(0);
    setMaximum(100);
    setFixedWidth(400);
    setupLabel();
}

void RecognitionProgressDialog::connectToQueue(PageRecognitionQueue * queue) {
    if(!queue)
        return;

    connect(queue, SIGNAL(started()), SLOT(show()));
//    connect(queue, SIGNAL(finished(int)), SLOT(close()));
    connect(queue, SIGNAL(percentDone(int)), SLOT(setValue(int)));
    connect(queue, SIGNAL(pageStarted(QString)), SLOT(setCurrentPage(QString)));
    connect(this, SIGNAL(canceled()), queue, SLOT(abort()));
}

void RecognitionProgressDialog::setCurrentPage(const QString& path) {
    setLabelText(tr("Page recognition: \"%1\"").arg(path));
}

void RecognitionProgressDialog::setupLabel() {
    QLabel * label = new QLabel();
    label->setAlignment(Qt::AlignLeft);
    label->setTextFormat(Qt::PlainText);
    label->setScaledContents(false);
    setLabel(label);
}
