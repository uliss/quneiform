/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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


#ifndef RECOGNITIONSETTINGS_H
#define RECOGNITIONSETTINGS_H

class QDataStream;

class RecognitionSettings
{
public:
    RecognitionSettings();
    bool operator==(const RecognitionSettings& opts);
    bool dotMatrix() const;
    bool fax() const;
    bool oneColumn() const;
    bool picturesSearch() const;
    void setDotMatrix(bool value);
    void setFax(bool value);
    void setOneColumn(bool value);
    void setPicturesSearch(bool value);
    bool spelling() const;
    void useSpelling(bool value);
private:
    bool fax_;
    bool dot_matrix_;
    bool cf_spelling_;
    bool onecolumn_layout_;
    bool search_pictures_;
private:
    friend QDataStream& operator<<(QDataStream& stream, const RecognitionSettings& opts);
    friend QDataStream& operator>>(QDataStream& stream, RecognitionSettings& opts);
};

#endif // RECOGNITIONSETTINGS_H
