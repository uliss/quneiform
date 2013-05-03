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

#ifndef KSCANDIALOG_H
#define KSCANDIALOG_H

#include <QDialog>

namespace KSaneIface {
class KSaneWidget;
}

class QHBoxLayout;
class QProgressDialog;

class KScanDialog : public QDialog
{
    Q_OBJECT
public:
    KScanDialog(QWidget * parent = 0);
    QStringList imagePath() const;
    int run();
private:
    void initLayout();
    void initUi();
private:
    KSaneIface::KSaneWidget * sane_widget_;
    QHBoxLayout * layout_;
    QImage * image_;
    QString saved_;
    QProgressDialog * progress_;
};

#endif // KSCANDIALOG_H
