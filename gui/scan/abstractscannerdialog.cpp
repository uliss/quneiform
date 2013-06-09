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
#include "externalappscannerdialog.h"
#include "settingskeys.h"

namespace {

bool registerDialogs()
{
    DummyScannerDialog::registerDialog(2);
    ExternalAppScannerDialog::registerDialog(1);

#ifdef WITH_SANE
    SaneScannerDialog::registerDialog(0);
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
        int idx = QSettings().value(KEY_SCAN_CURRENT_TAB, -1).toInt();
        switch(idx) {
        case 0:
        case 1:
        case 2:
            res = func_.value(idx);
            break;
        default:
            res = func_.value(2);
            break;
        }
    }

    if(!res) {
        qWarning() << Q_FUNC_INFO << "no registered functions for scanning";
        return NULL;
    }

    AbstractScannerDialog * d = res();
    d->setParent(parent);
    return d;
}

void AbstractScannerDialog::registerDialogFunc(AbstractScannerDialog::dialogFunc f, int order)
{
    if(func_.contains(order))
        qWarning() << Q_FUNC_INFO << "overwriting dialog function for key:" << order;

    func_[order] = f;
}
