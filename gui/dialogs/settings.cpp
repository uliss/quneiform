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

#include <QSettings>
#include <QColorDialog>
#include <QFontDialog>

#include "settings.h"
#include "ui_settings.h"

QString fontName(const QFont& f) {
    QString res = f.family();
    res += QString(", %1pt").arg(f.pointSize());
    return res;
}

Settings::Settings(QWidget * parent) :
    QDialog(parent),
    ui_(new Ui::Settings)
{
    ui_->setupUi(this);
    connectSignals();
    load();
}

Settings::~Settings() {
    delete ui_;
}

void Settings::connectSignals() {
    connect(this, SIGNAL(accepted()), SLOT(save()));
    connect(this, SIGNAL(destroyed()), SLOT(saveDialogState()));
    connect(this->ui_->fontChoose, SIGNAL(clicked()), SLOT(showFontDialog()));
}

void Settings::load() {
    loadDebug();
    loadFormat();
    loadDialogState();
}

void Settings::loadDebug() {
    QSettings settings;
    settings.beginGroup("debug");
    ui_->printCuneiformDebug->setChecked(settings.value("printCuneiformDebug", false).toBool());
    settings.beginGroup("format");
    ui_->showSectionsBBox->setChecked(settings.value("showSectionsBBox", false).toBool());
    ui_->showColumnsBBox->setChecked(settings.value("showColumnsBBox", false).toBool());
    ui_->showParagraphsBBox->setChecked(settings.value("showParagraphsBBox", false).toBool());
    ui_->showLinesBBox->setChecked(settings.value("showLinesBBox", false).toBool());
    ui_->showCharactersBBox->setChecked(settings.value("showCharactersBBox", false).toBool());
    ui_->showPicturesBBox->setChecked(settings.value("showPicturesBBox", false).toBool());
}

void Settings::loadDialogState() {
    QSettings settings;
    settings.beginGroup("settings");
    settings.beginGroup("dialog");
    ui_->listWidget->setCurrentRow(settings.value("currentRow", 0).toInt());
}

void Settings::loadFormat() {
    QSettings settings;
    settings.beginGroup("format");
    ui_->currentCharacterColorButton->setColor(
            settings.value("currentCharColor", Qt::red).value<QColor>());
    ui_->alternativeColorButton->setColor(
            settings.value("alternativeColor", Qt::blue).value<QColor>());
    ui_->showCurrentCharacterCheckBox->setChecked(
            settings.value("showCurrentCharacter", true).toBool());

    QFont editorFont = settings.value("editorFont", QFont()).value<QFont>();
    ui_->fontChoose->setText(fontName(editorFont));
}

void Settings::save() {
    saveDebug();
    saveFormat();
}

void Settings::saveDebug() {
    QSettings settings;
    settings.beginGroup("debug");
    settings.setValue("printCuneiformDebug", ui_->printCuneiformDebug->isChecked());
    settings.beginGroup("format");
    settings.setValue("showSectionsBBox", ui_->showSectionsBBox->isChecked());
    settings.setValue("showColumnsBBox", ui_->showColumnsBBox->isChecked());
    settings.setValue("showParagraphsBBox", ui_->showParagraphsBBox->isChecked());
    settings.setValue("showLinesBBox", ui_->showLinesBBox->isChecked());
    settings.setValue("showCharactersBBox", ui_->showCharactersBBox->isChecked());
    settings.setValue("showPicturesBBox", ui_->showPicturesBBox->isChecked());
}

void Settings::saveDialogState() {
    QSettings settings;
    settings.beginGroup("settings");
    settings.beginGroup("dialog");
    settings.setValue("currentTab", ui_->listWidget->currentRow());
}

void Settings::saveFormat() {
    QSettings settings;
    settings.beginGroup("format");
    settings.setValue("currentCharColor", ui_->currentCharacterColorButton->color());
    settings.setValue("alternativeColor", ui_->alternativeColorButton->color());
    settings.setValue("showCurrentCharacter", ui_->showCurrentCharacterCheckBox->isChecked());
}

void Settings::showFontDialog()
{
    QSettings settings;
    settings.beginGroup("format");
    QFont initial = settings.value("editorFont", QFont()).value<QFont>();

    bool ok;
    QFont font = QFontDialog::getFont(&ok, initial, this);
    if (ok) {
        ui_->fontChoose->setText(fontName(font));
        settings.setValue("editorFont", font);
    }
}
