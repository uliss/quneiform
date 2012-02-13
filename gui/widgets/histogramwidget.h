/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <vector>
#include <QLabel>

class HistogramWidget : public QLabel
{
    Q_OBJECT
public:
    typedef std::vector<int> Histogram;

    HistogramWidget(QWidget * parent = 0, const Histogram& h = Histogram());

    /**
      * Returns chart color
      */
    QColor color() const;

    /**
      * Sets chart color
      */
    void setColor(const QColor& color);

    /**
      * Sets histogram data
      */
    void setData(const Histogram& hist);
public slots:
    void clear();
protected:
    void paintEvent(QPaintEvent * event);
private:
    QColor color_;
    std::vector<int> data_;
};

#endif // HISTOGRAMWIDGET_H
