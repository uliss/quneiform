/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2009-05-11
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

#include "../libksane/ksane.h"
#include "../libksane/ksane.moc"

// Qt includes
#include <QMap>
#include <QHBoxLayout>
#include <QTimer>
#include <QDebug>

// KDE includes
#include <klocalizedstring.h>
#include <kpushbutton.h>

// Local includes.
#include "twainiface.h"

namespace KSaneIface
{


KSaneWidget::KSaneWidget(QWidget* parent)
    : QWidget(parent), d(new KSaneWidgetPrivate)
{
    KPushButton *openDialog = new KPushButton;
    openDialog->setText(i18n("Open scan dialog"));
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(openDialog);
    setLayout(lay);

    connect(openDialog, SIGNAL(clicked()), d, SLOT(ReOpenDialog()));

    // Forward signals from the private class
    //connect(d, SIGNAL(scanProgress(int)), this, SIGNAL(scanProgress(int)));
    connect(d, SIGNAL(ImageReady(QByteArray&,int,int,int,int)),
            this, SIGNAL(imageReady(QByteArray&,int,int,int,int)));
    //connect(d, SIGNAL(scanDone(int,QString)), this, SIGNAL(scanDone(int,QString)));
    


}

KSaneWidget::~KSaneWidget()
{
    delete d;
}

QString KSaneWidget::selectDevice(QWidget* parent)
{
  //qDebug() << "KSaneWidget::selectDevice";
  return d->SelectSource();
}

bool KSaneWidget::openDevice(const QString &device_name)
{
    //qDebug() << "KSaneWidget::openDevice" << device_name;
    if (!d->OpenSource(device_name)) return false;
    QTimer::singleShot(0, d, SLOT(OpenDialog()));

    return true;
}


bool KSaneWidget::closeDevice()
{
    //qDebug() << "KSaneWidget::closeDevice";
    d->CloseDSM();
    return true;
}

QImage KSaneWidget::toQImage(const QByteArray &data,
                              int width,
                              int height,
                              int bytes_per_line,
                              ImageFormat format)
{
    QImage img = QImage::fromData(data, "BMP");
    return img;
}

QImage KSaneWidget::toQImageSilent(const QByteArray &data,
                                   int width,
                                   int height,
                                   int bytes_per_line,
                                   ImageFormat format)
{
    return KSaneWidget::toQImage(data,
                                width,
                                height,
                                bytes_per_line,
                                format);
}


void KSaneWidget::scanFinal()
{
}

void KSaneWidget::scanCancel()
{
    emit scanProgress(0);
}

// stubbed functions that will not get an implementation any time soon.
QString KSaneWidget::vendor() const {return QString();}
QString KSaneWidget::make() const {return QString();}
QString KSaneWidget::model() const {return QString();}
void KSaneWidget::getOptVals(QMap <QString, QString> &opts) {opts.clear();}
bool KSaneWidget::getOptVal(const QString &optname, QString &value){return false;}
int KSaneWidget::setOptVals(const QMap <QString, QString> &opts) {return 0;}
bool KSaneWidget::setOptVal(const QString &option, const QString &value) {return false;}
void KSaneWidget::setScanButtonText(const QString &scanLabel) {}
void KSaneWidget::setPreviewButtonText(const QString &previewLabel) {}
void KSaneWidget::enableAutoSelect(bool enable) {}
float KSaneWidget::currentDPI() {return 0.0;}
void KSaneWidget::setPreviewResolution(float dpi){Q_UNUSED(dpi);}
void KSaneWidget::setSelection(QPointF lefttop, QPointF rightbottom){Q_UNUSED(lefttop); Q_UNUSED(rightbottom);}
void KSaneWidget::setOptionsCollapsed(bool){}
void KSaneWidget::setScanButtonHidden(bool){}
void KSaneWidget::initGetDeviceList() const {}


}  // NameSpace KSaneIface
