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

#ifndef GENERALPREFERENCES_H
#define GENERALPREFERENCES_H

#include "preferenceswidget.h"

class QFormLayout;
class QLineEdit;

class GeneralPreferences : public PreferencesWidget
{
    Q_OBJECT
public:
    GeneralPreferences(QWidget * parent = NULL);
private:
    void setupExternalEditor();
    void setupIconTheme();
    void setupLayout();
private slots:
    void showSelectApplicationDialog();
private:
    static bool loadExternalEditor(QWidget * w, const QVariant& data);
    static bool saveExternalEditor(QWidget * w, const QVariant& data);
    static bool loadIconTheme(QWidget * w, const QVariant& data);
    static bool saveIconTheme(QWidget * w, const QVariant& data);
private:
    QFormLayout * layout_;
    QLineEdit * ext_editor_;
};

#endif // GENERALPREFERENCES_H
