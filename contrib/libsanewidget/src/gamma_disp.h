/*
   Copyright (C) 2007 Kåre Särs <kare.sars@kolumbus.fi>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef GAMMA_DISP_H
#define GAMMA_DISP_H

#include <QWidget>

/**
 *@author Kåre Särs
 *
 * This is the widget that displays the gamma table.
 */

class GammaDisp : public QWidget
{
    Q_OBJECT

    public:
       /**
        * Create a gamma display.
        * \param parent parent widget
        */
        GammaDisp(QWidget *parent, QVector<int> *tbl);
        ~GammaDisp() {};

        QSize sizeHint(void) const;
        QSize minimumSizeHint() const;

        void setColor(QColor color);

    protected:
        void paintEvent (QPaintEvent *);
        void resizeEvent(QResizeEvent *);

    private:
        QVector<int> *gam_tbl;
        QColor gam_color;
};

#endif

