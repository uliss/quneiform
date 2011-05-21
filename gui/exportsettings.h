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

#ifndef EXPORTSETTINGS_H
#define EXPORTSETTINGS_H

#include <QString>
#include <QMap>

class ExportSettings
{
public:
    enum FormatType {
        PLAINTEXT = 0,
        SMARTTEXT = 1,
        HTML = 2,
        ODF = 3,
        DJVUTXT = 4,
        DJVUXML = 5,
        HOCR = 6
    };

    ExportSettings(FormatType format = PLAINTEXT);

    /**
      * Returns format_t
      */
    int cfFormatType() const;

    /**
      * Returns format extension
      */
    QString extension() const;

    /**
      * Returns export format
      * @see setFormat()
      */
    FormatType format() const;

    /**
      * Sets export format
      * @see format()
      */
    void setFormat(FormatType format);
public:
    static QList<FormatType> supportedFormats();
private:
    static void initFormatMap();
private:
    FormatType format_;
    typedef QMap<FormatType, int> FormatMap;
    static FormatMap format_map_;
};

#endif // EXPORTSETTINGS_H
