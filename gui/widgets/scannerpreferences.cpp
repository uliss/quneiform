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

#include <QDebug>
#include <QDesktopServices>
#include <QImageWriter>
#include <QFileDialog>
#include <QSettings>

#include "scannerpreferences.h"
#include "ui_scannerpreferences.h"
#include "iconutils.h"
#include "settingskeys.h"
#include "scan/scandialogtypes.h"

enum {
    USE_OS_DIALOG = 2,
    USE_QF_DIALOG = 0,
    USE_EXTERNAL_APP = 1
};

enum {
    PLACE_CHOOSE_IDX = 0,
    PLACE_PACKET_IDX = 1,
    PLACE_DIR_IDX = 2
};

static const int PLACE_COMBO_WIDTH = 250;

ScannerPreferences::ScannerPreferences(QWidget *parent) :
    PreferencesWidget(parent),
    ui_(new Ui::ScannerPreferences)
{
    setIcon(iconFromTheme("scanner"));
    setTitle(tr("Scanning"));

    ui_->setupUi(this);
    setupUi();
}

ScannerPreferences::~ScannerPreferences()
{
    delete ui_;
}

void ScannerPreferences::chooseAutosaveDir(int idx)
{
    if(idx != PLACE_CHOOSE_IDX)
        return;

    QString path = QFileDialog::getExistingDirectory(NULL,
                                                     tr("Choose autosave directory"),
                                                     QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));

    if(!path.isEmpty()) {
        QString text = ui_->autosavePlace->fontMetrics().elidedText(path, Qt::ElideLeft, PLACE_COMBO_WIDTH);

        if(ui_->autosavePlace->count() == 2) { // add new
            ui_->autosavePlace->addItem(text, path);
        }
        else { // update old
            ui_->autosavePlace->setItemText(PLACE_DIR_IDX, text);
            ui_->autosavePlace->setItemData(PLACE_DIR_IDX, path);
        }

        ui_->autosavePlace->setCurrentIndex(PLACE_DIR_IDX);
    }
    else {
        ui_->autosavePlace->setCurrentIndex(PLACE_PACKET_IDX);
    }
}

void ScannerPreferences::handlePageSelect(int idx)
{
    int data = ui_->scanTypeComboBox->itemData(idx).toInt();
    switch(data) {
    case SCAN_DIALOG_EXTERNAL_APP:
        ui_->stackedLayout->setCurrentIndex(USE_EXTERNAL_APP);
        break;
    case SCAN_DIALOG_QUNEIFORM:
        ui_->stackedLayout->setCurrentIndex(USE_QF_DIALOG);
        break;
    case SCAN_DIALOG_OS:
        ui_->stackedLayout->setCurrentIndex(USE_OS_DIALOG);
        break;
    default:
        qWarning() << Q_FUNC_INFO << "unknown data index:" << data;
    }
}

void ScannerPreferences::handleAutosaveToggle(bool value)
{
    QWidget * label = ui_->page1_Layout->labelForField(ui_->autosavePlace);
    if(label)
        label->setVisible(value);

    ui_->autosavePlace->setVisible(value);
}

void ScannerPreferences::setupUi()
{
    // uliss: NOTE
    // strange trick, but it works
    QMargins m = ui_->page1_Layout->contentsMargins();
    m.setTop(11);
    m.setBottom(11);
    ui_->page1_Layout->setContentsMargins(m);

    setupStack();
    setupExternalScanApp();
    setupUseLastScanner();
    setupFormat();
    setupImageQuality();
    setupAutosave();
}

void ScannerPreferences::setupStack()
{
    ui_->scanTypeComboBox->addItem(tr("Use operating system scan dialog"), SCAN_DIALOG_OS);

#ifdef WITH_SANE
    ui_->scanTypeComboBox->addItem(tr("Use quneiform dialog"), SCAN_DIALOG_QUNEIFORM);
#endif

    ui_->scanTypeComboBox->addItem(tr("Use external application"), SCAN_DIALOG_EXTERNAL_APP);

    connect(ui_->scanTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(handlePageSelect(int)));
    connectControl(ui_->scanTypeComboBox, SIGNAL(currentIndexChanged(int)), standartCallbacks(KEY_SCAN_DIALOG_TYPE));
    handlePageSelect(ui_->scanTypeComboBox->currentIndex());
}

void ScannerPreferences::setupFormat()
{
    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    typedef QList<QByteArray>::iterator Iterator;
    for(Iterator it = formats.begin(); it != formats.end(); ++it) {
        *it = it->toUpper();
    }

    if(formats.contains("PNG"))
        ui_->saveFormat->addItem("PNG", "PNG");
    if(formats.contains("BMP"))
        ui_->saveFormat->addItem("BMP", "BMP");
    if(formats.contains("TIFF"))
        ui_->saveFormat->addItem("TIFF", "TIFF");
    if(formats.contains("JPEG"))
        ui_->saveFormat->addItem("JPEG", "JPEG");

    connectControl(ui_->saveFormat, SIGNAL(currentIndexChanged(int)), standartCallbacks(KEY_SCAN_IMAGE_FORMAT));
}


void ScannerPreferences::setupExternalScanApp()
{
    ui_->selectApplicationWidget->setDirectory(QDesktopServices::storageLocation(QDesktopServices::ApplicationsLocation));
    ui_->selectApplicationWidget->setTitle(tr("Select application"));
    Callbacks cb(&loadExternalApp, &saveExternalApp);
    connectControl(ui_->selectApplicationWidget, SIGNAL(textChanged(QString)), cb);
}

void ScannerPreferences::setupUseLastScanner()
{
    connectControl(ui_->useLastScanner, SIGNAL(toggled(bool)), standartCallbacks(KEY_USE_LAST_SCANNER));
}

void ScannerPreferences::setupImageQuality()
{
    ui_->imageQuality->setRange(0, 100);
    ui_->imageQualitySlider->setRange(0, 100);
    connect(ui_->imageQuality, SIGNAL(valueChanged(int)), ui_->imageQualitySlider, SLOT(setValue(int)));
    connect(ui_->imageQualitySlider, SIGNAL(sliderMoved(int)), ui_->imageQuality, SLOT(setValue(int)));

    ui_->imageQualitySlider->setTracking(true);
    connectControl(ui_->imageQualitySlider, SIGNAL(valueChanged(int)), standartCallbacks(KEY_SCAN_IMAGE_QUALITY));
    ui_->imageQuality->setValue(ui_->imageQualitySlider->value());
}

void ScannerPreferences::setupAutosave()
{
    connectControl(ui_->autosave, SIGNAL(toggled(bool)), standartCallbacks(KEY_SCAN_AUTOSAVE));
    connect(ui_->autosave, SIGNAL(toggled(bool)), this, SLOT(handleAutosaveToggle(bool)));

    // NOTE: do not change order, cause PLACE_DIR_IDX and PLACE_PACKET_DIR are set constant
    ui_->autosavePlace->addItem(tr("Choose directory"), "");
    ui_->autosavePlace->addItem(tr("Save to packet directory"), "");
    connect(ui_->autosavePlace, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseAutosaveDir(int)));
    connectControl(ui_->autosavePlace, SIGNAL(currentIndexChanged(int)), Callbacks(&loadAutosaveType, &saveAutosaveType));
    handleAutosaveToggle(ui_->autosave->isChecked());
}

bool ScannerPreferences::loadAutosaveType(QWidget * w, const QVariant& /*data*/)
{
    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    QString path = QSettings().value(KEY_SCAN_AUTOSAVE_PLACE).toString();
    if(path.isEmpty()) {
        cb->setCurrentIndex(PLACE_PACKET_IDX);
        return true;
    }

    QString text = cb->fontMetrics().elidedText(path, Qt::ElideLeft, PLACE_COMBO_WIDTH);

    // add dir item
    if(cb->count() == 2) {
        cb->addItem(text, path);
    }

    cb->setItemData(PLACE_DIR_IDX, path);
    cb->setItemText(PLACE_DIR_IDX, text);
    cb->setCurrentIndex(PLACE_DIR_IDX);
    return true;
}

bool ScannerPreferences::saveAutosaveType(QWidget * w, const QVariant&/* data*/)
{
    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    int idx = cb->currentIndex();

    switch(idx) {
    case PLACE_CHOOSE_IDX: {
        return false;
    }
    case PLACE_PACKET_IDX:
        QSettings().setValue(KEY_SCAN_AUTOSAVE_PLACE, "");
        return true;
    case PLACE_DIR_IDX:
        QSettings().setValue(KEY_SCAN_AUTOSAVE_PLACE, cb->itemData(PLACE_DIR_IDX));
        return true;
    default:
        qWarning() << Q_FUNC_INFO << "Unknown item index:" << idx;
        return false;
    }
}

bool ScannerPreferences::loadExternalApp(QWidget * w, const QVariant&)
{
    FileSelectWidget * fs = qobject_cast<FileSelectWidget*>(w);
    if(!fs) {
        qWarning() << Q_FUNC_INFO << "invalid sender";
        return false;
    }

    fs->setPath(QSettings().value(KEY_SCAN_EXTERNAL_APP).toString());
    return true;
}

bool ScannerPreferences::saveExternalApp(QWidget * w, const QVariant&)
{
    FileSelectWidget * fs = qobject_cast<FileSelectWidget*>(w);
    if(!fs) {
        qWarning() << Q_FUNC_INFO << "invalid sender";
        return false;
    }

    QSettings().setValue(KEY_SCAN_EXTERNAL_APP, fs->path());
    return true;
}
