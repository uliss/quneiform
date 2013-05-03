/* ============================================================
*
* This file is part of the KDE project
*
* Date        : 2010
* Description : Sane authentication helpers.
*
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

#include "ksane_auth.h"

// Qt includes
#include <QMutex>
#include <QList>

// KDE includes.
#include <QDebug>

namespace KSaneIface
{

static KSaneAuth *s_instance = 0;
static QMutex     s_mutex;

struct KSaneAuth::Private
{
    struct AuthStruct
    {
        QString resource;
        QString username;
        QString password;
    };

    QList<AuthStruct> authList;
};

KSaneAuth *KSaneAuth::getInstance()
{
    s_mutex.lock();

    if (s_instance == 0) {
        s_instance = new KSaneAuth();
    }
    s_mutex.unlock();

    return s_instance;
}

KSaneAuth::KSaneAuth() : d(new Private) {}

KSaneAuth::~KSaneAuth()
{
    d->authList.clear();
    delete d;
}

void KSaneAuth::setDeviceAuth(const QString &resource, const QString &username, const QString &password)
{
    // This is a short list so we do not need a QMap...
    int i;
    for (i=0; i<d->authList.size(); i++) {
        if (resource == d->authList.at(i).resource) {
            // update the existing node
            d->authList[i].username = username;
            d->authList[i].password = password;
            break;
        }
    }
    if (i==d->authList.size()) {
        // Add a new list node
        Private::AuthStruct tmp;
        tmp.resource = resource;
        tmp.username = username;
        tmp.password = password;
        d->authList << tmp;
    }
}

void KSaneAuth::clearDeviceAuth(const QString &resource)
{
    // This is a short list so we do not need a QMap...
    for (int i=0; i<d->authList.size(); i++) {
        if (resource == d->authList.at(i).resource) {
            d->authList.removeAt(i);
            return;
        }
    }
}

/** static function called by sane_open to get authorization from user */
void KSaneAuth::authorization(SANE_String_Const resource, SANE_Char *username, SANE_Char *password)
{
    qDebug() << resource;
    // This is vague in the standard... what can I find in the resource string?
    // I have found that "resource contains the backend name + "$MD5$....."
    // it does not contain unique identifiers like ":libusb:001:004"
    // -> remove $MD5 and later before comparison...
    QString res(resource);
    int end = res.indexOf("$MD5$");
    res = res.left(end);
    qDebug() << res;
    
    QList<Private::AuthStruct> list = getInstance()->d->authList;
    for (int i=0; i<list.size(); i++) {
        qDebug() << res << list.at(i).resource;
        if (list.at(i).resource.contains(res)) {
            qstrncpy(username, list.at(i).username.toLocal8Bit(), SANE_MAX_USERNAME_LEN);
            qstrncpy(password, list.at(i).password.toLocal8Bit(), SANE_MAX_PASSWORD_LEN);
            break;
        }
    }
}




}

