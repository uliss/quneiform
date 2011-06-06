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

#include <QFileDialog>
#include <QFileInfo>

#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(const QString& imagePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog),
    image_path_(imagePath)
{
    ui->setupUi(this);
    ui->formatComboBox->setCurrentIndex(0);
    setupFormatList();
    connect(ui->formatComboBox, SIGNAL(currentIndexChanged(int)), SLOT(handleFormatChange(int)));
    connect(this, SIGNAL(accepted()), SLOT(showOpenFileDialog()));
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

QString ExportDialog::fileName() const {
    return file_name_;
}

void ExportDialog::handleFormatChange(int index) {
    settings_.setFormat(static_cast<ExportSettings::FormatType>(ui->formatComboBox->itemData(index).toInt()));
}

ExportSettings ExportDialog::settings() const
{
    return settings_;
}

void ExportDialog::setupFormatList() {
    ui->formatComboBox->addItem(tr("Plain text"), ExportSettings::PLAINTEXT);
    ui->formatComboBox->addItem(tr("HTML"), ExportSettings::HTML);
    ui->formatComboBox->addItem(tr("ODF"), ExportSettings::ODF);
    ui->formatComboBox->addItem(tr("HOCR"), ExportSettings::HOCR);
    ui->formatComboBox->addItem(tr("DJVU text"), ExportSettings::DJVUTXT);
    ui->formatComboBox->addItem(tr("DJVU xml"), ExportSettings::DJVUXML);
}

void ExportDialog::showOpenFileDialog() {
    QString filter = "*." + settings_.extension();
    QFileInfo info(image_path_);
    QString filename_suggest = info.baseName() + "." + settings_.extension();

    file_name_ = QFileDialog::getSaveFileName(this,
                                                 tr("Export to"),
                                                 filename_suggest,
                                                 filter);
}

