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


#include "recognitionsettingsdialog.h"
#include "ui_recognitionsettingsdialog.h"

RecognitionSettingsDialog::RecognitionSettingsDialog(const RecognitionSettings& opts,
                                               QWidget * parent) :
    QDialog(parent),
    ui_(new Ui::RecognitionSettingsDialog),
    opts_(opts) {
    ui_->setupUi(this);
    setup();
    connect(this, SIGNAL(accepted()), SLOT(save()));
}

RecognitionSettingsDialog::~RecognitionSettingsDialog() {
    delete ui_;
}

const RecognitionSettings& RecognitionSettingsDialog::settings() const {
    return opts_;
}

void RecognitionSettingsDialog::save() {
    opts_.setDotMatrix(ui_->dotMatrix_->isChecked());
    opts_.setFax(ui_->fax_->isChecked());
    opts_.setOneColumn(ui_->oneColumn_->isChecked());
    opts_.setPicturesSearch(ui_->searchPictures_->isChecked());
    opts_.useSpelling(ui_->useSpelling_->isChecked());
}

void RecognitionSettingsDialog::setup() {
    ui_->dotMatrix_->setChecked(opts_.dotMatrix());
    ui_->fax_->setChecked(opts_.fax());
    ui_->useSpelling_->setChecked(opts_.spelling());
    ui_->oneColumn_->setChecked(opts_.oneColumn());
    ui_->searchPictures_->setChecked(opts_.picturesSearch());
}
