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


#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class QVBoxLayout;
class ImageView;
class Page;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget * parent = 0);
    void showPage(Page * p);
    QSize sizeHint () const;
signals:
    void pageDeleted();

    /**
      * Emitted after success scale attemt
      */
    void scaled();

    /**
      * Emitted after scale attempt if scale is too big
      */
    void scaleIsTooBig();

    /**
      * Emitted after scale attempt if scale is too small
      */
    void scaleIsTooSmall();
public slots:
    void fitPage();
    void fitWidth();
    void originalSize();
    void updateFormatLayout();
    void updateSettings();
    void zoom(qreal value);
    void zoomIn();
    void zoomOut();
private:
    void setupLayout();
    void setupView();
private:
    QVBoxLayout * layout_;
    ImageView * view_;
};

#endif // IMAGEWIDGET_H
