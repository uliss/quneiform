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

#include <QFontDialog>

#include "fontbutton.h"

static QString fontName(const QFont& f)
{
    return QString("%1, %2pt").arg(f.family()).arg(f.pointSize());
}

FontButton::FontButton(const QFont& font, QWidget * parent) :
    QPushButton(parent),
    font_(font)
{
    connect(this, SIGNAL(clicked()), SLOT(showFontDialog()));
}

QFont FontButton::currentFont() const
{
    return font_;
}

void FontButton::setCurrentFont(const QFont& font)
{
    if(font != font_) {
        font_ = font;
        setText(fontName(font_));
    }
}

void FontButton::showFontDialog()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, font_, this);
    if(ok) {
        setText(fontName(font));
        setCurrentFont(font);
        emit changed(font);
    }

    activateWindow();
}
