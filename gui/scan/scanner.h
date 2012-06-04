/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QImage>
#include <QStringList>

#include "scanneroption.h"

namespace cf {
class IScanner;
}

class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(QObject * parent = 0);
    ~Scanner();
    QStringList listDevices() const;

    ScannerOption option(const QString& name) const;
signals:

public slots:
    void close();
    bool open(const QString& name);
    QImage start();
private:
    void collectOptions();
private:
    cf::IScanner * scanner_;
    QMap<QString, ScannerOption> options_;
};

#endif // SCANNER_H
