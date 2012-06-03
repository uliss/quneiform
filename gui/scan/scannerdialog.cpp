/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include "scannerdialog.h"
#include "ui_scannerdialog.h"
#include "scanner.h"

ScannerDialog::ScannerDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::ScannerDialog),
    scanner_(NULL)
{
    ui_->setupUi(this);
    scanner_ = new Scanner(this);
    setupUi();
}

ScannerDialog::~ScannerDialog()
{
    delete ui_;
    delete scanner_;
}

void ScannerDialog::setupUi()
{
    QStringList l = scanner_->listDevices();
    foreach(QString s, l) {
        ui_->scannerComboBox->addItem(s);
    }

    l.pop_back();

    if(l.count() > 1) {
        ui_->stackedWidget->setCurrentWidget(ui_->scannerList);
    }
    else if(l.count() == 1) {
        handleScannerSelect(0);
        ui_->stackedWidget->setCurrentWidget(ui_->scanDialog);
    }

    connect(ui_->scannerComboBox,
            SIGNAL(currentIndexChanged(int)),
            SLOT(handleScannerSelect(int)));
}

void ScannerDialog::handleScannerSelect(int idx)
{
    QString scanner_name = ui_->scannerComboBox->itemText(idx);
    ui_->scannerName->setText(scanner_name);

    bool rc = scanner_->open(scanner_name);

    if(!rc)
        qDebug() << "can't open scanner: " << scanner_name;

    ui_->scanDialog->layout()->addWidget(scanModeWidget());
}

QComboBox * ScannerDialog::scanModeWidget()
{
    QComboBox * res = new QComboBox(this);

    if(!scanner_)
        return res;

    ScannerOption modes = scanner_->option("mode");
    if(!modes.isValid())
        return res;

    foreach(QVariant mode, modes.allowedValues()) {
        qDebug() << mode.toString();
        res->addItem(mode.toString());
    }

    return res;
}

