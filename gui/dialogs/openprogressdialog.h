/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef OPENPROGRESSDIALOG_H
#define OPENPROGRESSDIALOG_H

#include <QProgressDialog>

class OpenProgressDialog : public QProgressDialog
{
    Q_OBJECT
public:
    OpenProgressDialog(QWidget * parent = 0);
    void load(const QString& path, int page = -1);
    void loadDone();
    void start(const QStringList& files);
    void stop();
private:
    QString elidedPath() const;
    void setPath(const QString& path);
    void setPage(int page);
    void updateLabel();
private:
    QString current_path_;
    int page_;
    QLabel * label_;
};

#endif // OPENPROGRESSDIALOG_H
