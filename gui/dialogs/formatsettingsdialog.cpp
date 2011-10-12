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
#include <QSet>

#include "formatsettingsdialog.h"
#include "ui_formatsettingsdialog.h"
#include "page.h"

static const char * ALT_COLOR_KEY = "alternativeColor";

FormatSettingsDialog::FormatSettingsDialog() :
    ui_(new Ui::FormatSettingsDialog)
{
    ui_->setupUi(this);
    connect(this, SIGNAL(accepted()), SLOT(save()));
}

FormatSettingsDialog::~FormatSettingsDialog()
{
    delete ui_;
}

void FormatSettingsDialog::load() {
    loadAlternatives();
    loadFonts();
    loadFormat();
}

void FormatSettingsDialog::loadAlternatives() {
    setCheckboxState(ui_->showAlternatives, &FormatSettings::showAlternatives);
    ui_->alternativeColorButton->setEnabled(ui_->showAlternatives->checkState() == Qt::Checked);

    QSettings global_settings;
    global_settings.beginGroup("format");
    ui_->alternativeColorButton->setColor(
            global_settings.value(ALT_COLOR_KEY, Qt::blue).value<QColor>());
}

void FormatSettingsDialog::loadFonts() {
    setCheckboxState(ui_->useBold, &FormatSettings::isBoldUsed);
    setCheckboxState(ui_->useItalic, &FormatSettings::isItalicUsed);
    setCheckboxState(ui_->useUnderlined, &FormatSettings::isUnderlinedUsed);
    setCheckboxState(ui_->useFontSize, &FormatSettings::isFontSizeUsed);
}

void FormatSettingsDialog::loadFormat() {
    setCheckboxState(ui_->preserveLineBreaks, &FormatSettings::preserveLineBreaks);

    if(pages_.empty()) {
        return;
    }
    else if(pages_.count() == 1) {
        QChar uc = pages_.first()->formatSettings().unrecognizedChar();
        setUnrecognzedChar(uc);
    }
    else {
        QSet<QChar> chars;
        foreach(Page * page, pages_) {
            chars.insert(page->formatSettings().unrecognizedChar());
        }

        if(chars.count() == 1) {
            setUnrecognzedChar(*chars.begin());
        }
        else {
            ui_->unrecognizedChar->insertItem(0, " ");
            ui_->unrecognizedChar->setCurrentIndex(0);
        }
    }
}

void FormatSettingsDialog::save() {
    saveAlternatives();
    saveFonts();
    saveFormat();
}

void FormatSettingsDialog::saveAlternatives() {
    saveCheckboxState(ui_->showAlternatives, &FormatSettings::setShowAlternatives);

    QSettings settings;
    settings.beginGroup("format");
    settings.setValue(ALT_COLOR_KEY, ui_->alternativeColorButton->color());
}

void FormatSettingsDialog::saveFonts() {
    saveCheckboxState(ui_->useBold, &FormatSettings::setBoldUsed);
    saveCheckboxState(ui_->useItalic, &FormatSettings::setItalicUsed);
    saveCheckboxState(ui_->useUnderlined, &FormatSettings::setUnderlinedUsed);
    saveCheckboxState(ui_->useFontSize, &FormatSettings::setFontSizeUsed);
}

void FormatSettingsDialog::saveFormat() {
    saveCheckboxState(ui_->preserveLineBreaks, &FormatSettings::setPreserveLineBreaks);

    QChar uc = ui_->unrecognizedChar->currentText().at(0);
    if(uc.isSpace())
        return;

    foreach(Page * page, pages_) {
        FormatSettings fs = page->formatSettings();
        fs.setUnrecognizedChar(uc);
        page->setFormatSettings(fs);
    }
}

void FormatSettingsDialog::setup(Page * page)
{
    pages_.clear();
    pages_.append(page);
    load();
}

void FormatSettingsDialog::setup(const QList<Page*> pages)
{
    pages_ = pages;
    load();
}

void FormatSettingsDialog::setCheckboxState(QCheckBox * checkbox, FormatSettingsDialog::getFuncPtr fptr)
{
    if(pages_.isEmpty()) {
        return;
    }
    else if(pages_.count() == 1) {
        checkbox->setChecked((pages_.first()->formatSettings().*fptr)());
    }
    else {
        int count_on = 0;
        foreach(Page * page, pages_) {
            if((page->formatSettings().*fptr)())
                count_on++;
        }

        if(count_on == 0) {
            checkbox->setChecked(false);
        }
        else if(pages_.count() == count_on) {
            checkbox->setChecked(true);
        }
        else {
            checkbox->setTristate(true);
            checkbox->setCheckState(Qt::PartiallyChecked);
        }
    }
}

void FormatSettingsDialog::saveCheckboxState(QCheckBox * checkbox, FormatSettingsDialog::setFuncPtr fptr)
{
    if(checkbox->checkState() == Qt::PartiallyChecked)
        return;

    bool value = (checkbox->checkState() == Qt::Checked);

    foreach(Page * page, pages_) {
        FormatSettings settings = page->formatSettings();
        (settings.*fptr)(value);
        page->setFormatSettings(settings);
    }
}

void FormatSettingsDialog::setUnrecognzedChar(const QChar& c)
{
    if(c.isNull() || c.isSpace())
        return;

    for(int i = 0; i < ui_->unrecognizedChar->count(); i++) {
        if(ui_->unrecognizedChar->itemText(i) == c) {
            ui_->unrecognizedChar->setCurrentIndex(i);
            break;
        }
    }
}
