/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2007-09-13
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2009 by Grzegorz Kurtyka <grzegorz dot kurtyka at gmail dot com>
 * Copyright (C) 2010 by Kare Sars <kare dot sars at iki dot fi>
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

#include "ksane_find_devices_thread.h"

#include "ksane_widget_private.h"

// Sane includes.
extern "C"
{
#include <sane/saneopts.h>
#include <sane/sane.h>
}

// KDE includes.
#include <QDebug>

// Qt includes
#include <QMutex>

namespace KSaneIface
{
static FindSaneDevicesThread *s_instancesane = 0;
static QMutex s_mutexsane;

FindSaneDevicesThread *FindSaneDevicesThread::getInstance()
{
    s_mutexsane.lock();

    if (s_instancesane == 0) {
        s_instancesane = new FindSaneDevicesThread();
    }
    s_mutexsane.unlock();

    return s_instancesane;
}

FindSaneDevicesThread::FindSaneDevicesThread() : QThread(0)
{
}

FindSaneDevicesThread::~FindSaneDevicesThread()
{
    s_mutexsane.lock();
    wait();
    s_mutexsane.unlock();
}


void FindSaneDevicesThread::run()
{
    SANE_Device const **devList;
    //SANE_Int            version;
    SANE_Status         status;

    // This is unfortunately not very reliable as many back-ends do not refresh
    // the device list after the sane_init() call...
    status = sane_get_devices(&devList, SANE_FALSE);

    m_deviceList.clear();
    if (status == SANE_STATUS_GOOD) {
        int i = 0;
        KSaneWidget::DeviceInfo tmp;

        while(devList[i] != 0) {
            tmp.name = devList[i]->name;
            tmp.vendor = devList[i]->vendor;
            tmp.model = devList[i]->model;
            tmp.type = devList[i]->type;
            m_deviceList << tmp;
            i++;
        }
    }
}

const QList<KSaneWidget::DeviceInfo> FindSaneDevicesThread::devicesList() const
{
    return m_deviceList;
}

}
