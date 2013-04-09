/*
   Copyright (C) 2007 Kåre Särs <kare.sars@kolumbus.fi>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <qlayout.h>

#include "labeled_checkbox.h"

LabeledCheckbox::LabeledCheckbox(QWidget *parent, const QString& ltext)
   : QFrame(parent)
{
    QHBoxLayout *hb = new QHBoxLayout(this);
    hb->setSpacing(2);
    hb->setMargin(2);
    chbx = new QCheckBox(ltext, this);
    hb->addWidget(chbx);
    hb->activate();

    connect(chbx, SIGNAL(toggled(bool)),
            this, SLOT(prToggled(bool)));
}

LabeledCheckbox::~LabeledCheckbox()
{
}

void LabeledCheckbox::setChecked(bool is_checked)
{
    if (is_checked != chbx->isChecked()) chbx->setChecked(is_checked);
}

void LabeledCheckbox::prToggled(bool on)
{
    emit toggled(on);
}

