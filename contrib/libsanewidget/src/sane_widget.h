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
    ~SaneWidget(void);
    QString selectDevice(QWidget* parent=0);
    bool closeDevice();
    bool openDevice(const QString& deviceName);
    QImage * getFinalImage(void);
    bool setIcon(const QIcon& icon, IconType t);
    bool setIconColorMode(const QIcon &icon);
    bool setIconGrayMode(const QIcon &icon);
    bool setIconBWMode(const QIcon &icon);
public slots:
    void scanCancel(void);
signals:
    void scanStart(void);
    void scanProgress(int);
    void scanDone(void);
    void imageReady(void);
    void scanFaild(void);
private slots:
    void opt_level_change(int level);
    void scheduleValReload(void);
    void optReload(void);
    void valReload(void);
    void handleSelection(const QRect& rect);
    void scanPreview(void);
    void scanFinal(void);
    void setTLX(float x);
    void setTLY(float y);
    void setBRX(float x);
    void setBRY(float y);
private:
    SaneOption * getOption(const QString &name);
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
    SANE_Device const **dev_list;
    SANE_Device const *device;
    SANE_Handle s_handle;
    QString devname;
    QString modelname_;

    // Option variables
    QScrollArea *opt_area_;
    bool options_read;
    QList<SaneOption *> optList;
    SaneOption *opt_mode;
    SaneOption *opt_depth;
    SaneOption *opt_res;
    SaneOption *opt_res_y;
    SaneOption *opt_tl_x;
    SaneOption *opt_tl_y;
    SaneOption *opt_br_x;
    SaneOption *opt_br_y;
    QWidget *color_opts;
    QWidget *remain_opts;
    SaneOption *opt_gam_r;
    SaneOption *opt_gam_g;
    SaneOption *opt_gam_b;
    QTimer r_val_tmr;

    // preview variables
    PreviewWidget * preview_;
    float previewWidth;
    float previewHeight;

    // general scanning
    SANE_Parameters params;
    int progress;
    SANE_Byte * img_data;
    SANE_Byte px_colors[3];
    unsigned int px_c_index;
    int pixel_x, pixel_y;
    ReadStatus read_status;
    QImage *scan_img;
    QImage the_img;
    QTranslator tr_;
};

#endif
