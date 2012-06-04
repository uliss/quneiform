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

#ifndef SCANNEROPTIONRANGE_H
#define SCANNEROPTIONRANGE_H

#include <QVariant>
#include <QPair>

class ScannerOptionRange
{
public:
    ScannerOptionRange();
    ScannerOptionRange(const QVariant& min, const QVariant& max, const QVariant& step = QVariant());

    void clear();

    bool isNull() const;

    QPair<QVariant, QVariant> range() const;
    void setRange(const QVariant& min, const QVariant& max);
    void setRange(const QPair<QVariant, QVariant>& r);

    QVariant min() const;
    QVariant max() const;
    QVariant step() const;

    void setMin(const QVariant& v);
    void setMax(const QVariant& v);
    void setStep(const QVariant& v);
private:
    QVariant min_;
    QVariant max_;
    QVariant step_;
};

#endif // SCANNEROPTIONRANGE_H
