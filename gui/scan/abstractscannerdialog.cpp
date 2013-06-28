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

#include <QDebug>
#include <QWidget>
#include <QSettings>

#include "abstractscannerdialog.h"
#include "dummyscannerdialog.h"
#include "sanescannerdialog.h"
#include "twainscannerdialog.h"
#include "externalappscannerdialog.h"
#include "settingskeys.h"
#include "scandialogtypes.h"
#include "macscannerdialog.h"

namespace {

bool registerDialogs()
{
    DummyScannerDialog::registerDialog();
    ExternalAppScannerDialog::registerDialog();

#ifdef WITH_SANE
    SaneScannerDialog::registerDialog();
#endif

#ifdef WITH_TWAIN
    TwainScannerDialog::registerDialog();
#endif

#ifdef Q_OS_MAC
    MacScannerDialog::registerDialog();
#endif

    return true;
}

}

QMap<int, AbstractScannerDialog::dialogFunc> AbstractScannerDialog::func_;
bool AbstractScannerDialog::registered_ = registerDialogs();

AbstractScannerDialog::AbstractScannerDialog(QObject *parent) :
    QObject(parent)
{}

AbstractScannerDialog * AbstractScannerDialog::make(QWidget * parent)
{
    dialogFunc res = NULL;

    if(!func_.values().isEmpty()) {
        int idx = QSettings().value(KEY_SCAN_DIALOG_TYPE, -1).toInt();
        switch(idx) {
        case SCAN_DIALOG_EXTERNAL_APP:
        case SCAN_DIALOG_OS:
        case SCAN_DIALOG_QUNEIFORM:
        case SCAN_DIALOG_DUMMY:
            res = func_.value(idx);
            break;
        default: // dialog not specified
            res = func_.value(SCAN_DIALOG_OS);
            break;
        }
    }

    if(!res) {
        qWarning() << Q_FUNC_INFO << "no registered functions for scanning. Check scanner settings.";
        return new DummyScannerDialog(parent);
    }

    AbstractScannerDialog * d = res();
    d->setParent(parent);
    return d;
}

void AbstractScannerDialog::registerDialogFunc(AbstractScannerDialog::dialogFunc f, int type)
{
    if(func_.contains(type))
        qWarning() << Q_FUNC_INFO << "overwriting dialog function for key:" << type;

    func_[type] = f;
}
