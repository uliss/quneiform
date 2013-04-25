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
#include <QSettings>

#include "scanpreferences.h"
#include "iconutils.h"
#include "settingskeys.h"

ScanPreferences::ScanPreferences(QWidget * parent) :
    PreferencesWidget(parent),
    layout_(NULL),
    autosave_dir_(NULL)
{
    setIcon(iconFromTheme("scanner"));
    setTitle(tr("Scanning"));

    setupLayout();
    setupUseLastScanner();
    setupAutosave();
}

void ScanPreferences::handleDirectoryInput(int idx)
{
    QComboBox * s = qobject_cast<QComboBox*>(sender());
    if(!s)
        return;

    if(s->itemData(idx).toString() == "dir") {
        autosave_dir_->setEnabled(true);
    }
    else {
        autosave_dir_->setEnabled(false);
    }
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

    QCheckBox * autosave = new QCheckBox(this);
    connectControl(autosave, SIGNAL(toggled(bool)), standartCallbacks(KEY_SCAN_AUTOSAVE));

    autosave_dir_ = new QLineEdit(this);

    QComboBox * autosave_type = new QComboBox(this);
    autosave_type->setEnabled(autosave->isChecked());
    autosave_dir_->setEnabled(autosave->isChecked());
    autosave_type->addItem(tr("Save to directory"), "dir");
    autosave_type->addItem(tr("Save to packet directory"), "packet");

    Callbacks autosave_type_cb(&loadAutosaveType, &saveAutosaveType);
    connectControl(autosave_type, SIGNAL(activated(int)), autosave_type_cb);
    connect(autosave, SIGNAL(toggled(bool)), autosave_type, SLOT(setEnabled(bool)));
    connect(autosave, SIGNAL(toggled(bool)), autosave_dir_, SLOT(setEnabled(bool)));
    connect(autosave_type, SIGNAL(activated(int)), this, SLOT(handleDirectoryInput(int)));

    layout_->addRow(tr("Autosave:"), autosave);
    layout_->addRow("", autosave_type);
    layout_->addRow("", autosave_dir_);
}

bool ScanPreferences::loadAutosaveType(QWidget * w, const QVariant& data)
{
    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    int current_idx = cb->findData(QSettings().value(KEY_SCAN_AUTOSAVE_METHOD, "packet").toString());
    if(current_idx >= 0)
        cb->setCurrentIndex(current_idx);

    return true;
}

bool ScanPreferences::saveAutosaveType(QWidget * w, const QVariant& data)
{
    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    int idx = cb->currentIndex();
    if(idx < 0)
        return false;

    QSettings().setValue(KEY_SCAN_AUTOSAVE_METHOD, cb->itemData(idx).toString());
    return true;
}
