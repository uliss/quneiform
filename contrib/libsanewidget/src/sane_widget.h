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

#include <QScrollArea>
#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QToolBar>
#include <QTimer>

class SaneOption;
class PreviewArea;

extern "C" {
#include <sane/sane.h>
}

#define MAX_NUM_OPTIONS 100
#define IMG_DATA_R_SIZE 1000
#define PROGRESS_MAX 100
#define PROGRESS_MIN 0

typedef enum
{
    READ_NOT_READING,
    READ_ON_GOING,
    READ_ERROR,
    READ_CANCEL,
    READ_FINISHED
} ReadStatus;

class SaneWidget : public QWidget
{
    Q_OBJECT

public:
    SaneWidget(QWidget* parent=0);
    ~SaneWidget(void);
    QString selectDevice(QWidget* parent=0);
    bool openDevice(const QString &device_name);
    QImage *getFinalImage(void);
    bool setIconColorMode(const QIcon &icon);
    bool setIconGrayMode(const QIcon &icon);
    bool setIconBWMode(const QIcon &icon);
    bool setIconPreview(const QIcon &icon);
    bool setIconFinal(const QIcon &icon);
    bool setIconZoomIn(const QIcon &icon);
    bool setIconZoomOut(const QIcon &icon);
    bool setIconZoomSel(const QIcon &icon);
    bool setIconZoomFit(const QIcon &icon);


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
    void handleSelection(float tl_x, float tl_y, float br_x, float br_y);
    void scanPreview(void);
    void scanFinal(void);
    void setTLX(float x);
    void setTLY(float y);
    void setBRX(float x);
    void setBRY(float y);

private:
    SaneOption *getOption(const QString &name);
    void createOptInterface(void);
    void updatePreviewSize(void);
    void processData(void);
    void setDefaultValues(void);

    // device info
    SANE_Device const **dev_list;
    SANE_Device const *device;
    SANE_Handle s_handle;
    QString devname;
    QString modelname;

    // Option variables
    QScrollArea *opt_area;
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


    QPushButton *scan_btn;
    QPushButton *prev_btn;

    QPushButton *z_in_btn;
    QPushButton *z_out_btn;
    QPushButton *z_sel_btn;
    QPushButton *z_fit_btn;
    // preview variables
    PreviewArea *preview;
    QImage *pr_img;
    float previewWidth;
    float previewHeight;

    // general scanning
    SANE_Parameters params;
    //QSocketNotifier *sn;
    int progress;
    SANE_Byte img_data[IMG_DATA_R_SIZE];
    SANE_Byte px_colors[3];
    unsigned int px_c_index;
    int pixel_x, pixel_y;
    ReadStatus read_status;
    QImage *scan_img;
    QImage the_img;

};

#endif
