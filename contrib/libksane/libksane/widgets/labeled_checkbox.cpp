/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2007-09-13
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2007-2011 by Kare Sars <kare.sars@iki .fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================ */

// Local includes.
#include "labeled_checkbox.h"

// Qt includes
#include <QDebug>

namespace KSaneIface
{

LabeledCheckbox::LabeledCheckbox(QWidget *parent, const QString& ltext)
: KSaneOptionWidget(parent, QString())
{
    chbx = new QCheckBox(ltext, this);
    m_layout->addWidget(chbx, 0, 1);
    m_layout->setColumnStretch(1, 50);

    connect(chbx, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));
}

LabeledCheckbox::~LabeledCheckbox()
{
}

void LabeledCheckbox::setChecked(bool is_checked)
{
    if (is_checked != chbx->isChecked()) chbx->setChecked(is_checked);
}

bool LabeledCheckbox::isChecked()
{
    return chbx->isChecked();
}

}  // NameSpace KSaneIface
