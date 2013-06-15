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
#include "labeled_combo.h"

// Qt includes.
#include <QLabel>

// KDE includes
#include <QComboBox>
#include <QDebug>

namespace KSaneIface
{

LabeledCombo::LabeledCombo(QWidget *parent, const QString& ltext, const QStringList& list)
: KSaneOptionWidget(parent, ltext)
{
    m_combo  = new QComboBox(this);
    m_combo->addItems(list);

    m_label->setBuddy(m_combo);

    connect(m_combo, SIGNAL(activated(int)), this, SIGNAL(activated(int)));

    m_layout->addWidget(m_combo, 0, 1);
    m_layout->addWidget(new QWidget(this), 0, 2);
    m_layout->setColumnStretch(1, 0);
    m_layout->setColumnStretch(2, 50);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
}

void LabeledCombo::addItems(const QStringList &list)
{
    m_combo->addItems(list);

    QString tmp;
    for (int i=0; i<m_combo->count(); i++) {
        tmp = m_combo->itemText(i);
        m_combo->setItemData(i, tmp, Qt::ToolTipRole);
    }
}


void LabeledCombo::setCurrentText(const QString &t)
{
    int i;
    for (i=0; i<m_combo->count(); i++) {
        if (m_combo->itemText(i) == t) {
            m_combo->setCurrentIndex(i);
        }
    }
}

QString LabeledCombo::currentText()
{
    return m_combo->currentText();
}


bool LabeledCombo::setIcon(const QIcon &icon, const QString& str)
{
    for (int i=0; i < m_combo->count(); i++) {
        if (m_combo->itemText(i) == str) {
            m_combo->setItemIcon(i, icon);
            return true;
        }
    }
    return false;
}

void LabeledCombo::setCurrentIndex(int i)
{
    m_combo->setCurrentIndex(i);
}

}  // NameSpace KSaneIface
