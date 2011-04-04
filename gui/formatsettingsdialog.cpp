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
    settings_(settings),
    pixmap_(32, 32)
{
    ui->setupUi(this);
    load();
    connect(this, SIGNAL(accepted()), SLOT(save()));
    connect(ui->alternativeColorSelect, SIGNAL(clicked()), SLOT(showAlternativeColorDialog()));
}

FormatSettingsDialog::~FormatSettingsDialog()
{
    delete ui;
}

void FormatSettingsDialog::load() {
    ui->useBold->setChecked(settings_.isBoldUsed());
    ui->useItalic->setChecked(settings_.isItalicUsed());
    ui->useUnderlined->setChecked(settings_.isUnderlinedUsed());
    ui->preserveLineBreaks->setChecked(settings_.preserveLineBreaks());
    ui->useFontSize->setChecked(settings_.isFontSizeUsed());

    for(int i = 0; i < ui->unrecognizedChar->count(); i++) {
        if(ui->unrecognizedChar->itemText(i) == settings_.unrecognizedChar())
            ui->unrecognizedChar->setCurrentIndex(i);
    }

    ui->showAlternatives->setChecked(settings_.showAlternatives());

    QSettings global_settings;
    global_settings.beginGroup("format");
    QColor alt_color = global_settings.value("alternativeColor", QColor()).value<QColor>();
    if(alt_color.isValid()) {
        pixmap_.fill(alt_color);
        ui->alternativeColorSelect->setIcon(QIcon(pixmap_));
    }
}
void FormatSettingsDialog::save() {
    settings_.setBoldUsed(ui->useBold->isChecked());
    settings_.setItalicUsed(ui->useItalic->isChecked());
    settings_.setUnderlinedUsed(ui->useUnderlined->isChecked());
    settings_.setFontSizeUsed(ui->useFontSize->isChecked());
    settings_.setPreserveLineBreaks(ui->preserveLineBreaks->isChecked());
    settings_.setUnrecognizedChar(ui->unrecognizedChar->currentText().at(0));
    settings_.setShowAlternatives(ui->showAlternatives->isChecked());
}

void FormatSettingsDialog::showAlternativeColorDialog() {
    QColorDialog dialog;
    if(QDialog::Accepted == dialog.exec()) {
        QSettings global_settings;
        global_settings.beginGroup("format");
        global_settings.setValue("alternativeColor", dialog.selectedColor());
        pixmap_.fill(dialog.selectedColor());
        ui->alternativeColorSelect->setIcon(QIcon(pixmap_));
    }
}

const FormatSettings& FormatSettingsDialog::settings() const {
    return settings_;
}
