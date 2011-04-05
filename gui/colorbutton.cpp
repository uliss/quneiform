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

#include <QColorDialog>
#include "colorbutton.h"

ColorButton::ColorButton(QWidget * parent) :
    QToolButton(parent),
    pixmap_(32, 32)
{
    connectSignals();
}

ColorButton::ColorButton(const QColor& color, QWidget * parent) :
    QToolButton(parent),
    pixmap_(32, 32)
{
    setColor(color);
    connectSignals();
}

QColor ColorButton::color() const {
    return color_;
}

void ColorButton::connectSignals() {
    connect(this, SIGNAL(clicked()), SLOT(showColorDialog()));
}

void ColorButton::setColor(const QColor& color) {
    color_ = color;
    pixmap_.fill(color);
    setIcon(QIcon(pixmap_));
}

void ColorButton::showColorDialog() {
    QColorDialog * dialog = new QColorDialog(color_, this);
    if(QDialog::Accepted == dialog->exec())
        setColor(dialog->selectedColor());

    delete dialog;
}
