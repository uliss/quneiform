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
#include <sys/ioctl.h>
#include <sane/saneopts.h>
#include <iostream>
#include <libintl.h> // for gettext

#include <QEventLoop>
#include <QApplication>
#include <QComboBox>
#include <QTranslator>
#include <QDebug>
#include <QScrollArea>

#include "sane_option.h"
#include "previewwidget.h"
#include "sane_widget.h"
#include "labeled_separator.h"
#include "radio_select.h"
#include "labeled_gamma.h"

static inline QString my_i18n(const char * msg)
{
    return QString::fromUtf8(gettext(msg));
}

namespace {
bool init_resources()
{
//    Q_INIT_RESOURCE(sanewidget_i18n);
    return true;
}

bool init_resources_res = init_resources();
}

enum {
    IMG_DATA_R_SIZE = 1024 * 50
};

SaneWidget::SaneWidget(QWidget* parent)
    : QWidget(parent),
      device(NULL),
      preview_(NULL),
      img_data(NULL),
      opt_area_(0),
      opt_tl_x(0),
      opt_tl_y(0),
      opt_br_x(0),
      opt_br_y(0),
      opt_res(0),
      opt_res_y(0),
      opt_gam_r(0),
      opt_gam_g(0),
      opt_gam_b(0),
      previewWidth(0),
      previewHeight(0),
      read_status(READ_NOT_READING),
      px_c_index(0),
      color_opts(0),
      remain_opts(0)
{
    loadTranslations();
    SANE_Int version;

    the_img = QImage(10, 10, QImage::Format_RGB32);

    img_data = new SANE_Byte[IMG_DATA_R_SIZE];

    SANE_Status status = sane_init(&version, 0);
    if (status != SANE_STATUS_GOOD) {
        qWarning() << Q_FUNC_INFO << QString("SaneWidget: sane_init() failed()").
                      arg(sane_strstatus(status));
    }
    else {
        qDebug() << QString("Sane Version = %1.%2.%3").
                    arg(SANE_VERSION_MAJOR(version)).
                    arg(SANE_VERSION_MINOR(version)).
                    arg(SANE_VERSION_BUILD(version));
    }
    r_val_tmr.setSingleShot(true);
    connect (&r_val_tmr, SIGNAL(timeout(void)), this, SLOT(valReload()));
}

SaneWidget::~SaneWidget(void)
{
    QApplication::removeTranslator(&tr_);
    delete[] img_data;
    optList.clear();
    sane_exit();
}

QString SaneWidget::selectDevice(QWidget * parent)
{
    QStringList scanner_list = scannerList();

    RadioSelect sel;
    sel.setWindowTitle(qApp->applicationName());
    int idx = sel.getSelectedIndex(parent, tr("Select Scanner"), scanner_list, 0);

    if (idx == scanner_list.count())
        return QString("test:0");

    if ((idx < 0) || (idx >= scanner_list.count()))
        return QString("");

    return scanner_list.at(idx).split("\n").at(0);
}

bool SaneWidget::closeDevice()
{
    if(!s_handle)
        return false;

    sane_close(s_handle);
    s_handle = NULL;
    return true;
}

bool SaneWidget::openDevice(const QString& deviceName)
{
    if(!openSaneDevice(deviceName))
        return false;

    if(!fillSaneOptions())
        return false;

    createPreview();
    // create base layout
    createLayout();
    // Create the options interface
    createOptInterface();

    // try to set SaneWidget default values
    setDefaultValues();

    // estimate the preview size and create an empty image
    // this is done so that you can select scanarea without
    // having to scan a preview.
    updatePreviewSize();

    //preview->zoom2Fit();

    return true;
}

void SaneWidget::createOptInterface(void)
{
    // create the container widget
    QWidget * opt_container = new QWidget(opt_area_);
    opt_area_->setWidget(opt_container);
    QVBoxLayout *opt_layout = new QVBoxLayout(opt_container);
    opt_layout->setSpacing(4);
    opt_layout->setMargin(3);

    // add the options
    // (Should Vendor and model always be visible?)
    LabeledSeparator *model_label = new LabeledSeparator(opt_container, modelname_);
    opt_layout->addWidget(model_label);

    // basic/intermediate/All options
    QStringList strl;
    strl << tr("Basic") << tr("Advanced") << tr("All Options");
    LabeledCombo *opt_level = new LabeledCombo(opt_container, tr("Option Level"), strl);
    opt_layout->addWidget(opt_level);

    // add separator line
    opt_layout->addSpacing(4);
    QFrame *line0 = new QFrame(opt_container);
    line0->setFrameShape(QFrame::HLine);
    line0->setFrameShadow(QFrame::Sunken);
    opt_layout->addWidget(line0);
    opt_layout->addSpacing(4);

    SaneOption *option;
    // Scan Source
    if ((option = getOption(SANE_NAME_SCAN_SOURCE)) != 0) {
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    // film-type
    if ((option = getOption(QString("film-type"))) != 0)
    {
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    else if ((option = getOption(SANE_NAME_NEGATIVE)) != 0)
    {
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    // Scan mode
    if ((option = getOption(SANE_NAME_SCAN_MODE)) != 0) {
        opt_mode = option;
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    // Bitdepth
    if ((option = getOption(SANE_NAME_BIT_DEPTH)) != 0) {
        opt_depth = option;
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    // Threshold
    if ((option = getOption(SANE_NAME_THRESHOLD)) != 0) {
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    // Resolution
    if ((option = getOption(SANE_NAME_SCAN_RESOLUTION)) != 0) {
        opt_res = option;
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    else if ((option = getOption(SANE_NAME_SCAN_X_RESOLUTION)) != 0) {
        opt_res = option;
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_SCAN_Y_RESOLUTION)) != 0) {
        opt_res_y = option;
        option->createWidget(opt_container);
        opt_layout->addWidget(option->widget());
    }
    // scan area
    if ((option = getOption(SANE_NAME_SCAN_TL_X)) != 0) {
        opt_tl_x = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setTLX(float)));
    }
    if ((option = getOption(SANE_NAME_SCAN_TL_Y)) != 0) {
        opt_tl_y = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setTLY(float)));
    }
    if ((option = getOption(SANE_NAME_SCAN_BR_X)) != 0) {
        opt_br_x = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setBRX(float)));
    }
    if ((option = getOption(SANE_NAME_SCAN_BR_Y)) != 0) {
        opt_br_y = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setBRY(float)));
    }

    // Color Options Frame
    color_opts = new QWidget(opt_container);
    opt_layout->addWidget(color_opts);
    QVBoxLayout *color_lay = new QVBoxLayout(color_opts);
    color_lay->setSpacing(2);
    color_lay->setMargin(0);

    // add separator line
    color_lay->addSpacing(6);
    QFrame *line1 = new QFrame(color_opts);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    color_lay->addWidget(line1);
    color_lay->addSpacing(2);

    if ((option = getOption(SANE_NAME_BRIGHTNESS)) != 0) {
        option->createWidget(color_opts);
        color_lay->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_CONTRAST)) != 0) {
        option->createWidget(color_opts);
        color_lay->addWidget(option->widget());
    }

    // gamma tables
    QWidget *gamma_frm = new QWidget(color_opts);
    color_lay->addWidget(gamma_frm);
    QVBoxLayout *gam_frm_l = new QVBoxLayout(gamma_frm);
    gam_frm_l->setSpacing(2);
    gam_frm_l->setMargin(0);

    if ((option = getOption(SANE_NAME_GAMMA_VECTOR_R)) != 0) {
        opt_gam_r= option;
        option->createWidget(gamma_frm);
        gam_frm_l->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_GAMMA_VECTOR_G)) != 0) {
        opt_gam_g= option;
        option->createWidget(gamma_frm);
        gam_frm_l->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_GAMMA_VECTOR_B)) != 0) {
        opt_gam_b= option;
        option->createWidget(gamma_frm);
        gam_frm_l->addWidget(option->widget());
    }

    if ((opt_gam_r != 0) && (opt_gam_g != 0) && (opt_gam_b != 0)) {
        LabeledGamma *lgamma = new LabeledGamma(color_opts,
                                  my_i18n(SANE_TITLE_GAMMA_VECTOR),
                                  opt_gam_r->lgamma->size());
        lgamma->setToolTip(my_i18n(SANE_DESC_GAMMA_VECTOR));
        color_lay->addWidget(lgamma);
        connect(lgamma, SIGNAL(gammaChanged(int,int,int)),
                opt_gam_r->lgamma, SLOT(setValues(int,int,int)));
        connect(lgamma, SIGNAL(gammaChanged(int,int,int)),
                opt_gam_g->lgamma, SLOT(setValues(int,int,int)));
        connect(lgamma, SIGNAL(gammaChanged(int,int,int)),
                opt_gam_b->lgamma, SLOT(setValues(int,int,int)));

        QCheckBox *split_gam_btn = new QCheckBox(tr("Separate color intensity tables"), opt_container);
        color_lay->addWidget(split_gam_btn);
        connect (split_gam_btn, SIGNAL(toggled(bool)), gamma_frm, SLOT(setVisible(bool)));
        connect (split_gam_btn, SIGNAL(toggled(bool)), lgamma, SLOT(setHidden(bool)));
        gamma_frm->hide();

    }

    if ((option = getOption(SANE_NAME_BLACK_LEVEL)) != 0) {
        option->createWidget(color_opts);
        color_lay->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_WHITE_LEVEL)) != 0) {
        option->createWidget(color_opts);
        color_lay->addWidget(option->widget());
    }

    // Remaining (un known) Options Frame
    remain_opts = new QWidget(opt_container);
    opt_layout->addWidget(remain_opts);
    QVBoxLayout *remain_lay = new QVBoxLayout(remain_opts);
    remain_lay->setSpacing(2);
    remain_lay->setMargin(0);

    // add separator line
    remain_lay->addSpacing(4);
    QFrame *line2 = new QFrame(remain_opts);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    remain_lay->addWidget(line2);
    remain_lay->addSpacing(4);

    // add remaining parameters
    for (int i=0; i<optList.size(); i++) {
        if ((optList.at(i)->widget() == 0) &&
             (optList.at(i)->name() != SANE_NAME_SCAN_TL_X) &&
             (optList.at(i)->name() != SANE_NAME_SCAN_TL_Y) &&
             (optList.at(i)->name() != SANE_NAME_SCAN_BR_X) &&
             (optList.at(i)->name() != SANE_NAME_SCAN_BR_Y) &&
             (optList.at(i)->sw_type() != SW_GROUP)
           )
        {
            optList.at(i)->createWidget(remain_opts);
            remain_lay->addWidget(optList.at(i)->widget());
        }
    }

    // connect showing/hiding finctionality
    connect (opt_level, SIGNAL(activated(int)), this, SLOT(opt_level_change(int)));

    // add a stretch to the end to ceep the parameters at the top
    opt_layout->addStretch();

    // encsure that you do not get a scrollbar at the bottom of the option of the options
    opt_area_->setMinimumWidth(opt_container->sizeHint().width()+20);

    // this could/should be set by saved settings.
    color_opts->setVisible(false);
    remain_opts->setVisible(false);
}

void SaneWidget::createPreview()
{
    preview_ = new PreviewWidget(this);
    connect(preview_, SIGNAL(newSelection(QRect)), this, SLOT(handleSelection(QRect)));
    connect(preview_, SIGNAL(scan()), this, SLOT(scanFinal()));
    connect(preview_, SIGNAL(preview()), this, SLOT(scanPreview()));
}

bool SaneWidget::fillSaneOptions()
{
    // Read the options (start with option 0 the number of parameters)
    const SANE_Option_Descriptor * num_option_d = sane_get_option_descriptor(s_handle, 0);
    if (num_option_d == 0) {
        qWarning() << Q_FUNC_INFO << "no options";
        return false;
    }

    char data[num_option_d->size];
    SANE_Int res;
    SANE_Status status = sane_control_option(s_handle, 0, SANE_ACTION_GET_VALUE, data, &res);
    if (status != SANE_STATUS_GOOD) {
        qWarning() << Q_FUNC_INFO << "can't read option";
        return false;
    }

    SANE_Word num_sane_options = *(SANE_Word*)data;

    // read the rest of the options
    for (int i = 1; i < num_sane_options; i++) {
        optList.append(new SaneOption(s_handle, i));
    }

    // do the connections of the option parameters
    for (int i = 1; i < optList.size(); i++) {
        connect (optList.at(i), SIGNAL(optsNeedReload()), this, SLOT(optReload()));
        connect (optList.at(i), SIGNAL(valsNeedReload()), this, SLOT(scheduleValReload()));
    }

    return true;
}

void SaneWidget::loadTranslations()
{
    loadTranslationsQt();
    loadTranslationsGettext();
}

void SaneWidget::loadTranslationsQt()
{
    QLocale locale;
    QString tr_fname = "sanewidget_" + locale.name();
    bool res = tr_.load(tr_fname, ":/sanewidget");
    if(!res) {
        qWarning() << Q_FUNC_INFO << "can't load translation: " << tr_fname;
        return;
    }

    QApplication::installTranslator(&tr_);
}

void SaneWidget::loadTranslationsGettext()
{
    QLocale locale;
    setlocale(LC_ALL, locale.name().toAscii().constData());
    bindtextdomain("sane-backends", "/usr/local/share/locale");
    textdomain( "sane-backends");
}

//************************************************************
void SaneWidget::opt_level_change(int level)
{
    if (color_opts == 0) return;
    if (remain_opts == 0) return;

    switch (level)
    {
        case 1:
            color_opts->setVisible(true);
            remain_opts->setVisible(false);
            break;
        case 2:
            color_opts->setVisible(true);
            remain_opts->setVisible(true);
            break;
        default:
            color_opts->setVisible(false);
            remain_opts->setVisible(false);
    }
}

//************************************************************
void SaneWidget::setDefaultValues(void)
{
    SaneOption *option;

    // Try to get Color mode by default
    if ((option = getOption(SANE_NAME_SCAN_MODE)) != 0) {
        option->setValue(QString::fromUtf8(gettext(SANE_VALUE_SCAN_MODE_COLOR)));
    }

    // Try to set 8 bit color
    if ((option = getOption(SANE_NAME_BIT_DEPTH)) != 0) {
        option->setValue(8);
    }

    // Try to set Scan resolution to 600 DPI
    if (opt_res != 0) {
        opt_res->setValue(600);
    }
}

//************************************************************
void SaneWidget::scheduleValReload(void)
{
    r_val_tmr.start(5);
}

//************************************************************
void SaneWidget::optReload(void)
{
    int i;
    //printf("Reload Options\n");

    for (i=0; i<optList.size(); i++)
    {
        optList.at(i)->readOption();
        // Also read the values
        optList.at(i)->readValue();
    }
    // estimate the preview size and create an empty image
    // this is done so that you can select scanarea without
    // having to scan a preview.
    updatePreviewSize();
}

//************************************************************
void SaneWidget::valReload(void)
{
    int i;
    QString tmp;
    //printf("Reload Values\n");

    for (i=0; i<optList.size(); i++)
    {
        optList.at(i)->readValue();
        /*
        if (optList.at(i)->getValue(&tmp)) {
            printf("option(%s)=%s\n",
                   qPrintable(optList.at(i)->name()),
                   qPrintable(tmp));
        }
        */
    }
}

//************************************************************
SaneOption *SaneWidget::getOption(const QString &name)
{
    int i;

    for (i=0; i<optList.size(); i++)
    {
        if (optList.at(i)->name() == name) {
            return optList.at(i);
        }
    }
    return 0;
}

void SaneWidget::createLayout()
{
    QHBoxLayout * base_layout = new QHBoxLayout;
    base_layout->setSpacing(2);
    base_layout->setMargin(0);
    setLayout(base_layout);

    // Create Option Scroll Area
    opt_area_ = new QScrollArea(this);
    opt_area_->setWidgetResizable(true);
    opt_area_->setFrameShape(QFrame::NoFrame);

    QVBoxLayout * preview_layout = new QVBoxLayout;
    QVBoxLayout * opt_layout = new QVBoxLayout;

    base_layout->addLayout(opt_layout, 0);
    base_layout->addLayout(preview_layout, 100);

    opt_layout->addWidget(opt_area_);
    preview_layout->addWidget(preview_);
}

void SaneWidget::handleSelection(const QRect& rect)
{
    float max_x, max_y;
    opt_br_x->getMaxValue(&max_x);
    opt_br_y->getMaxValue(&max_y);

    float ftl_x = rect.left() * max_x;
    float ftl_y = rect.top() * max_y;
    float fbr_x = rect.right() * max_x;
    float fbr_y = rect.bottom() * max_y;

    if(opt_tl_x != 0) opt_tl_x->setValue(ftl_x);
    if(opt_tl_y != 0) opt_tl_y->setValue(ftl_y);
    if(opt_br_x != 0) opt_br_x->setValue(fbr_x);
    if(opt_br_y != 0) opt_br_y->setValue(fbr_y);
}

void SaneWidget::setTLX(float ftlx) {
    float max, ratio;

    opt_br_x->getMaxValue(&max);
    ratio = ftlx / max;
    preview_->setTLX(ratio);
}

void SaneWidget::setTLY(float ftly) {
    float max, ratio;

    //std::cout << "setTLY " << ftly;
    opt_br_y->getMaxValue(&max);
    ratio = ftly / max;
    //std::cout << " -> " << ratio << std::endl;
    preview_->setTLY(ratio);
}

void SaneWidget::setBRX(float fbrx) {
    float max, ratio;

    //std::cout << "setBRX " << fbrx;
    opt_br_x->getMaxValue(&max);
    ratio = fbrx / max;
    //std::cout << " -> " << ratio << std::endl;
    preview_->setBRX(ratio);
}

//************************************************************
void SaneWidget::setBRY(float fbry) {
    float max, ratio;

    //std::cout << "setBRY " << fbry;
    opt_br_y->getMaxValue(&max);
    ratio = fbry / max;
    //std::cout << " -> " << ratio << std::endl;
    preview_->setBRY(ratio);
}

void SaneWidget::updatePreviewSize(void)
{
    SANE_Status status;
    int i, j, dpi;
    float max_x=0, max_y=0;

    // check if an update is necessary
    if (opt_br_x != 0) {
        opt_br_x->getMaxValue(&max_x);
    }
    if (opt_br_y != 0) {
        opt_br_y->getMaxValue(&max_y);
    }
    if ((max_x == previewWidth) && (max_y == previewHeight)) {
        return;
    }

    previewWidth = max_x;
    previewHeight = max_y;
    // set the scan area to the whole area
    if (opt_tl_x != 0) {
        opt_tl_x->setValue(0);
    }
    if (opt_tl_y != 0) {
        opt_tl_y->setValue(0);
    }

    if (opt_br_x != 0) {
        opt_br_x->setValue(max_x);
    }
    if (opt_br_y != 0) {
        opt_br_y->setValue(max_y);
    }

    // set the resopution to 100 dpi and increase if necessary
    dpi = 0;
    do {
        // Increase the dpi value
        dpi += 100;
        if (opt_res != 0) {
            opt_res->setValue(dpi);
        }
        if (opt_res_y != 0) {
            opt_res_y->setValue(dpi);
        }
        //check what image size we would get in a scan
        status = sane_get_parameters(s_handle, &params);
        if (status != SANE_STATUS_GOOD) {
            printf("ERROR status=%s\n", sane_strstatus(status));
            return;
        }
        //printf("dpi = %d\n", dpi);
        //printf("lines = %d\n", params.lines);
        //printf("pixels_per_line = %d\n", params.pixels_per_line);
        if (dpi > 800) break;
    } while ((params.pixels_per_line < 300) || (params.lines < 300));


    preview_->updateScanSize(params.pixels_per_line, params.lines);
}

bool SaneWidget::openSaneDevice(const QString& deviceName)
{
    SANE_Device const **dev_list;
    // get the device list to get the vendor and model info
    SANE_Status status = sane_get_devices(&dev_list, SANE_TRUE);
    if(status != SANE_STATUS_GOOD) {
        qWarning() << Q_FUNC_INFO << "can't get device list" << sane_strstatus(status);
        return false;
    }

    int i = 0;
    while(dev_list[i] != 0) {
        if (QString(dev_list[i]->name) == deviceName) {
            modelname_ = QString("%1 %2").arg(dev_list[i]->vendor).arg(dev_list[i]->model);
            break;
        }
        i++;
    }

    if (dev_list[i] == 0) {
        qWarning() << Q_FUNC_INFO << QString("device '%1' not found").arg(deviceName);
        return false;
    }

    // Try to open the device
    status = sane_open(deviceName.toLatin1(), &s_handle);
    if (status != SANE_STATUS_GOOD) {
        qWarning() << Q_FUNC_INFO << QString("openDevice: sane_open(\"%1\", &handle) failed! '%2'").
                      arg(deviceName).arg(sane_strstatus(status));
        return false;
    }

    devname = deviceName;
    return true;
}

void SaneWidget::scanPreview(void)
{
    SANE_Status status;
    float max;
    int i, j, dpi;

    //std::cout << "scanPreview" << std::endl;

    // store the current settings of parameters to be changed
    if (opt_depth != 0) opt_depth->storeCurrentData();
    if (opt_res != 0) opt_res->storeCurrentData();
    if (opt_res_y != 0) opt_res_y->storeCurrentData();
    if (opt_tl_x != 0) opt_tl_x->storeCurrentData();
    if (opt_tl_y != 0) opt_tl_y->storeCurrentData();
    if (opt_br_x != 0) opt_br_x->storeCurrentData();
    if (opt_br_y != 0) opt_br_y->storeCurrentData();

    // set 8 bits per color if possible
    if (opt_depth != 0) {
        opt_depth->setValue(8);
    }

    // select the whole area
    if (opt_tl_x != 0) {
        opt_tl_x->setValue(0);
    }
    if (opt_tl_y != 0) {
        opt_tl_y->setValue(0);
    }

    if (opt_br_x != 0) {
        opt_br_x->getMaxValue(&max);
        opt_br_x->setValue(max);
    }
    if (opt_br_y != 0) {
        opt_br_y->getMaxValue(&max);
        opt_br_y->setValue(max);
    }

    // set the resopution to 100 dpi and increase if necessary
    dpi = 0;
    do {
        // Increase the dpi value
        dpi += 100;
        if (opt_res != 0) {
            opt_res->setValue(dpi);
        }
        if (opt_res_y != 0) {
            opt_res_y->setValue(dpi);
        }
        //check what image size we would get in a scan
        status = sane_get_parameters(s_handle, &params);
        if (status != SANE_STATUS_GOOD) {
            printf("ERROR status=%s\n", sane_strstatus(status));
            return;
        }
        //printf("dpi = %d\n", dpi);
        //printf("lines = %d\n", params.lines);
        //printf("pixels_per_line = %d\n", params.pixels_per_line);
        if (dpi > 800) break;
    } while ((params.pixels_per_line < 300) || (params.lines < 300));

    // execute valReload if there is a pending value reload
    while (r_val_tmr.isActive()) {
        r_val_tmr.stop();
        valReload();
    }

    // Start the scanning
    status = sane_start(s_handle);
    if (status != SANE_STATUS_GOOD) {
        printf("sane_start ERROR: status=%s\n", sane_strstatus(status));
        sane_cancel(s_handle);
        return;
    }

    status = sane_get_parameters(s_handle, &params);
    if (status != SANE_STATUS_GOOD) {
        printf("sane_get_parameters ERROR: status=%s\n", sane_strstatus(status));
        sane_cancel(s_handle);
        return;
    }

    //printf("format = %d\n", params.format);
    //printf("last_frame = %d\n", params.last_frame);
    //printf("lines = %d\n", params.lines);
    //printf("depth = %d\n", params.depth);
    //printf("pixels_per_line = %d\n", params.pixels_per_line);
    //printf("bytes_per_line = %d\n", params.bytes_per_line);

    // create a new image if necessary
    //(This image should be small so who cares about waisted memory :)
    // FIXME optimize size
    scan_img = preview_->previewImage();
    preview_->updatePreviewSize(params.pixels_per_line, params.lines);

    read_status = READ_ON_GOING;
    pixel_x = 0;
    pixel_y = 0;
    px_c_index = 0;

    this->setDisabled(true);

    while (read_status == READ_ON_GOING) {
        processData();
    }

    preview_->updateScaledImg();

    // restore the original settings of the changed parameters
    if (opt_depth != 0) opt_depth->restoreSavedData();
    if (opt_res != 0) opt_res->restoreSavedData();
    if (opt_res_y != 0) opt_res_y->restoreSavedData();
    if (opt_tl_x != 0) opt_tl_x->restoreSavedData();
    if (opt_tl_y != 0) opt_tl_y->restoreSavedData();
    if (opt_br_x != 0) opt_br_x->restoreSavedData();
    if (opt_br_y != 0) opt_br_y->restoreSavedData();

    this->setDisabled(false);
}

//************************************************************
void SaneWidget::scanFinal(void)
{
    SANE_Status status;
    float v1,v2;

    //std::cout << "scanFinal" << std::endl;

    if ((opt_tl_x != 0) && (opt_br_x != 0)) {
        opt_tl_x->getValue(&v1);
        opt_br_x->getValue(&v2);
        if (v1 == v2) {
            opt_tl_x->setValue(0);
            opt_br_x->getMaxValue(&v2);
            opt_br_x->setValue(v2);
        }
    }

    if ((opt_tl_y != 0) && (opt_br_y != 0)) {
        opt_tl_y->getValue(&v1);
        opt_br_y->getValue(&v2);
        if (v1 == v2) {
            opt_tl_y->setValue(0);
            opt_br_y->getMaxValue(&v2);
            opt_br_y->setValue(v2);
        }
    }

    // execute a pending value reload
    while (r_val_tmr.isActive()) {
        r_val_tmr.stop();
        valReload();
    }

    // Start the scanning
    emit scanStart();
    status = sane_start(s_handle);
    if (status != SANE_STATUS_GOOD) {
        printf("sane_start ERROR: status=%s\n", sane_strstatus(status));
        sane_cancel(s_handle);
        return;
    }
    //printf("start OK\n");

    status = sane_get_parameters(s_handle, &params);
    if (status != SANE_STATUS_GOOD) {
        printf("sane_get_parameters ERROR: status=%s\n", sane_strstatus(status));
        sane_cancel(s_handle);
        return;
    }

    //printf("format = %d\n", params.format);
    //printf("last_frame = %d\n", params.last_frame);
    //printf("lines = %d\n", params.lines);
    //printf("depth = %d\n", params.depth);
    //printf("pixels_per_line = %d\n", params.pixels_per_line);
    //printf("bytes_per_line = %d\n", params.bytes_per_line);

    // create a new image
    //(This image should be small so who cares about waisted memory :)
    // FIXME optimize size
    scan_img = &the_img;
    *scan_img = QImage(params.pixels_per_line, params.lines, QImage::Format_RGB32);

    // Signal for a progress dialog
    emit scanProgress(0);

    read_status = READ_ON_GOING;
    pixel_x = 0;
    pixel_y = 0;
    px_c_index = 0;

    this->setDisabled(true);
    while (read_status == READ_ON_GOING) {
        processData();
    }
    if (read_status != READ_FINISHED) {
        emit scanFaild();
    }
    this->setDisabled(false);
}

//************************************************************
void SaneWidget::processData(void)
{
    SANE_Status status = SANE_STATUS_GOOD;
    SANE_Int read_bytes = 0;
    int i, j;

    //printf("Pre read()\n");
    status = sane_read(s_handle, img_data, IMG_DATA_R_SIZE, &read_bytes);
    //printf("Post read() read=%d\n", read_bytes);

    if (status == SANE_STATUS_EOF) {
        //printf("Read finished read_bytes=%d\n", read_bytes);
        if (pixel_y < params.lines) {
            printf("pixel_y(%d) < params.lines(%d)\n", pixel_y, params.lines);
            sleep(1);
            //sane_cancel(s_handle);
        }
        if (params.last_frame == SANE_TRUE) {
            // this is where it all ends well :)
            read_status = READ_FINISHED;
            if (scan_img == &the_img) {
                emit scanDone();
                emit imageReady();
            }
            return;
        }
        else {
            sane_start(s_handle);
            if (status != SANE_STATUS_GOOD) {
                printf("sane_start ERROR: status=%s\n", sane_strstatus(status));
                sane_cancel(s_handle);
                read_status = READ_ERROR;
                return;
            }
        }
    }
    else if (status != SANE_STATUS_GOOD) {
        printf("Reading error, status=%s\n", sane_strstatus(status));
        sane_cancel(s_handle);
        read_status = READ_ERROR;
        return;
    }

    switch (params.format)
    {
        case SANE_FRAME_RGB:
            if (params.depth == 8) {
                // go through the data
                for (i=0; i<read_bytes; i++) {
                    if (pixel_y >= params.lines) {
                        printf("processData: reached image height before EOF\n");
                        sane_cancel(s_handle);
                        read_status = READ_ERROR;
                        return;
                    }
                    px_colors[px_c_index] = img_data[i];
                    px_c_index++;
                    if (px_c_index >= 3) px_c_index = 0;

                    if (px_c_index == 0) {
                        scan_img->setPixel(pixel_x, pixel_y,
                                         qRgb(px_colors[0],
                                              px_colors[1],
                                              px_colors[2]));
                        pixel_x++;
                        if (pixel_x >= params.pixels_per_line) {
                            pixel_x = 0;
                            pixel_y++;
                        }
                    }
                }
            }
            else {
                printf("Only 8-bit colors are supported!\n");
                sane_cancel(s_handle);
                read_status = READ_ERROR;
                return;
            }
            break;

        case SANE_FRAME_GRAY:
            if (params.depth == 8) {
                for (i=0; i<read_bytes; i++) {
                    if (pixel_y >= params.lines) {
                        printf("reached image height before EOF\n");
                        sane_cancel(s_handle);
                        read_status = READ_ERROR;
                        return;
                    }
                    scan_img->setPixel(pixel_x, pixel_y,
                                     qRgb(img_data[i],
                                          img_data[i],
                                          img_data[i]));
                    pixel_x++;
                    if (pixel_x >= params.pixels_per_line) {
                        pixel_x = 0;
                        pixel_y++;
                    }
                }
            }
            else if (params.depth == 1){
                for (i=0; i<read_bytes; i++) {
                    if (pixel_y >= params.lines) {
                        printf("reached image height before EOF\n");
                        sane_cancel(s_handle);
                        read_status = READ_ERROR;
                        return;
                    }
                    for (j=7; j>=0; j--) {
                        if ((img_data[i] & (1<<j)) == 0) {
                            scan_img->setPixel(pixel_x, pixel_y, qRgb(255,255,255));
                        }
                        else {
                            scan_img->setPixel(pixel_x, pixel_y, qRgb(0,0,0));
                        }
                        pixel_x++;
                        if(pixel_x >= params.pixels_per_line) {
                            pixel_x = 0;
                            pixel_y++;
                            break;
                        }
                        if (pixel_y >= params.lines) break;
                    }
                }
            }
            else {
                printf("Only 1 and 8-bit colors are supported for grayscale!\n");
                sane_cancel(s_handle);
                read_status = READ_ERROR;
                return;
            }
            break;

        /*
        case SANE_FRAME_RED:
        case SANE_FRAME_GREEN:
        case SANE_FRAME_BLUE:
            for (i=0; i<read_bytes; i++) {
                printf("%d\n", pr_img_data[i]);
                if (pixel_y >= params.lines) {
                    printf("reached image height before EOF\n");
                    sane_cancel(s_handle);
                    read_status = READ_ERROR;
                    return;
                }

                color = pr_img->pixel(pixel_x, pixel_y);
                red   = qRed(color);
                green = qGreen(color);
                blue  = qBlue(color);

                switch(params.format)
                {
                    case SANE_FRAME_RED :
                        newColor = qRgb(pr_img_data[i], green, blue);
                        break;
                    case SANE_FRAME_GREEN :
                        newColor = qRgb(red, pr_img_data[i], blue);
                        break;
                    case SANE_FRAME_BLUE :
                        newColor = qRgb(red , green, pr_img_data[i]);
                        break;
                    default:
                        // we cannot come here but just in any case :)
                        newColor = qRgb(0,0,0);
                        break;
                }
                scan_img->setPixel(pixel_x, pixel_y, newColor);
                pixel_x++;
                if(pixel_x >= params.pixels_per_line) {
                    pixel_x = 0;
                    pixel_y++;
                    break;
                }
            }
            break;
        */
        default :
            printf("This frame format (%d) is not yet suppoeted!\n", params.format);
            sane_cancel(s_handle);
            read_status = READ_ERROR;
            return;
    }
    if (params.lines > 0) {
        int new_progress = (int)( ((double)PROGRESS_MAX / params.lines) * pixel_y);
        if (abs (new_progress - progress) > 5) {
            progress = new_progress;
            if (scan_img == preview_->previewImage()) {
                preview_->updateScaledImg();
            }
            if ((progress < PROGRESS_MAX) && (scan_img == &the_img)) {
                emit scanProgress(progress);
            }
            qApp->processEvents();
        }
    }
}

QStringList SaneWidget::scannerList() const
{
    QStringList res;

    SANE_Device const **dev_list;
    SANE_Status status = sane_get_devices(&dev_list, SANE_TRUE);
    if(status != SANE_STATUS_GOOD) {
        qWarning() << Q_FUNC_INFO << "can't get device list:" << sane_strstatus(status);
        return res;
    }

    int i = 0;
    while(dev_list[i] != 0) {
        qDebug() << Q_FUNC_INFO << QString("scanner: i=%1 name='%2' vendor='%3' model='%4' type='%5'").
                    arg(i).
                    arg(dev_list[i]->name).
                    arg(dev_list[i]->vendor).
                    arg(dev_list[i]->model).
                    arg(dev_list[i]->type);

        res << QString("%1\n%2 : %3").
                arg(dev_list[i]->name).
                arg(dev_list[i]->vendor).
                arg(dev_list[i]->model);
        i++;
    }

    return res;
}

QImage * SaneWidget::getFinalImage(void)
{
    return &the_img;
}

bool SaneWidget::setIcon(const QIcon& icon, IconType t)
{
    if(!preview_)
        return false;

    preview_->setIcon(icon, t);
    return true;
}

void SaneWidget::scanCancel(void)
{
    sane_cancel(s_handle);
    read_status = READ_CANCEL;
}

bool SaneWidget::setIconColorMode(const QIcon &icon)
{
    if ((opt_mode != 0) && (opt_mode->widget() != 0)) {
        opt_mode->setIconColorMode(icon);
        return true;
    }
    return false;
}

bool SaneWidget::setIconGrayMode(const QIcon &icon)
{
    if ((opt_mode != 0) && (opt_mode->widget() != 0)) {
        opt_mode->setIconGrayMode(icon);
        return true;
    }
    return false;
}

bool SaneWidget::setIconBWMode(const QIcon &icon)
{
    if ((opt_mode != 0) && (opt_mode->widget() != 0)) {
        opt_mode->setIconBWMode(icon);
        return true;
    }
    return false;
}

