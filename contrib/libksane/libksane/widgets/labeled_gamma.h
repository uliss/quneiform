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

#ifndef LABELED_GAMMA_H
#define LABELED_GAMMA_H

// Local includes.
#include "labeled_slider.h"
#include "gamma_disp.h"

/**
  *@author Kåre Särs
  */

namespace KSaneIface
{

/**
 * A wrapper for a checkbox
 */
class LabeledGamma : public KSaneOptionWidget
{
    Q_OBJECT

public:

   /**
    * Create the checkbox.
    *
    * \param parent parent widget
    * \param text is the text describing the checkbox.
    */
    LabeledGamma(QWidget *parent, const QString& text, int elements);
    ~LabeledGamma();

    void setColor(const QColor &color);
    void setSize(int size);
    const QVector<int> &gammaTablePtr() { return m_gam_tbl; }
    int size() {return (int)(m_max_val+1);}

    bool getValues(int &bri, int &con, int &gam);

public Q_SLOTS:
    void setValues(int bri, int con, int gam);
    void setValues(const QString &values);
    
private Q_SLOTS:
    void calculateGT();

Q_SIGNALS:

    void gammaChanged(int bri, int con, int gam);
    void gammaTableChanged(const QVector<int> &gamma_tbl);

private:

    LabeledSlider *m_bri_slider;
    LabeledSlider *m_con_slider;
    LabeledSlider *m_gam_slider;

    QVector<int>   m_gam_tbl;
    double         m_max_val;

    GammaDisp     *m_gamma_disp;
};

}  // NameSpace KSaneIface

#endif // LABELED_GAMMA_H
