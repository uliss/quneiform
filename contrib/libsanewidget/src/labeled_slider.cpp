/*
   Copyright (C) 2007 Kåre Särs <kare.sars@kolumbus.fi>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <qspinbox.h>
#include <qtooltip.h>
#include <qlabel.h>
#include <qslider.h>

#include "labeled_slider.h"

LabeledSlider::LabeledSlider(QWidget *parent, const QString& ltext,
                        int min, int max, int ste)
   : QFrame(parent)
{
    layout = new QGridLayout(this);
    layout->setSpacing(3);
    layout->setMargin(0);
    label = new QLabel(ltext, this);
    step = ste; if (step == 0) step = 1;

    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setSingleStep(step);

    spinb = new QSpinBox(this);
    spinb->setMinimum(min);
    spinb->setMaximum(max);
    slider->setSingleStep(step);
    spinb->setValue(max);
    //spinb->setMinimumWidth(spinb->sizeHint().width()+35);
    spinb->setAlignment(Qt::AlignRight);
    spinb->setValue(min);

    spinb->setValue(min);
    label->setBuddy(slider);

    connect(spinb, SIGNAL(valueChanged(int)), this, SLOT(syncValues(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(syncValues(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(fixValue()));

    layout->addWidget(label, 0, 0);
    layout->addWidget(slider, 0, 1);
    layout->addWidget(spinb, 0, 2);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 50);
    layout->setColumnStretch(2, 0);
}

void LabeledSlider::setSuffix(const QString &text)
{
    spinb->setSuffix(text);
}

void LabeledSlider::wigetSizeHints(int *lab_w, int *spi_w)
{
    if (lab_w != 0) *lab_w = label->sizeHint().width();
    if (spi_w != 0) *spi_w = spinb->sizeHint().width();
}

void LabeledSlider::setColumnWidths(int lab_w, int spi_w)
{
    layout->setColumnMinimumWidth(0, lab_w);
    layout->setColumnMinimumWidth(2, spi_w);
}

void LabeledSlider::setValue(int value)
{
    if      (value != slider->value()) slider->setValue(value);
    else if (value != spinb->value()) spinb->setValue(value);
}


void LabeledSlider::setRange(int min, int max)
{
    //std::cout << "min=" << min << ", max=" << max << std::endl;
    slider->setRange(min, max);
    spinb->setRange(min, max);
}

void LabeledSlider::setStep(int st)
{
    step = st;
    if (step == 0) step = 1;
    slider->setSingleStep(step);
    spinb->setSingleStep(step);
}


void LabeledSlider::syncValues(int value)
{
    if      (value != spinb->value()) spinb->setValue(value);
    else if (value != slider->value()) {
        //ensure that the value step is followed also for the slider
        if ((value - slider->minimum())%step != 0) {
            if (value > spinb->value()) {
                slider->setValue(slider->value()+(step-(value - spinb->value())));
            }
            else {
                slider->setValue(slider->value()-(step-(spinb->value() - value)));
            }
            // this function will be reentered with the signal fom the slider
        }
        else {
            slider->setValue(value);
        }
    }
    else emit valueChanged(value);
}

void LabeledSlider::fixValue(void)
{
    //ensure that the value step is followed also for the slider
    int rest = (slider->value() - slider->minimum())%step;

    if (rest != 0) {
        if (rest > (step/2)) slider->setValue(slider->value()+(step-rest));
        else                 slider->setValue(slider->value()-rest);
        spinb->setValue(slider->value());
    }
}

LabeledSlider::~LabeledSlider()
{
}
