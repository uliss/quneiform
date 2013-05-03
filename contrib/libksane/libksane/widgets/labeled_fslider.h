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

#ifndef LABELED_FSLIDER_H
#define LABELED_FSLIDER_H

#include "ksane_option_widget.h"

// Qt includes.
#include <QSlider>
#include <QDoubleSpinBox>

/**
 *@author Kåre Särs
 */

namespace KSaneIface
{

/**
 * A combination of a label a slider and a spinbox.
 * The slider is connected to the spinbox so that they have the same value.
 */
class LabeledFSlider : public KSaneOptionWidget
{
    Q_OBJECT

public:

   /**
    * Create the slider.
    *
    * \param parent parent widget
    * \param text is the text describing the slider value. If the text
    *        contains a '&', a buddy for the slider will be created.
    * \param min minimum slider value
    * \param max maximum slider value
    * \param quant is the step between values.
    */
    LabeledFSlider(QWidget *parent, const QString& text,
                   float min, float max, float st);
    ~LabeledFSlider();

   /**
    * \return the slider value.
    */
    float value() const {return (float)m_spinb->value();}
    float step() const {return m_fstep;}

public Q_SLOTS:

    /** Set the slider/spinbox value */
    void setValue(float);
    void setRange(float, float);
    void setStep(float);
    /** Set the unit */
    void setSuffix(const QString &text);

private Q_SLOTS:

    /**
     * Sync the values and emit valueChanged()
     */
    // FIXME this has to be changes later
    void syncValues(int);
    void syncValues(double);
    void fixValue();

Q_SIGNALS:

    /**
     * Emit the slider value changes
     */
    void valueChanged(float);

private:

    QSlider        *m_slider;
    QDoubleSpinBox *m_spinb;
    float           m_fstep;
    int             m_istep;
};

}  // NameSpace KSaneIface

#endif // LABELED_SFSLIDER_H
