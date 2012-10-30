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

#include "blocktype.h"

class QVBoxLayout;
class QToolBar;
class QAction;
class ImageView;
class Page;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget * parent = 0);

    void showPage(Page * p);
    void showPageBinarized();
    QSize sizeHint () const;
signals:
    /**
      * Emitted after gesture rotate attempt
      */
    void gestureRotateAttempt(int angle);

    /**
      * Emitted after connected page deletion
      */
    void pageDeleted();

    /**
      * Emitted on recognize request
      */
    void recognize(Page*);

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

    /**
     * Emmitted on segmentation request
     */
    void segment(Page * p);
public slots:
    void fitPage();
    void fitWidth();
    void handleActionBinarize(bool checked);
    void handleActionAddImageBlock();
    void handleActionAddTextBlock();
    void handleActionSegment();
    void handleActionSelectArea();
    void originalSize();
    void showChar(const QRect& bbox);
    void toggleLayoutBlocks();
    void updateSettings();
    void zoom(qreal value);
    void zoomIn();
    void zoomOut();
private:
    void setupLayout();
    void setupToolBar();
    void setupView();
    void updateActions();
    void updateFormatLayout();
private:
    QVBoxLayout * layout_;
    ImageView * view_;
    QToolBar * toolbar_;
    QAction * act_bin_;
    QAction * act_add_area_;
    QAction * act_add_image_;
    QAction * act_add_text_;
    QAction * act_toggle_layout_;
    QAction * act_segment_;
};

#endif // IMAGEWIDGET_H
