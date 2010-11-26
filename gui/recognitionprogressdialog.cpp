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

#include <QDebug>
#include <QFileInfo>
#include <QLabel>

#include "page.h"
#include "recognitionprogressdialog.h"

RecognitionProgressDialog::RecognitionProgressDialog(QWidget * parent) :
        QProgressDialog(parent) {
    setModal(true);
    setWindowModality(Qt::ApplicationModal);
    setMinimumDuration(0);
    setMinimum(0);
    setMaximum(100);
    setMaximumWidth(300);
    QLabel * label = new QLabel();
    label->setAlignment(Qt::AlignLeft);
    label->setTextFormat(Qt::PlainText);
    setLabel(label);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void RecognitionProgressDialog::setCurrentPage(Page * const p) {
    Q_CHECK_PTR(p);

    QFileInfo fi(p->imagePath());
    setLabelText(tr("Page recognition: \"%1\"").arg(fi.fileName()));
}
