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

HistogramWidget::HistogramWidget(QWidget *parent, const Histogram& h) :
    QLabel(parent),
    color_(Qt::blue),
    data_(h)
{
    setMinimumWidth(100);
    setMinimumHeight(40);
}

void HistogramWidget::clear()
{
    data_.clear();
}

//void HistogramWidget::show(const std::vector<int>& hist)
//{
//    if(hist.empty()) {
//        qDebug() << Q_FUNC_INFO << "histogram is empty";
//        return;
//    }

//    const int HEIGHT = 100;
//    const int WIDTH = hist.size();
//    int hist_height = *std::max_element(hist.begin(), hist.end());
//    qreal fraction = qreal(HEIGHT) / hist_height;

//    QPixmap * pixmap = new QPixmap(WIDTH, HEIGHT);
//    pixmap->fill();
//    QPainter p(pixmap);
//    p.setPen(QPen(color_, 1));

//    for(size_t i = 0; i < hist.size(); i++) {
//        const int h = HEIGHT - (hist[i] * fraction);
//        if(h == HEIGHT)
//            continue;

//        p.drawLine(i, HEIGHT, i, h);
//    }

//    p.end();
//    setPixmap(*pixmap);
//    delete pixmap;
//}

QColor HistogramWidget::color() const
{
    return color_;
}

void HistogramWidget::setColor(const QColor& color)
{
    color_ = color;
}

static void drawChart(QPainter& p, const QRect& rect, const HistogramWidget::Histogram& data)
{
    qreal x_stretch = rect.width() / (qreal) data.size();
    qDebug() << "x stretch:" << x_stretch;
    qreal y_stretch = rect.height() / (qreal) (*std::max_element(data.begin(), data.end()));
    qDebug() << "y stretch:" << y_stretch;

    for(size_t i = 0; i < data.size(); i++) {
        int item_height = data[i] * y_stretch;
        p.drawLine(rect.left() + qRound(i * x_stretch),
                   rect.bottom(),
                   rect.left() + qRound(i * x_stretch),
                   rect.bottom() - item_height);
    }
}

static void drawHoriontalScale(QPainter& p, const QRect& bbox, int size)
{
    static const int RULER_FONT_SIZE = 9;
    static const int STEP_WIDTH = 24;
    QFont ruler_font;
    ruler_font.setPixelSize(RULER_FONT_SIZE);
    p.setFont(ruler_font);

    const int text_y_pos = bbox.bottom() - RULER_FONT_SIZE;

    qreal x_stretch = bbox.width() / (qreal) size;

    for(int i = 0; i < size; i += (STEP_WIDTH / qRound(x_stretch))) {
        int x_pos = bbox.left() + qRound(i * x_stretch);
        p.drawText(QRect(x_pos - STEP_WIDTH / 2, text_y_pos, STEP_WIDTH, RULER_FONT_SIZE),
                   Qt::AlignCenter,
                   QString("%1").arg(i));
    }
}

static QRect chartRect(const QRect& r)
{
    static const int LEFT_MARGIN = 20;
    static const int BOTTOM_MARGIN = 13;
    static const int TOP_MARGIN = 3;

    return QRect(QPoint(r.left() + LEFT_MARGIN, r.top() + TOP_MARGIN),
                 QPoint(r.right(), r.bottom() - BOTTOM_MARGIN));
}

static QRect horizontalScaleRect(const QRect& r)
{
    static const int LEFT_MARGIN = 20;
    static const int BOTTOM_MARGIN = 20;
    return QRect(QPoint(r.left() + LEFT_MARGIN, r.bottom() - BOTTOM_MARGIN),
                 QPoint(r.right(), r.bottom()));
}

void HistogramWidget::paintEvent(QPaintEvent * event)
{
    if(data_.empty()) {
        qDebug() << Q_FUNC_INFO << "histogram is empty";
        return;
    }

    QPainter p(this);
    p.fillRect(rect(), Qt::white);

    // draw chart
    p.setPen(QPen(color_, 1));
    drawChart(p, chartRect(rect()), data_);

    // draw horizontal scale
    p.setPen(QColor(Qt::black));
    drawHoriontalScale(p, horizontalScaleRect(rect()), data_.size());

    // draw vertical scale
//    drawVertivcalScale(p, rect().bottomLeft(), rect().height(), 2);
    p.end();
}

void HistogramWidget::setData(const HistogramWidget::Histogram& hist)
{
    data_ = hist;
    repaint();
}
