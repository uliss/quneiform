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

#include "language.h"
#include "page.h"
#include "recognitionsettings.h"
#include "recognitionsettingsdialog.h"
#include "ui_recognitionsettingsdialog.h"

RecognitionSettingsDialog::RecognitionSettingsDialog(Page * page, QWidget * parent) :
    QDialog(parent),
    ui_(new Ui::RecognitionSettingsDialog),
    page_(page) {
    ui_->setupUi(this);
    setup();
    connect(this, SIGNAL(accepted()), SLOT(save()));
}

RecognitionSettingsDialog::~RecognitionSettingsDialog() {
    delete ui_;
}

void RecognitionSettingsDialog::save() {
    Q_CHECK_PTR(page_);

    RecognitionSettings s = page_->recognitionSettings();
    s.setDotMatrix(ui_->dotMatrix_->isChecked());
    s.setFax(ui_->fax_->isChecked());
    s.setOneColumn(ui_->oneColumn_->isChecked());
    s.setPicturesSearch(ui_->searchPictures_->isChecked());
    s.useSpelling(ui_->useSpelling_->isChecked());
    page_->setRecognitionSettings(s);
}

void RecognitionSettingsDialog::setup() {
    Q_CHECK_PTR(page_);

    RecognitionSettings s = page_->recognitionSettings();
    ui_->dotMatrix_->setChecked(s.dotMatrix());
    ui_->fax_->setChecked(s.fax());
    ui_->useSpelling_->setChecked(s.spelling());
    ui_->oneColumn_->setChecked(s.oneColumn());
    ui_->searchPictures_->setChecked(s.picturesSearch());
}
