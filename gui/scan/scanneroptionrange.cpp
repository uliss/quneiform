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

#include "scanneroptionrange.h"

ScannerOptionRange::ScannerOptionRange()
{
}


ScannerOptionRange::ScannerOptionRange(const QVariant& min,
                                       const QVariant& max,
                                       const QVariant& step) :
    min_(min),
    max_(max),
    step_(step)
{
}

void ScannerOptionRange::clear()
{
    min_.clear();
    max_.clear();
    step_.clear();
}

bool ScannerOptionRange::isNull() const
{
    return min_.isNull() && max_.isNull();
}

QPair<QVariant, QVariant> ScannerOptionRange::range() const
{
    return QPair<QVariant, QVariant>(min_, max_);
}

void ScannerOptionRange::setRange(const QVariant& min, const QVariant& max)
{
    min_ = min;
    max_ = max;
}

void ScannerOptionRange::setRange(const QPair<QVariant, QVariant>& r)
{
    min_ = r.first;
    max_ = r.second;
}

QVariant ScannerOptionRange::min() const
{
    return min_;
}

QVariant ScannerOptionRange::max() const
{
    return max_;
}

QVariant ScannerOptionRange::step() const
{
    return step_;
}

void ScannerOptionRange::setMin(const QVariant& v)
{
    min_ = v;
}

void ScannerOptionRange::setMax(const QVariant& v)
{
    max_ = v;
}

void ScannerOptionRange::setStep(const QVariant& v)
{
    step_ = v;
}

