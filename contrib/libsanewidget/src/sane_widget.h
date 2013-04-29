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

#ifndef SANE_WIDGET_H
#define SANE_WIDGET_H

#include <sane/sane.h>

#include <QWidget>
#include <QTimer>
#include <QTranslator>

#include "icon_type.h"

class SaneOption;
class PreviewArea;
class QImage;
class QScrollArea;
class QPushButton;
class PreviewWidget;

enum {
    PROGRESS_MAX = 100,
    PROGRESS_MIN = 0
};

enum ReadStatus
{
    READ_NOT_READING,
    READ_ON_GOING,
    READ_ERROR,
    READ_CANCEL,
    READ_FINISHED
};

class SaneWidget : public QWidget
{
    Q_OBJECT
public:
    SaneWidget(QWidget* parent=0);
    ~SaneWidget();
    QString selectDevice(QWidget * parent=0);
    bool closeDevice();
    bool openDevice(const QString& deviceName);
    QImage * getFinalImage();
    bool setIcon(const QIcon& icon, IconType t);
    bool setIconColorMode(const QIcon& icon);
    bool setIconGrayMode(const QIcon& icon);
    bool setIconBWMode(const QIcon& icon);
public slots:
    void scanCancel();
signals:
    void scanStart();
    void scanProgress(int);
    void scanDone();
    void imageReady();
    void scanFaild();
private slots:
    void opt_level_change(int level);
    void scheduleValReload();
    void optReload();
    void valReload();
    void handleSelection(const QRect& rect);
    void scanPreview();
    void scanFinal();
    void setTLX(float x);
    void setTLY(float y);
    void setBRX(float x);
    void setBRY(float y);
private:
    SaneOption * getOption(const QString& name);
    void createLayout();
    void createOptInterface();
    void createPreview();
    bool fillSaneOptions();
    void loadTranslations();
    void loadTranslationsQt();
    void loadTranslationsGettext();
    void updatePreviewSize();
    bool openSaneDevice(const QString& deviceName);
    void processData();
    QStringList scannerList() const;
    void setDefaultValues();
private:
    // device info
    SANE_Handle s_handle_;
    QString devname_;
    QString modelname_;

    // Option variables
    QScrollArea * opt_area_;
    QList<SaneOption*> opt_list_;
    SaneOption * opt_mode_;
    SaneOption * opt_depth_;
    SaneOption * opt_res_;
    SaneOption * opt_res_y_;
    SaneOption * opt_tl_x_;
    SaneOption * opt_tl_y_;
    SaneOption * opt_br_x_;
    SaneOption * opt_br_y_;
    QWidget * color_opts_;
    QWidget * remain_opts_;
    SaneOption * opt_gam_r_;
    SaneOption * opt_gam_g_;
    SaneOption * opt_gam_b_;
    QTimer r_val_tmr_;

    // preview variables
    PreviewWidget * preview_;
    float preview_width_;
    float preview_height_;

    // general scanning
    SANE_Parameters params_;
    int progress_;
    SANE_Byte * img_data_;
    SANE_Byte px_colors_[3];
    unsigned int px_c_index_;
    int pixel_x_;
    int pixel_y_;
    ReadStatus read_status_;
    QImage * scan_img_;
    QImage the_img_;
    QTranslator tr_;
};

#endif
