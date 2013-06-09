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

#include <QSettings>
#include <QFileDialog>
#include <QDebug>

#include "settingskeys.h"
#include "externalappscannerdialog.h"
#include "workspace.h"
#include "scandialogtypes.h"

namespace {

AbstractScannerDialog * createExternalScannerDialog()
{
    ExternalAppScannerDialog * d = new ExternalAppScannerDialog();
    return d;
}

}

ExternalAppScannerDialog::ExternalAppScannerDialog(QObject * parent) :
    AbstractScannerDialog(parent)
{
}

void ExternalAppScannerDialog::exec()
{
    QSettings s;
    QString app_path = s.value(KEY_SCAN_EXTERNAL_APP).toString();

    // try to choose scanning application
    if(app_path.isEmpty())
        app_path = Workspace::showChooseApplicationDialog();

    if(app_path.isEmpty())
        return;

    bool rc = Workspace::launchApplication(app_path);

    if(rc) {
        s.setValue(KEY_SCAN_EXTERNAL_APP, app_path);
    }
    else {
        qDebug() << Q_FUNC_INFO << "can't launch application:" << app_path;
    }
}

void ExternalAppScannerDialog::registerDialog()
{
    AbstractScannerDialog::registerDialogFunc(&createExternalScannerDialog, SCAN_DIALOG_EXTERNAL_APP);
}
