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

#ifndef PREVIEW_IMAGE_H
#define PREVIEW_IMAGE_H

#include <QAction>
#include <QFrame>
#include <QImage>

enum sel_change
{
    MOVE_NONE,
    MOVE_TOP_LEFT,
    MOVE_TOP_RIGHT,
    MOVE_BOTTOM_LEFT,
    MOVE_BOTTOM_RIGHT,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_TOP,
    MOVE_BOTTOM,
    MOVE_ALL
};
/**
 *@author Kåre Särs
 */

/**
 * This is the frame that will contain the preview image
 */
class PreviewImage : public QWidget
{
    Q_OBJECT

    public:
   /**
     * Create the frame.
     * \param parent parent widget
    */
        PreviewImage(QWidget *parent);
        ~PreviewImage();

        QSize sizeHint(void) const;

        void clearSelection(void);
        QImage *getImage(void) {return &orig_img;}
        void updateScaledImg(void);

    public slots:
        void setTLX(float percent);
        void setTLY(float percent);
        void setBRX(float percent);
        void setBRY(float percent);

        void zoomIn(void);
        void zoomOut(void);
        void zoomSel(void);
        void zoom2Fit(void);

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    signals:
        void newSelection(float tl_x, float tl_y, float br_x, float br_y);
        void requestVisibility(int tl_x, int tl_y);

    private:
        QImage image;
        QImage orig_img;

        int x1, y1;
        // tl == Top Left, br == Botom Right
        int tl_x, tl_y, br_x, br_y;
        int dtlx, dtly, dbrx, dbry;
        // tl_x_r = tl_x / image.width()
        float tl_x_r, tl_y_r, br_x_r, br_y_r;

        bool lmb;
        bool have_selection;
        sel_change change;
        float scalefactor;
};


#endif
