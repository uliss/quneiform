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

#include "histogramwidget.h"

#include <algorithm>
#include <QDebug>
#include <QPainter>
#include <QPixmap>

HistogramWidget::HistogramWidget(QWidget *parent) :
    QLabel(parent),
    color_(Qt::blue)
{
}

void HistogramWidget::clear()
{
}

void HistogramWidget::show(const std::vector<int>& hist)
{
    if(hist.empty()) {
        qDebug() << Q_FUNC_INFO << "histogram is empty";
        return;
    }

    const int HEIGHT = 100;
    const int WIDTH = hist.size();
    int hist_height = *std::max_element(hist.begin(), hist.end());
    qreal fraction = qreal(HEIGHT) / hist_height;

    QPixmap * pixmap = new QPixmap(WIDTH, HEIGHT);
    pixmap->fill();
    QPainter p(pixmap);
    p.setPen(QPen(color_, 1));

    for(size_t i = 0; i < hist.size(); i++) {
        const int h = HEIGHT - (hist[i] * fraction);
        if(h == HEIGHT)
            continue;

        p.drawLine(i, HEIGHT, i, h);
    }

    p.end();
    setPixmap(*pixmap);
    delete pixmap;
}

QColor HistogramWidget::color() const
{
    return color_;
}

void HistogramWidget::setColor(const QColor& color)
{
    color_ = color;
}
