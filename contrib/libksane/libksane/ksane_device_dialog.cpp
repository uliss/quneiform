/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2007-09-13
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2007-2008 by Kare Sars <kare dot sars at iki dot fi>
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

// Local includes.
#include "ksane_device_dialog.h"
#include "i18n.h"


// Sane includes.
extern "C"
{
#include <sane/saneopts.h>
#include <sane/sane.h>
}


// KDE includes.
#include <QDebug>
#include <QPushButton>
#include <QScrollArea>

namespace KSaneIface
{

KSaneDeviceDialog::KSaneDeviceDialog(QWidget *parent)
    : QDialog(parent)
{

//    setButtons(QDialog::User1 | KDialog::Ok | KDialog::Cancel);
//    setButtonText(User1, i18n("Reload devices list"));
    
    m_btnGroup = new QButtonGroup(this);
    
    m_btnBox = new QGroupBox;
    m_btnLayout = new QVBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    m_btnContainer = new QWidget;
    m_btnLayout = new QVBoxLayout(m_btnContainer);
    QScrollArea *area = new QScrollArea;
    
    m_btnBox->setLayout(layout);
    
    layout->addWidget(area);
    layout->setContentsMargins(0,0,0,0);

    area->setWidgetResizable(true);
    area->setFrameShape(QFrame::NoFrame);
    area->setWidget(m_btnContainer);
    
//    setMainWidget(m_btnBox);
    setMinimumHeight(200);
    m_findDevThread = FindSaneDevicesThread::getInstance();

    connect(m_findDevThread, SIGNAL(finished()), this, SLOT(updateDevicesList()));
    connect(this, SIGNAL(user1Clicked()),        this, SLOT(reloadDevicesList()));

    reloadDevicesList();
}

KSaneDeviceDialog::~KSaneDeviceDialog() {
    ///@todo wait for thread to finish if its running
}

void KSaneDeviceDialog::reloadDevicesList()
{
    setAvailable(false);
    while (!m_btnGroup->buttons().isEmpty()) {
        delete m_btnGroup->buttons().takeFirst();
    }
    m_btnBox->setTitle(i18n("Looking for devices. Please wait."));
//    enableButton(QDialog::User1, false);

    if(!m_findDevThread->isRunning()) {
        m_findDevThread->start();
    }
}

void KSaneDeviceDialog::setAvailable(bool avail)
{
//    enableButtonOk(avail);
    if(avail) {
        m_selectedDevice = getSelectedName();
//        setButtonFocus(KDialog::Ok);
    }
}

void KSaneDeviceDialog::setDefault(QString defaultBackend)
{
        m_selectedDevice = defaultBackend;
}

QString KSaneDeviceDialog::getSelectedName() {
    QAbstractButton *selectedButton = m_btnGroup->checkedButton();
    if(selectedButton) {
        return selectedButton->objectName();
    }
    return QString();
}

void KSaneDeviceDialog::updateDevicesList()
{
    while (!m_btnGroup->buttons().isEmpty()) {
        delete m_btnGroup->buttons().takeFirst();
    }

    const QList<KSaneWidget::DeviceInfo> list = m_findDevThread->devicesList();
    if (list.isEmpty()) {
        m_btnBox->setTitle(i18n("Sorry. No devices found."));
//        enableButton(KDialog::User1, true);
        return;
    }

    delete m_btnLayout;
    m_btnLayout = new QVBoxLayout;
    m_btnContainer->setLayout(m_btnLayout);
    m_btnBox->setTitle(i18n("Found devices:"));
    
    for (int i=0; i< list.size(); i++) {
        QRadioButton *b = new QRadioButton(this);
        b->setObjectName(list[i].name);
        b->setToolTip(list[i].name);
        b->setText(QString("%1 : %2\n%3")
                    .arg(list[i].vendor)
                    .arg(list[i].model)
                    .arg(list[i].name));

        m_btnLayout->addWidget(b);
        m_btnGroup->addButton(b);
        connect(b, SIGNAL(clicked(bool)), this, SLOT(setAvailable(bool)));
        if((i==0) || (list[i].name == m_selectedDevice)) {
            b->setChecked(true);
            setAvailable(true);
        }
    }

    m_btnLayout->addStretch();

    if(list.size() == 1) {
//        button(KDialog::Ok)->animateClick();
    }

//    enableButton(KDialog::User1, true);
}

}  // NameSpace KSaneIface
