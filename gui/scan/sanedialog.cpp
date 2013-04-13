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

#include <QMessageBox>
#include <QSettings>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QProgressDialog>

#include "sanedialog.h"
#include "sane_widget.h"
#include "settingskeys.h"
#include "iconutils.h"

SaneDialog::SaneDialog(QWidget * parent) :
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

QImage * SaneDialog::image()
{
    return image_;
}

QString SaneDialog::imagePath() const
{
    return saved_;
}

int SaneDialog::run()
{
    QString device;
    QSettings s;
    if(s.value(KEY_USE_LAST_SCANNER, false).toBool())
        device = s.value(KEY_LAST_SCANNER).toString();

    // scanner open dialog
    if(device.isEmpty())
        device = sane_widget_->selectDevice(NULL);

    if(device.isEmpty()) // nothing selected
        return Rejected;

    if(!sane_widget_->openDevice(device)) {
        QMessageBox::warning(this, tr("Scan error"), tr("Can't open selected scanner: \"%1\"").arg(device));
        s.setValue(KEY_LAST_SCANNER, QString());
        return Rejected;
    }

    // update last scanner
    s.setValue(KEY_LAST_SCANNER, device);
    setWindowTitle(tr("Scanner: %1").arg(device));
    initIcons();

    int rc = exec();

    sane_widget_->closeDevice();
    return rc;
}

void SaneDialog::initIcons()
{
//    sane_widget_->setIconColorMode(iconFromTheme("color"));
//    sane_widget_->setIconGrayMode(iconFromTheme("gray"));
//    sane_widget_->setIconBWMode(iconFromTheme("binarize"));
//    sane_widget_->setIconPreview(iconFromTheme("preview"));
    sane_widget_->setIconFinal(iconFromTheme("document-save"));
    sane_widget_->setIconZoomIn(iconFromTheme("zoom-in"));
    sane_widget_->setIconZoomOut(iconFromTheme("zoom-out"));
//    sane_widget_->setIconZoomSel(iconFromTheme("zoom-fit-selection"));
    sane_widget_->setIconZoomFit(iconFromTheme("zoom-fit-best"));
}

void SaneDialog::initUi()
{
    sane_widget_ = new SaneWidget(this);
    connect(sane_widget_, SIGNAL(scanStart()), this, SLOT(scanStart()));
    connect(sane_widget_, SIGNAL(scanFaild()), this, SLOT(scanFailed()));
    connect(sane_widget_, SIGNAL(scanDone()),  this, SLOT(scanEnd()));
    connect(sane_widget_, SIGNAL(imageReady()), this, SLOT(imageReady()));
}

void SaneDialog::initLayout()
{
    layout_ = new QHBoxLayout(this);
    layout_->addWidget(sane_widget_);
}

void SaneDialog::scanStart()
{
    if(!progress_) {
        progress_ = new QProgressDialog(this);
        progress_->setWindowTitle(tr("Scanning..."));
    }

    progress_->setMinimum(PROGRESS_MIN);
    progress_->setMaximum(PROGRESS_MAX);
    connect(progress_, SIGNAL(canceled()), sane_widget_, SLOT(scanCancel()));
    connect(sane_widget_, SIGNAL(scanProgress(int)), progress_, SLOT(setValue(int)));
    progress_->show();
}

void SaneDialog::scanEnd()
{
    if(!progress_)
        return;

    progress_->setValue(PROGRESS_MAX);
    progress_->hide();
}

void SaneDialog::scanFailed()
{
    if(!progress_)
        return;

    progress_->hide();
}

void SaneDialog::imageReady()
{
    QImage * img = sane_widget_->getFinalImage();
    QSettings s;
    if(s.value(KEY_SCAN_AUTOSAVE).toBool()) {

    }
    else {
        QString path = QFileDialog::getSaveFileName(this, tr("Save image to"), ".", "*.jpg");
        if(path.isEmpty()) {
            return;
        }

        if(!img->save(path, "JPEG")) {

        }

        saved_ = path;
        image_ = img;
        accept();
    }
}
