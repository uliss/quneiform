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

#include "common/binarizeoptions.h"
#include "binarizationsettings.h"

BinarizationSettings::BinarizationSettings()
{
}

BinarizationSettings::BinarizationSettings(const cf::BinarizeOptions& opts)
{
    set("binarizator", (int) opts.binarizator());
}

cf::BinarizeOptions BinarizationSettings::toCfOptions() const
{
    cf::BinarizeOptions res;
#ifdef Q_OS_MAC
    res.setBinarizator(cf::BINARIZATOR_OTSU);
#else
    res.setBinarizator(static_cast<cf::binarizator_t>(getInt("binarizator", cf::BINARIZATOR_KRONROD)));
#endif
    return res;
}

bool BinarizationSettings::getBool(const QString& key, bool fallback) const
{
    QVariant res = get(key, fallback);
    if(res.type() == QVariant::Bool)
        return res.toBool();

    return fallback;
}

int BinarizationSettings::getInt(const QString& key, int fallback) const
{
    QVariant res = get(key, fallback);
    if(res.type() == QVariant::Int)
        return res.toInt();

    return fallback;
}

QString BinarizationSettings::getString(const QString& key, const QString& fallback) const
{
    QVariant res = get(key, fallback);
    if(res.type() == QVariant::String)
        return res.toString();

    return fallback;
}

void BinarizationSettings::set(const QString& key, const QVariant& value)
{
    settings_.insert(key, value);
}

QVariant BinarizationSettings::get(const QString& key, const QVariant& fallback) const
{
    Table::const_iterator it = settings_.find(key);
    if(it == settings_.end())
        return fallback;

    return it.value();
}

QDataStream& operator<<(QDataStream& stream, const BinarizationSettings& s)
{
    stream << s.settings_;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, BinarizationSettings& s)
{
    stream >> s.settings_;
    return stream;
}
