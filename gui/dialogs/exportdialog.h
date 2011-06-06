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

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

#include "exportsettings.h"

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExportDialog(const QString& imagePath, QWidget * parent = 0);
    ~ExportDialog();

    QString fileName() const;
    ExportSettings settings() const;
private slots:
    void handleFormatChange(int index);
    void showOpenFileDialog();
private:
    void setupFormatList();
private:
    Ui::ExportDialog * ui;
    ExportSettings settings_;
    QString image_path_;
    QString file_name_;
};

#endif // EXPORTDIALOG_H
