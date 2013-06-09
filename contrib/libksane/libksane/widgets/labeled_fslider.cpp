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
#include "labeled_fslider.h"

#define FLOAT_MULTIP 32768.0
#define TO_FLOAT(v) ((float)v / FLOAT_MULTIP)
#define TO_FIX(v) ((int)(v * FLOAT_MULTIP))

namespace KSaneIface
{

LabeledFSlider::LabeledFSlider(QWidget *parent, const QString& ltext,
                               float min, float max, float step)
: KSaneOptionWidget(parent, ltext)
{
    int imin = TO_FIX(min);
    int imax = TO_FIX(max);
    m_istep = TO_FIX(step);
    m_fstep = step;
    if (m_istep == 0) {
        m_istep = 1;
        m_fstep = TO_FLOAT(m_istep);
    }

    //std::cout << "min=" << min << ", max=" << max << ", m_fstep="<<m_fstep<<std::endl;
    //std::cout << "imin=" << imin << ", imax=" << imax << ", m_istep="<<m_istep<<std::endl;
    m_slider = new QSlider(this);
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setMinimum(imin);
    m_slider->setMaximum(imax);
    m_slider->setSingleStep(m_istep);
    m_slider->setValue(imin);

    m_spinb = new QDoubleSpinBox(this);
    m_spinb->setMinimum(min);
    m_spinb->setMaximum(max);
    m_spinb->setSingleStep(m_fstep);
    int decimals=0;
    float tmp_step = m_fstep;
    while (tmp_step < 1) {
        tmp_step *= 10;
        decimals++;
        if (decimals > 5) break;
    }
    m_spinb->setDecimals(decimals);
    m_spinb->setValue(max);
    //m_spinb->setMinimumWidth(m_spinb->sizeHint().width()+35);
    m_spinb->setMinimumWidth(m_spinb->sizeHint().width());
    m_spinb->setAlignment(Qt::AlignRight);
    m_spinb->setValue(min);

    m_label->setBuddy(m_spinb);

    connect(m_spinb,  SIGNAL(valueChanged(double)), this, SLOT(syncValues(double)));
    connect(m_slider, SIGNAL(valueChanged(int)),    this, SLOT(syncValues(int)));
    connect(m_slider, SIGNAL(sliderReleased()),   this, SLOT(fixValue()));

    m_layout->addWidget(m_slider, 0,2);
    m_layout->addWidget(m_spinb, 0, 1);
    m_layout->setColumnStretch(1, 0);
    m_layout->setColumnStretch(2, 50);
    m_layout->activate();
}

LabeledFSlider::~LabeledFSlider()
{
}

void LabeledFSlider::setSuffix(const QString &text)
{
    m_spinb->setSuffix(text);
}

void LabeledFSlider::setRange(float min, float max)
{
    //kDebug() << "min,max(" << m_spinb->minimum() << " - " << m_spinb->maximum();
    //kDebug() << ") -> (" << min << " - " << max << ")" << std::endl;
    int imin=TO_FIX(min);
    int imax=TO_FIX(max);
    m_slider->setRange(imin, imax);
    //std::cout << "fixedmin=" << imin << " fixedmax=" << imax << std::endl;

    m_spinb->setRange(min, max);
}

void LabeledFSlider::setStep(float step)
{
    m_istep = TO_FIX(step);
    m_fstep = step;
    if (m_istep == 0) {
        m_istep = 1;
        m_fstep = TO_FLOAT(m_istep);
    }
    m_slider->setSingleStep(m_istep);
    m_spinb->setSingleStep(m_fstep);

    int decimals=0;
    float tmp_step = m_fstep;
    while (tmp_step < 1) {
        tmp_step *= 10;
        decimals++;
        if (decimals > 5) break;
    }
    m_spinb->setDecimals(decimals);
}

void LabeledFSlider::setValue(float value)
{
    int ivalue = TO_FIX(value);

    if (((value - m_spinb->value()) > m_fstep) || ((m_spinb->value() - value) > m_fstep)) {
        m_spinb->setValue(value);
    }
    else if (ivalue != m_slider->value()) m_slider->setValue(ivalue);
}

void LabeledFSlider::syncValues(int ivalue)
{
    double value = TO_FLOAT(ivalue);
    if (((value - m_spinb->value()) > m_fstep) || ((m_spinb->value() - value) > m_fstep)) {
        m_spinb->setValue(value);
    }
    else if (ivalue != m_slider->value())m_slider->setValue(ivalue);
    else emit valueChanged(value);
}

void LabeledFSlider::syncValues(double value)
{
    int ivalue = TO_FIX(value);
    if (((value - m_spinb->value()) > m_fstep) || ((m_spinb->value() - value) > m_fstep)) {
        m_spinb->setValue(value);
    }
    else if (ivalue != m_slider->value())m_slider->setValue(ivalue);
    else emit valueChanged((float)value);
}

void LabeledFSlider::fixValue()
{
    //ensure that the value step is followed also for the m_slider
    int rest = (m_slider->value() - m_slider->minimum())%m_istep;

    if (rest != 0) {
        if (rest > (m_istep/2)) m_slider->setValue(m_slider->value()+(m_istep-rest));
        else                  m_slider->setValue(m_slider->value()-rest);
        m_spinb->setValue(TO_FLOAT(m_slider->value()));
    }
}

}  // NameSpace KSaneIface
