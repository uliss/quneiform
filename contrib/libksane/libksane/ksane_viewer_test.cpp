/* ============================================================
*
* This file is part of the KDE project
*
* Date        : 2010-03-17
* Description : Preview image viewer that can handle a selection.
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

#include "ksane_viewer.h"

#include <KDebug>
#include <QApplication>

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc != 2) {
        kDebug() << "An image filename is needed.";
        return 1;
    }
    QImage img(argv[1]);

    KSaneIface::KSaneViewer viewer(&img);

    viewer.findSelections();

    viewer.show();

    viewer.zoom2Fit();
    
    return app.exec();
}
