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

#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSettings>
#include <QApplication>
#include <QFileDialog>
#include <QDesktopServices>
#include <QIcon>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>

#include "generalpreferences.h"
#include "iconutils.h"
#include "settingskeys.h"

GeneralPreferences::GeneralPreferences(QWidget * parent) :
    PreferencesWidget(parent),
    layout_(NULL),
    ext_editor_(NULL)
{
    setIcon(iconFromTheme("configure"));
    setTitle(tr("General"));

    setupLayout();
    setupExternalEditor();
    setupIconTheme();
    setupAutosave();
}

void GeneralPreferences::setupAutosave()
{
    QCheckBox * autosave = new QCheckBox(this);
    connectControl(autosave, SIGNAL(toggled(bool)), standartCallbacks(KEY_AUTOSAVE));

    QComboBox * autosave_interval = new QComboBox(this);
    autosave_interval->setEnabled(autosave->isChecked());
    autosave_interval->addItem(tr("1 min"), 60);
    autosave_interval->addItem(tr("3 min"), 60 * 3);
    autosave_interval->addItem(tr("5 min"), 60 * 5);
    autosave_interval->addItem(tr("10 min"), 60 * 10);
    autosave_interval->addItem(tr("20 min"), 60 * 20);

    layout_->addRow(tr("Autosave:"), autosave);
    layout_->addRow(tr("Autosave interval:"), autosave_interval);

    Callbacks autosave_int_cb(&loadAutosaveInterval, &saveAutosaveInterval);
    connectControl(autosave_interval, SIGNAL(activated(int)), autosave_int_cb);
    connect(autosave, SIGNAL(toggled(bool)), autosave_interval, SLOT(setEnabled(bool)));
}

void GeneralPreferences::setupExternalEditor()
{
    Q_CHECK_PTR(layout_);

    ext_editor_ = new QLineEdit(this);
    ext_editor_->setMinimumWidth(200);
    QPushButton * ext_editor_btn = new QPushButton(tr("Select"), this);
    connect(ext_editor_btn, SIGNAL(clicked()), this, SLOT(showSelectApplicationDialog()));
    QHBoxLayout * ext_editor_layout = new QHBoxLayout;
    ext_editor_layout->addWidget(ext_editor_);
    ext_editor_layout->addWidget(ext_editor_btn);

    layout_->addRow(tr("External editor:"), ext_editor_layout);

    Callbacks cb(&loadExternalEditor, &saveExternalEditor);
    connectControl(ext_editor_, SIGNAL(textChanged(QString)), cb);
}

void GeneralPreferences::setupIconTheme()
{
    Q_CHECK_PTR(layout_);

    QComboBox * theme_select_ = new QComboBox(this);
    layout_->addRow(tr("Icon theme:"), theme_select_);

    Callbacks cb(&loadIconTheme, &saveIconTheme);
    connectControl(theme_select_, SIGNAL(currentIndexChanged(int)), cb);
}

void GeneralPreferences::setupLayout()
{
    layout_ = new QFormLayout;
    layout_->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    setLayout(layout_);
}

void GeneralPreferences::showSelectApplicationDialog()
{
    QString path = QFileDialog::getOpenFileName(this,
                                           tr("Select application"),
                                           QDesktopServices::storageLocation(QDesktopServices::ApplicationsLocation));

    activateWindow();

    if(path.isNull())
        return;

    ext_editor_->setText(path);
}

bool GeneralPreferences::loadExternalEditor(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);

    QLineEdit * le = qobject_cast<QLineEdit*>(w);
    if(!le)
        return false;

    QSettings s;
    le->blockSignals(true);
    le->setText(s.value(KEY_EXTERNAL_EDITOR, QString()).toString());
    le->blockSignals(false);
    return true;
}

bool GeneralPreferences::saveExternalEditor(QWidget * w, const QVariant &data)
{
    Q_UNUSED(data);

    QLineEdit * le = qobject_cast<QLineEdit*>(w);
    if(!le)
        return false;

    QSettings s;
    s.setValue(KEY_EXTERNAL_EDITOR, le->text());
    return true;
}

bool GeneralPreferences::loadIconTheme(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);

    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    QStringList themes = availableIconThemes();
    foreach(QString s, themes) {
        cb->addItem(s);
    }

    int item_idx = cb->findText(QIcon::themeName());
    if(item_idx == -1)
        return false;

    cb->setCurrentIndex(item_idx);
    return true;
}

bool GeneralPreferences::saveIconTheme(QWidget * w, const QVariant &data)
{
    Q_UNUSED(data);

    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    QSettings settings;
    int current_idx = cb->currentIndex();
    QString theme = cb->itemText(current_idx);

    if(QIcon::themeName() != theme && cb->count() > 1) { // not the only theme
        settings.setValue(KEY_ICON_THEME, theme);
        QIcon::setThemeName(theme);

        QMessageBox::information(NULL, tr("Theme changed"), tr("Restart application to apply icon theme settings."));
    }

    return  true;
}

bool GeneralPreferences::loadAutosaveInterval(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);

    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    int current_idx = cb->findData(QSettings().value(KEY_AUTOSAVE_INTERVAL, 60 * 10).toInt());
    if(current_idx >= 0)
        cb->setCurrentIndex(current_idx);

    return true;
}

bool GeneralPreferences::saveAutosaveInterval(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);

    QComboBox * cb = qobject_cast<QComboBox*>(w);
    if(!cb)
        return false;

    int idx = cb->currentIndex();
    if(idx < 0)
        return false;

    QSettings().setValue(KEY_AUTOSAVE_INTERVAL, cb->itemData(idx).toInt());
    return true;
}
