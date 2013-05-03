/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2007-09-13
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2007-2008 by Kare Sars <kare dot sars at iki dot fi>
 * Copyright (C) 2007-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================ */

// Local includes.
#include "ksane_widget_private.h"
#include "i18n.h"

// Qt includes.
#include <QImage>
#include <QScrollArea>
#include <QScrollBar>
#include <QList>
#include <QLabel>

// KDE includes
#include <QPushButton>

#define SCALED_PREVIEW_MAX_SIDE 400

static const int ActiveSelection = 100000;

namespace KSaneIface
{

KSaneWidgetPrivate::KSaneWidgetPrivate(KSaneWidget *parent):
q(parent)
{
    // Device independent UI variables
    m_optsTabWidget = 0;
    m_basicOptsTab  = 0;
    m_otherOptsTab  = 0;
    m_zInBtn        = 0;
    m_zOutBtn       = 0;
    m_zSelBtn       = 0;
    m_zFitBtn       = 0;
    m_prevBtn       = 0;
    m_scanBtn       = 0;
    m_cancelBtn     = 0;
    m_previewViewer = 0;
    m_autoSelect    = true;
    m_selIndex      = ActiveSelection;
    m_warmingUp     = 0;
    m_progressBar   = 0;

    // scanning variables
    m_isPreview     = false;

    m_saneHandle    = 0;
    m_previewThread = 0;
    m_scanThread    = 0;

    m_splitGamChB   = 0;
    m_commonGamma   = 0;
    m_previewDPI    = 0;
    m_invertColors  = 0;
    
    m_previewWidth  = 0;
    m_previewHeight = 0;
    
    clearDeviceOptions();
    
    m_findDevThread = FindSaneDevicesThread::getInstance();
    connect(m_findDevThread, SIGNAL(finished()), this, SLOT(devListUpdated()));
    connect(m_findDevThread, SIGNAL(finished()), this, SLOT(signalDevListUpdate()));
    
    m_auth = KSaneAuth::getInstance();
    m_optionPollTmr.setInterval(100);
    connect(&m_optionPollTmr, SIGNAL(timeout()), this, SLOT(pollPollOptions()));
}

void KSaneWidgetPrivate::clearDeviceOptions()
{
    m_optSource     = 0;
    m_colorOpts     = 0;
    m_optNegative   = 0;
    m_optFilmType   = 0;
    m_optMode       = 0;
    m_optDepth      = 0;
    m_optRes        = 0;
    m_optResX       = 0;
    m_optResY       = 0;
    m_optTlX        = 0;
    m_optTlY        = 0;
    m_optBrX        = 0;
    m_optBrY        = 0;
    m_optGamR       = 0;
    m_optGamG       = 0;
    m_optGamB       = 0;
    m_optPreview    = 0;
    m_optWaitForBtn = 0;
    m_scanOngoing   = false;
    m_closeDevicePending = false;

    // delete all the options in the list.
    while (!m_optList.isEmpty()) {
        delete m_optList.takeFirst();
    }
    m_pollList.clear();
    m_optionPollTmr.stop();

    // remove the remaining layouts/widgets and read thread
    delete m_basicOptsTab;
    m_basicOptsTab = 0;

    delete m_otherOptsTab;
    m_otherOptsTab = 0;

    delete m_previewThread;
    m_previewThread = 0;

    delete m_scanThread;
    m_scanThread = 0;

    m_devName.clear();
}

void KSaneWidgetPrivate::devListUpdated()
{
    if (m_vendor.isEmpty()) {
        const QList<KSaneWidget::DeviceInfo> list = m_findDevThread->devicesList();
        if (list.size() == 0) return;
        for (int i=0; i<list.size(); i++) {
            qDebug() << list[i].name;
            if (list[i].name == m_devName) {
                m_vendor    = list[i].vendor;
                m_model     = list[i].model;
                break;
            }
        }
    }
}

void KSaneWidgetPrivate::signalDevListUpdate()
{
    emit q->availableDevices(m_findDevThread->devicesList());
}

KSaneWidget::ImageFormat KSaneWidgetPrivate::getImgFormat(SANE_Parameters &params)
{
    switch(params.format)
    {
        case SANE_FRAME_GRAY:
            switch(params.depth)
            {
                case 1:
                    return KSaneWidget::FormatBlackWhite;
                case 8:
                    return KSaneWidget::FormatGrayScale8;
                case 16:
                    return KSaneWidget::FormatGrayScale16;
                default:
                    return KSaneWidget::FormatNone;
            }
        case SANE_FRAME_RGB:
        case SANE_FRAME_RED:
        case SANE_FRAME_GREEN:
        case SANE_FRAME_BLUE:
            switch (params.depth)
            {
                case 8:
                    return KSaneWidget::FormatRGB_8_C;
                case 16:
                    return KSaneWidget::FormatRGB_16_C;
                default:
                    return KSaneWidget::FormatNone;
            }
}
    return KSaneWidget::FormatNone;
}

int KSaneWidgetPrivate::getBytesPerLines(SANE_Parameters &params)
{
    switch (getImgFormat(params))
    {
        case KSaneWidget::FormatBlackWhite:
        case KSaneWidget::FormatGrayScale8:
        case KSaneWidget::FormatGrayScale16:
            return params.bytes_per_line;

        case KSaneWidget::FormatRGB_8_C:
            return params.pixels_per_line*3;

        case KSaneWidget::FormatRGB_16_C:
            return params.pixels_per_line*6;

        case KSaneWidget::FormatNone:
        case KSaneWidget::FormatBMP: // to remove warning (BMP is omly valid in the twain wrapper)
            return 0;
    }
    return 0;
}


KSaneOption *KSaneWidgetPrivate::getOption(const QString &name)
{
    int i;
    for (i=0; i<m_optList.size(); i++) {
        if (m_optList.at(i)->name() == name) {
            return m_optList.at(i);
        }
    }
    return 0;
}

void KSaneWidgetPrivate::createOptInterface()
{
    m_basicOptsTab = new QWidget;
    m_basicScrollA->setWidget(m_basicOptsTab);

    QVBoxLayout *basic_layout = new QVBoxLayout(m_basicOptsTab);
    KSaneOption *option;
    // Scan Source
    if ((option = getOption(SANE_NAME_SCAN_SOURCE)) != 0) {
        m_optSource = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
        connect(m_optSource, SIGNAL(valueChanged()), this, SLOT(checkInvert()), Qt::QueuedConnection);
    }
    // film-type (note: No translation)
    if ((option = getOption(QString("film-type"))) != 0) {
        m_optFilmType = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
        connect(m_optFilmType, SIGNAL(valueChanged()), this, SLOT(checkInvert()), Qt::QueuedConnection);
    }
    else if ((option = getOption(SANE_NAME_NEGATIVE)) != 0) {
        m_optNegative = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }
    // Scan mode
    if ((option = getOption(SANE_NAME_SCAN_MODE)) != 0) {
        m_optMode = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }
    // Bitdepth
    if ((option = getOption(SANE_NAME_BIT_DEPTH)) != 0) {
        m_optDepth = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }
    // Threshold
    if ((option = getOption(SANE_NAME_THRESHOLD)) != 0) {
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }
    // Resolution
    if ((option = getOption(SANE_NAME_SCAN_RESOLUTION)) != 0) {
        m_optRes = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }
    // These two next resolution options are a bit tricky.
    if ((option = getOption(SANE_NAME_SCAN_X_RESOLUTION)) != 0) {
        m_optResX = option;
        if (!m_optRes) m_optRes = m_optResX;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_SCAN_Y_RESOLUTION)) != 0) {
        m_optResY = option;
        option->createWidget(m_basicOptsTab);
        basic_layout->addWidget(option->widget());
    }

    // save a pointer to the preview option if possible
    if ((option = getOption(SANE_NAME_PREVIEW)) != 0) {
        m_optPreview = option;
    }

    // save a pointer to the "wait-for-button" option if possible (Note: No translation)
    if ((option = getOption("wait-for-button")) != 0) {
        m_optWaitForBtn = option;
    }

    // scan area (Do not add the widgets)
    if ((option = getOption(SANE_NAME_SCAN_TL_X)) != 0) {
        m_optTlX = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setTLX(float)));
    }
    if ((option = getOption(SANE_NAME_SCAN_TL_Y)) != 0) {
        m_optTlY = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setTLY(float)));
    }
    if ((option = getOption(SANE_NAME_SCAN_BR_X)) != 0) {
        m_optBrX = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setBRX(float)));
    }
    if ((option = getOption(SANE_NAME_SCAN_BR_Y)) != 0) {
        m_optBrY = option;
        connect (option, SIGNAL(fValueRead(float)), this, SLOT(setBRY(float)));
    }

    // Color Options Frame
    m_colorOpts = new QWidget(m_basicOptsTab);
    basic_layout->addWidget(m_colorOpts);
    QVBoxLayout *color_lay = new QVBoxLayout(m_colorOpts);
    color_lay->setContentsMargins(0,0,0,0);
    
    // Add Color correction to the color "frame"
    if ((option = getOption(SANE_NAME_BRIGHTNESS)) != 0) {
        option->createWidget(m_colorOpts);
        color_lay->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_CONTRAST)) != 0) {
        option->createWidget(m_colorOpts);
        color_lay->addWidget(option->widget());
    }
    
    // Add gamma tables to the color "frame"
    QWidget *gamma_frm = new QWidget(m_colorOpts);
    color_lay->addWidget(gamma_frm);
    QVBoxLayout *gam_frm_l = new QVBoxLayout(gamma_frm);
    gam_frm_l->setContentsMargins(0,0,0,0);

    if ((option = getOption(SANE_NAME_GAMMA_VECTOR_R)) != 0) {
        m_optGamR = option;
        option->createWidget(gamma_frm);
        gam_frm_l->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_GAMMA_VECTOR_G)) != 0) {
        m_optGamG = option;
        option->createWidget(gamma_frm);
        gam_frm_l->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_GAMMA_VECTOR_B)) != 0) {
        m_optGamB = option;
        option->createWidget(gamma_frm);
        gam_frm_l->addWidget(option->widget());
    }

    if ((m_optGamR != 0) && (m_optGamG != 0) && (m_optGamB != 0)) {
        LabeledGamma *gamma = reinterpret_cast<LabeledGamma *>(m_optGamR->widget());
        m_commonGamma = new LabeledGamma(m_colorOpts, i18n(SANE_TITLE_GAMMA_VECTOR), gamma->size());
        
        color_lay->addWidget(m_commonGamma);
        
        m_commonGamma->setToolTip(i18n(SANE_DESC_GAMMA_VECTOR));
        
        connect(m_commonGamma, SIGNAL(gammaChanged(int,int,int)), m_optGamR->widget(), SLOT(setValues(int,int,int)));
        connect(m_commonGamma, SIGNAL(gammaChanged(int,int,int)), m_optGamG->widget(), SLOT(setValues(int,int,int)));
        connect(m_commonGamma, SIGNAL(gammaChanged(int,int,int)), m_optGamB->widget(), SLOT(setValues(int,int,int)));

        m_splitGamChB = new LabeledCheckbox(m_colorOpts, i18n("Separate color intensity tables"));
        color_lay->addWidget(m_splitGamChB);
        
        connect (m_splitGamChB, SIGNAL(toggled(bool)), gamma_frm, SLOT(setVisible(bool)));
        connect (m_splitGamChB, SIGNAL(toggled(bool)), m_commonGamma, SLOT(setHidden(bool)));
        
        gamma_frm->hide();
    }
    
    if ((option = getOption(SANE_NAME_BLACK_LEVEL)) != 0) {
        option->createWidget(m_colorOpts);
        color_lay->addWidget(option->widget());
    }
    if ((option = getOption(SANE_NAME_WHITE_LEVEL)) != 0) {
        option->createWidget(m_colorOpts);
        color_lay->addWidget(option->widget());
    }
    
    m_invertColors = new LabeledCheckbox(m_colorOpts, i18n("Invert colors"));
    color_lay->addWidget(m_invertColors);
    m_invertColors->setChecked(false);
    connect(m_invertColors, SIGNAL(toggled(bool)), this, SLOT(invertPreview()));
    
    // add a stretch to the end to keep the parameters at the top
    basic_layout->addStretch();

    
    // Remaining (un known) options go to the "Other Options"
    m_otherOptsTab = new QWidget;
    m_otherScrollA->setWidget(m_otherOptsTab);
    
    QVBoxLayout *other_layout = new QVBoxLayout(m_otherOptsTab);
    
    // add the remaining parameters
    for (int i=0; i<m_optList.size(); i++) {
        if ((m_optList.at(i)->widget() == 0) &&
            (m_optList.at(i)->name() != SANE_NAME_SCAN_TL_X) &&
            (m_optList.at(i)->name() != SANE_NAME_SCAN_TL_Y) &&
            (m_optList.at(i)->name() != SANE_NAME_SCAN_BR_X) &&
            (m_optList.at(i)->name() != SANE_NAME_SCAN_BR_Y) &&
            (m_optList.at(i)->name() != SANE_NAME_PREVIEW) &&
            (m_optList.at(i)->hasGui()))
        {
            m_optList.at(i)->createWidget(m_otherOptsTab);
            other_layout->addWidget(m_optList.at(i)->widget());
        }
    }
    
    // add a stretch to the end to keep the parameters at the top
    other_layout->addStretch();
    
    // calculate label widths
    int labelWidth = 0;
    KSaneOptionWidget *tmpOption;
    // Basic Options
    for (int i=0; i<basic_layout->count(); i++) {
        if (basic_layout->itemAt(i) && basic_layout->itemAt(i)->widget()) {
            tmpOption = qobject_cast<KSaneOptionWidget *>(basic_layout->itemAt(i)->widget());
            if (tmpOption) {
                labelWidth = qMax(labelWidth, tmpOption->labelWidthHint());
            }
        }
    }
    // Color Options
    for (int i=0; i<basic_layout->count(); i++) {
        if (basic_layout->itemAt(i) && basic_layout->itemAt(i)->widget()) {
            tmpOption = qobject_cast<KSaneOptionWidget *>(basic_layout->itemAt(i)->widget());
            if (tmpOption) {
                labelWidth = qMax(labelWidth, tmpOption->labelWidthHint());
            }
        }
    }
    // Set label widths
    for (int i=0; i<basic_layout->count(); i++) {
        if (basic_layout->itemAt(i) && basic_layout->itemAt(i)->widget()) {
            tmpOption = qobject_cast<KSaneOptionWidget *>(basic_layout->itemAt(i)->widget());
            if (tmpOption) {
                tmpOption->setLabelWidth(labelWidth);
            }
        }
    }
    for (int i=0; i<color_lay->count(); i++) {
        if (color_lay->itemAt(i) && color_lay->itemAt(i)->widget()) {
            tmpOption = qobject_cast<KSaneOptionWidget *>(color_lay->itemAt(i)->widget());
            if (tmpOption) {
                tmpOption->setLabelWidth(labelWidth);
            }
        }
    }
    // Other Options
    labelWidth=0;
    for (int i=0; i<other_layout->count(); i++) {
        if (other_layout->itemAt(i) && other_layout->itemAt(i)->widget()) {
            tmpOption = qobject_cast<KSaneOptionWidget *>(other_layout->itemAt(i)->widget());
            if (tmpOption) {
                labelWidth = qMax(labelWidth, tmpOption->labelWidthHint());
            }
        }
    }
    for (int i=0; i<other_layout->count(); i++) {
        if (other_layout->itemAt(i) && other_layout->itemAt(i)->widget()) {
            tmpOption = qobject_cast<KSaneOptionWidget *>(other_layout->itemAt(i)->widget());
            if (tmpOption) {
                tmpOption->setLabelWidth(labelWidth);
            }
        }
    }
    
    // encsure that we do not get a scrollbar at the bottom of the option of the options
    int min_width = m_basicOptsTab->sizeHint().width();
    if (min_width < m_otherOptsTab->sizeHint().width()) {
        min_width = m_otherOptsTab->sizeHint().width();
    }

    m_optsTabWidget->setMinimumWidth(min_width + m_basicScrollA->verticalScrollBar()->sizeHint().width() + 5);
}

void KSaneWidgetPrivate::setDefaultValues()
{
    KSaneOption *option;

    // Try to get Color mode by default
    if ((option = getOption(SANE_NAME_SCAN_MODE)) != 0) {
        option->setValue(i18n(SANE_VALUE_SCAN_MODE_COLOR));
    }

    // Try to set 8 bit color
    if ((option = getOption(SANE_NAME_BIT_DEPTH)) != 0) {
        option->setValue(8);
    }

    // Try to set Scan resolution to 600 DPI
    if (m_optRes != 0) {
        m_optRes->setValue(600);
    }
}

void KSaneWidgetPrivate::scheduleValReload()
{
    m_readValsTmr.start(5);
}

void KSaneWidgetPrivate::optReload()
{
    int i;

    for (i=0; i<m_optList.size(); i++) {
        m_optList.at(i)->readOption();
        // Also read the values
        m_optList.at(i)->readValue();
    }
    // Gamma table special case
    if (m_optGamR && m_optGamG && m_optGamB) {
        m_commonGamma->setHidden(m_optGamR->state() == KSaneOption::STATE_HIDDEN);
        m_splitGamChB->setHidden(m_optGamR->state() == KSaneOption::STATE_HIDDEN);
    }

    // estimate the preview size and create an empty image
    // this is done so that you can select scan area without
    // having to scan a preview.
    updatePreviewSize();
    
    // ensure that we do not get a scrollbar at the bottom of the option of the options
    int min_width = m_basicOptsTab->sizeHint().width();
    if (min_width < m_otherOptsTab->sizeHint().width()) {
        min_width = m_otherOptsTab->sizeHint().width();
    }
    
    m_optsTabWidget->setMinimumWidth(min_width + m_basicScrollA->verticalScrollBar()->sizeHint().width() + 5);
    
    m_previewViewer->zoom2Fit();
}

void KSaneWidgetPrivate::valReload()
{
    int i;
    QString tmp;
    
    for (i=0; i<m_optList.size(); i++) {
        m_optList.at(i)->readValue();
    }
    
}

void KSaneWidgetPrivate::handleSelection(float tl_x, float tl_y, float br_x, float br_y) {
    
    if ((m_optTlX == 0) || (m_optTlY == 0) || (m_optBrX == 0) || (m_optBrY == 0)) {
        // clear the selection since we can not set one
        m_previewViewer->setTLX(0);
        m_previewViewer->setTLY(0);
        m_previewViewer->setBRX(0);
        m_previewViewer->setBRY(0);
        return;
    }
    float max_x, max_y;
    
    if ((m_previewImg.width()==0) || (m_previewImg.height()==0)) return;
    
    m_optBrX->getMaxValue(max_x);
    m_optBrY->getMaxValue(max_y);
    float ftl_x = tl_x*max_x;
    float ftl_y = tl_y*max_y;
    float fbr_x = br_x*max_x;
    float fbr_y = br_y*max_y;
    
    m_optTlX->setValue(ftl_x);
    m_optTlY->setValue(ftl_y);
    m_optBrX->setValue(fbr_x);
    m_optBrY->setValue(fbr_y);
}

void KSaneWidgetPrivate::setTLX(float ftlx)
{
    // ignore this during an active scan
    if (m_previewThread->isRunning()) return;
    if (m_scanThread->isRunning()) return;
    if (m_scanOngoing) return;
    
    float max, ratio;
    
    //kDebug() << "setTLX " << ftlx;
    m_optBrX->getMaxValue(max);
    ratio = ftlx / max;
    //kDebug() << " -> " << ratio;
    m_previewViewer->setTLX(ratio);
}

void KSaneWidgetPrivate::setTLY(float ftly)
{
    // ignore this during an active scan
    if (m_previewThread->isRunning()) return;
    if (m_scanThread->isRunning()) return;
    if (m_scanOngoing) return;
    
    float max, ratio;
    
    //kDebug() << "setTLY " << ftly;
    m_optBrY->getMaxValue(max);
    ratio = ftly / max;
    //kDebug() << " -> " << ratio;
    m_previewViewer->setTLY(ratio);
}

void KSaneWidgetPrivate::setBRX(float fbrx)
{
    // ignore this during an active scan
    if (m_previewThread->isRunning()) return;
    if (m_scanThread->isRunning()) return;
    if (m_scanOngoing) return;
    
    float max, ratio;
    
    //kDebug() << "setBRX " << fbrx;
    m_optBrX->getMaxValue(max);
    ratio = fbrx / max;
    //kDebug() << " -> " << ratio;
    m_previewViewer->setBRX(ratio);
}

void KSaneWidgetPrivate::setBRY(float fbry)
{
    // ignore this during an active scan
    if (m_previewThread->isRunning()) return;
    if (m_scanThread->isRunning()) return;
    if (m_scanOngoing) return;
    
    float max, ratio;
    
    //kDebug() << "setBRY " << fbry;
    m_optBrY->getMaxValue(max);
    ratio = fbry / max;
    //kDebug() << " -> " << ratio;
    m_previewViewer->setBRY(ratio);
}

void KSaneWidgetPrivate::updatePreviewSize()
{
    float max_x=0, max_y=0;
    float ratio;
    int x,y;
    
    // check if an update is necessary
    if (m_optBrX != 0) {
        m_optBrX->getMaxValue(max_x);
    }
    if (m_optBrY != 0) {
        m_optBrY->getMaxValue(max_y);
    }
    if ((max_x == m_previewWidth) && (max_y == m_previewHeight)) {
        //kDebug() << "no preview size change";
        return;
    }
    
    // The preview size has changed 
    m_previewWidth  = max_x;
    m_previewHeight = max_y;

    // set the scan area to the whole area
    m_previewViewer->clearSelections();
    if (m_optTlX != 0) {
        m_optTlX->setValue(0);
    }
    if (m_optTlY != 0) {
        m_optTlY->setValue(0);
    }
    
    if (m_optBrX != 0) {
        m_optBrX->setValue(max_x);
    }
    if (m_optBrY != 0) {
        m_optBrY->setValue(max_y);
    }
    
    // create a "scaled" image of the preview
    ratio = max_x/max_y;
    if (ratio < 1) {
        x=SCALED_PREVIEW_MAX_SIDE;
        y=(int)(SCALED_PREVIEW_MAX_SIDE/ratio);
    }
    else {
        y=SCALED_PREVIEW_MAX_SIDE;
        x=(int)(SCALED_PREVIEW_MAX_SIDE/ratio);
    }
    
    m_previewImg = QImage(x, y, QImage::Format_RGB32);
    m_previewImg.fill(0xFFFFFFFF);
    
    // set the new image
    m_previewViewer->setQImage(&m_previewImg);
}

void KSaneWidgetPrivate::startPreviewScan()
{
    if (m_scanOngoing) return;
    m_scanOngoing = true;
    
    SANE_Status status;
    float max_x, max_y;
    float dpi;
        
    // store the current settings of parameters to be changed
    if (m_optDepth != 0) m_optDepth->storeCurrentData();
    if (m_optRes != 0) m_optRes->storeCurrentData();
    if (m_optResX != 0) m_optResX->storeCurrentData();
    if (m_optResY != 0) m_optResY->storeCurrentData();
    if (m_optPreview != 0) m_optPreview->storeCurrentData();
    
    // check if we can modify the selection
    if ((m_optTlX != 0) && (m_optTlY != 0) &&
        (m_optBrX != 0) && (m_optBrY != 0))
    {
        // get maximums
        m_optBrX->getMaxValue(max_x);
        m_optBrY->getMaxValue(max_y);
        // select the whole area
        m_optTlX->setValue(0);
        m_optTlY->setValue(0);
        m_optBrX->setValue(max_x);
        m_optBrY->setValue(max_y);
        
    }
    else {
        // no use to try auto selections if you can not use them
        m_autoSelect = false;
    }
    
    if (m_optRes != 0) {
        if (m_previewDPI >= 25.0) {
            m_optRes->setValue(m_previewDPI);
            if ((m_optResY != 0) && (m_optRes->name() == SANE_NAME_SCAN_X_RESOLUTION)) {
                m_optResY->setValue(m_previewDPI);
            }
        }
        else {
            // set the resopution to getMinValue and increase if necessary
            SANE_Parameters params;
            m_optRes->getMinValue(dpi);
            do {
                m_optRes->setValue(dpi);
                if ((m_optResY != 0) && (m_optRes->name() == SANE_NAME_SCAN_X_RESOLUTION)) {
                    m_optResY->setValue(dpi);
                }
                //check what image size we would get in a scan
                status = sane_get_parameters(m_saneHandle, &params);
                if (status != SANE_STATUS_GOOD) {
                    qDebug() << "sane_get_parameters=" << sane_strstatus(status);
                    previewScanDone();
                    return;
                }
                
                if (dpi > 600) break;
            
                // Increase the dpi value
                dpi += 25.0;
            }
            while ((params.pixels_per_line < 300) || ((params.lines > 0) && (params.lines < 300)));
            
            if (params.pixels_per_line == 0) {
                // This is a security measure for broken backends
                m_optRes->getMinValue(dpi);
                m_optRes->setValue(dpi);
                qDebug() << "Setting minimum DPI value for a broken back-end";
            }
        }
    }
    
    // set preview option to true if possible
    if (m_optPreview != 0) m_optPreview->setValue(SANE_TRUE);
    
    // execute valReload if there is a pending value reload
    while (m_readValsTmr.isActive()) {
        m_readValsTmr.stop();
        valReload();
    }
    
    // clear the preview
    m_previewViewer->clearHighlight();
    m_previewViewer->clearSelections();
    m_previewImg.fill(0xFFFFFFFF);
    updatePreviewSize();
    
    setBusy(true);
    
    m_progressBar->setValue(0);
    m_isPreview = true;
    m_previewThread->setPreviewInverted(m_invertColors->isChecked());
    m_previewThread->start();
    m_updProgressTmr.start();
}

void KSaneWidgetPrivate::previewScanDone()
{
    // even if the scan is finished successfully we need to call sane_cancel()
    sane_cancel(m_saneHandle);
    
    if (m_closeDevicePending) {
        setBusy(false);
        sane_close(m_saneHandle);
        m_saneHandle = 0;
        clearDeviceOptions();
        emit q->scanDone(KSaneWidget::NoError, "");
        return;
    }

    // restore the original settings of the changed parameters
    if (m_optDepth != 0) m_optDepth->restoreSavedData();
    if (m_optRes != 0) m_optRes->restoreSavedData();
    if (m_optResX != 0) m_optResX->restoreSavedData();
    if (m_optResY != 0) m_optResY->restoreSavedData();
    if (m_optPreview != 0) m_optPreview->restoreSavedData();

    if ((m_previewThread->status != SANE_STATUS_GOOD) &&
        (m_previewThread->status != SANE_STATUS_EOF))
    {
        alertUser(KSaneWidget::ErrorGeneral, i18n(sane_strstatus(m_previewThread->status)));
    }
    else if (m_autoSelect) {
        m_previewViewer->findSelections();
    }
    
    setBusy(false);
    m_scanOngoing = false;
    m_updProgressTmr.stop();
    
    m_previewViewer->updateImage();
    
    emit q->scanDone(KSaneWidget::NoError, "");
    
    return;
}


void KSaneWidgetPrivate::startFinalScan()
{
    if (m_scanOngoing) return;
    m_scanOngoing = true;
    m_isPreview = false;
    
    float x1=0,y1=0, x2=0,y2=0, max_x, max_y;

    m_selIndex = 0;
    
    if ((m_optTlX != 0) && (m_optTlY != 0) &&  (m_optBrX != 0) && (m_optBrY != 0)) {
        // get maximums
        m_optBrX->getMaxValue(max_x);
        m_optBrY->getMaxValue(max_y);

        // reead the selection from the viewer
        m_previewViewer->selectionAt(m_selIndex, x1,y1,x2,y2);
        m_previewViewer->setHighlightArea(x1,y1,x2,y2);
        m_selIndex++;

        // calculate the option values
        x1 *= max_x; y1 *= max_y;
        x2 *= max_x; y2 *= max_y;

        // now set the selection
        m_optTlX->setValue(x1);
        m_optTlY->setValue(y1);
        m_optBrX->setValue(x2);
        m_optBrY->setValue(y2);
    }

    // execute a pending value reload
    while (m_readValsTmr.isActive()) {
        m_readValsTmr.stop();
        valReload();
    }
    
    setBusy(true);
    m_updProgressTmr.start();
    m_scanThread->setImageInverted(m_invertColors->isChecked());
    m_scanThread->start();
}

void KSaneWidgetPrivate::oneFinalScanDone()
{
    m_updProgressTmr.stop();
    updateProgress();
    
    if (m_closeDevicePending) {
        setBusy(false);
        sane_close(m_saneHandle);
        m_saneHandle = 0;
        clearDeviceOptions();
        return;
    }
    
    if (m_scanThread->frameStatus() == KSaneScanThread::READ_READY) 
    {
        // scan finished OK
        SANE_Parameters params = m_scanThread->saneParameters();
        int lines = params.lines;
        if (lines == -1) {
            // this is probably a handscanner -> calculate the size from the read data
            int bytesPerLine = qMax(getBytesPerLines(params), 1); // ensure no div by 0
            lines = m_scanData.size() / bytesPerLine;
        }
        emit q->imageReady(m_scanData,
                         params.pixels_per_line,
                         lines,
                         getBytesPerLines(params),
                         (int)getImgFormat(params));

        // now check if we should have automatic ADF batch scaning
        if (m_optSource){
            QString source;
            m_optSource->getValue(source);

            if (source.contains("Automatic Document Feeder")) {
                // in batch mode only one area can be scanned per page
                //kDebug() << "source == \"Automatic Document Feeder\"";
                m_updProgressTmr.start();
                m_scanThread->start();
                return;
            }
        }

        // Check if we have a "wait for button" batch scanning
        if (m_optWaitForBtn) {
            qDebug() << m_optWaitForBtn->name();
            QString wait;
            m_optWaitForBtn->getValue(wait);

            qDebug() << "wait ==" << wait;
            if (wait == "true") {
                // in batch mode only one area can be scanned per page
                //kDebug() << "source == \"Automatic Document Feeder\"";
                m_updProgressTmr.start();
                m_scanThread->start();
                return;
            }
        }

        // not batch scan, call sane_cancel to be able to change parameters.
        sane_cancel(m_saneHandle);

        //kDebug() << "index=" << m_selIndex << "size=" << m_previewViewer->selListSize();
        // check if we have multiple selections.
        if (m_previewViewer->selListSize() > m_selIndex)
        {
            if ((m_optTlX != 0) && (m_optTlY != 0) &&  (m_optBrX != 0) && (m_optBrY != 0)) {
                float x1=0,y1=0, x2=0,y2=0, max_x, max_y;
                
                // get maximums
                m_optBrX->getMaxValue(max_x);
                m_optBrY->getMaxValue(max_y);
                
                // read the selection from the viewer
                m_previewViewer->selectionAt(m_selIndex, x1,y1,x2,y2);

                // set the highlight
                m_previewViewer->setHighlightArea(x1,y1,x2,y2);
                
                // calculate the option values
                x1 *= max_x; y1 *= max_y;
                x2 *= max_x; y2 *= max_y;
                
                // now set the selection
                m_optTlX->setValue(x1);
                m_optTlY->setValue(y1);
                m_optBrX->setValue(x2);
                m_optBrY->setValue(y2);
                m_selIndex++;
                
                // execute a pending value reload
                while (m_readValsTmr.isActive()) {
                    m_readValsTmr.stop();
                    valReload();
                }
                m_updProgressTmr.start();
                m_scanThread->start();
                return;
            }
        }
        emit q->scanDone(KSaneWidget::NoError, "");
    }
    else {
        switch(m_scanThread->saneStatus()) 
        {
            case SANE_STATUS_GOOD:
            case SANE_STATUS_CANCELLED:
            case SANE_STATUS_EOF:
                break;
            case SANE_STATUS_NO_DOCS:
                emit q->scanDone(KSaneWidget::Information, i18n(sane_strstatus(m_scanThread->saneStatus())));
                alertUser(KSaneWidget::Information, i18n(sane_strstatus(m_scanThread->saneStatus())));
                break;
            case SANE_STATUS_UNSUPPORTED:
            case SANE_STATUS_IO_ERROR:
            case SANE_STATUS_NO_MEM:
            case SANE_STATUS_INVAL:
            case SANE_STATUS_JAMMED:
            case SANE_STATUS_COVER_OPEN:
            case SANE_STATUS_DEVICE_BUSY:
            case SANE_STATUS_ACCESS_DENIED:
                emit q->scanDone(KSaneWidget::ErrorGeneral, i18n(sane_strstatus(m_scanThread->saneStatus())));
                alertUser(KSaneWidget::ErrorGeneral, i18n(sane_strstatus(m_scanThread->saneStatus())));
                break;
        }
    }

    sane_cancel(m_saneHandle);

    // clear the highlight
    m_previewViewer->setHighlightArea(0,0,1,1);
    setBusy(false);
    m_scanOngoing = false;
}

void KSaneWidgetPrivate::setBusy(bool busy)
{
    if (busy) {
        m_warmingUp->show();
        m_activityFrame->hide();
        m_btnFrame->hide();
        m_optionPollTmr.stop();
    }
    else {
        m_warmingUp->hide();
        m_activityFrame->hide();
        m_btnFrame->show();
        if (m_pollList.size() > 0) {
            m_optionPollTmr.start();
        }
    }
    
    m_optsTabWidget->setDisabled(busy);
    m_previewViewer->setDisabled(busy);
    
    m_scanBtn->setFocus(Qt::OtherFocusReason);
}

void KSaneWidgetPrivate::checkInvert()
{
    if (!m_optSource) return;
    if (!m_optFilmType) return;
    if (m_scanOngoing) return;
    
    QString source;
    QString filmtype;
    m_optSource->getValue(source);
    m_optFilmType->getValue(filmtype);
    
    if ((source.contains(i18nc("This is compared to the option string returned by sane", 
        "Transparency"), Qt::CaseInsensitive)) &&
        (filmtype.contains(i18nc("This is compared to the option string returned by sane", 
        "Negative"), Qt::CaseInsensitive)))
    {
        m_invertColors->setChecked(true);
    }
    else {
        m_invertColors->setChecked(false);
    }
}

void KSaneWidgetPrivate::invertPreview()
{
    m_previewImg.invertPixels();
    m_previewViewer->updateImage();
}

void KSaneWidgetPrivate::updateProgress()
{
    int progress;
    if (m_isPreview) {
        progress = m_previewThread->scanProgress();
        if (m_previewThread->saneStartDone()) {
            if (!m_progressBar->isVisible() || m_previewThread->imageResized()) {
                m_warmingUp->hide();
                m_activityFrame->show();
                // the image size might have changed
                m_previewThread->imgMutex.lock();
                m_previewViewer->setQImage(&m_previewImg);
                m_previewViewer->zoom2Fit();
                m_previewThread->imgMutex.unlock();
            }
            else {
                m_previewThread->imgMutex.lock();
                m_previewViewer->updateImage();
                m_previewThread->imgMutex.unlock();
            }
        }
    }
    else {
        if (!m_progressBar->isVisible() && (m_scanThread->saneStartDone())) {
            m_warmingUp->hide();
            m_activityFrame->show();
        }
        progress = m_scanThread->scanProgress();
        m_previewViewer->setHighlightShown(progress);
    }
    
    m_progressBar->setValue(progress);
    emit q->scanProgress(progress);
}

void KSaneWidgetPrivate::alertUser(int type, const QString &strStatus)
{
    if (q->receivers(SIGNAL(userMessage(int,QString))) == 0) {
        switch (type) {
            case KSaneWidget::ErrorGeneral:
                QMessageBox::warning(0, "", strStatus);
                break;
            default:
                QMessageBox::information(0, "", strStatus);
        }
    }
    else {
        emit q->userMessage(type, strStatus);
    }
}

void KSaneWidgetPrivate::pollPollOptions()
{
    for (int i=1; i<m_pollList.size(); ++i) {
        m_pollList.at(i)->readValue();
    }
}


}  // NameSpace KSaneIface
