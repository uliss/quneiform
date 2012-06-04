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

#include "scanneroptionrange.h"

class ScannerOption
{
public:
    // do not change values!
    enum Type {
        UNKNOWN = 0,
        BOOL,
        FLOAT,
        INT,
        STRING
    };

    // do not change values!
    enum Constraint {
        CONSTRAINT_NONE,
        LIST,
        RANGE
    };

    /**
     * Creates empty, invalid option
     */
    ScannerOption();

    /**
     * Returns list of allowed values, if option has constraints
     */
    QList<QVariant> allowedValues() const;

    /**
     * Adds allowed value
     */
    void addAllowedValue(const QVariant& value);

    /**
     * Removes all allowed values
     */
    void clearAllowedValues();
    bool hadAllowedValues() const;
    void setAllowedValues(const QList<QVariant>& v);

    void clearRange();
    const ScannerOptionRange& range() const;
    void setRange(const ScannerOptionRange& r);

    /**
     * Returns true if option has bool type
     * @see isInt(), isFloat(), isString()
     */
    bool isBool() const;

    /**
     * Returns true if option has int type
     * @see isBool(), isFloat(), isString()
     */
    bool isInt() const;

    /**
     * Returns true if option has float type
     * @see isBool(), isInt(), isString()
     */
    bool isFloat() const;

    /**
     * Returns true if option has stirng type
     * @see isBool(), isInt(), isFloat()
     */
    bool isString() const;

    bool isValid() const;
    void setValid(bool value = true);

    /**
     * Returns option name
     * @see setName()
     */
    QString name() const;

    /**
     * Sets option name
     * @see name()
     */
    void setName(const QString& name);

    QVariant value() const;
    void setValue(const QVariant& value);

    bool toBool() const;
    int toInt() const;
    float toFloat() const;
    QString toString() const;

    Type type() const;
    void setType(Type t);

    Constraint constraint() const;
    void setConstraint(Constraint c);
private:
    QString name_;
    QString title_;
    QString description_;
    QVariant value_;
    Type type_;
    Constraint constraint_;
    bool valid_;
    QList<QVariant> allowed_values_;
    ScannerOptionRange range_;
};

#endif // SCANNEROPTION_H
