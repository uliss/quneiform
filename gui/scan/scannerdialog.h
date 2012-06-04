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

#ifndef SCANNERDIALOG_H
#define SCANNERDIALOG_H

#include <QDialog>
#include <QMap>

#include "scanneroption.h"

namespace Ui {
class ScannerDialog;
}

class Scanner;
class QComboBox;

class ScannerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScannerDialog(QWidget * parent = 0);
    ~ScannerDialog();
private:
    enum OptionWidgetType {
        UNKNOWN_WIDGET = 0,
        COMBOBOX,
        FLOAT_RANGE,
        FLOAT_INPUT,
        INT_RANGE,
        INT_INPUT,
        STRING_INPUT,
        CHECKBOX
    };
private:
    void addDialogOptionWidget(const QString& name, QWidget * w);
    void setupUi();
    void setupScanMode();
    void setupScanResolution();
    static OptionWidgetType widgetType(const ScannerOption& opt);
    QWidget * makeOptionWidget(const ScannerOption& opt);
public slots:
    void handleScannerSelect(int idx);
private slots:
    void save();
    void handleModeChange(int idx);
    void handleOptionChange();
private:
    Ui::ScannerDialog * ui_;
    Scanner * scanner_;
    QMap<QString, ScannerOption> changed_options_;
};

#endif // SCANNERDIALOG_H
