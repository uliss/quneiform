/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2009-01-31
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
#include "ksane_opt_gamma.h"
#include "i18n.h"

#include "labeled_gamma.h"

// Qt includes
#include <QtCore/QVarLengthArray>

// KDE includes
#include <QDebug>
#include <QLocale>

namespace KSaneIface
{

KSaneOptGamma::KSaneOptGamma(const SANE_Handle handle, const int index)
: KSaneOption(handle, index), m_gamma(0)
{
}

void KSaneOptGamma::createWidget(QWidget *parent)
{
    if (m_widget) return;

    readOption();

    if (!m_optDesc) {
        kDebug() << "This is a bug";
        m_widget = new KSaneOptionWidget(parent, "");
        return;
    }

    m_widget = m_gamma = new LabeledGamma(parent, i18n(m_optDesc->title),
                                          m_optDesc->size/sizeof(SANE_Word));
    connect(m_gamma, SIGNAL(gammaTableChanged(QVector<int>)),
             this, SLOT(gammaTableChanged(QVector<int>)));
    if (strcmp(m_optDesc->name, SANE_NAME_GAMMA_VECTOR_R) == 0) m_gamma->setColor(Qt::red);
    if (strcmp(m_optDesc->name, SANE_NAME_GAMMA_VECTOR_G) == 0) m_gamma->setColor(Qt::green);
    if (strcmp(m_optDesc->name, SANE_NAME_GAMMA_VECTOR_B) == 0) m_gamma->setColor(Qt::blue);

    m_widget->setToolTip(i18n(m_optDesc->desc));
    updateVisibility();
    readValue();
}

void KSaneOptGamma::gammaTableChanged(const QVector<int> &gam_tbl)
{
    QVector<int> copy = gam_tbl;
    writeData(copy.data());
}

void KSaneOptGamma::readValue()
{
    // Unfortunately gamma table to brigthness, contrast and gamma is
    // not easy nor fast.. ergo not done
}

bool KSaneOptGamma::getValue(float &) {return false;}
bool KSaneOptGamma::setValue(float) {return false;}

bool KSaneOptGamma::getValue(QString &val)
{
    if (!m_gamma) return false;
    if (state() == STATE_HIDDEN) return false;
    int bri;
    int con;
    int gam;
    m_gamma->getValues(bri, con, gam);
    val = QString().sprintf("%d:%d:%d", bri, con, gam);
    return true;
}

bool KSaneOptGamma::setValue(const QString &val)
{
    if (!m_gamma) return false;
    if (state() == STATE_HIDDEN) return false;

    m_gamma->setValues(val);
    return true;
}

}  // NameSpace KSaneIface
