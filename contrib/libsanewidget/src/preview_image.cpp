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

#include <stdio.h>
#include <math.h>
#include <iostream>

#include <QPainter>
#include <QMouseEvent>

#include "preview_image.h"

#define MIN_SCALE 0.4
#define MAX_SCALE 4.0

PreviewImage::PreviewImage(QWidget *parent)
: QWidget(parent)
{
    orig_img = QImage(400, 700, QImage::Format_RGB32);
    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 255; j++) {
            orig_img.setPixel(j, i, qRgb((int)sqrt(i*j),(int)sqrt(i*j),(int)sqrt(i*j)));
        }
    }

    x1=0; y1=0;
    tl_x = tl_y = br_x = br_y = 0;
    dtlx = dtly = dbrx = dbry = 0;
    scalefactor = 1;
    lmb = false;
    have_selection = false;
    change = MOVE_NONE;
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);
    updateScaledImg();
}

PreviewImage::~PreviewImage()
{
}


QSize PreviewImage::sizeHint() const
{
    return image.size();
}

void PreviewImage::clearSelection(void)
{
    have_selection = false;
    //tl_x = tl_y = br_x = br_y = 0;
    setCursor(Qt::CrossCursor);
    change = MOVE_NONE;

}

void PreviewImage::updateScaledImg(void)
{
    image = orig_img.scaledToWidth((int)(scalefactor*orig_img.width() +1));
    resize(image.size());
}

void PreviewImage::zoomIn(void)
{
    scalefactor *= 1.2;
    if (scalefactor > MAX_SCALE) scalefactor = MAX_SCALE;
    updateScaledImg();
    // FIXME we need a limit
    setTLX(tl_x_r);
    setTLY(tl_y_r);
    setBRX(br_x_r);
    setBRY(br_y_r);
    emit requestVisibility(tl_x, tl_y);
}

void PreviewImage::zoomOut(void)
{
    scalefactor /= 1.2;
    if (scalefactor < MIN_SCALE) scalefactor = MIN_SCALE;
    updateScaledImg();
    // FIXME we need a limit
    setTLX(tl_x_r);
    setTLY(tl_y_r);
    setBRX(br_x_r);
    setBRY(br_y_r);
    emit requestVisibility(tl_x, tl_y);
}

void PreviewImage::zoomSel(void)
{
    QWidget *parent = parentWidget();
    if (parent != 0) {
        float parent_size;
        float select_size;
        float w_scale;
        float h_scale;

        parent_size = (float)(parent->size().width()) - 40;
        if (parent_size < 40) {
            return;
        }
        select_size = (br_x_r - tl_x_r) * (float)(orig_img.width());
        if (select_size <= 50.0) {
            return;
        }
        w_scale = parent_size / select_size;

        parent_size = (float)(parent->size().height());
        if (parent_size < 40) {
            return;
        }
        select_size = (br_y_r - tl_y_r) * (float)(orig_img.height());
        if (select_size <= 50.0) {
            return;
        }
        h_scale = parent_size / select_size;

        scalefactor = (w_scale < h_scale) ? w_scale:h_scale;

        if (scalefactor > MAX_SCALE) scalefactor = MAX_SCALE;
        if (scalefactor < MIN_SCALE) scalefactor = MIN_SCALE;

        updateScaledImg();
        setTLX(tl_x_r);
        setTLY(tl_y_r);
        setBRX(br_x_r);
        setBRY(br_y_r);
    }
    emit requestVisibility(tl_x, tl_y);
}

void PreviewImage::zoom2Fit(void)
{
    if (orig_img.width() <= 10) {
        return;
    }
    QWidget *parent = parentWidget();
    if (parent != 0) {
        float w_scale = (float)(parent->size().width()-4) / (float)(orig_img.width());;
        float h_scale = (float)(parent->size().height()-4) / (float)(orig_img.height());
        scalefactor = (w_scale < h_scale) ? w_scale:h_scale;
        updateScaledImg();
        setTLX(tl_x_r);
        setTLY(tl_y_r);
        setBRX(br_x_r);
        setBRY(br_y_r);
    }
    emit requestVisibility(tl_x, tl_y);
}


void PreviewImage::mousePressEvent(QMouseEvent *event)
{
    //std::cout << "Button pressed=" << event->button() << std::endl;
    if (event->button() == Qt::LeftButton) {
        lmb=true;
        x1=event->x();
        y1=event->y();
        if ((have_selection == true) && (change == MOVE_ALL)) {
            dtlx = x1 - tl_x; dtly = y1 - tl_y;
            dbrx = br_x - x1; dbry = br_y - y1;
        }
        mouseMoveEvent(event);
    }
}


void PreviewImage::mouseMoveEvent(QMouseEvent *event)
{
    int min_tlx = tl_x;
    int min_tly = tl_y;
    int max_brx = br_x;
    int max_bry = br_y;

    int tmp_x=event->x();
    int tmp_y=event->y();
    if (tmp_x < 0) tmp_x = 0;
    if (tmp_y < 0) tmp_y = 0;
    if (tmp_x >= image.width()) tmp_x = image.width() - 1;
    if (tmp_y >= image.height()) tmp_y = image.height() - 1;
    //std::cout << "Move to x=" << tmp_x << " y=" << tmp_y << std::endl;
    if (lmb) {
        switch (change)
        {
            case MOVE_NONE:
                tmp_x=tmp_x;
                tmp_y=tmp_y;
                have_selection = true;
                if (tmp_x > x1) {tl_x=x1; br_x=tmp_x;}
                else         {tl_x=tmp_x; br_x=x1;}
                if (tmp_y > y1) {tl_y=y1; br_y=tmp_y;}
                else         {tl_y=tmp_y; br_y=y1;}
                break;
            case MOVE_TOP_LEFT:
                tl_x = tmp_x; tl_y = tmp_y;
                break;
            case MOVE_TOP_RIGHT:
                br_x = tmp_x; tl_y = tmp_y;
                break;
            case MOVE_BOTTOM_LEFT:
                tl_x = tmp_x; br_y = tmp_y;
                break;
            case MOVE_BOTTOM_RIGHT:
                br_x = tmp_x; br_y = tmp_y;
                break;
            case MOVE_LEFT:
                tl_x = tmp_x;
                break;
            case MOVE_RIGHT:
                br_x = tmp_x;
                break;
            case MOVE_TOP:
                tl_y = tmp_y;
                break;
            case MOVE_BOTTOM:
                br_y = tmp_y;
                break;
            case MOVE_ALL:
                // check low borders
                if ((tmp_x - dtlx) < 0) tmp_x = dtlx;
                if ((tmp_y - dtly) < 0) tmp_y = dtly;
                // check high borders
                if ((tmp_x + dbrx) > image.width()) tmp_x = image.width() - dbrx-1;
                if ((tmp_y + dbry) > image.height()) tmp_y = image.height() - dbry-1;

                tl_x = tmp_x - dtlx;
                tl_y = tmp_y - dtly;

                br_x = tmp_x + dbrx;
                br_y = tmp_y + dbry;
                break;
        }
        if (tl_x != br_x) {
            if (tl_x < min_tlx) min_tlx = tl_x;
            if (tl_y < min_tly) min_tly = tl_y;
            if (br_x > max_brx) max_brx = br_x;
            if (br_y > max_bry) max_bry = br_y;

            update(min_tlx-1, min_tly-1, max_brx-min_tlx+3, max_bry-min_tly+3);
        } else {
            update();
        }
    }
    // change cursor if needed
    else if (have_selection) {
        //std::cout << " tl_x=" << tl_x << " tl_y=" << tl_y;
        //std::cout << " br_x=" << br_x << " br_y=" << br_y << std::endl;
        // left border
        if (abs(tmp_x-tl_x) < 2) {
            if (abs(tmp_y - br_y) < 2) {
                setCursor(Qt::SizeBDiagCursor);
                change = MOVE_BOTTOM_LEFT;
            }
            else if (abs(tmp_y - tl_y) < 2) {
                setCursor(Qt::SizeFDiagCursor);
                change = MOVE_TOP_LEFT;
            }
            else if ((tmp_y < br_y) && (tmp_y > tl_y))
            {
                setCursor(Qt::SizeHorCursor);
                change = MOVE_LEFT;
            }
            else {
                setCursor(Qt::CrossCursor);
                change = MOVE_NONE;
            }
        }
        // right border
        else if (abs(tmp_x-br_x) < 2) {
            if (abs(tmp_y - br_y) < 2) {
                setCursor(Qt::SizeFDiagCursor);
                change = MOVE_BOTTOM_RIGHT;
            }
            else if (abs(tmp_y - tl_y) < 2) {
                setCursor(Qt::SizeBDiagCursor);
                change = MOVE_TOP_RIGHT;
            }
            else if ((tmp_y < br_y) && (tmp_y > tl_y)) {
                setCursor(Qt::SizeHorCursor);
                change = MOVE_RIGHT;
            }
            else {
                setCursor(Qt::CrossCursor);
                change = MOVE_NONE;
            }
        }
        // middle
        else if ((tmp_x > tl_x) && (tmp_x < br_x)) {
            if (abs(tmp_y - br_y) < 2) {
                setCursor(Qt::SizeVerCursor);
                change = MOVE_BOTTOM;
            }
            else if (abs(tmp_y - tl_y) < 2) {
                setCursor(Qt::SizeVerCursor);
                change = MOVE_TOP;
            }
            else if ((tmp_y < br_y) && (tmp_y > tl_y)) {
                setCursor(Qt::SizeAllCursor);
                change = MOVE_ALL;
            }
            else {
                setCursor(Qt::CrossCursor);
                change = MOVE_NONE;
            }
        }
        // else out side
        else {
            setCursor(Qt::CrossCursor);
            change = MOVE_NONE;
        }
    }
}

void PreviewImage::mouseReleaseEvent(QMouseEvent *event)
{
    //std::cout << "Button released=" << event->button() << std::endl;
    if (lmb) {
        lmb=false;
        if ((tl_x==br_x) && (tl_y==br_y)) have_selection = false;
        // fix the order of tl and br
        if (tl_x > br_x) {int tmp = br_x; br_x = tl_x; tl_x = tmp;}
        if (tl_y > br_y) {int tmp = br_y; br_y = tl_y; tl_y = tmp;}
        mouseMoveEvent(event);

        tl_x_r = (float)tl_x / (float)image.width();
        tl_y_r = (float)tl_y / (float)image.height();
        // +1 is to also include the last pixel in to the selection
        br_x_r = (float)(br_x+1) / (float)image.width();
        br_y_r = (float)(br_y+1) / (float)image.height();

        if (have_selection == true) {
            //printf("mouseReleaseEvent tl %d,%d br %d,%d\n", tl_x, tl_y, br_x, br_y);
            emit newSelection(tl_x_r, tl_y_r, br_x_r, br_y_r);
        }
        else  emit newSelection(0,0,0,0);
    }
}

void PreviewImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (!event->region().boundingRect().isNull()) {

        painter.drawImage(event->region().boundingRect(), image, event->region().boundingRect());

        if ((tl_x != br_x) && (tl_y != br_y) && (have_selection == true)) {
            painter.setPen(QPen( QColor(0,0,0), 1, Qt::SolidLine));
            painter.drawLine(tl_x,tl_y,br_x,tl_y);
            painter.drawLine(br_x,tl_y,br_x,br_y);
            painter.drawLine(br_x,br_y,tl_x,br_y);
            painter.drawLine(tl_x,br_y,tl_x,tl_y);
            painter.setPen(QPen( QColor(255,255,255), 1, Qt::DotLine));
            painter.drawLine(tl_x,tl_y,br_x,tl_y);
            painter.drawLine(br_x,tl_y,br_x,br_y);
            painter.drawLine(br_x,br_y,tl_x,br_y);
            painter.drawLine(tl_x,br_y,tl_x,tl_y);
        }
    }
}

void PreviewImage::setTLX(float ratio)
{
    tl_x = (int)((image.width()*ratio)+0.5);

    if (tl_x < 0) tl_x = 0;
    if (tl_x > image.width()) tl_x = image.width();
}

void PreviewImage::setTLY(float ratio)
{
    tl_y = (int)((image.height()*ratio)+0.5);

    if (tl_y < 0) tl_y = 0;
    if (tl_y > image.height()) tl_y = image.height();
}

void PreviewImage::setBRX(float ratio)
{
    br_x = (int)((image.width()*ratio)+0.5) -1;

    if (br_x < 0) tl_x = 0;
    if (br_x > image.width()) br_x = image.width();
}

void PreviewImage::setBRY(float ratio)
{
    br_y = (int)((image.height()*ratio)+0.5) -1;

    if (br_y < 0) tl_y = 0;
    if (br_y > image.height()) br_y = image.height();
}

