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

    if(QIcon::themeName() != theme) {
        settings.setValue(KEY_ICON_THEME, theme);
        QIcon::setThemeName(theme);

        QMessageBox::information(NULL, tr("Theme changed"), tr("Restart application to apply icon theme settings."));
    }

    return  true;
}
