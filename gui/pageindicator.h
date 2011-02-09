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


#ifndef PAGEINDICATOR_H
#define PAGEINDICATOR_H

#include <QWidget>
#include <QPixmap>

class QLabel;

/**
  * @class PageIndicator represents Page state.
  * by default all indicators are shown
  * you can turn then on/off calling appropriate functions
  */
class PageIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit PageIndicator(QWidget * parent = 0);
    QSize sizeHint() const;

    /**
      * Shows or hides recognize indicator
      */
    void setRecognized(bool value);

    /**
      * Shows or hides save indicator
      */
    void setSaved(bool value);

    /**
     * Shows or hides warning indicator
     */
    void setWarning(bool value);
signals:
    /**
      * Emits on click on warning indicator
      */
    void showWarningDetails();
protected:
    void mousePressEvent(QMouseEvent * event);
private:
    QPixmap indicatorIcon(const QString& path);
private:
    QLabel * recognized_;
    QLabel * saved_;
    QLabel * warning_;
};

#endif // PAGEINDICATOR_H
