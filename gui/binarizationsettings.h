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

#ifndef BINARIZATIONSETTINGS_H
#define BINARIZATIONSETTINGS_H

#include <QDataStream>
#include <QHash>
#include <QVariant>
#include <QString>

namespace cf {
    class BinarizeOptions;
}

class BinarizationSettings
{
public:
    BinarizationSettings();
    BinarizationSettings(const cf::BinarizeOptions& opts);
    cf::BinarizeOptions toCfOptions() const;

    bool getBool(const QString& key, bool fallback) const;
    int getInt(const QString& key, int fallback) const;
    QString getString(const QString& key, const QString& fallback) const;

    QVariant get(const QString& key, const QVariant& fallback = QVariant()) const;
    void set(const QString& key, const QVariant& get);
private:
    friend QDataStream& operator<<(QDataStream& stream, const BinarizationSettings& s);
    friend QDataStream& operator>>(QDataStream& stream, BinarizationSettings& s);
private:
    typedef QHash<QString, QVariant> Table;
    Table settings_;
};

#endif // BINARIZATIONSETTINGS_H
