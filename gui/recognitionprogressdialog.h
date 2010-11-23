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


#ifndef RECOGNITIONPROGRESSDIALOG_H
#define RECOGNITIONPROGRESSDIALOG_H

#include <QDialog>

class Page;

namespace Ui {
    class RecognitionProgressDialog;
}

class RecognitionProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecognitionProgressDialog(QWidget * parent = 0);
    ~RecognitionProgressDialog();
signals:
    /**
      * Emitted when Abort button is clicked
      */
    void aborted();

    /**
      * Emitted when Pause button is clicked
      */
    void paused();

    /**
      * Emitted when Resume button is clicked
      */
    void resumed();
public slots:
    /**
      * Resets progress dialog
      */
    void reset();

    /**
      * Sets path of current page
      */
    void setCurrentPage(Page * const p);

    /**
      * Sets current progress in percent
      * @param value - shoud be value in 0...100 range
      */
    void setTotalValue(int value);
private slots:
    void pause();
private:
    Ui::RecognitionProgressDialog * ui;
    bool paused_;
};

#endif // RECOGNITIONPROGRESSDIALOG_H
