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

#ifndef SANEDIALOG_H
#define SANEDIALOG_H

#include <QDialog>

class SaneWidget;
class QHBoxLayout;
class QImage;
class QProgressDialog;

class SaneDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SaneDialog(QWidget * parent = 0);
    QImage * image();
    QString imagePath() const;
public slots:
    int run();
private slots:
    void scanStart();
    void scanEnd();
    void scanFailed();
    void imageReady();
private:
    void initIcons();
    void initUi();
    void initLayout();
private:
    SaneWidget * sane_widget_;
    QHBoxLayout * layout_;
    QImage * image_;
    QString saved_;
    QProgressDialog * progress_;
};

#endif // SANEDIALOG_H
