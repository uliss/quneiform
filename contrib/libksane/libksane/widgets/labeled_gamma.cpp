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
#include "labeled_gamma.h"
#include "i18n.h"

// C++ includes.
#include <cmath>

// Qt includes.
#include <QGroupBox>

namespace KSaneIface
{

LabeledGamma::LabeledGamma(QWidget *parent, const QString& text, int size)
: KSaneOptionWidget(parent, text)
{
    m_bri_slider = new LabeledSlider(this, tr("Brightness"), -50, 50, 1);
    m_bri_slider->setValue(0);

    m_con_slider = new LabeledSlider(this, tr("Contrast"), -50, 50, 1);
    m_con_slider->setValue(0);

    m_gam_slider = new LabeledSlider(this, tr("Gamma"), 30, 300, 1);
    m_gam_slider->setValue(100);

    // Calculate the size of the widgets in the sliders
    int labelMax = m_bri_slider->labelWidthHint();
    labelMax = qMax(labelMax, m_con_slider->labelWidthHint());
    labelMax = qMax(labelMax, m_gam_slider->labelWidthHint());
    // set the calculated widths
    m_bri_slider->setLabelWidth(labelMax);
    m_con_slider->setLabelWidth(labelMax);
    m_gam_slider->setLabelWidth(labelMax);

    m_gam_tbl.resize(size);
    for (int i=0; i<m_gam_tbl.size(); i++) {
        m_gam_tbl[i] = i;
    }
    m_max_val = size-1; // assume a gamma table 0 -> max

    m_gamma_disp = new GammaDisp(this, &m_gam_tbl);

    QGroupBox *groupBox = new QGroupBox(text, this);
    QGridLayout *gr_lay = new QGridLayout(groupBox);

    gr_lay->addWidget(m_bri_slider, 0, 0);
    gr_lay->addWidget(m_con_slider, 1, 0);
    gr_lay->addWidget(m_gam_slider, 2, 0);
    gr_lay->addWidget(m_gamma_disp, 0, 1, 3, 1);

    m_label->hide();
    m_layout->addWidget(groupBox, 1, 0, 1,3);

    connect(m_bri_slider, SIGNAL(valueChanged(int)), this, SLOT(calculateGT()));
    connect(m_con_slider, SIGNAL(valueChanged(int)), this, SLOT(calculateGT()));
    connect(m_gam_slider, SIGNAL(valueChanged(int)), this, SLOT(calculateGT()));
}

LabeledGamma::~LabeledGamma()
{
}

void LabeledGamma::setColor(const QColor &color)
{
    if (m_gamma_disp != 0) {
        m_gamma_disp->setColor(color);
    }
}

void LabeledGamma::setValues(int bri, int con, int gam)
{
    m_bri_slider->blockSignals(true);
    m_con_slider->blockSignals(true);
    m_gam_slider->blockSignals(true);

    m_bri_slider->setValue(bri);
    m_con_slider->setValue(con);
    m_gam_slider->setValue(gam);

    calculateGT();

    m_bri_slider->blockSignals(false);
    m_con_slider->blockSignals(false);
    m_gam_slider->blockSignals(false);
}

void LabeledGamma::setValues(const QString &values)
{
    m_bri_slider->blockSignals(true);
    m_con_slider->blockSignals(true);
    m_gam_slider->blockSignals(true);

    QStringList gammaValues;
    int bri;
    int con;
    int gam;
    bool ok = true;
    
    gammaValues = values.split(':');
    bri = gammaValues.at(0).toInt(&ok);
    if (ok) con = gammaValues.at(1).toInt(&ok);
    if (ok) gam = gammaValues.at(2).toInt(&ok);
    
    if (ok) {
        m_bri_slider->setValue(bri);
        m_con_slider->setValue(con);
        m_gam_slider->setValue(gam);
        calculateGT();
    }
    
    m_bri_slider->blockSignals(false);
    m_con_slider->blockSignals(false);
    m_gam_slider->blockSignals(false);
}


bool LabeledGamma::getValues(int &bri, int &con, int &gam)
{

    bri = m_bri_slider->value();
    con = m_con_slider->value();
    gam = m_gam_slider->value();
    return true;
}

void LabeledGamma::setSize(int size)
{
    m_gam_tbl.resize(size);
    for (int i=0; i<m_gam_tbl.size(); i++) {
        m_gam_tbl[i] = i;
    }
    m_bri_slider->setValue(0);
    m_con_slider->setValue(0);
    m_gam_slider->setValue(0);
}


void LabeledGamma::calculateGT()
{
    double gam      = 100.0/m_gam_slider->value();
    double con      = (200.0/(100.0 - m_con_slider->value()))-1;
    double half_max = m_max_val/2.0;
    double bri      = (m_bri_slider->value()/half_max) * m_max_val;
    double x;

    for (int i = 0; i<m_gam_tbl.size(); i++) {
        // apply gamma
        x = std::pow(i/m_max_val, gam) * m_max_val;

        // apply contrast
        x = (con*(x-half_max)) + half_max;

        // apply brightness + rounding
        x += bri + 0.5;

        // ensure correct value
        if (x > m_max_val) x = m_max_val;
        if (x < 0) x = 0;

        m_gam_tbl[i] = (int)x;
    }

    m_gamma_disp->update();
    emit gammaChanged(m_bri_slider->value(), m_con_slider->value(), m_gam_slider->value());
    emit gammaTableChanged(m_gam_tbl);
}


}  // NameSpace KSaneIface
