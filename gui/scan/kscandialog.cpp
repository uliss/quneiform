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
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopServices>

using namespace KSaneIface;

typedef QMap<QString, QString> ScanOptions;

static ScanOptions toScanOpts(const QMap<QString, QVariant>& map)
{
    ScanOptions res;

    QMap<QString, QVariant>::const_iterator it = map.constBegin();
    while (it != map.constEnd()) {
        res.insert(it.key(), it.value().toString());
        ++it;
    }

    return res;
}

static QMap<QString, QVariant> toMap(const ScanOptions& opts)
{
    QMap<QString, QVariant> res;

    ScanOptions::const_iterator it = opts.constBegin();
    while (it != opts.constEnd()) {
        res.insert(it.key(), it.value());
        ++it;
    }

    return res;
}

KScanDialog::KScanDialog(QWidget * parent) :
    QDialog(parent),
    sane_widget_(NULL),
    layout_(NULL),
    image_(new QImage)
{
    setWindowTitle(tr("Scanning"));
    initUi();
    initLayout();
}

QString KScanDialog::imagePath() const
{
    return saved_;
}

QString KScanDialog::autosaveDir() const
{
    QSettings s;

    QString place = s.value(KEY_SCAN_AUTOSAVE_PLACE).toString();
    return place;

    return QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
}

QString KScanDialog::autosaveImageName(const QString& dir) const
{
    QSettings s;

    int img_number = 0;
    const QString fname = s.value(KEY_SCAN_AUTOSAVE_FILENAME_TEMPLATE, "page").toString();
    const QString img_format = s.value(KEY_SCAN_IMAGE_FORMAT, "PNG").toString().toLower();
    QString full_name;
    do {
        full_name = QString("%1/%2%3.%4").arg(dir).arg(fname).arg(++img_number).arg(img_format);
    }
    while(QFileInfo(full_name).exists());

    return full_name;
}

void KScanDialog::initLayout()
{
    layout_ = new QHBoxLayout(this);
    layout_->addWidget(sane_widget_);
}

void KScanDialog::initUi()
{
    sane_widget_ = new KSaneWidget(this);

    connect(sane_widget_, SIGNAL(imageReady(QByteArray &, int, int, int, int)),
            this,     SLOT(imageReady(QByteArray &, int, int, int, int)));
//    connect(sane_widget_, SIGNAL(availableDevices(QList<KSaneWidget::DeviceInfo>)),
//            this,     SLOT(availableDevices(QList<KSaneWidget::DeviceInfo>)));
//    connect(sane_widget_, SIGNAL(userMessage(int, QString)),
//            this,     SLOT(alertUser(int, QString)));
//    connect(sane_widget_, SIGNAL(buttonPressed(QString, QString, bool)),
//            this,     SLOT(buttonPressed(QString, QString, bool)));

    sane_widget_->initGetDeviceList();
}

void KScanDialog::saveImage(const QString& path)
{
    if(!image_) {
        qWarning() << Q_FUNC_INFO << "image in NULL";
        return;
    }

    QSettings s;
    QString format = s.value(KEY_SCAN_IMAGE_FORMAT, "PNG").toString();
    int quality = s.value(KEY_SCAN_IMAGE_QUALITY, -1).toInt();

    if(!image_->save(path, format.toAscii().constData(), quality)) {
        QMessageBox::warning(this, tr("Warning"), tr("Image saving failed to \"%1\"").arg(path));
        return;
    }

    saved_ = path;
}

int KScanDialog::run()
{
    QSettings settings;

    ScanOptions opts = toScanOpts(settings.value("scan_options").toMap());
//    sane_widget_->setOptVals(opts);

    QString device;
    // use last scanner
    if(settings.value(KEY_USE_LAST_SCANNER, false).toBool()) {
        if(settings.value(KEY_LAST_SCANNER).isNull()) // no last scanner, open selection dialog
            device = sane_widget_->selectDevice(this);
        else // open last used scanner
            device = settings.value(KEY_LAST_SCANNER).toString();
    }
    else {
        // open selection dialog with default last scanner
        device = sane_widget_->selectDevice(this, settings.value(KEY_LAST_SCANNER).toString());
    }

    // nothing selected
    if(device.isEmpty())
        return Rejected;

    if(!sane_widget_->openDevice(device)) {
        QMessageBox::warning(this, tr("Scan error"), tr("Can't open selected scanner: \"%1\"").arg(device));
        settings.setValue(KEY_LAST_SCANNER, QString());
        return Rejected;
    }

    // update last scanner
    settings.setValue(KEY_LAST_SCANNER, device);
    setWindowTitle(tr("Scanner: %1").arg(device));

//    initIcons();

    int rc = exec();

    sane_widget_->getOptVals(opts);
    settings.setValue("scan_options", toMap(opts));
    sane_widget_->closeDevice();
    return rc;
}

void KScanDialog::imageReady(QByteArray& data, int width, int height, int bytes_per_line, int format)
{
    *image_ = sane_widget_->toQImageSilent(data, width, height, bytes_per_line,
                                           (KSaneWidget::ImageFormat)format);

    QSettings s;
    if(s.value(KEY_SCAN_AUTOSAVE).toBool()) {
        QString dir = autosaveDir();
        QString full_path = autosaveImageName(dir);
        saveImage(full_path);
        accept();
    }
    else {
        QString path = QFileDialog::getSaveFileName(this, tr("Save image to"), ".", "*.jpg");
        if(path.isEmpty()) {
            return;
        }

        if(!image_->save(path, "JPEG")) {

        }

        saved_ = path;
        accept();
    }
}
