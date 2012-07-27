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

#ifndef FONTBUTTON_H
#define FONTBUTTON_H

#include <QPushButton>
#include <QFont>

class FontButton : public QPushButton
{
    Q_OBJECT
public:
    FontButton(const QFont& font = QFont(), QWidget * parent = 0);
    QFont currentFont() const;
    void setCurrentFont(const QFont&);
signals:
    void changed(const QFont&);
private slots:
    void showFontDialog();
private:
    QFont font_;
};

#endif // FONTBUTTON_H
