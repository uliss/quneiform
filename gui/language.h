/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>
#include <QList>

class QDataStream;

class Language
{
public:
    /**
      * Constructs language by given code.
      * if code is invalid - invalid language constructed.
      * it can be checked via isValid() call.
      */
    explicit Language(int code);

    /**
      * Returns language code
      */
    int code() const;

    /**
      * Returns true is language is valid
      */
    bool isValid() const;

    /**
      * Returns language name, for example: "English"
      */
    QString name() const;

    /**
      * Returns translated to current locale language name
      */
    QString trName() const;

    enum Sorting {
        NO_SORT,
        BY_NAME,
        BY_TR_NAME
    };

public:
    static QList<Language> supportedLanguages(Sorting sort = NO_SORT);
public:
    friend QDataStream& operator<<(QDataStream& stream, const Language& lang);
    friend QDataStream& operator>>(QDataStream& stream, Language& lang);
private:
    QString name_;
    int code_;
};

QDataStream& operator<<(QDataStream& stream, const Language& lang);
QDataStream& operator>>(QDataStream& stream, Language& lang);

#endif // LANGUAGE_H
