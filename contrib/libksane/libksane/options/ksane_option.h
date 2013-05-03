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

#ifndef KSANE_OPTION_H
#define KSANE_OPTION_H

// Qt includes.
#include <QFrame>

// Sane includes.
extern "C"
{
#include <sane/sane.h>
#include <sane/saneopts.h>
}

namespace KSaneIface
{

class KSaneOptionWidget;
    
class KSaneOption : public QObject
{
    Q_OBJECT

public:
    
    typedef enum
    {
        TYPE_DETECT_FAIL,
        TYPE_CHECKBOX,
        TYPE_SLIDER,
        TYPE_F_SLIDER,
        TYPE_COMBO,
        TYPE_ENTRY,
        TYPE_GAMMA,
        TYPE_BUTTON
    } KSaneOptType;
    
    typedef enum
    {
        STATE_HIDDEN,
        STATE_DISABLED,
        STATE_SHOWN
    } KSaneOptWState;

    KSaneOption(const SANE_Handle handle, const int index);
    ~KSaneOption();
    static KSaneOptType optionType(const SANE_Option_Descriptor *optDesc);
    
    KSaneOptionWidget *widget() {return m_widget;}
    virtual bool hasGui() {return false;}
    bool needsPolling();
    KSaneOptWState state();
    QString name();
    
    virtual void createWidget(QWidget *parent);

    virtual void readOption();
    virtual void readValue();
    
    virtual bool getMinValue(float &max);
    virtual bool getMaxValue(float &max);
    virtual bool getValue(float &val);
    virtual bool setValue(float val);
    virtual bool getValue(QString &val);
    virtual bool setValue(const QString &val);
    virtual int  getUnit();

    bool storeCurrentData();
    bool restoreSavedData();

Q_SIGNALS:
    void optsNeedReload();
    void valsNeedReload();

protected:
    
    SANE_Word toSANE_Word(unsigned char *data);
    void fromSANE_Word(unsigned char *data, SANE_Word from);
    bool writeData(void *data);
    QString unitString();
    QString unitDoubleString();
    void updateVisibility();
    
    SANE_Handle                   m_handle; 
    int                           m_index;
    const SANE_Option_Descriptor *m_optDesc; ///< This pointer is provided by sane 
    unsigned char                *m_data;
    KSaneOptionWidget            *m_widget;
};

}  // NameSpace KSaneIface

#endif // KSANE_OPTION_H
