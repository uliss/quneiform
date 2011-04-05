/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <QColorDialog>
#include <QDebug>
#include <QSettings>
#include <QColor>
#include "formatsettingsdialog.h"
#include "ui_formatsettingsdialog.h"

FormatSettingsDialog::FormatSettingsDialog(const FormatSettings& settings) :
    ui(new Ui::FormatSettingsDialog),
    settings_(settings)
{
    ui->setupUi(this);
    load();
    connect(this, SIGNAL(accepted()), SLOT(save()));
}

FormatSettingsDialog::~FormatSettingsDialog()
{
    delete ui;
}

void FormatSettingsDialog::load() {
    loadAlternatives();
    loadFonts();
    loadFormat();
}

void FormatSettingsDialog::loadAlternatives() {
    ui->showAlternatives->setChecked(settings_.showAlternatives());

    QSettings global_settings;
    global_settings.beginGroup("format");
    ui->alternativeColorButton->setColor(
            global_settings.value("alternativeColor", Qt::blue).value<QColor>());
}

void FormatSettingsDialog::loadFonts() {
    ui->useBold->setChecked(settings_.isBoldUsed());
    ui->useItalic->setChecked(settings_.isItalicUsed());
    ui->useUnderlined->setChecked(settings_.isUnderlinedUsed());
    ui->useFontSize->setChecked(settings_.isFontSizeUsed());
}

void FormatSettingsDialog::loadFormat() {
    // line breaks
    ui->preserveLineBreaks->setChecked(settings_.preserveLineBreaks());

    // unrecognized char
    for(int i = 0; i < ui->unrecognizedChar->count(); i++) {
        if(ui->unrecognizedChar->itemText(i) == settings_.unrecognizedChar()) {
            ui->unrecognizedChar->setCurrentIndex(i);
            break;
        }
    }
}

void FormatSettingsDialog::save() {
    saveAlternatives();
    saveFonts();
    saveFormat();
}

void FormatSettingsDialog::saveAlternatives() {
    settings_.setShowAlternatives(ui->showAlternatives->isChecked());
    QSettings settings;
    settings.beginGroup("format");
    settings.setValue("alternativeColor", ui->alternativeColorButton->color());
}

void FormatSettingsDialog::saveFonts() {
    settings_.setBoldUsed(ui->useBold->isChecked());
    settings_.setItalicUsed(ui->useItalic->isChecked());
    settings_.setUnderlinedUsed(ui->useUnderlined->isChecked());
    settings_.setFontSizeUsed(ui->useFontSize->isChecked());
}

void FormatSettingsDialog::saveFormat() {
    settings_.setPreserveLineBreaks(ui->preserveLineBreaks->isChecked());
    settings_.setUnrecognizedChar(ui->unrecognizedChar->currentText().at(0));
}

const FormatSettings& FormatSettingsDialog::settings() const {
    return settings_;
}
