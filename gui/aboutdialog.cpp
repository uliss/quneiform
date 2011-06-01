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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "config-version.h" // for CF_VERSION
#include "build_number.h" // for CF_BUILD_NUMBER

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::AboutDialog)
{
    ui_->setupUi(this);
    setText();
}

AboutDialog::~AboutDialog()
{
    delete ui_;
}

void AboutDialog::setText() {
    ui_->about_label_->setText(tr("Quneiform OCR<br/>"
    "Author: Serge Poltavski<br/>"
    "Email: <a href=\"mailto:serge.poltavski@gmail.com\">serge.poltavski@gmail.com</a><br/>"
    "Translator: Serge Poltavski<br/>"
    "License: <a href=\"http://www.gnu.org/licenses/gpl.html\">GPL v.3</a><br/>"
    "Version: %1<br/>"
    "Build number: %2<br/><br/>"
    "This program uses refactoring version of libcuneiform from "
        "<a href=\"https://github.com/uliss/quneiform\">GitHub</a> "
        "released under GPL v.3 license.<br/>"
    "Cuneiform was originally released by Congitive Technologies under BSD license.<br/><br/>"
    "Uses icons from <a href=\"http://www.oxygen-icons.org/\">Oxygen</a> theme.")
                               .arg(CF_VERSION).arg(CF_BUILD_NUMBER));
}
