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

#include <QMessageBox>
#include <QDebug>

#include "twainscannerdialog.h"
#include "scandialogtypes.h"
#include "kscandialog.h"

namespace {

AbstractScannerDialog * createTwainScannerDialog()
{
    TwainScannerDialog * d = new TwainScannerDialog();
    return d;
}

}

TwainScannerDialog::TwainScannerDialog(QObject *parent) :
    AbstractScannerDialog(parent),
    dlg_(NULL)
{
}

void TwainScannerDialog::exec()
{
    if(!dlg_) {
        dlg_ = new KScanDialog(qobject_cast<QWidget*>(parent()));
        connect(dlg_, SIGNAL(pageSaved(QString)), this, SIGNAL(pageSaved(QString)));
    }

    dlg_->run();
}

void TwainScannerDialog::registerDialog()
{
    AbstractScannerDialog::registerDialogFunc(&createTwainScannerDialog, SCAN_DIALOG_OS);
}
