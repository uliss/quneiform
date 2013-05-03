/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include "kscandialog.h"
#include "ksane.h"
#include "settingskeys.h"

#include <QSettings>
#include <QMessageBox>
#include <QProgressDialog>
#include <QHBoxLayout>

using namespace KSaneIface;

KScanDialog::KScanDialog(QWidget * parent) :
    QDialog(parent),
    sane_widget_(NULL),
    layout_(NULL),
    image_(NULL),
    progress_(NULL)
{
    setWindowTitle(tr("Scanning"));
    initUi();
    initLayout();
}

QStringList KScanDialog::imagePath() const
{
    return QStringList();
}

void KScanDialog::initLayout()
{
    layout_ = new QHBoxLayout(this);
    layout_->addWidget(sane_widget_);
}

void KScanDialog::initUi()
{
    sane_widget_ = new KSaneWidget(this);
    connect(sane_widget_, SIGNAL(scanStart()), this, SLOT(scanStart()));
    connect(sane_widget_, SIGNAL(scanFaild()), this, SLOT(scanFailed()));
    connect(sane_widget_, SIGNAL(scanDone()),  this, SLOT(scanEnd()));
    connect(sane_widget_, SIGNAL(imageReady()), this, SLOT(imageReady()));
}

int KScanDialog::run()
{
    QString device;
    QSettings s;
    if(s.value(KEY_USE_LAST_SCANNER, false).toBool())
        device = s.value(KEY_LAST_SCANNER).toString();

    int last_scanner_idx = QSettings().value(KEY_LAST_SCANNER_INDEX).toInt();
    // scanner open dialog
    int selected_idx = 0;
    if(device.isEmpty())
        device = sane_widget_->selectDevice(NULL);

//        device = sane_widget_->selectDevice(NULL, last_scanner_idx, &selected_idx);

    if(device.isEmpty()) { // nothing selected
        return Rejected;
    }

    if(!sane_widget_->openDevice(device)) {
        QMessageBox::warning(this, tr("Scan error"), tr("Can't open selected scanner: \"%1\"").arg(device));
        s.setValue(KEY_LAST_SCANNER, QString());
        return Rejected;
    }

    QSettings().setValue(KEY_LAST_SCANNER_INDEX, selected_idx);

    // update last scanner
    s.setValue(KEY_LAST_SCANNER, device);
    setWindowTitle(tr("Scanner: %1").arg(device));
//    initIcons();

    int rc = exec();

    sane_widget_->closeDevice();
    return rc;
}
