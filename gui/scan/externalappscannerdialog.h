/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef EXTERNALAPPSCANNERDIALOG_H
#define EXTERNALAPPSCANNERDIALOG_H

#include "abstractscannerdialog.h"

class ExternalAppScannerDialog : public AbstractScannerDialog
{
    Q_OBJECT
public:
    explicit ExternalAppScannerDialog(QObject * parent = 0);
    void exec();
public:
    static void registerDialog(int order);
};

#endif // EXTERNALAPPSCANNERDIALOG_H
