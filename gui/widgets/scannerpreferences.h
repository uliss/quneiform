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

#ifndef SCANNERPREFERENCES_H
#define SCANNERPREFERENCES_H

#include "preferenceswidget.h"

namespace Ui {
class ScannerPreferences;
}

class ScannerPreferences : public PreferencesWidget
{
    Q_OBJECT  
public:
    explicit ScannerPreferences(QWidget *parent = 0);
    ~ScannerPreferences();
private slots:
    void chooseAutosaveDir(int idx);
    void handlePageSelect(int idx);
    void handleAutosaveToggle(bool state);
private:
    void setupFormat();
    void setupUi();
    void setupStack();
    void setupExternalScanApp();
    void setupUseLastScanner();
    void setupImageQuality();
    void setupAutosave();
private:
    static bool loadAutosaveType(QWidget * w, const QVariant& data);
    static bool saveAutosaveType(QWidget * w, const QVariant& data);
    static bool loadExternalApp(QWidget * w, const QVariant& data);
    static bool saveExternalApp(QWidget * w, const QVariant& data);
private:
    Ui::ScannerPreferences * ui_;
};

#endif // SCANNERPREFERENCES_H
