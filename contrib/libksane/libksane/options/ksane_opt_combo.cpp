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
#include "ksane_opt_combo.h"
#include "i18n.h"

#include "labeled_combo.h"

// Qt includes
#include <QtCore/QVarLengthArray>

// KDE includes
#include <QIcon>
#include <QDebug>
#include <QLocale>

namespace KSaneIface
{
static const char tmp_binary[] = "Binary";

KSaneOptCombo::KSaneOptCombo(const SANE_Handle handle, const int index)
: KSaneOption(handle, index), m_combo(0)
{
}

void KSaneOptCombo::createWidget(QWidget *parent)
{
    if (m_widget) return;

    m_widget = m_combo = new LabeledCombo(parent, "", QStringList());
    readOption();
    m_widget->setToolTip(i18n(m_optDesc->desc));
    connect(m_combo, SIGNAL(activated(int)), this, SLOT(comboboxChangedIndex(int)));
    readValue();
}

void KSaneOptCombo::readValue()
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

    
    m_currentText = getSaneComboString(data.data());
    if (m_combo != 0) {
        if (m_combo->currentText() != m_currentText) {
            m_combo->setCurrentText(m_currentText);
            emit valueChanged();
        }
    }
}

void KSaneOptCombo::readOption()
{
    KSaneOption::readOption();
    
    if (!m_combo) return;
    
    QString saved = m_combo->currentText();
    
    m_strList = genComboStringList();
    m_combo->clear();
    m_combo->setLabelText(i18n(m_optDesc->title));
    m_combo->addItems(m_strList);
    m_combo->setIcon(QIcon("color"), getSaneComboString((unsigned char*)SANE_VALUE_SCAN_MODE_COLOR));
    m_combo->setIcon(QIcon("gray-scale"),
                     getSaneComboString((unsigned char*)SANE_VALUE_SCAN_MODE_GRAY));
    m_combo->setIcon(QIcon("black-white"),
                     getSaneComboString((unsigned char*)SANE_VALUE_SCAN_MODE_LINEART));
                     // The epkowa/epson backend uses "Binary" which is the same as "Lineart"
    m_combo->setIcon(QIcon("black-white"), i18n(tmp_binary));
    
    // set the previous value
    m_combo->setCurrentText(saved);
}


QStringList &KSaneOptCombo::genComboStringList()
{
    int i;
    m_strList.clear();
    
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
            for (i=1; i<=m_optDesc->constraint.word_list[0]; ++i) {
                m_strList += getSaneComboString((int)m_optDesc->constraint.word_list[i]);
            }
            break;
        case SANE_TYPE_FIXED:
            for (i=1; i<=m_optDesc->constraint.word_list[0]; ++i) {
                m_strList += getSaneComboString((float)SANE_UNFIX(m_optDesc->constraint.word_list[i]));
            }
            break;
        case SANE_TYPE_STRING:
            i=0;
            while (m_optDesc->constraint.string_list[i] != 0) {
                m_strList += getSaneComboString((unsigned char *)m_optDesc->constraint.string_list[i]);
                i++;
            }
            break;
        default :
            m_strList += "NOT HANDELED";
    }
    return m_strList;
}

QString KSaneOptCombo::getSaneComboString(int ival)
{
    switch(m_optDesc->unit)
    {
        case SANE_UNIT_NONE:        break;
        case SANE_UNIT_PIXEL:       return QString("%1 Pixel").arg(ival);
        case SANE_UNIT_BIT:         return QString("%1 Bit").arg(ival);
        case SANE_UNIT_MM:          return QString("%1 mm").arg(ival);
        case SANE_UNIT_DPI:         return QString("%1 DPI").arg(ival);
        case SANE_UNIT_PERCENT:     return QString("%1 %").arg(ival);
        case SANE_UNIT_MICROSECOND: return QString("%1 µs").arg(ival);
    }
    return QString::number(ival);
}

QString KSaneOptCombo::getSaneComboString(float fval)
{
    switch(m_optDesc->unit)
    {
        case SANE_UNIT_NONE:        break;
        case SANE_UNIT_PIXEL:       return QString("%1 Pixel").arg(fval);
        case SANE_UNIT_BIT:         return QString("%1 Bit").arg(fval);
        case SANE_UNIT_MM:          return QString("%1 mm").arg(fval);
        case SANE_UNIT_DPI:         return QString("%1 DPI").arg(fval);
        case SANE_UNIT_PERCENT:     return QString("%1 %").arg(fval);
        case SANE_UNIT_MICROSECOND: return QString("%1 µs").arg(fval);
    }
    return QString::number(fval, 'F', 4);
}

QString KSaneOptCombo::getSaneComboString(unsigned char *data)
{
    QString tmp;
    if (data == 0) return QString();
        
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
            return getSaneComboString((int)toSANE_Word(data));
        case SANE_TYPE_FIXED:
            return getSaneComboString((float)SANE_UNFIX(toSANE_Word(data)));
        case SANE_TYPE_STRING:
            tmp = i18n(reinterpret_cast<char*>(data));
            tmp = tmp.simplified();
            return tmp;
        default :
            break;
    }
    return QString();
}



void KSaneOptCombo::comboboxChangedIndex(int i)
{
    if (m_combo && (m_combo->currentText() == m_currentText)) {
        return;
    }
    
    unsigned char data[4];
    void *dataPtr;
    
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
        case SANE_TYPE_FIXED:
            fromSANE_Word(data, m_optDesc->constraint.word_list[i+1]);
            dataPtr = data;
            break;
        case SANE_TYPE_STRING:
            dataPtr = (void *)m_optDesc->constraint.string_list[i];
            break;
        default:
            kDebug() << "can not handle type:" << m_optDesc->type;
            return;
    }
    writeData(dataPtr);
    readValue();
    emit valueChanged();
}

bool KSaneOptCombo::getMinValue(float &val)
{
    if (state() == STATE_HIDDEN) return false;
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
            val = (float)m_optDesc->constraint.word_list[1];
            for (int i=2; i<=m_optDesc->constraint.word_list[0]; i++) {
                val = qMin((float)m_optDesc->constraint.word_list[i], val);
            }
            break;
        case SANE_TYPE_FIXED:
            val = (float)SANE_UNFIX(m_optDesc->constraint.word_list[1]);
            for (int i=2; i<=m_optDesc->constraint.word_list[0]; i++) {
                val = qMin((float)SANE_UNFIX(m_optDesc->constraint.word_list[i]), val);
            }
            break;
        default:
            kDebug() << "can not handle type:" << m_optDesc->type;
            return false;
    }
    return true;
}

bool KSaneOptCombo::getValue(float &val)
{
    if (state() == STATE_HIDDEN) return false;

    // read that current value
    QVarLengthArray<unsigned char> data(m_optDesc->size);
    SANE_Status status;
    SANE_Int res;
    status = sane_control_option (m_handle, m_index, SANE_ACTION_GET_VALUE, data.data(), &res);
    if (status != SANE_STATUS_GOOD) {
        kDebug() << m_optDesc->name << "sane_control_option returned" << status;
        return false;
    }
    
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
            val = (float)toSANE_Word(data.data());
            return true;
        case SANE_TYPE_FIXED:
            val = SANE_UNFIX(toSANE_Word(data.data()));
            return true;
        default:
            kDebug() << "Type" << m_optDesc->type << "not supported!";
    }
    return false;
}

bool KSaneOptCombo::setValue(float value)
{
    unsigned char data[4];
    float tmp;
    float minDiff;
    int i;
    int minIndex = 1;
    
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
            tmp = (float)m_optDesc->constraint.word_list[minIndex];
            minDiff = qAbs(value - tmp);
            for (i=2; i<=m_optDesc->constraint.word_list[0]; ++i) {
                tmp = (float)m_optDesc->constraint.word_list[i];
                if (qAbs(value - tmp) < minDiff) {
                    minDiff = qAbs(value - tmp);
                    minIndex = i;
                }
            }
            fromSANE_Word(data, m_optDesc->constraint.word_list[minIndex]);
            writeData(data);
            readValue();
            return (minDiff < 1.0);
        case SANE_TYPE_FIXED:
            tmp = (float)SANE_UNFIX(m_optDesc->constraint.word_list[minIndex]);
            minDiff = qAbs(value - tmp);
            for (i=2; i<=m_optDesc->constraint.word_list[0]; ++i) {
                tmp = (float)SANE_UNFIX(m_optDesc->constraint.word_list[i]);
                if (qAbs(value - tmp) < minDiff) {
                    minDiff = qAbs(value - tmp);
                    minIndex = i;
                }
            }
            fromSANE_Word(data, m_optDesc->constraint.word_list[minIndex]);
            writeData(data);
            readValue();
            return (minDiff < 1.0);
        default:
            kDebug() << "can not handle type:" << m_optDesc->type;
            break;
    }
    return false;
}

bool KSaneOptCombo::getValue(QString &val)
{
    if (state() == STATE_HIDDEN) return false;
    val = m_currentText;
    return true;
}

bool KSaneOptCombo::setValue(const QString &val)
{
    if (state() == STATE_HIDDEN) return false;
    if (val == m_currentText) return true;
    
    unsigned char data[4];
    void *data_ptr;
    SANE_Word fixed;
    int i;
    float f;
    bool ok;
    QString tmp;
    
    switch (m_optDesc->type)
    {
        case SANE_TYPE_INT:
            tmp = val.left(val.indexOf(' ')); // strip the unit
            // accept float formating of the string
            i = (int)(tmp.toFloat(&ok));
            if (ok == false) return false;
            fromSANE_Word(data, i);
            data_ptr = data;
            break;
        case SANE_TYPE_FIXED:
            tmp = val.left(val.indexOf(' ')); // strip the unit
            f = tmp.toFloat(&ok);
            if (ok == false) return false;
            fixed = SANE_FIX(f);
            fromSANE_Word(data, fixed);
            data_ptr = data;
            break;
        case SANE_TYPE_STRING:
            i = 0;
            while (m_optDesc->constraint.string_list[i] != 0) {
                tmp = getSaneComboString((unsigned char *)m_optDesc->constraint.string_list[i]);
                if (val == tmp) {
                    data_ptr = (void *)m_optDesc->constraint.string_list[i];
                    break;
                }
                i++;
            }
            if (m_optDesc->constraint.string_list[i] == 0) return false;
            break;
        default:
            kDebug() << "can only handle SANE_TYPE: INT, FIXED and STRING";
            return false;
    }
    writeData(data_ptr);

    readValue();
    return true;
}


}  // NameSpace KSaneIface
