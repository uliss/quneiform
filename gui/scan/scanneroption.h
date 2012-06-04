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

#ifndef SCANNEROPTION_H
#define SCANNEROPTION_H

#include <QList>
#include <QVariant>

class ScannerOption
{
public:
    enum Type {
        UNKNOWN,
        BOOL,
        FLOAT,
        INT,
        STRING
    };

    ScannerOption();

    QList<QVariant> allowedValues() const;
    void addAllowedValue(const QVariant& value);
    void clearAllowedValues();
    bool hadAllowedValues() const;
    void setAllowedValues(const QList<QVariant>& v);

    bool isValid() const;
    bool setValid(bool value = true);

    QString name() const;
    void setName(const QString& name);

    QVariant value() const;
    void setValue(const QVariant& value);

    bool toBool() const;
    int toInt() const;
    float toFloat() const;
    QString toString() const;

    Type type() const;
    void setType(Type t);
private:
    QString name_;
    QString title_;
    QString description_;
    QVariant value_;
    Type type_;
    bool valid_;
    QList<QVariant> allowed_values_;
};

#endif // SCANNEROPTION_H
