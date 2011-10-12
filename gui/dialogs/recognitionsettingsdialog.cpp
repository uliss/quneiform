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

RecognitionSettingsDialog::RecognitionSettingsDialog(QWidget * parent) :
    QDialog(parent),
    ui_(new Ui::RecognitionSettingsDialog)
{
    ui_->setupUi(this);
    connect(this, SIGNAL(accepted()), SLOT(save()));
}

RecognitionSettingsDialog::~RecognitionSettingsDialog() {
    delete ui_;
}

void RecognitionSettingsDialog::save() {
    saveCheckboxState(ui_->dotMatrix_, &RecognitionSettings::setDotMatrix);
    saveCheckboxState(ui_->fax_, &RecognitionSettings::setFax);
    saveCheckboxState(ui_->useSpelling_, &RecognitionSettings::useSpelling);
    saveCheckboxState(ui_->oneColumn_, &RecognitionSettings::setOneColumn);
    saveCheckboxState(ui_->searchPictures_, &RecognitionSettings::setPicturesSearch);
}

void RecognitionSettingsDialog::setup() {
    setCheckboxState(ui_->dotMatrix_, &RecognitionSettings::dotMatrix);
    setCheckboxState(ui_->fax_, &RecognitionSettings::fax);
    setCheckboxState(ui_->useSpelling_, &RecognitionSettings::spelling);
    setCheckboxState(ui_->oneColumn_, &RecognitionSettings::oneColumn);
    setCheckboxState(ui_->searchPictures_, &RecognitionSettings::picturesSearch);
}

void RecognitionSettingsDialog::setCheckboxState(QCheckBox * checkbox,
                                                 RecognitionSettingsDialog::getFuncPtr fptr)
{
    if(pages_.isEmpty()) {
        return;
    }
    else if(pages_.count() == 1) {
        checkbox->setTristate(false);
        checkbox->setChecked((pages_.first()->recognitionSettings().*fptr)());
    }
    else {
        int checked = 0;
        foreach(Page * page, pages_) {
            if((page->recognitionSettings().*fptr)())
                checked++;
        }

        if(checked == 0) {
            checkbox->setCheckState(Qt::Unchecked);
        }
        else if(checked == pages_.size()) {
            checkbox->setCheckState(Qt::Checked);
        }
        else {
            checkbox->setTristate(true);
            checkbox->setCheckState(Qt::PartiallyChecked);
        }
    }
}

void RecognitionSettingsDialog::saveCheckboxState(QCheckBox * checkbox,
                                                  RecognitionSettingsDialog::setFuncPtr fptr)
{
    if(checkbox->checkState() == Qt::PartiallyChecked)
        return;

    bool value = (checkbox->checkState() == Qt::Checked);

    foreach(Page * page, pages_) {
        RecognitionSettings settings = page->recognitionSettings();
        (settings.*fptr)(value);
        page->setRecognitionSettings(settings);
    }
}

void RecognitionSettingsDialog::setup(Page * p)
{
    pages_.append(p);
    setup();
}

void RecognitionSettingsDialog::setup(const QList<Page*>& pages) {
    pages_ = pages;
    setup();
}
