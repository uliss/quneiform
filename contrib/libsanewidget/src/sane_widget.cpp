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
extern "C" {
#include <stdio.h>
#include <sys/ioctl.h>
#include <sane/saneopts.h>
}
#include <iostream>

//#include <qlayout.h>
#include <qeventloop.h>
#include <qapplication.h>
#include <QComboBox>

#include "sane_option.h"
#include "preview_area.h"
#include "sane_widget.h"
#include "sane_option.h"
#include "labeled_separator.h"
#include "radio_select.h"
#include "labeled_gamma.h"

#define ENABLE_DEBUG

//************************************************************
SaneWidget::SaneWidget(QWidget* parent)
: QWidget(parent)
{
    SANE_Int version;

    device=0;
    previewWidth = 0;
    previewHeight = 0;
    read_status = READ_NOT_READING;
    px_c_index = 0;
    opt_area = 0;
    opt_tl_x = 0;
    opt_tl_y = 0;
    opt_br_x = 0;
    opt_br_y = 0;
    opt_res = 0;
    opt_res_y = 0;
    opt_gam_r = 0;
    opt_gam_g = 0;
    opt_gam_b = 0;
    color_opts = 0;
    remain_opts = 0;
    pr_img = 0;
    the_img = QImage(10, 10, QImage::Format_RGB32);

    SANE_Status status;
    status = sane_init(&version, 0);
    if (status != SANE_STATUS_GOOD) {
        printf("SaneWidget: sane_init() failed(%s)\n", sane_strstatus(status));
    }
    else {
        printf("Sane Version = %d.%d.%d\n",
               SANE_VERSION_MAJOR(version),
               SANE_VERSION_MINOR(version),
               SANE_VERSION_BUILD(version));
    }
    r_val_tmr.setSingleShot(true);
    connect (&r_val_tmr, SIGNAL(timeout(void)), this, SLOT(valReload()));
}

//************************************************************
SaneWidget::~SaneWidget(void)
{
    optList.clear();
    sane_exit();
}

//************************************************************
QString SaneWidget::selectDevice(QWidget* parent)
{
    int i=0;
    int num_scaners;
    SANE_Status status;
    SANE_Device const **dev_list;
    QStringList dev_name_list;
    QString tmp;

    status = sane_get_devices(&dev_list, SANE_TRUE);

    while(dev_list[i] != 0) {
        printf("i=%d name='%s' vendor='%s' model='%s' type='%s'\n",
               i, dev_list[i]->name,  dev_list[i]->vendor,
               dev_list[i]->model, dev_list[i]->type);
        tmp = QString(dev_list[i]->name);
        tmp += "\n" + QString(dev_list[i]->vendor);
        tmp += " : " + QString(dev_list[i]->model);
        dev_name_list += tmp;
        i++;
    }
    num_scaners = i;

    // add the debug test scanner to the end
#ifdef ENABLE_DEBUG
    tmp = QString("test:0");
    tmp += "\n" + QString("Tester");
    tmp += " : " + QString("Model");
    dev_name_list += tmp;
#endif

    RadioSelect sel;
    sel.setWindowTitle(qApp->applicationName());
    i = sel.getSelectedIndex(parent, QString("Select Scanner"), dev_name_list, 0);
    printf("i=%d\n", i);

    if (i == num_scaners) {
        return QString("test:0");
    }

    if ((i < 0) || (i >= num_scaners)) {
        return QString("");
    }


    return QString(dev_list[i]->name);
}

//************************************************************
bool SaneWidget::openDevice(const QString &device_name)
{
    SANE_Status status;
    int i=0;
    const SANE_Option_Descriptor *num_option_d;
    SANE_Word num_sane_options;
    SANE_Int res;
    SANE_Device const **dev_list;

    // get the device list to get the vendor and model info
    status = sane_get_devices(&dev_list, SANE_TRUE);

    while(dev_list[i] != 0) {
        if (QString(dev_list[i]->name) == device_name) {
            modelname = QString(dev_list[i]->vendor) + " " + QString(dev_list[i]->model);
            break;
        }
        i++;
    }

    if (dev_list[i] == 0) {
#ifdef ENABLE_DEBUG
        modelname = QString("Test Scanner");
#else
        printf("openDevice: device '%s' not found\n", qPrintable(device_name));
        return false;
#endif
    }

    // Try to open the device
    if (sane_open(device_name.toLatin1(), &s_handle) != SANE_STATUS_GOOD) {
        printf("openDevice: sane_open(\"%s\", &handle) failed!\n", qPrintable(device_name));
        return false;
    }
    //printf("openDevice: sane_open(\"%s\", &handle) == SANE_STATUS_GOOD\n", qPrintable(device_name));

    devname = device_name;

    // Read the options (start with option 0 the number of parameters)
    num_option_d = sane_get_option_descriptor(s_handle, 0);
    if (num_option_d == 0) {
        return false;
    }
    char data[num_option_d->size];
    status = sane_control_option(s_handle, 0, SANE_ACTION_GET_VALUE, data, &res);
    if (status != SANE_STATUS_GOOD) {
        return false;
    }
    num_sane_options = *(SANE_Word*)data;

    // read the rest of the options
    for (i=1; i<num_sane_options; i++) {
        optList.append(new SaneOption(s_handle, i));
    }

    // do the connections of the option parameters
    for (i=1; i<optList.size(); i++) {
        connect (optList.at(i), SIGNAL(optsNeedReload()), this, SLOT(optReload()));
        connect (optList.at(i), SIGNAL(valsNeedReload()), this, SLOT(scheduleValReload()));
    }

    // create the layout
    QHBoxLayout *base_layout = new QHBoxLayout;
    base_layout->setSpacing(2);
    base_layout->setMargin(0);
    setLayout(base_layout);
    QVBoxLayout *opt_lay = new QVBoxLayout;
    opt_lay->setSpacing(2);
    opt_lay->setMargin(0);
    QVBoxLayout *pr_layout = new QVBoxLayout;

    base_layout->addLayout(opt_lay, 0);
    base_layout->addLayout(pr_layout, 100);

    // Create Option Scroll Area
    opt_area = new QScrollArea(this);
    opt_area->setWidgetResizable(true);
    opt_area->setFrameShape(QFrame::NoFrame);
    opt_lay->addWidget(opt_area, 0);
    opt_lay->setSpacing(2);
    opt_lay->setMargin(0);

    // Create the options interface
    createOptInterface();

    // create the preview
    preview = new PreviewArea(this);
    connect (preview, SIGNAL(newSelection(float,float,float,float)),
             this, SLOT(handleSelection(float,float,float,float)));
    pr_img = preview->getImage();


    z_in_btn = new QPushButton(tr("Zoom In"));
    z_out_btn = new QPushButton(tr("Zoom Out"));
    z_sel_btn = new QPushButton(tr("Zoom to Selection"));
    z_fit_btn = new QPushButton(tr("Zoom to Fit"));
    prev_btn = new QPushButton(tr("Preview"));;
    scan_btn = new QPushButton(tr("Final Scan"));

    connect(z_in_btn, SIGNAL(clicked(void)), preview, SLOT(zoomIn(void)));
    connect(z_out_btn, SIGNAL(clicked(void)), preview, SLOT(zoomOut(void)));
    connect(z_sel_btn, SIGNAL(clicked(void)), preview, SLOT(zoomSel(void)));
    connect(z_fit_btn, SIGNAL(clicked(void)), preview, SLOT(zoom2Fit(void)));
    connect (scan_btn, SIGNAL(clicked(void)), this, SLOT(scanFinal(void)));
    connect (prev_btn, SIGNAL(clicked(void)), this, SLOT(scanPreview(void)));


    QHBoxLayout *zoom_layout = new QHBoxLayout;

    pr_layout->addWidget(preview, 100);
    pr_layout->addLayout(zoom_layout, 0);


    zoom_layout->addWidget(z_in_btn);
    zoom_layout->addWidget(z_out_btn);
    zoom_layout->addWidget(z_sel_btn);
    zoom_layout->addWidget(z_fit_btn);
    zoom_layout->addStretch();
    zoom_layout->addWidget(prev_btn);
    zoom_layout->addWidget(scan_btn);

    //QHBoxLayout *scan_layout = new QHBoxLayout;
    //opt_lay->addLayout(scan_layout, 0);

    //scan_layout->addStretch();
    //scan_layout->addWidget(prev_btn);
    //scan_layout->addWidget(scan_btn);


    // try to set SaneWidget default values
    setDefaultValues();

    // estimate the preview size and create an empty image
    // this is done so that you can select scanarea without
    // having to scan a preview.
    updatePreviewSize();

    //preview->zoom2Fit();

    return true;
}

//************************************************************
void SaneWidget::createOptInterface(void)
{
    // create the container widget
    QWidget *opt_container = new QWidget(opt_area);
    opt_area->setWidget(opt_container);
    QVBoxLayout *opt_layout = new QVBoxLayout(opt_container);
    opt_layout->setSpacing(4);
    opt_layout->setMargin(3);

    // add the options
    // (Should Vendor and model always be visible?)
    LabeledSeparator *model_label = new LabeledSeparator(opt_container, modelname);
    opt_layout->addWidget(model_label);

    // basic/intermediate/All options
    QStringList strl;
    strl << "Basic" << "Advanced" << "All Options";
    LabeledCombo *opt_level = new LabeledCombo(opt_container, QString("Option Level"), strl);
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
                                  QString(SANE_TITLE_GAMMA_VECTOR),
                                  opt_gam_r->lgamma->size());
        color_lay->addWidget(lgamma);
        connect(lgamma, SIGNAL(gammaChanged(int,int,int)),
                opt_gam_r->lgamma, SLOT(setValues(int,int,int)));
        connect(lgamma, SIGNAL(gammaChanged(int,int,int)),
                opt_gam_g->lgamma, SLOT(setValues(int,int,int)));
        connect(lgamma, SIGNAL(gammaChanged(int,int,int)),
                opt_gam_b->lgamma, SLOT(setValues(int,int,int)));

        QCheckBox *split_gam_btn = new QCheckBox("Separate color intensity tables", opt_container);
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
    opt_area->setMinimumWidth(opt_container->sizeHint().width()+20);

    // this could/should be set by saved settings.
    color_opts->setVisible(false);
    remain_opts->setVisible(false);
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
        option->setValue(QString(SANE_VALUE_SCAN_MODE_COLOR));
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

//************************************************************
void SaneWidget::handleSelection(float tl_x, float tl_y, float br_x, float br_y) {
    float max_x, max_y;

    //printf("handleSelection0: %f %f %f %f\n", tl_x, tl_y, br_x, br_y);
    if ((pr_img->width()==0) || (pr_img->height()==0)) return;

    opt_br_x->getMaxValue(&max_x);
    opt_br_y->getMaxValue(&max_y);
    float ftl_x = tl_x*max_x;
    float ftl_y = tl_y*max_y;
    float fbr_x = br_x*max_x;
    float fbr_y = br_y*max_y;

    //printf("handleSelection1: %f %f %f %f\n", ftl_x, ftl_y, fbr_x, fbr_y);

    if (opt_tl_x != 0) opt_tl_x->setValue(ftl_x);
    if (opt_tl_y != 0) opt_tl_y->setValue(ftl_y);
    if (opt_br_x != 0) opt_br_x->setValue(fbr_x);
    if (opt_br_y != 0) opt_br_y->setValue(fbr_y);
}

//************************************************************
void SaneWidget::setTLX(float ftlx) {
    float max, ratio;

    //std::cout << "setTLX " << ftlx;
    opt_br_x->getMaxValue(&max);
    ratio = ftlx / max;
    //std::cout << " -> " << ratio << std::endl;
    preview->setTLX(ratio);
}

//************************************************************
void SaneWidget::setTLY(float ftly) {
    float max, ratio;

    //std::cout << "setTLY " << ftly;
    opt_br_y->getMaxValue(&max);
    ratio = ftly / max;
    //std::cout << " -> " << ratio << std::endl;
    preview->setTLY(ratio);
}

//************************************************************
void SaneWidget::setBRX(float fbrx) {
    float max, ratio;

    //std::cout << "setBRX " << fbrx;
    opt_br_x->getMaxValue(&max);
    ratio = fbrx / max;
    //std::cout << " -> " << ratio << std::endl;
    preview->setBRX(ratio);
}

//************************************************************
void SaneWidget::setBRY(float fbry) {
    float max, ratio;

    //std::cout << "setBRY " << fbry;
    opt_br_y->getMaxValue(&max);
    ratio = fbry / max;
    //std::cout << " -> " << ratio << std::endl;
    preview->setBRY(ratio);
}

//************************************************************
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


    if ((pr_img->width() != params.pixels_per_line) || (pr_img->height() != params.lines)) {
        *pr_img = QImage(params.pixels_per_line, params.lines, QImage::Format_RGB32);
        for (i=0; i<pr_img->height(); i++) {
            for (j=0; j<pr_img->width(); j++) {
                pr_img->setPixel(j, i, qRgb(255,255,255));
            }
        }

        // clear the selection
        preview->clearSelection();

        // update the size of the preview widget.
        preview->updateScaledImg();
        //preview->resize(preview->sizeHint());
    }
}

//************************************************************
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
    scan_img = pr_img;
    if ((pr_img->height() != params.lines) ||
         (pr_img->width() != params.pixels_per_line))
    {
        *pr_img = QImage(params.pixels_per_line, params.lines, QImage::Format_RGB32);
    }

    // clear the old image
    for (i=0; i<pr_img->height(); i++) {
        for (j=0; j<pr_img->width(); j++) {
            pr_img->setPixel(j, i, qRgb(255,255,255));
        }
    }

    // update the size of the preview widget.
    preview->zoom2Fit();

    read_status = READ_ON_GOING;
    pixel_x = 0;
    pixel_y = 0;
    px_c_index = 0;

    this->setDisabled(true);

    while (read_status == READ_ON_GOING) {
        processData();
    }

    preview->updateScaledImg();

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
            if (scan_img == pr_img) {
                preview->updateScaledImg();
            }
            if ((progress < PROGRESS_MAX) && (scan_img == &the_img)) {
                emit scanProgress(progress);
            }
            qApp->processEvents();
        }
    }
}

//************************************************************
QImage *SaneWidget::getFinalImage(void)
{
    return &the_img;
}

//************************************************************
void SaneWidget::scanCancel(void)
{
    sane_cancel(s_handle);
    read_status = READ_CANCEL;
}


//************************************************************
bool SaneWidget::setIconColorMode(const QIcon &icon)
{
    if ((opt_mode != 0) && (opt_mode->widget() != 0)) {
        opt_mode->setIconColorMode(icon);
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconGrayMode(const QIcon &icon)
{
    if ((opt_mode != 0) && (opt_mode->widget() != 0)) {
        opt_mode->setIconGrayMode(icon);
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconBWMode(const QIcon &icon)
{
    if ((opt_mode != 0) && (opt_mode->widget() != 0)) {
        opt_mode->setIconBWMode(icon);
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconPreview(const QIcon &icon)
{
    if (prev_btn != 0) {
        prev_btn->setIcon(icon);
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconFinal(const QIcon &icon)
{
    if (scan_btn != 0) {
        scan_btn->setIcon(icon);
        return true;
    }
    return true;
}

//************************************************************
bool SaneWidget::setIconZoomIn(const QIcon &icon)
{
    if (preview != 0) {
        preview->setIconZoomIn(icon);
        z_in_btn->setIcon(icon);
        z_in_btn->setToolTip(z_in_btn->text());
        z_in_btn->setText("");
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconZoomOut(const QIcon &icon)
{
    if (preview != 0) {
        z_out_btn->setIcon(icon);
        z_out_btn->setToolTip(z_out_btn->text());
        z_out_btn->setText("");
        preview->setIconZoomOut(icon);
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconZoomSel(const QIcon &icon)
{
    if (preview != 0) {
        z_sel_btn->setIcon(icon);
        z_sel_btn->setToolTip(z_sel_btn->text());
        z_sel_btn->setText("");
        preview->setIconZoomSel(icon);
        return true;
    }
    return false;
}

//************************************************************
bool SaneWidget::setIconZoomFit(const QIcon &icon)
{
    if (preview != 0) {
        z_fit_btn->setIcon(icon);
        z_fit_btn->setToolTip(z_fit_btn->text());
        z_fit_btn->setText("");
        preview->setIconZoomFit(icon);
        return true;
    }
    return false;
}

