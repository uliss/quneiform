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

#ifndef ABSTRACTSCANNERDIALOG_H
#define ABSTRACTSCANNERDIALOG_H

#include <QObject>
#include <QMap>

class AbstractScannerDialog : public QObject
{
    Q_OBJECT
protected:
    AbstractScannerDialog(QObject * parent = 0);
    AbstractScannerDialog(const AbstractScannerDialog&);
public:
    virtual void exec() = 0;
public:
    static AbstractScannerDialog * make(QWidget * parent);
    typedef AbstractScannerDialog *(*dialogFunc)();
    static void registerDialogFunc(dialogFunc f, int order);
signals:
    void pageSaved(const QString& path);
private:
    static QMap<int, dialogFunc> func_;
    static bool registered_;
};

#endif // ABSTRACTSCANNERDIALOG_H
