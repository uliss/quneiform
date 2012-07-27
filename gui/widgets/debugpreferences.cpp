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

#include <QCheckBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

#include "debugpreferences.h"
#include "settingskeys.h"
#include "iconutils.h"

DebugPreferences::DebugPreferences(QWidget * parent) :
    PreferencesWidget(parent),
    layout_(NULL)
{
    setTitle(tr("Debug"));
    setIcon(iconFromTheme("tools-report-bug"));
    setupLayout();
    setupUI();
}

void DebugPreferences::setupLayout()
{
    Q_ASSERT(!layout_);
    layout_ = new QVBoxLayout;
    setLayout(layout_);
}

void DebugPreferences::setupUI()
{
    Q_CHECK_PTR(layout_);

    QCheckBox * cb_print_cf_debug = new QCheckBox(this);
    cb_print_cf_debug->setText(tr("Print libcuneiform debug information"));
    layout_->addWidget(cb_print_cf_debug);
    connectControl(cb_print_cf_debug, SIGNAL(clicked()), standartCallbacks(KEY_PRINT_CUNEIFORM_DEBUG));

    QCheckBox * cb_cimage_debug = new QCheckBox(this);
    cb_cimage_debug->setText(tr("CImage module debugging"));
    layout_->addWidget(cb_cimage_debug);
    connectControl(cb_cimage_debug, SIGNAL(clicked()), standartCallbacks(KEY_MODULE_CIMAGE_DEBUG));

    QCheckBox * cb_process = new QCheckBox(this);
    cb_process->setText(tr("Recognize in separate process"));
    layout_->addWidget(cb_process);
    connectControl(cb_process, SIGNAL(clicked()), standartCallbacks(KEY_PROCESS_RECOGNITION));

    const int SECTION_SPACE = 10;
    layout_->addSpacing(SECTION_SPACE);

    setupUIFormatBBoxes();
    adjustSize();
}

void DebugPreferences::setupUIFormatBBoxes()
{
    QGroupBox * gb_format = new QGroupBox(this);
    gb_format->setTitle(tr("Show formatter bounding boxes for:"));
    layout_->addWidget(gb_format);

    QVBoxLayout * format_layout = new QVBoxLayout;
    QCheckBox * cb_sections = new QCheckBox(this);
    cb_sections->setText(tr("sections"));
    connectControl(cb_sections, SIGNAL(clicked()), standartCallbacks(KEY_DEBUG_SECTIONS_BBOX));

    QCheckBox * cb_columns = new QCheckBox(this);
    cb_columns->setText(tr("columns"));
    connectControl(cb_columns, SIGNAL(clicked()), standartCallbacks(KEY_DEBUG_COLUMNS_BBOX));

    QCheckBox * cb_paragraphs = new QCheckBox(this);
    cb_paragraphs->setText(tr("paragraphs"));
    connectControl(cb_paragraphs, SIGNAL(clicked()), standartCallbacks(KEY_DEBUG_PARAGRAPHS_BBOX));

    QCheckBox * cb_lines = new QCheckBox(this);
    cb_lines->setText(tr("lines"));
    connectControl(cb_lines, SIGNAL(clicked()), standartCallbacks(KEY_DEBUG_LINES_BBOX));

    QCheckBox * cb_chars = new QCheckBox(this);
    cb_chars->setText(tr("characters"));
    connectControl(cb_chars, SIGNAL(clicked()), standartCallbacks(KEY_DEBUG_CHARACTERS_BBOX));

    QCheckBox * cb_pictures = new QCheckBox(this);
    cb_pictures->setText(tr("pictures"));
    connectControl(cb_pictures, SIGNAL(clicked()), standartCallbacks(KEY_DEBUG_PICTURES_BBOX));

    format_layout->addWidget(cb_sections);
    format_layout->addWidget(cb_columns);
    format_layout->addWidget(cb_paragraphs);
    format_layout->addWidget(cb_lines);
    format_layout->addWidget(cb_chars);
    format_layout->addWidget(cb_pictures);
    gb_format->setLayout(format_layout);
}
