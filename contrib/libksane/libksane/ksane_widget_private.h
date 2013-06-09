/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2009-01-21
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2009 by Kare Sars <kare dot sars at iki dot fi>
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

#ifndef KSANE_WIDGET_PRIVATE_H
#define KSANE_WIDGET_PRIVATE_H

// Sane includes.
extern "C"
{
    #include <sane/saneopts.h>
    #include <sane/sane.h>
}

// Qt includes.
#include <QtGui/QWidget>
#include <QCheckBox>
#include <QTimer>
#include <QTime>
#include <QProgressBar>

// KDE includes
#include <QTabWidget>
#include <QLocale>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QToolBar>

// Local includes
#include "ksane.h"
#include "ksane_option.h"
#include "ksane_viewer.h"
#include "labeled_gamma.h"
#include "labeled_checkbox.h"
#include "splittercollapser.h"
#include "ksane_scan_thread.h"
#include "ksane_preview_thread.h"
#include "ksane_find_devices_thread.h"
#include "ksane_auth.h"

#define IMG_DATA_R_SIZE 100000

/** This namespace collects all methods and classes in LibKSane. */
namespace KSaneIface
{
    class KSaneWidgetPrivate: public QObject
    {
        Q_OBJECT

        public:
            KSaneWidgetPrivate(KSaneWidget *);
            void clearDeviceOptions();
            void createOptInterface();
            void updatePreviewSize();
            void setDefaultValues();
            void setBusy(bool busy);
            KSaneOption *getOption(const QString &name);
            KSaneWidget::ImageFormat getImgFormat(SANE_Parameters &params);
            int getBytesPerLines(SANE_Parameters &params);

        public Q_SLOTS:
            void devListUpdated();
            void signalDevListUpdate();
            void startFinalScan();
            void previewScanDone();
            void oneFinalScanDone();
            void updateProgress();

        private Q_SLOTS:
            void scheduleValReload();
            void optReload();
            void valReload();
            void handleSelection(float tl_x, float tl_y, float br_x, float br_y);
            void setTLX(float x);
            void setTLY(float y);
            void setBRX(float x);
            void setBRY(float y);

            void startPreviewScan();

            void checkInvert();
            void invertPreview();
            void pollPollOptions();

        public:
            void alertUser(int type, const QString &strStatus);

        public:
            // backend independent
            QTabWidget         *m_optsTabWidget;
            QScrollArea        *m_basicScrollA;
            QWidget            *m_basicOptsTab;
            QWidget            *m_colorOpts;
            QScrollArea        *m_otherScrollA;
            QWidget            *m_otherOptsTab;
            LabeledCheckbox    *m_invertColors;

            QSplitter          *m_splitter;
            SplitterCollapser  *m_optionsCollapser;

            QWidget            *m_previewFrame;
            KSaneViewer        *m_previewViewer;
            QWidget            *m_btnFrame;
            QToolButton        *m_zInBtn;
            QToolButton        *m_zOutBtn;
            QToolButton        *m_zSelBtn;
            QToolButton        *m_zFitBtn;
            QPushButton        *m_scanBtn;
            QPushButton        *m_prevBtn;

            QWidget            *m_activityFrame;
            QLabel             *m_warmingUp;
            QProgressBar       *m_progressBar;
            QPushButton        *m_cancelBtn;

            // device info
            SANE_Handle         m_saneHandle;
            QString             m_devName;
            QString             m_vendor;
            QString             m_model;

            // Option variables
            QList<KSaneOption*> m_optList;
            QList<KSaneOption*> m_pollList;
            KSaneOption        *m_optSource;
            KSaneOption        *m_optNegative;
            KSaneOption        *m_optFilmType;
            KSaneOption        *m_optMode;
            KSaneOption        *m_optDepth;
            KSaneOption        *m_optRes;
            KSaneOption        *m_optResX;
            KSaneOption        *m_optResY;
            KSaneOption        *m_optTlX;
            KSaneOption        *m_optTlY;
            KSaneOption        *m_optBrX;
            KSaneOption        *m_optBrY;
            KSaneOption        *m_optPreview;
            KSaneOption        *m_optGamR;
            KSaneOption        *m_optGamG;
            KSaneOption        *m_optGamB;
            LabeledCheckbox    *m_splitGamChB;
            LabeledGamma       *m_commonGamma;
            KSaneOption        *m_optWaitForBtn;

            // preview variables
            float               m_previewWidth;
            float               m_previewHeight;
            float               m_previewDPI;
            QImage              m_previewImg;
            bool                m_isPreview;
            bool                m_autoSelect;

            int                 m_selIndex;

            bool                m_scanOngoing;
            bool                m_closeDevicePending;

            // final image data
            QByteArray          m_scanData;

            // option handling
            QTimer              m_readValsTmr;
            QTimer              m_updProgressTmr;
            QTimer              m_optionPollTmr;
            KSaneScanThread    *m_scanThread;
            KSanePreviewThread *m_previewThread;

            QString             m_saneUserName;
            QString             m_sanePassword;

            FindSaneDevicesThread *m_findDevThread;
            KSaneAuth             *m_auth;
            KSaneWidget           *q;
    };


}  // NameSpace KSaneIface

#endif // SANE_WIDGET_H
