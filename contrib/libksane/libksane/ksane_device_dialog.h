/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2007-09-13
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2007-2010 by Kare Sars <kare dot sars at iki dot fi>
 * Copyright (C) 2009 by Grzegorz Kurtyka <grzegorz dot kurtyka at gmail dot com>
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

#ifndef KSANE_DEVICE_DIALOG_H
#define KSANE_DEVICE_DIALOG_H

#include "ksane_find_devices_thread.h"

// Qt includes.
#include <QGroupBox>
#include <QStringList>
#include <QButtonGroup>
#include <QToolTip>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QDialog>

namespace KSaneIface
{

class KSaneDeviceDialog : public QDialog
{
    Q_OBJECT
public:
    KSaneDeviceDialog(QWidget *parent=0);
    ~KSaneDeviceDialog();

    QString getSelectedName();
    void setDefault(QString);
public Q_SLOTS:
    void reloadDevicesList();
private Q_SLOTS:
    void setAvailable(bool avail);
    void updateDevicesList();
private:
    QWidget                *m_btnContainer;
    QGroupBox              *m_btnBox;
    QButtonGroup           *m_btnGroup;
    QVBoxLayout            *m_btnLayout;
    QString                 m_defaultBackend;
    QString                 m_selectedDevice;
    FindSaneDevicesThread  *m_findDevThread;
    QPushButton            *m_ok;
    QPushButton            *m_cancel;
    QPushButton            *m_reload;
};

}  // NameSpace KSaneIface

#endif
