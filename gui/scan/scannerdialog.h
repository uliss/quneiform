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

namespace Ui {
class ScannerDialog;
}

class Scanner;
class QComboBox;

class ScannerDialog : public QDialog
{
public:
    explicit ScannerDialog(QWidget * parent = 0);
    ~ScannerDialog();
private:
    void setupUi();
public slots:
    void handleScannerSelect(int idx);
private:
    QComboBox * scanModeWidget();
private:
    Ui::ScannerDialog * ui_;
    Scanner * scanner_;
};

#endif // SCANNERDIALOG_H
