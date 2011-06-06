/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef RECOGNITIONSETTINGSDIALOG_H
#define RECOGNITIONSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class RecognitionSettingsDialog;
}

class Page;

class RecognitionSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    RecognitionSettingsDialog(Page * page, QWidget * parent = 0);
    ~RecognitionSettingsDialog();
private slots:
    void save();
private:
    void setup();
private:
    Ui::RecognitionSettingsDialog * ui_;
    Page * page_;
};

#endif // RECOGNITIONSETTINGSDIALOG_H
