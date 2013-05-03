/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2009-01-24
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2007-2010 by Kare Sars <kare dot sars at iki dot fi>
 * Copyright (C) 2009 by Matthias Nagl <matthias at nagl dot info>
 * Copyright (C) 2009 by Grzegorz Kurtyka <grzegorz dot kurtyka at gmail dot com>
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

#include "ksane.h"

#include <unistd.h>
#include <libintl.h>

// Qt includes
#include <QApplication>
#include <QVarLengthArray>
#include <QLabel>
#include <QSplitter>
#include <QMutex>
#include <QPointer>
#include <QPushButton>
#include <QTranslator>
#include <QDebug>

// Local includes.
#include "ksane_widget_private.h"
#include "ksane_option.h"
#include "ksane_opt_button.h"
#include "ksane_opt_checkbox.h"
#include "ksane_opt_combo.h"
#include "ksane_opt_entry.h"
#include "ksane_opt_fslider.h"
#include "ksane_opt_gamma.h"
#include "ksane_opt_slider.h"
#include "ksane_device_dialog.h"
#include "labeled_gamma.h"
#include "icons.h"

namespace KSaneIface
{
static int     s_objectCount = 0;
static QMutex  s_objectMutex;

static const QString InvetColorsOption = QString("KSane::InvertColors");

namespace {

void loadTranslationsQt()
{
    static QTranslator tr;

    QLocale locale;
    QString tr_fname = "ksane_" + locale.name();
    bool res = tr.load(tr_fname, ":/ksane");
    if(!res) {
        qWarning() << Q_FUNC_INFO << "can't load translation: " << tr_fname;
        return;
    }

    QApplication::installTranslator(&tr);
}

void loadTranslationsGettext()
{
    QLocale locale;

#ifdef Q_OS_MAC
    setlocale(LC_ALL, locale.name().toAscii().constData());
#endif

#ifndef SANE_GETTEXT_DIR
#define SANE_GETTEXT_DIR "/usr/share/locale"
#endif

    bindtextdomain("sane-backends", SANE_GETTEXT_DIR);
    textdomain("sane-backends");
}

}

KSaneWidget::KSaneWidget(QWidget* parent)
    : QWidget(parent), d(new KSaneWidgetPrivate(this))
{
    loadTranslationsQt();
    loadTranslationsGettext();

    SANE_Int    version;
    SANE_Status status;

    s_objectMutex.lock();
    s_objectCount++;

    if (s_objectCount == 1) {
        // only call sane init for the first instance
        status = sane_init(&version, &KSaneAuth::authorization);
        if (status != SANE_STATUS_GOOD) {
            qCritical() << "libksane: sane_init() failed(" << sane_strstatus(status) << ")";
        }
    }
    s_objectMutex.unlock();

    // read the device list to get a list of vendor and model info
    d->m_findDevThread->start();

    d->m_readValsTmr.setSingleShot(true);
    connect(&d->m_readValsTmr,   SIGNAL(timeout()), d, SLOT(valReload()));

    d->m_updProgressTmr.setSingleShot(false);
    d->m_updProgressTmr.setInterval(300);
    connect(&d->m_updProgressTmr, SIGNAL(timeout()), d, SLOT(updateProgress()));
    
    // Create the static UI
    // create the preview
    d->m_previewViewer = new KSaneViewer(&(d->m_previewImg), this);
    connect(d->m_previewViewer, SIGNAL(newSelection(float,float,float,float)),
            d, SLOT(handleSelection(float,float,float,float)));

    
    d->m_warmingUp = new QLabel;
    d->m_warmingUp->setText(tr("Waiting for the scan to start."));
    d->m_warmingUp->setAlignment(Qt::AlignCenter);
    d->m_warmingUp->setAutoFillBackground(true);
    d->m_warmingUp->setBackgroundRole(QPalette::Highlight);
    //d->m_warmingUp->setForegroundRole(QPalette::HighlightedText);
    d->m_warmingUp->hide();

    d->m_progressBar = new QProgressBar;
    d->m_progressBar->setMaximum(100);

    d->m_cancelBtn   = new QPushButton;
    d->m_cancelBtn->setIcon(KSaneIcons::get("process-stop"));
    d->m_cancelBtn->setToolTip(tr("Cancel current scan operation"));
    connect(d->m_cancelBtn, SIGNAL(clicked()), this, SLOT(scanCancel()));
    
    d->m_activityFrame = new QWidget;
    QHBoxLayout *progress_lay = new QHBoxLayout(d->m_activityFrame);
    progress_lay->setContentsMargins(0,0,0,0);
    progress_lay->addWidget(d->m_progressBar, 100);
    progress_lay->addWidget(d->m_cancelBtn, 0);
    d->m_activityFrame->hide();
    
    d->m_zInBtn  = new QToolButton(this);
    d->m_zInBtn->setAutoRaise(true);
    d->m_zInBtn->setIcon(KSaneIcons::get("zoom-in"));
    d->m_zInBtn->setToolTip(tr("Zoom In"));
    connect(d->m_zInBtn, SIGNAL(clicked()), d->m_previewViewer, SLOT(zoomIn()));
    
    d->m_zOutBtn = new QToolButton(this);
    d->m_zOutBtn->setAutoRaise(true);
    d->m_zOutBtn->setIcon(KSaneIcons::get("zoom-out"));
    d->m_zOutBtn->setToolTip(tr("Zoom Out"));
    connect(d->m_zOutBtn, SIGNAL(clicked()), d->m_previewViewer, SLOT(zoomOut()));
    
    d->m_zSelBtn = new QToolButton(this);
    d->m_zSelBtn->setAutoRaise(true);
    d->m_zSelBtn->setIcon(KSaneIcons::get("zoom-fit-best"));
    d->m_zSelBtn->setToolTip(tr("Zoom to Selection"));
    connect(d->m_zSelBtn, SIGNAL(clicked()), d->m_previewViewer, SLOT(zoomSel()));
    
    d->m_zFitBtn = new QToolButton(this);
    d->m_zFitBtn->setAutoRaise(true);
    d->m_zFitBtn->setIcon(KSaneIcons::get("document-preview"));
    d->m_zFitBtn->setToolTip(tr("Zoom to Fit"));
    connect(d->m_zFitBtn, SIGNAL(clicked()), d->m_previewViewer, SLOT(zoom2Fit()));
    
    d->m_prevBtn = new QPushButton(this);
    d->m_prevBtn->setIcon(KSaneIcons::get("image-x-generic"));
    d->m_prevBtn->setToolTip(tr("Scan Preview Image"));
    d->m_prevBtn->setText(tr("Preview"));
    connect(d->m_prevBtn,   SIGNAL(clicked()), d, SLOT(startPreviewScan()));
    
    d->m_scanBtn = new QPushButton(this);
    d->m_scanBtn->setIcon(KSaneIcons::get("document-save"));
    d->m_scanBtn->setToolTip(tr("Scan Final Image"));
    d->m_scanBtn->setText(tr("Scan"));
    d->m_scanBtn->setFocus(Qt::OtherFocusReason);
    connect(d->m_scanBtn,   SIGNAL(clicked()), d, SLOT(startFinalScan()));
    
    d->m_btnFrame = new QWidget;
    QHBoxLayout *btn_lay = new QHBoxLayout(d->m_btnFrame);
    btn_lay->setContentsMargins(0,0,0,0);
    btn_lay->addWidget(d->m_zInBtn);
    btn_lay->addWidget(d->m_zOutBtn);
    btn_lay->addWidget(d->m_zSelBtn);
    btn_lay->addWidget(d->m_zFitBtn);
    btn_lay->addStretch(100);
    btn_lay->addWidget(d->m_prevBtn);
    btn_lay->addWidget(d->m_scanBtn);
    
    // calculate the height of the waiting/scanning/buttons frames to avoid jumpiness.
    int minHeight = d->m_btnFrame->sizeHint().height();
    if (d->m_activityFrame->sizeHint().height() > minHeight) minHeight = d->m_activityFrame->sizeHint().height();
    if (d->m_warmingUp->sizeHint().height() > minHeight)     minHeight = d->m_warmingUp->sizeHint().height();
    d->m_btnFrame->setMinimumHeight(minHeight);
    d->m_activityFrame->setMinimumHeight(minHeight);
    d->m_warmingUp->setMinimumHeight(minHeight);
    
    d->m_previewFrame = new QWidget;
    QVBoxLayout *preview_layout = new QVBoxLayout(d->m_previewFrame);
    preview_layout->setContentsMargins(0,0,0,0);
    preview_layout->addWidget(d->m_previewViewer, 100);
    preview_layout->addWidget(d->m_warmingUp, 0);
    preview_layout->addWidget(d->m_activityFrame, 0);
    preview_layout->addWidget(d->m_btnFrame, 0);
    
    // Create Options Widget
    d->m_optsTabWidget = new QTabWidget;
    
    // Add the basic options tab
    d->m_basicScrollA = new QScrollArea;
    d->m_basicScrollA->setWidgetResizable(true);
    d->m_basicScrollA->setFrameShape(QFrame::NoFrame);
    d->m_optsTabWidget->addTab(d->m_basicScrollA, tr("Basic Options"));

    // Add the other options tab
    d->m_otherScrollA = new QScrollArea;
    d->m_otherScrollA->setWidgetResizable(true);
    d->m_otherScrollA->setFrameShape(QFrame::NoFrame);
    d->m_optsTabWidget->addTab(d->m_otherScrollA, tr("Scanner Specific Options"));
    
    
    d->m_splitter = new QSplitter(this);
    d->m_splitter->addWidget(d->m_optsTabWidget);
    d->m_splitter->setStretchFactor(0,0);
    d->m_splitter->addWidget(d->m_previewFrame);
    d->m_splitter->setStretchFactor(1,100);
    
    d->m_optionsCollapser = new SplitterCollapser(d->m_splitter, d->m_optsTabWidget);
    
    QHBoxLayout *base_layout = new QHBoxLayout(this);
    base_layout->addWidget(d->m_splitter);
    base_layout->setContentsMargins(0,0,0,0);

    // disable the interface in case no device is opened.
    d->m_optsTabWidget->setDisabled(true);
    d->m_previewViewer->setDisabled(true);
    d->m_btnFrame->setDisabled(true);
    
}

KSaneWidget::~KSaneWidget()
{
    while (!closeDevice()) usleep(1000);
    // wait for any thread to exit

    s_objectMutex.lock();
    s_objectCount--;
    if (s_objectCount <= 0) {
        // only delete the find-devices and authorization singletons and call sane_exit
        // if this is the last instance
        delete d->m_findDevThread;
        delete d->m_auth;
        sane_exit();
    }
    s_objectMutex.unlock();
    delete d;
}

QString KSaneWidget::vendor() const 
{
    d->m_findDevThread->wait();
    d->devListUpdated(); // this is just a wrapped if (m_vendor.isEmpty()) statement if the vendor is known
    // devListUpdated here is to ensure that we do not come in between finished and the devListUpdated slot

    return d->m_vendor;
}
QString KSaneWidget::make() const 
{
    return vendor();
}
QString KSaneWidget::model() const 
{
    d->m_findDevThread->wait();
    d->devListUpdated(); // this is just a wrapped if (m_vendor.isEmpty()) statement if the vendor is known
    // devListUpdated here is to ensure that we do not come in between finished and the devListUpdated slot

    return d->m_model;
}

QString KSaneWidget::selectDevice(QWidget* parent, const QString& defaultScanner)
{
    QString device;
    QPointer<KSaneDeviceDialog> sel = new KSaneDeviceDialog(parent, defaultScanner);

    if(sel->exec() == QDialog::Accepted)
        device = sel->getSelectedName();

    delete sel;

    return device;
}

void KSaneWidget::initGetDeviceList() const
{
    // update the device list if needed to get the vendor and model info
    if (d->m_findDevThread->devicesList().size() == 0) {
        //kDebug() << "initGetDeviceList() starting thread...";
        d->m_findDevThread->start();
    }
    else {
        //kDebug() << "initGetDeviceList() have existing data...";
        d->signalDevListUpdate();
    }
}

bool KSaneWidget::openDevice(const QString &deviceName)
{
    int                            i=0;
    const SANE_Option_Descriptor  *optDesc;
    SANE_Status                    status;
    SANE_Word                      numSaneOptions;
    SANE_Int                       res;
//    KPasswordDialog               *dlg;
//    KWallet::Wallet               *saneWallet;
    QString                        myFolderName("ksane");
    QMap<QString, QString>         wallet_entry;

    // don't bother trying to open if the device string is empty
    if (deviceName.isEmpty()) {
        return false;
    }
    // save the device name
    d->m_devName = deviceName;

    // Try to open the device
    status = sane_open(deviceName.toLatin1(), &d->m_saneHandle);

    bool password_dialog_ok = true;

    // prepare wallet for authentication and create password dialog
    if(status == SANE_STATUS_ACCESS_DENIED) {
//        saneWallet = KWallet::Wallet::openWallet(KWallet::Wallet::LocalWallet(), winId() );
//
//        if(saneWallet) {
//            dlg = new KPasswordDialog(this, KPasswordDialog::ShowUsernameLine | KPasswordDialog::ShowKeepPassword);
//            if(!saneWallet->hasFolder(myFolderName)) {
//                saneWallet->createFolder(myFolderName);
//            }
//            saneWallet->setFolder(myFolderName);
//            saneWallet->readMap(deviceName.toLatin1(), wallet_entry);
//            if(!wallet_entry.empty() || true) {
//                dlg->setUsername( wallet_entry["username"] );
//                dlg->setPassword( wallet_entry["password"] );
//                dlg->setKeepPassword( true );
//            }
//        } else {
//            qWarning() << Q_FUNC_INFO << "not implemented yet";
//            dlg = new KPasswordDialog(this, KPasswordDialog::ShowUsernameLine);
//        }
//        dlg->setPrompt(i18n("Authentication required for resource: %1")/*, deviceName )*/);

    }

    // sane_open failed due to insufficient authorization
    // retry opening device with user provided data assisted with kwallet records
    while (status == SANE_STATUS_ACCESS_DENIED) {
        return false;

//        password_dialog_ok = dlg->exec();
//        if(!password_dialog_ok) {
//            delete dlg;
//            d->m_devName.clear();
//            return false; //the user canceled
//        }

//        // add/update the device user-name and password for authentication
//        d->m_auth->setDeviceAuth(d->m_devName, dlg->username(), dlg->password());

//        status = sane_open(deviceName.toLatin1(), &d->m_saneHandle);

//        // store password in wallet on successful authentication
//        if(dlg->keepPassword() && status != SANE_STATUS_ACCESS_DENIED) {
//            QMap<QString, QString> entry;
//            entry["username"] = dlg->username().toUtf8();
//            entry["password"] = dlg->password().toUtf8();
//            saneWallet->writeMap(deviceName.toLatin1(), entry);
//        }
    }

    if (status != SANE_STATUS_GOOD) {
        qDebug() << "sane_open(\"" << deviceName << "\", &handle) failed! status = " << sane_strstatus(status);
        d->m_auth->clearDeviceAuth(d->m_devName);
        d->m_devName.clear();
        return false;
    }

    // update the device list if needed to get the vendor and model info
    if (d->m_findDevThread->devicesList().size() == 0) {
        d->m_findDevThread->start();
    }
    else {
        // use the "old" existing list
        d->devListUpdated();
        // if m_vendor is not updated it means that the list needs to be updated.
        if (d->m_vendor.isEmpty()) {
            d->m_findDevThread->start();
        }
    }

    // Read the options (start with option 0 the number of parameters)
    optDesc = sane_get_option_descriptor(d->m_saneHandle, 0);
    if (optDesc == 0) {
        d->m_auth->clearDeviceAuth(d->m_devName);
        d->m_devName.clear();
        return false;
    }
    QVarLengthArray<char> data(optDesc->size);
    status = sane_control_option(d->m_saneHandle, 0, SANE_ACTION_GET_VALUE, data.data(), &res);
    if (status != SANE_STATUS_GOOD) {
        d->m_auth->clearDeviceAuth(d->m_devName);
        d->m_devName.clear();
        return false;
    }
    numSaneOptions = *reinterpret_cast<SANE_Word*>(data.data());

    // read the rest of the options
    for (i=1; i<numSaneOptions; ++i) {
        switch (KSaneOption::optionType(sane_get_option_descriptor(d->m_saneHandle, i))) {
            case KSaneOption::TYPE_DETECT_FAIL:
                d->m_optList.append(new KSaneOption(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_CHECKBOX:
                d->m_optList.append(new KSaneOptCheckBox(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_SLIDER:
                d->m_optList.append(new KSaneOptSlider(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_F_SLIDER:
                d->m_optList.append(new KSaneOptFSlider(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_COMBO:
                d->m_optList.append(new KSaneOptCombo(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_ENTRY:
                d->m_optList.append(new KSaneOptEntry(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_GAMMA:
                d->m_optList.append(new KSaneOptGamma(d->m_saneHandle, i));
                break;
            case KSaneOption::TYPE_BUTTON:
                d->m_optList.append(new KSaneOptButton(d->m_saneHandle, i));
                break;
        }
    }

    // do the connections of the option parameters
    for (i=1; i<d->m_optList.size(); ++i) {
        //kDebug() << d->m_optList.at(i)->name();
        connect (d->m_optList.at(i), SIGNAL(optsNeedReload()), d, SLOT(optReload()));
        connect (d->m_optList.at(i), SIGNAL(valsNeedReload()), d, SLOT(scheduleValReload()));

        if (d->m_optList.at(i)->needsPolling()) {
            //kDebug() << d->m_optList.at(i)->name() << " needs polling";
            d->m_pollList.append(d->m_optList.at(i));
            KSaneOptCheckBox *buttonOption = qobject_cast<KSaneOptCheckBox *>(d->m_optList.at(i));
            if (buttonOption) {
                connect(buttonOption, SIGNAL(buttonPressed(QString,QString,bool)),
                        this, SIGNAL(buttonPressed(QString,QString,bool)));
            }
        }
    }

    // start polling the poll options
    if (d->m_pollList.size() > 0) {
        d->m_optionPollTmr.start();
    }
    
    // Create the preview thread
    d->m_previewThread = new KSanePreviewThread(d->m_saneHandle, &d->m_previewImg);
    connect(d->m_previewThread, SIGNAL(finished()), d, SLOT(previewScanDone()));
    
    // Create the read thread
    d->m_scanThread = new KSaneScanThread(d->m_saneHandle, &d->m_scanData);
    connect(d->m_scanThread, SIGNAL(finished()), d, SLOT(oneFinalScanDone()));
    
    // Create the options interface
    d->createOptInterface();
    
    // try to set KSaneWidget default values
    d->setDefaultValues();
    
    // Enable the interface
    d->m_optsTabWidget->setDisabled(false);
    d->m_previewViewer->setDisabled(false);
    d->m_btnFrame->setDisabled(false);

    // estimate the preview size and create an empty image
    // this is done so that you can select scan area without
    // having to scan a preview.
    d->updatePreviewSize();
    QTimer::singleShot(1000, d->m_previewViewer, SLOT(zoom2Fit()));
    return true;
}


bool KSaneWidget::closeDevice()
{
    if (!d->m_saneHandle) {
        return true;
    }
    
    if (d->m_scanThread->isRunning()) {
        d->m_scanThread->cancelScan();
        d->m_closeDevicePending = true;
        return false;
    }
    
    if (d->m_previewThread->isRunning()) {
        d->m_previewThread->cancelScan();
        d->m_closeDevicePending = true;
        return false;
    }

    d->m_auth->clearDeviceAuth(d->m_devName);
    // else 
    sane_close(d->m_saneHandle);
    d->m_saneHandle = 0;
    d->clearDeviceOptions();

    // disable the interface until a new device is opened.
    d->m_optsTabWidget->setDisabled(true);
    d->m_previewViewer->setDisabled(true);
    d->m_btnFrame->setDisabled(true);
    
    return true;
}

#define inc_pixel(x,y,ppl) { x++; if (x>=ppl) { y++; x=0;} }

QImage KSaneWidget::toQImageSilent(const QByteArray &data,
                                   int width,
                                   int height,
                                   int bytes_per_line,
                                   ImageFormat format)
{
    QImage img;
    int j=0;
    int pixel_x = 0;
    int pixel_y = 0;
    QVector<QRgb> table;

    switch (format)
    {
        case FormatBlackWhite:
            img = QImage((uchar*)data.data(),
                          width,
                          height,
                          bytes_per_line,
                          QImage::Format_Mono);
            // The color table must be set
            table.append(0xFFFFFFFF);
            table.append(0xFF000000);
            img.setColorTable(table);
            break;

        case FormatGrayScale8:
            img = QImage(width,
                         height,
                         QImage::Format_RGB32);
            j=0;
            for (int i=0; i<data.size(); i++) {
                img.bits()[j+0] = data.data()[i];
                img.bits()[j+1] = data.data()[i];
                img.bits()[j+2] = data.data()[i];
                j+=4;
            }
            break;

        case FormatGrayScale16:
            img = QImage(width,
                         height,
                         QImage::Format_RGB32);
            j=0;
            for (int i=1; i<data.size(); i+=2) {
                img.bits()[j+0] = data.data()[i];
                img.bits()[j+1] = data.data()[i];
                img.bits()[j+2] = data.data()[i];
                j+=4;
            }
            break;

        case FormatRGB_8_C:
            pixel_x = 0;
            pixel_y = 0;

            img = QImage(width,
                         height,
                         QImage::Format_RGB32);

            for (int i=0; i<data.size(); i+=3) {
                img.setPixel(pixel_x,
                             pixel_y,
                             qRgb(data[i],
                                  data[i+1],
                                  data[i+2]));

                inc_pixel(pixel_x, pixel_y, width);
            }
            break;

        case FormatRGB_16_C:
            pixel_x = 0;
            pixel_y = 0;

            img = QImage(width,
                         height,
                         QImage::Format_RGB32);

            for (int i=1; i<data.size(); i+=6) {
                img.setPixel(pixel_x,
                             pixel_y,
                             qRgb(data[i],
                                  data[i+2],
                                  data[i+4]));

                inc_pixel(pixel_x, pixel_y, width);
            }
            break;

        case FormatNone:
        default:
            qDebug() << "Unsupported conversion";
            break;
    }
    float dpm = currentDPI() * (1000.0 / 25.4);
    img.setDotsPerMeterX(dpm);
    img.setDotsPerMeterY(dpm);
    return img;
}

QImage KSaneWidget::toQImage(const QByteArray &data,
                              int width,
                              int height,
                              int bytes_per_line,
                              ImageFormat format)
{
    
    if ((format == FormatRGB_16_C) || (format == FormatGrayScale16)) {
        d->alertUser(KSaneWidget::ErrorGeneral, tr("The image data contained 16 bits per color, "
                    "but the color depth has been truncated to 8 bits per color."));
    }
    return toQImageSilent(data, width, height, bytes_per_line, format);
}

void KSaneWidget::scanFinal()
{
    if (d->m_btnFrame->isEnabled()) {
        d->startFinalScan();
    }
    else {
        // if the button frame is disabled, there is no open device to scan from
        emit scanDone(KSaneWidget::ErrorGeneral, "");
    }
}

void KSaneWidget::scanCancel()
{
    if (d->m_scanThread->isRunning()) {
        d->m_scanThread->cancelScan();
    }
    
    if (d->m_previewThread->isRunning()) {
        d->m_previewThread->cancelScan();
    }
}

void KSaneWidget::setPreviewResolution(float dpi)
{
    d->m_previewDPI = dpi;
}

void KSaneWidget::getOptVals(QMap <QString, QString> &opts)
{
    KSaneOption *option;
    opts.clear();
    QString tmp;

    for (int i=1; i<d->m_optList.size(); i++) {
        option = d->m_optList.at(i);
        if (option->getValue(tmp)) {
            opts[option->name()] = tmp;
        }
    }
    // Special handling for non sane option
    opts[InvetColorsOption] = d->m_invertColors->isChecked() ? "true" : "false";
}

bool KSaneWidget::getOptVal(const QString &optname, QString &value)
{
    KSaneOption *option;

    if ((option = d->getOption(optname)) != 0) {
        return option->getValue(value);
    }
    // Special handling for non sane option
    if (optname == InvetColorsOption) {
        value = d->m_invertColors->isChecked() ? "true" : "false";
        return true;
    }
    return false;
}

int KSaneWidget::setOptVals(const QMap <QString, QString> &opts)
{
    QString tmp;
    int i;
    int ret=0;

    for (i=0; i<d->m_optList.size(); i++) {
        if (opts.contains(d->m_optList.at(i)->name())) {
            tmp = opts[d->m_optList.at(i)->name()];
            if (d->m_optList.at(i)->setValue(tmp) == false) {
                ret++;
            }
        }
    }
    if ((d->m_splitGamChB) &&
        (d->m_optGamR) &&
        (d->m_optGamG) &&
        (d->m_optGamB))
    {
        // check if the current gamma values are identical. if they are identical,
        // uncheck the "Separate color intensity tables" checkbox
        QString redGamma;
        QString greenGamma;
        QString blueGamma;
        d->m_optGamR->getValue(redGamma);
        d->m_optGamG->getValue(greenGamma);
        d->m_optGamB->getValue(blueGamma);
        if ((redGamma == greenGamma) && (greenGamma == blueGamma)) {
            d->m_splitGamChB->setChecked(false);
            // set the values to the common gamma widget
            d->m_commonGamma->setValues(redGamma);
        }
        else {
            d->m_splitGamChB->setChecked(true);
        }
    }

    // special handling for non-sane option
    if (opts.contains(InvetColorsOption)) {
        tmp = opts[InvetColorsOption];
        if ((tmp.compare("true", Qt::CaseInsensitive) == 0) ||
            (tmp.compare("1") == 0))
        {
            d->m_invertColors->setChecked(true);
        }
        else {
            d->m_invertColors->setChecked(false);
        }
    }
    return ret;
}

bool KSaneWidget::setOptVal(const QString &option, const QString &value)
{
    KSaneOption *opt;

    if ((opt = d->getOption(option)) != 0) {
        if (opt->setValue(value)) {
            if ((d->m_splitGamChB) &&
                (d->m_optGamR) &&
                (d->m_optGamG) &&
                (d->m_optGamB) &&
                ((opt == d->m_optGamR) ||
                (opt == d->m_optGamG) ||
                (opt == d->m_optGamB)))
            {
                // check if the current gamma values are identical. if they are identical,
                // uncheck the "Separate color intensity tables" checkbox
                QString redGamma;
                QString greenGamma;
                QString blueGamma;
                d->m_optGamR->getValue(redGamma);
                d->m_optGamG->getValue(greenGamma);
                d->m_optGamB->getValue(blueGamma);
                if ((redGamma == greenGamma) && (greenGamma == blueGamma)) {
                    d->m_splitGamChB->setChecked(false);
                    // set the values to the common gamma widget
                    d->m_commonGamma->setValues(redGamma);
                }
                else {
                    d->m_splitGamChB->setChecked(true);
                }
            }
            return true;
        }
    }

    // special handling for non-sane option
    if (option == InvetColorsOption) {
        if ((value.compare("true", Qt::CaseInsensitive) == 0) ||
            (value.compare("1") == 0))
        {
            d->m_invertColors->setChecked(true);
        }
        else {
            d->m_invertColors->setChecked(false);
        }
        return true;
    }
    
    return false;
}

void KSaneWidget::setScanButtonText(const QString &scanLabel)
{
    if (d->m_scanBtn == 0) {
        qWarning() << "setScanButtonText was called before KSaneWidget was initialized";
        return;
    }
    d->m_scanBtn->setText(scanLabel);
}

void KSaneWidget::setPreviewButtonText(const QString &previewLabel)
{
    if (d->m_scanBtn == 0) {
        qWarning() << "setPreviewButtonText was called before KSaneWidget was initialized";
        return;
    }
    d->m_prevBtn->setText(previewLabel);
}

void KSaneWidget::enableAutoSelect(bool enable)
{
    d->m_autoSelect = enable;
}

float KSaneWidget::currentDPI()
{
    if (d->m_optRes) {
        float value;
        if (d->m_optRes->getValue(value)) {
            return value;
        }
    }
    return 0.0;
}

float KSaneWidget::scanAreaWidth()
{
    float result = 0.0;
    if (d->m_optBrX) {
        if (d->m_optBrX->getUnit() == SANE_UNIT_PIXEL) {
            d->m_optBrX->getMaxValue(result);
            result = result / currentDPI() / 25.4;
        }
        else if (d->m_optBrX->getUnit() == SANE_UNIT_MM) {
            d->m_optBrX->getMaxValue(result);
        }
    }
    return result;
}

float KSaneWidget::scanAreaHeight()
{
    float result = 0.0;
    if (d->m_optBrY) {
        if (d->m_optBrY->getUnit() == SANE_UNIT_PIXEL) {
            d->m_optBrY->getMaxValue(result);
            result = result / currentDPI() / 25.4;
        }
        else if (d->m_optBrY->getUnit() == SANE_UNIT_MM) {
            d->m_optBrY->getMaxValue(result);
        }
    }
    return result;
}

void KSaneWidget::setSelection(QPointF topLeft, QPointF bottomRight)
{
    if (!d->m_optBrX || !d->m_optBrY || !d->m_optTlX || !d->m_optTlY) {
        return;
    }
    float xmax, ymax;
    d->m_optBrX->getMaxValue(xmax);
    d->m_optBrY->getMaxValue(ymax);
    if (topLeft.x() < 0.0 || topLeft.y() < 0.0 || bottomRight.x() < 0.0 || bottomRight.y() < 0.0) {
        d->m_previewViewer->clearActiveSelection();
        d->m_optTlX->setValue(0.0);
        d->m_optTlY->setValue(0.0);
        d->m_optBrX->setValue(xmax);
        d->m_optBrY->setValue(ymax);
    }

    if (d->m_optBrY->getUnit() == SANE_UNIT_MM) {
        // clear selection if values are out of bounds
        if (topLeft.x() > xmax || topLeft.y() > ymax || bottomRight.x() > xmax || bottomRight.y() > ymax) {
            d->m_previewViewer->clearActiveSelection();
            d->m_optTlX->setValue(0.0);
            d->m_optTlY->setValue(0.0);
            d->m_optBrX->setValue(xmax);
            d->m_optBrY->setValue(ymax);
        }
        d->m_previewViewer->setSelection(topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y());
        d->m_optTlX->setValue(topLeft.x());
        d->m_optTlY->setValue(topLeft.y());
        d->m_optBrX->setValue(bottomRight.x());
        d->m_optBrY->setValue(bottomRight.y());
    }
    else if (d->m_optBrY->getUnit() == SANE_UNIT_PIXEL) {
        const float mmperinch = 25.4;
        const float dpi = currentDPI();
        const float m = dpi / mmperinch;
        if (m*topLeft.x() > xmax || m*topLeft.y() > ymax || m*bottomRight.x() > xmax || m*bottomRight.y() > ymax) {
            d->m_previewViewer->clearActiveSelection();
            d->m_optTlX->setValue(0.0);
            d->m_optTlY->setValue(0.0);
            d->m_optBrX->setValue(xmax);
            d->m_optBrY->setValue(ymax);
        }
        d->m_previewViewer->setSelection(m*topLeft.x(), m*topLeft.y(), m*bottomRight.x(), m*bottomRight.y());
        d->m_optTlX->setValue(m*topLeft.x());
        d->m_optTlY->setValue(m*topLeft.y());
        d->m_optBrX->setValue(m*bottomRight.x());
        d->m_optBrY->setValue(m*bottomRight.y());
    }
}

void KSaneWidget::setOptionsCollapsed(bool collapse)
{
    if (collapse) {
        QTimer::singleShot(0, d->m_optionsCollapser, SLOT(slotCollapse()));
    }
    else {
        QTimer::singleShot(0, d->m_optionsCollapser, SLOT(slotRestore()));
    }
}

void KSaneWidget::setScanButtonHidden(bool hidden)
{
    d->m_scanBtn->setHidden(hidden);
}

}  // NameSpace KSaneIface
