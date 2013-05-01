/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#ifndef SCANPREFERENCES_H
#define SCANPREFERENCES_H

#include "preferenceswidget.h"

class QFormLayout;
class QLineEdit;
class QSlider;
class QSpinBox;
class QComboBox;
class QCheckBox;

class ScanPreferences : public PreferencesWidget
{
    Q_OBJECT
public:
    ScanPreferences(QWidget * parent = NULL);
private slots:
    void handleAutosaveToggle(bool value);
private:
    void setupLayout();
    void setupUseLastScanner();
    void setupAutosave();
    void setupFormat();
    void setupQuality();
private:
    static bool loadAutosaveType(QWidget * w, const QVariant& data);
    static bool saveAutosaveType(QWidget * w, const QVariant& data);
private:
    QFormLayout * layout_;
    QSlider * save_quality_;
    QSpinBox * save_quality_box_;
    QComboBox * save_format_;
    QCheckBox * autosave_;
    QComboBox * autosave_dir_;
};

#endif // SCANPREFERENCES_H
