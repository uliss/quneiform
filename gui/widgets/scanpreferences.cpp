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

#include <QFormLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QImageWriter>
#include <QSettings>
#include <QDebug>

#include "scanpreferences.h"
#include "iconutils.h"
#include "settingskeys.h"

static const int PLACE_CHOOSE_IDX = 0;
static const int PLACE_PACKET_IDX = 1;
static const int PLACE_DIR_IDX = 2;
static const int PLACE_COMBO_WIDTH = 250;

ScanPreferences::ScanPreferences(QWidget * parent) :
    PreferencesWidget(parent),
    layout_(NULL),
    save_quality_(NULL),
    save_quality_box_(NULL),
    save_format_(NULL),
    autosave_(NULL),
    autosave_dir_(NULL)
{
    setIcon(iconFromTheme("scanner"));
    setTitle(tr("Scanning"));

    setupLayout();
    setupUseLastScanner();
    setupQuality();
    setupFormat();
    setupAutosave();
}

void ScanPreferences::handleAutosaveToggle(bool value)
{
    QWidget * label = layout_->labelForField(autosave_dir_);
    if(label)
        label->setVisible(value);

    autosave_dir_->setVisible(value);
}

void ScanPreferences::setupLayout()
{
    layout_ = new QFormLayout;
    layout_->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    setLayout(layout_);
}

void ScanPreferences::setupUseLastScanner()
{
    Q_CHECK_PTR(layout_);

    QCheckBox * use_last_scanner = new QCheckBox(this);
    connectControl(use_last_scanner, SIGNAL(toggled(bool)), standartCallbacks(KEY_USE_LAST_SCANNER));
    layout_->addRow(tr("Use last scanner:"), use_last_scanner);
}

void ScanPreferences::setupAutosave()
{
    Q_CHECK_PTR(layout_);

    autosave_ = new QCheckBox(this);
    connectControl(autosave_, SIGNAL(toggled(bool)), standartCallbacks(KEY_SCAN_AUTOSAVE));
    connect(autosave_, SIGNAL(toggled(bool)), this, SLOT(handleAutosaveToggle(bool)));
    layout_->addRow(tr("Autosave:"), autosave_);

    autosave_dir_ = new QComboBox(this);
    // NOTE: do not change order, cause PLACE_DIR_IDX and PLACE_PACKET_DIR are set constant
    autosave_dir_->addItem(tr("Choose directory"), "");
    autosave_dir_->addItem(tr("Save to packet directory"), "");
    connectControl(autosave_dir_, SIGNAL(currentIndexChanged(int)), Callbacks(&loadAutosaveType, &saveAutosaveType));

    layout_->addRow(tr("Autosave place:"), autosave_dir_);
    handleAutosaveToggle(autosave_->isChecked());
}

void ScanPreferences::setupFormat()
{
    save_format_ = new QComboBox(this);

    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    typedef QList<QByteArray>::iterator Iterator;
    for(Iterator it = formats.begin(); it != formats.end(); ++it) {
        *it = it->toUpper();
    }

    if(formats.contains("PNG"))
        save_format_->addItem("PNG", "PNG");
    if(formats.contains("BMP"))
        save_format_->addItem("BMP", "BMP");
    if(formats.contains("TIFF"))
        save_format_->addItem("TIFF", "TIFF");
    if(formats.contains("JPEG"))
        save_format_->addItem("JPEG", "JPEG");

    connectControl(save_format_, SIGNAL(currentIndexChanged(int)), standartCallbacks(KEY_SCAN_IMAGE_FORMAT));

    layout_->addRow(tr("Image format:"), save_format_);
}

void ScanPreferences::setupQuality()
{
    save_quality_ = new QSlider(Qt::Horizontal, this);
    save_quality_->setTracking(true);
    save_quality_->setRange(0, 100);
    connectControl(save_quality_, SIGNAL(valueChanged(int)), standartCallbacks(KEY_SCAN_IMAGE_QUALITY));

    save_quality_box_ = new QSpinBox(this);
    save_quality_box_->setRange(0, 100);
    save_quality_box_->setValue(save_quality_->sliderPosition());
    connect(save_quality_, SIGNAL(sliderMoved(int)), save_quality_box_, SLOT(setValue(int)));
    connect(save_quality_box_, SIGNAL(valueChanged(int)), save_quality_, SLOT(setValue(int)));

    QHBoxLayout * quality_layout = new QHBoxLayout();
    quality_layout->addWidget(save_quality_);
    quality_layout->addWidget(save_quality_box_);

    layout_->addRow(tr("Image quality:"), quality_layout);
}

bool ScanPreferences::loadAutosaveType(QWidget * w, const QVariant& /*data*/)
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

bool ScanPreferences::saveAutosaveType(QWidget * w, const QVariant& data)
{
    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    int idx = cb->currentIndex();

    switch(idx) {
    case PLACE_CHOOSE_IDX: {
        QString path = QFileDialog::getExistingDirectory(NULL,
                                                         tr("Choose autosave directory"),
                                                         QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));

        if(!path.isEmpty()) {
            QString text = cb->fontMetrics().elidedText(path, Qt::ElideLeft, PLACE_COMBO_WIDTH);

            if(cb->count() == 2) { // add new
                cb->addItem(text, path);
            }
            else { // update old
                cb->setItemText(PLACE_DIR_IDX, text);
                cb->setItemData(PLACE_DIR_IDX, path);
            }

            cb->setCurrentIndex(PLACE_DIR_IDX);
        }
        else {
            cb->setCurrentIndex(PLACE_PACKET_IDX);
        }

        return true;
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
