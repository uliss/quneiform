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
#include "labeled_entry.h"
#include "i18n.h"

// Qt includes.
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QPushButton>

namespace KSaneIface
{

LabeledEntry::LabeledEntry(QWidget *parent, const QString& ltext)
: KSaneOptionWidget(parent, ltext)
{
    m_entry = new QLineEdit(this);
    m_reset = new QPushButton(this);
    m_reset->setText(tr("Reset"));
    m_set = new QPushButton(this);
    m_set->setText(tr("Set"));

    m_layout->addWidget(m_entry, 1, 0, 1, 2);
    m_layout->addWidget(m_reset, 1, 2);
    m_layout->addWidget(m_set, 1, 3);
    m_layout->setColumnStretch(1, 50);
    
    connect(m_reset, SIGNAL(clicked()), this, SLOT(resetClicked()));
    connect(m_set,   SIGNAL(clicked()), this, SLOT(setClicked()));
}

LabeledEntry::~LabeledEntry()
{
}

void LabeledEntry::setText(const QString& text)
{
    m_eText = text;
    m_entry->setText(text);
}

void LabeledEntry::resetClicked()
{
    m_entry->setText(m_eText);
}

void LabeledEntry::setClicked()
{
    m_eText = m_entry->text();
    emit entryEdited(m_eText);
}

}  // NameSpace KSaneIface
