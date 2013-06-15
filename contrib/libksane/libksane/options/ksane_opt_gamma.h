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

#ifndef KSANE_OPT_GAMMA_H
#define KSANE_OPT_GAMMA_H

#include "ksane_option.h"

namespace KSaneIface
{

class LabeledGamma;

class KSaneOptGamma : public KSaneOption
{
    Q_OBJECT

public:
    KSaneOptGamma(const SANE_Handle handle, const int index);

    void createWidget(QWidget *parent);

    void readValue();

    bool getValue(float &val);
    bool setValue(float val);
    bool getValue(QString &val);
    bool setValue(const QString &val);
    bool hasGui() {return true;}
    
private Q_SLOTS:
    void gammaTableChanged(const QVector<int> &gam_tbl);

private:
    LabeledGamma *m_gamma;
};

}  // NameSpace KSaneIface

#endif
