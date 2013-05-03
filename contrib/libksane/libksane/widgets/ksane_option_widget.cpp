/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Description : Base class for option widgets
 *
 * Copyright (C) 2011 by Kare Sars <kare.sars@iki.fi>
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
#include "ksane_option_widget.h"
#include "i18n.h"

// Qt includes
#include <QDebug>
#include <QLocale>

namespace KSaneIface
{

KSaneOptionWidget::KSaneOptionWidget(QWidget *parent, const QString& labelText)
: QWidget(parent)
{
    m_label = new QLabel;
    setLabelText(labelText);

    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_label, 0, 0, Qt::AlignRight);
    m_layout->setColumnStretch(0, 0);
    m_layout->setContentsMargins(0,0,0,0);
}

KSaneOptionWidget::~KSaneOptionWidget()
{
}

void KSaneOptionWidget::setLabelText(const QString &text)
{
    if (text.isEmpty()) {
        m_label->setText(QString());
    } else {
        // FIXME!
//        m_label->setText(i18nc("Label for a scanner option", "%1:", text));
    }
}


int KSaneOptionWidget::labelWidthHint()
{
    return m_label->sizeHint().width();
}

void KSaneOptionWidget::setLabelWidth(int labelWidth)
{
    m_layout->setColumnMinimumWidth(0, labelWidth);
}

}  // NameSpace KSaneIface
