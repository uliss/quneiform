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

#ifndef LABELED_SLIDER_H
#define LABELED_SLIDER_H

#include <QFrame>
#include <QSlider>
#include <QGridLayout>
/**
  *@author Kåre Särs
  */

class QSpinBox;
class QLabel;

/**
 * A combination of a label a slider and a spinbox.
 * The slider is connected to the spinbox so that they have the same value.
 */
class LabeledSlider : public QFrame
{
    Q_OBJECT

public:
   /**
    * Create the slider.
    *
    * \param parent parent widget
    * \param text is the text describing the the slider value. If the text
    *        contains a '&', a buddy for the slider will be created.
    * \param min minimum slider value
    * \param max maximum slider value
    * \param quant is the step between values.
    */
    LabeledSlider(QWidget *parent, const QString& text,
                int min, int max, int st);
    ~LabeledSlider();

    void wigetSizeHints(int *lab_w, int *spi_w);
    void setColumnWidths(int lab_w, int spi_w);
    int value(void) const {return( slider->value());}

public slots:
  /** Set the slider value */
    void setValue(int);
    void setRange(int min, int max);
    void setStep(int);
    /** Set the unit */
    void setSuffix(const QString &text);

private slots:
    /** Sync the values and emit valueChanged() */
    void syncValues(int);
    /** Ensure step value is followed */
    void fixValue(void);

signals:
    /**
     * Emit the slider value changes
     */
    void valueChanged(int);

private:
    QLabel      *label;
    QSlider     *slider;
    QSpinBox    *spinb;
    int         step;
    QGridLayout *layout;
};


#endif
