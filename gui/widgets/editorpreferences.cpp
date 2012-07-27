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
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QSettings>
#include <QFont>
#include <QLineEdit>
#include <QDebug>

#include "editorpreferences.h"
#include "iconutils.h"
#include "settingskeys.h"
#include "fontbutton.h"
#include "colorbutton.h"

EditorPreferences::EditorPreferences(QWidget * parent) :
    PreferencesWidget(parent),
    layout_(NULL),
    editor_font_(NULL),
    current_char_color_(NULL),
    alt_char_color_(NULL)
{
    setIcon(iconFromTheme("accessories-text-editor"));
    setTitle(tr("Editor"));
    setupLayout();
    setupUI();
}

void EditorPreferences::setupLayout()
{
    layout_ = new QFormLayout;
    layout_->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    setLayout(layout_);
}

void EditorPreferences::setupUI()
{
    QCheckBox * show_current_char = new QCheckBox(this);
    layout_->addRow(tr("Show current character:"), show_current_char);
    connectControl(show_current_char, SIGNAL(clicked()), standartCallbacks(KEY_SHOW_CURRENT_CHAR));

    current_char_color_ = new ColorButton;
    current_char_color_->setEnabled(show_current_char->isChecked());
    layout_->addRow(tr("Current character color:"), current_char_color_);
    Callbacks cur_cb(&loadCurrentCharColor, &saveCurrentCharColor);
    connectControl(current_char_color_, SIGNAL(changed(QColor)), cur_cb);

    alt_char_color_ = new ColorButton;
    layout_->addRow(tr("Alternatives color:"), alt_char_color_);
    Callbacks alt_cb(&loadAlternativeCharColor, &saveAlternativeCharColor);
    connectControl(alt_char_color_, SIGNAL(changed(QColor)), alt_cb);

    editor_font_ = new FontButton;
    layout_->addRow(tr("Default font:"), editor_font_);
    Callbacks font_cb(&loadFont, &saveFont);
    connectControl(editor_font_, SIGNAL(changed(QFont)), font_cb);

    connect(show_current_char, SIGNAL(toggled(bool)), current_char_color_, SLOT(setEnabled(bool)));
}

bool EditorPreferences::loadFont(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);
    FontButton * btn = qobject_cast<FontButton*>(w);
    if(!btn) {
        qWarning() << Q_FUNC_INFO << "invalid widget given:" << w;
        return false;
    }

    QFont f = QSettings().value(KEY_EDITOR_FONT, QFont()).value<QFont>();
    btn->setCurrentFont(f);
    return true;
}

bool EditorPreferences::saveFont(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);
    FontButton * btn = qobject_cast<FontButton*>(w);
    if(!btn) {
        qWarning() << Q_FUNC_INFO << "invalid widget given:" << w;
        return false;
    }

    QSettings().setValue(KEY_EDITOR_FONT, btn->currentFont());
    return true;
}

bool EditorPreferences::loadAlternativeCharColor(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);
    ColorButton * btn = qobject_cast<ColorButton*>(w);
    if(!btn) {
        qWarning() << Q_FUNC_INFO << "invalid widget given:" << w;
        return false;
    }

    QColor c = QSettings().value(KEY_ALTERNATIVE_CHAR_COLOR, QColor(Qt::blue)).value<QColor>();
    btn->setColor(c);
    return true;
}

bool EditorPreferences::saveAlternativeCharColor(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);
    ColorButton * btn = qobject_cast<ColorButton*>(w);
    if(!btn) {
        qWarning() << Q_FUNC_INFO << "invalid widget given:" << w;
        return false;
    }

    QSettings().setValue(KEY_ALTERNATIVE_CHAR_COLOR, btn->color());
    return true;
}


bool EditorPreferences::loadCurrentCharColor(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);
    ColorButton * btn = qobject_cast<ColorButton*>(w);
    if(!btn) {
        qWarning() << Q_FUNC_INFO << "invalid widget given:" << w;
        return false;
    }

    QColor c = QSettings().value(KEY_CURRENT_CHAR_COLOR, QColor(Qt::red)).value<QColor>();
    btn->setColor(c);
    return true;
}

bool EditorPreferences::saveCurrentCharColor(QWidget * w, const QVariant& data)
{
    Q_UNUSED(data);
    ColorButton * btn = qobject_cast<ColorButton*>(w);
    if(!btn) {
        qWarning() << Q_FUNC_INFO << "invalid widget given:" << w;
        return false;
    }

    QSettings().setValue(KEY_CURRENT_CHAR_COLOR, btn->color());
    return true;
}

