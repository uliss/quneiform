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


#ifndef IMAGEGRAPHICSVIEW_H
#define IMAGEGRAPHICSVIEW_H

#include <QGraphicsView>

class QRubberBand;

class ImageGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageGraphicsView(QWidget * parent = 0);
signals:
    void pageAreaSelected(const QRectF&);
protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
private slots:
    void selectPageArea();
private:
    void updateCursor();
private:
    enum select_mode_t {
        NORMAL = 0,
        SELECT_PAGE,
        SELECT_TEXT,
        SELECT_IMAGE,
        SELECT_TABLE
    };

    select_mode_t select_mode_;
    QRubberBand * rubber_band_;
    QPoint selection_origin_;
};

#endif // IMAGEGRAPHICSVIEW_H
