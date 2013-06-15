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
#include "labeled_slider.h"

// KDE includes.
//#include <KNumInput>

// Qt includes.
#include <QLabel>
#include <QSlider>
#include <QSpinBox>

namespace KSaneIface
{

LabeledSlider::LabeledSlider(QWidget *parent, const QString& ltext,
                             int min, int max, int ste)
: KSaneOptionWidget(parent, ltext)
{
    m_step = ste;
    if (m_step == 0) m_step = 1;

    m_slider = new QSlider(this);
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setMinimum(min);
    m_slider->setMaximum(max);
    m_slider->setSingleStep(m_step);

    m_spinb = new QSpinBox(this);
    m_spinb->setMinimum(min);
    m_spinb->setMaximum(max);
    m_slider->setSingleStep(m_step);
    m_spinb->setValue(max);
    //m_spinb->setMinimumWidth(m_spinb->sizeHint().width()+35);
    m_spinb->setAlignment(Qt::AlignRight);
    m_spinb->setValue(min);

    m_spinb->setValue(min);
    m_label->setBuddy(m_spinb);

    connect(m_spinb,  SIGNAL(valueChanged(int)), this, SLOT(syncValues(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(syncValues(int)));
    connect(m_slider, SIGNAL(sliderReleased()),  this, SLOT(fixValue()));

    m_layout->addWidget(m_slider, 0, 2);
    m_layout->addWidget(m_spinb, 0, 1);
    m_layout->setColumnStretch(1, 0);
    m_layout->setColumnStretch(2, 50);
    
}

LabeledSlider::~LabeledSlider()
{
}

void LabeledSlider::setSuffix(const QString& text)
{
    m_spinb->setSuffix(text);
}

void LabeledSlider::setValue(int value)
{
    if      (value != m_slider->value()) m_slider->setValue(value);
    else if (value != m_spinb->value()) m_spinb->setValue(value);
}

void LabeledSlider::setRange(int min, int max)
{
    //std::cout << "min=" << min << ", max=" << max << std::endl;
    m_slider->setRange(min, max);
    m_spinb->setRange(min, max);
}

void LabeledSlider::setStep(int st)
{
    m_step = st;
    if (m_step == 0) m_step = 1;
    m_slider->setSingleStep(m_step);
    m_spinb->setSingleStep(m_step);
}

void LabeledSlider::syncValues(int value)
{
    if      (value != m_spinb->value()) m_spinb->setValue(value);
    else if (value != m_slider->value()) {
        //ensure that the value m_step is followed also for the m_slider
        if ((value - m_slider->minimum())%m_step != 0) {
            if (value > m_spinb->value()) {
                m_slider->setValue(m_slider->value()+(m_step-(value - m_spinb->value())));
            }
            else {
                m_slider->setValue(m_slider->value()-(m_step-(m_spinb->value() - value)));
            }
            // this function will be reentered with the signal fom the m_slider
        }
        else {
            m_slider->setValue(value);
        }
    }
    else emit valueChanged(value);
}

void LabeledSlider::fixValue()
{
    //ensure that the value m_step is followed also for the m_slider
    int rest = (m_slider->value() - m_slider->minimum())%m_step;

    if (rest != 0) {
        if (rest > (m_step/2)) m_slider->setValue(m_slider->value()+(m_step-rest));
        else                 m_slider->setValue(m_slider->value()-rest);
        m_spinb->setValue(m_slider->value());
    }
}

}  // NameSpace KSaneIface
