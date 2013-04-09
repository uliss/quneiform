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

#ifndef PREVIEW_AREA_H
#define PREVIEW_AREA_H

#include <QAction>
#include <QFrame>
#include <QImage>
#include <QScrollArea>

#include "preview_image.h"
/**
 *@author Kåre Särs
 */

/**
 * This is the ScrollArea that will contain the preview image
 */
class PreviewArea : public QScrollArea
{
    Q_OBJECT

    public:
        PreviewArea(QWidget *parent);
        ~PreviewArea();
        QSize sizeHint(void) const;

        void clearSelection(void);
        QImage *getImage(void);
        void updateScaledImg(void);
        bool setIconFinal(const QIcon &icon);
        bool setIconZoomIn(const QIcon &icon);
        bool setIconZoomOut(const QIcon &icon);
        bool setIconZoomSel(const QIcon &icon);
        bool setIconZoomFit(const QIcon &icon);

    public slots:
        void setTLX(float percent);
        void setTLY(float percent);
        void setBRX(float percent);
        void setBRY(float percent);

        void zoomIn(void);
        void zoomOut(void);
        void zoomSel(void);
        void zoom2Fit(void);

    private slots:
        void requestVisibility(int tl_x, int tl_y);

    signals:
        void newSelection(float tl_x, float tl_y, float br_x, float br_y);

    private:
        void createContextMenu(void);

        PreviewImage *image;
        QAction *zoomInAction;
        QAction *zoomOutAction;
        QAction *zoomSelAction;
        QAction *zoom2FitAction;
};


#endif
