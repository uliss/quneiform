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

#ifndef LABELED_GAMMA_H
#define LABELED_GAMMA_H

#include <QFrame>
#include "labeled_slider.h"
#include "gamma_disp.h"

/**
  *@author Kåre Särs
  */

/**
 * A wrapper for a checkbox
 */
class LabeledGamma : public QFrame
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

    void setColor(QColor color);
    void setSize(int size);
    const QVector<int> &gammaTablePtr(void) { return gam_tbl; }
    int size(void) {return (int)(max_val+1);}
    void widgetSizeHints(int *lab_w, int *spi_w);
    void setColumnWidhts(int lab_w, int spi_w);

public slots:
    void setValues(int bri, int con, int gam);

private slots:
    void calculateGT(void);

signals:
    void gammaChanged(int bri, int con, int gam);
    void gammaTableChanged(const QVector<int> &gamma_tbl);

private:

    LabeledSlider *bri_slider;
    LabeledSlider *con_slider;
    LabeledSlider *gam_slider;

    QVector<int> gam_tbl;
    double max_val;

    GammaDisp *gamma_disp;
};


#endif
