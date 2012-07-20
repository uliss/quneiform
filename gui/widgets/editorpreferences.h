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

#ifndef EDITORPREFERENCES_H
#define EDITORPREFERENCES_H

#include "preferenceswidget.h"

class QFormLayout;
class FontButton;
class ColorButton;

class EditorPreferences : public PreferencesWidget
{
    Q_OBJECT
public:
    EditorPreferences(QWidget * parent = 0);
private:
    void setupLayout();
    void setupUI();
private:
    static bool loadFont(QWidget * w, const QVariant& data);
    static bool saveFont(QWidget * w, const QVariant& data);
    static bool loadAlternativeCharColor(QWidget * w, const QVariant& data);
    static bool saveAlternativeCharColor(QWidget * w, const QVariant& data);
    static bool loadCurrentCharColor(QWidget * w, const QVariant& data);
    static bool saveCurrentCharColor(QWidget * w, const QVariant& data);
private:
    QFormLayout * layout_;
    FontButton * editor_font_;
    ColorButton * current_char_color_;
    ColorButton * alt_char_color_;
};

#endif // EDITORPREFERENCES_H
