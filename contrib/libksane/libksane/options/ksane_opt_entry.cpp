/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2009-01-21
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2009 by Kare Sars <kare dot sars at iki dot fi>
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
#include "ksane_opt_entry.h"
#include "i18n.h"

#include "labeled_entry.h"

// Qt includes
#include <QtCore/QVarLengthArray>

// KDE includes
#include <QDebug>
#include <QLocale>

namespace KSaneIface
{

KSaneOptEntry::KSaneOptEntry(const SANE_Handle handle, const int index)
: KSaneOption(handle, index), m_entry(0)
{
}

void KSaneOptEntry::createWidget(QWidget *parent)
{
    if (m_widget) return;

    readOption();

    if (!m_optDesc) {
        kDebug() << "This is a bug";
        m_widget = new KSaneOptionWidget(parent, "");
        return;
    }

    m_widget = m_entry = new LabeledEntry(parent, sane_i18n(m_optDesc->title));
    m_widget->setToolTip(sane_i18n(m_optDesc->desc));
    connect(m_entry, SIGNAL(entryEdited(QString)), this, SLOT(entryChanged(QString)));
    updateVisibility();
    readValue();
}

void KSaneOptEntry::entryChanged(const QString& text)
{
    QString tmp;
    tmp += text.left(m_optDesc->size);
    if (tmp != text) {
        m_entry->setText(tmp);
        writeData(tmp.toLatin1().data());
    }
}

void KSaneOptEntry::readValue()
{
    if (state() == STATE_HIDDEN) return;

    // read that current value
    QVarLengthArray<unsigned char> data(m_optDesc->size);
    SANE_Status status;
    SANE_Int res;
    status = sane_control_option (m_handle, m_index, SANE_ACTION_GET_VALUE, data.data(), &res);
    if (status != SANE_STATUS_GOOD) {
        return;
    }

    m_string = QString(reinterpret_cast<char*>(data.data()));
    if (m_entry != 0) {
        m_entry->setText(m_string);
    }
}

bool KSaneOptEntry::getValue(float &) {return false;}
bool KSaneOptEntry::setValue(float) {return false;}

bool KSaneOptEntry::getValue(QString &val)
{
    if (state() == STATE_HIDDEN) return false;
    val = m_string;
    return true;
}

bool KSaneOptEntry::setValue(const QString &val)
{
    if (state() == STATE_HIDDEN) return false;
    entryChanged(val);
    readValue();
    return true;
}

}  // NameSpace KSaneIface
