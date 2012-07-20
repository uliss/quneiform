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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>
#include <QColor>
#include <QPixmap>

class ColorButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget * parent = 0);
    ColorButton(const QColor& color, QWidget * parent = 0);

    /**
      * Returns button color
      * @see setColor()
      */
    QColor color() const;

    /**
      * Sets button color
      * @param color - new color
      * @see color()
      */
    void setColor(const QColor& color);
signals:
    void changed(const QColor&);
private slots:
    void showColorDialog();
private:
    void connectSignals();
private:
    QColor color_;
    QPixmap pixmap_;
};

#endif // COLORBUTTON_H
