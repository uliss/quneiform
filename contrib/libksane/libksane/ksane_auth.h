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

#ifndef KSANE_AUTH_H
#define KSANE_AUTH_H

// Qt includes.
#include <QString>

// Sane includes.
extern "C"
{
#include <sane/saneopts.h>
#include <sane/sane.h>
}

namespace KSaneIface
{

class KSaneAuth 
{
    public:
        static KSaneAuth *getInstance();
        ~KSaneAuth();

        void setDeviceAuth(const QString &resource, const QString &username, const QString &password);
        void clearDeviceAuth(const QString &resource);
        static void authorization(SANE_String_Const resource, SANE_Char *username, SANE_Char *password);

    private:
        KSaneAuth();
        struct Private;
        Private * const d;
};

}


#endif
