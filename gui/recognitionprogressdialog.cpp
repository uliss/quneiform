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
#include <QFileInfo>

#include "page.h"
#include "recognitionprogressdialog.h"
#include "ui_recognitionprogressdialog.h"

RecognitionProgressDialog::RecognitionProgressDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::RecognitionProgressDialog), paused_(false)
{
    ui->setupUi(this);
    connect(ui->abortButton, SIGNAL(clicked()), SLOT(close()));
    connect(ui->abortButton, SIGNAL(clicked()), SIGNAL(aborted()));
    connect(ui->pauseButton, SIGNAL(clicked()), SLOT(pause()));
}

RecognitionProgressDialog::~RecognitionProgressDialog()
{
    delete ui;
}

void RecognitionProgressDialog::pause() {
    paused_ = !paused_;
    ui->pauseButton->setText(paused_ ? tr("Resume") : tr("Pause"));
    if(paused_)
        emit paused();
    else
        emit resumed();
}

void RecognitionProgressDialog::reset() {
    ui->pageLabel->setText("");
    ui->totalProgress->reset();
}

void RecognitionProgressDialog::setCurrentPage(Page * const p) {
    Q_CHECK_PTR(p);

    QFileInfo fi(p->imagePath());
    ui->pageLabel->setText(tr("Page recognition: \"%1\"").arg(fi.fileName()));
}

void RecognitionProgressDialog::setTotalValue(int value) {
    ui->totalProgress->setValue(value);
}
