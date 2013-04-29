/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>

#include "icon_type.h"

class PreviewArea;
class QImage;
class QPushButton;

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget * parent = 0);
    QImage * previewImage();
    void setIcon(const QIcon& icon, IconType type);
    void setTLX(float percent);
    void setTLY(float percent);
    void setBRX(float percent);
    void setBRY(float percent);
    QSize sizeHint() const;
    void updatePreviewSize(int width, int height);
    void updatePreviewSize(const QSize& size);
    void updateScanSize(const QSize& size);
    void updateScanSize(int width, int height);
    void updateScaledImg();
signals:
    void newSelection(const QRect& selection);
    void preview();
    void scan();
private slots:
    void handleSelection(float left, float top, float right, float bottom);
private:
    PreviewArea * preview_;
    QImage * preview_image_;
    QPushButton * zoom_in_btn_;
    QPushButton * zoom_out_btn_;
    QPushButton * zoom_sel_btn_;
    QPushButton * zoom_fit_btn_;
    QPushButton * preview_btn_;
    QPushButton * scan_btn_;

};

#endif // PREVIEWWIDGET_H
