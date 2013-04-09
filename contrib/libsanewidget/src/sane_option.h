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

#ifndef SANE_OPTIONS_H
#define SANE_OPTIONS_H

#include <qframe.h>
#include <qcheckbox.h>
#include <qlayout.h>

extern "C" {
#include <sane/sane.h>
#include <sane/saneopts.h>
}

#include "labeled_separator.h"
#include "labeled_slider.h"
#include "labeled_fslider.h"
#include "labeled_combo.h"
#include "labeled_checkbox.h"
#include "labeled_entry.h"
#include "labeled_gamma.h"

typedef enum
{
    SW_DETECT_FAIL,
    SW_GROUP,
    SW_CHECKBOX,
    SW_SLIDER,
    SW_SLIDER_INT,
    SW_F_SLIDER,
    SW_F_SLIDER_FIX,
    SW_COMBO,
    SW_ENTRY,
    SW_GAMMA
} SaneOptWidget_t;

#define SW_INT_MAX (2147483647)
#define SW_INT_MIN (-2147483647-1)

#define SW_FIXED_MAX (32767.0)
#define SW_FIXED_MIN (-32768.0)

typedef enum
{
    SW_STATE_HIDDEN,
    SW_STATE_DISABLED,
    SW_STATE_SHOWN,
    SW_STATE_NO_CHANGE
} SaneOptWState;

class SaneOption : public QObject
{
    Q_OBJECT

public:
    SaneOption(const SANE_Handle, const int);
    ~SaneOption();

    void createWidget(QWidget *parent);

    void readOption(void);
    void readValue(void);

    QFrame *widget(void) {return (QFrame *)frame;}
    SaneOptWidget_t sw_type(void) {return type;}
    QString name(void);
    bool getMaxValue(float *max);
    bool getValue(float *val);
    bool setValue(float percent);
    bool getValue(QString *val);
    bool setValue(const QString &val);
    bool setChecked(bool check);
    bool storeCurrentData(void);
    bool restoreSavedData(void);
    bool setIconColorMode(const QIcon &icon);
    bool setIconGrayMode(const QIcon &icon);
    bool setIconBWMode(const QIcon &icon);

    LabeledGamma *lgamma;
    LabeledCombo *lcombx;
    LabeledSlider *lslider;
    LabeledFSlider *lfslider;

signals:
    void optsNeedReload(void);
    void valsNeedReload(void);
    void fValueRead(float val);
    void iValueRead(int val);
    void cbValueRead(bool val);

private slots:
    void comboboxChanged(int i);
    void checkboxChanged(bool toggled);
    bool comboboxChanged(const QString &value);
    void sliderChanged(int val);
    void fsliderChanged(float val);
    void entryChanged(const QString& text);
    void gammaTableChanged(const QVector<int> &gam_tbl);

private:
    // functions
    SaneOptWidget_t getWidgetType(void);
    QStringList *genComboStringList(void);
    QString getSaneComboString(unsigned char *data);
    SANE_Word toSANE_Word(unsigned char *data);
    void fromSANE_Word(unsigned char *data, SANE_Word from);
    QString unitString(void);
    bool writeData(unsigned char *data);
    bool comboboxChanged(float value);

    // gui object variables
    QFrame *frame;
    LabeledCheckbox *lchebx;
    QStringList *cstrl;
    LabeledEntry *lentry;

    SaneOptWidget_t type;
    SaneOptWState sw_state;
    QIcon *icon_color;
    QIcon *icon_gray;
    QIcon *icon_bw;

    // Sane variables
    SANE_Handle sane_handle;
    int opt_number;
    const SANE_Option_Descriptor *sane_option;
    unsigned char *sane_data;
    int iVal;
    float fVal;
    float min_change;
    bool bVal;
};

#endif
