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
#include "ksane_opt_fslider.h"
#include "i18n.h"

#include "labeled_fslider.h"

// Qt includes
#include <QtCore/QVarLengthArray>

// KDE includes
#include <QDebug>
#include <QLocale>

static const float FIXED_MAX = 32767.0;
static const float FIXED_MIN =-32768.0;
static const float MIN_FIXED_STEP = 0.0001;
namespace KSaneIface
{

KSaneOptFSlider::KSaneOptFSlider(const SANE_Handle handle, const int index)
: KSaneOption(handle, index), m_slider(0), m_fVal(0), m_minChange(MIN_FIXED_STEP)
{
}

void KSaneOptFSlider::createWidget(QWidget *parent)
{
    if (m_widget) return;

    m_widget = m_slider = new LabeledFSlider(parent, sane_i18n(m_optDesc->title), FIXED_MIN, FIXED_MAX, MIN_FIXED_STEP);
    readOption();
    m_widget->setToolTip(sane_i18n(m_optDesc->desc));
    connect(m_slider, SIGNAL(valueChanged(float)), this, SLOT(sliderChanged(float)));
    readValue();
}

void KSaneOptFSlider::readOption()
{
    KSaneOption::readOption();

    if (!m_slider) return;
    
    if (m_optDesc->constraint_type == SANE_CONSTRAINT_RANGE) {
        m_slider->setRange(SANE_UNFIX(m_optDesc->constraint.range->min),
                           SANE_UNFIX(m_optDesc->constraint.range->max));
        
        float tmpStep = SANE_UNFIX(m_optDesc->constraint.range->quant);
        if (tmpStep < MIN_FIXED_STEP) tmpStep = MIN_FIXED_STEP;
        m_slider->setStep(tmpStep);
    }
    else {
        m_slider->setRange(FIXED_MIN, FIXED_MAX);
        m_slider->setStep(MIN_FIXED_STEP);
    }
    m_minChange = m_slider->step()/2;
    m_slider->setSuffix(unitDoubleString());
    m_slider->setLabelText(sane_i18n(m_optDesc->title));
}

void KSaneOptFSlider::readValue()
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
    
    m_fVal = SANE_UNFIX(toSANE_Word(data.data()));
    if (m_slider != 0) {
        if (((m_slider->value() - m_fVal) >= m_minChange) ||
            ((m_fVal- m_slider->value()) >= m_minChange))
        {
            m_slider->setValue(m_fVal);
        }
    }
    emit fValueRead(m_fVal);
}


void KSaneOptFSlider::sliderChanged(float val)
{
    if (((val-m_fVal) >= m_minChange) || ((m_fVal-val) >= m_minChange)) {
        unsigned char data[4];
        SANE_Word fixed;
        //kDebug() <<m_optDesc->name << fVal << "!=" << val;
        m_fVal = val;
        fixed = SANE_FIX(val);
        fromSANE_Word(data, fixed);
        writeData(data);
    }
}

bool KSaneOptFSlider::getMinValue(float &val)
{
    if (m_optDesc->constraint_type == SANE_CONSTRAINT_RANGE) {
        val = SANE_UNFIX(m_optDesc->constraint.range->min);
    }
    else {
        val = FIXED_MIN;
    }
    return true;
}

bool KSaneOptFSlider::getMaxValue(float &max)
{
    if (m_optDesc->constraint_type == SANE_CONSTRAINT_RANGE) {
        max = SANE_UNFIX(m_optDesc->constraint.range->max);
    }
    else {
        max = FIXED_MAX;
    }
    return true;
}

bool KSaneOptFSlider::getValue(float &val)
{
    if (state() == STATE_HIDDEN) return false;
    val = m_fVal;
    return true;
}

bool KSaneOptFSlider::setValue(float val)
{
    if (state() == STATE_HIDDEN) return false;
    sliderChanged(val);
    readValue();
    return true;
}

bool KSaneOptFSlider::getValue(QString &val)
{
    if (state() == STATE_HIDDEN) return false;
    val = QString::number(m_fVal, 'F', 6);
    return true;
}

bool KSaneOptFSlider::setValue(const QString &val)
{
    if (state() == STATE_HIDDEN) return false;
    sliderChanged(val.toFloat());
    readValue();
    return true;
}


}  // NameSpace KSaneIface
