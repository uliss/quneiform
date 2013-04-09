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
#include <math.h> //pow

#include <qlayout.h>
#include <QGroupBox>

#include "labeled_gamma.h"

LabeledGamma::LabeledGamma(QWidget *parent, const QString& text, int size)
    : QFrame(parent)
{
    bri_slider = new LabeledSlider(this, tr("Brightness"), -50, 50, 1);
    bri_slider->setValue(0);

    con_slider = new LabeledSlider(this, tr("Contrast"), -50, 50, 1);
    con_slider->setValue(0);

    gam_slider = new LabeledSlider(this, tr("Gamma"), 30, 300, 1);
    gam_slider->setValue(100);

    // Calculate the size of the widgets in the sliders
    int lw, spw, lw_max=0, spw_max=0;
    bri_slider->wigetSizeHints(&lw, &spw);
    lw_max=lw;
    spw_max=spw;
    con_slider->wigetSizeHints(&lw, &spw);
    if (lw>lw_max) lw_max = lw;
    if (spw>spw_max) spw_max = spw;
    gam_slider->wigetSizeHints(&lw, &spw);
    if (lw>lw_max) lw_max = lw;
    if (spw>spw_max) spw_max = spw;
    // set the calculated widths
    bri_slider->setColumnWidths(lw_max, spw_max);
    con_slider->setColumnWidths(lw_max, spw_max);
    gam_slider->setColumnWidths(lw_max, spw_max);

    gam_tbl.resize(size);
    for (int i=0; i<gam_tbl.size(); i++) {
        gam_tbl[i] = i;
    }
    max_val = size-1; // assume a gamma table 0 -> max

    gamma_disp = new GammaDisp(this, &gam_tbl);

    QHBoxLayout *gbl = new QHBoxLayout(this);
    QGroupBox *groupBox = new QGroupBox(text, this);
    QGridLayout *gr_lay = new QGridLayout(groupBox);
    gr_lay->setSpacing(2);
    gr_lay->setMargin(2);

    gr_lay->addWidget(bri_slider, 0, 0);
    gr_lay->addWidget(con_slider, 1, 0);
    gr_lay->addWidget(gam_slider, 2, 0);
    gr_lay->addWidget(gamma_disp, 0, 1, 3, 1);
    gr_lay->activate();

    gbl->addWidget(groupBox);
    gbl->setSpacing(2);
    gbl->setMargin(0);
    gbl->activate();

    connect(bri_slider, SIGNAL(valueChanged(int)), this, SLOT(calculateGT()));
    connect(con_slider, SIGNAL(valueChanged(int)), this, SLOT(calculateGT()));
    connect(gam_slider, SIGNAL(valueChanged(int)), this, SLOT(calculateGT()));
}

LabeledGamma::~LabeledGamma()
{
}

void LabeledGamma::setColor(QColor color)
{
    if (gamma_disp != 0) {
        gamma_disp->setColor(color);
    }
}

void LabeledGamma::setValues(int bri, int con, int gam)
{
    bri_slider->blockSignals(true);
    con_slider->blockSignals(true);
    gam_slider->blockSignals(true);

    bri_slider->setValue(bri);
    con_slider->setValue(con);
    gam_slider->setValue(gam);

    calculateGT();

    bri_slider->blockSignals(false);
    con_slider->blockSignals(false);
    gam_slider->blockSignals(false);
}

void LabeledGamma::setSize(int size)
{
    gam_tbl.resize(size);
    for (int i=0; i<gam_tbl.size(); i++) {
        gam_tbl[i] = i;
    }
    bri_slider->setValue(0);
    con_slider->setValue(0);
    gam_slider->setValue(0);
}


void LabeledGamma::calculateGT(void)
{
    double gam = 100.0/gam_slider->value();
    double con = (200.0/(100.0 - con_slider->value()))-1;
    double half_max = max_val/2.0;
    double bri = (bri_slider->value()/half_max) * max_val;
    double x;

    for (int i = 0; i<gam_tbl.size(); i++) {
        // apply gamma
        x = pow(i/max_val, gam) * max_val;

        // apply contrast
        x = (con*(x-half_max)) + half_max;

        // apply brightness + rounding
        x += bri + 0.5;

        // ensure correct value
        if (x > max_val) x = max_val;
        if (x < 0) x = 0;

        gam_tbl[i] = (int)x;

        //if (i%16 == 0) printf("\n");
        //printf(" %d", (int)x);
    }
    //printf("\n");

    gamma_disp->update();
    //printf("emmit gammaTableChanged\n");
    emit gammaChanged(bri_slider->value(), con_slider->value(), gam_slider->value());
    emit gammaTableChanged(gam_tbl);
}


