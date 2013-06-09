/* ============================================================
 *
 * This file is a part of kdegraphics
 *
 * Date        : 2009-05-12
 * Description : Twain interface
 *
 * Copyright (C) 2002-2003 Stephan Stapel <stephan dot stapel at web dot de>
 * Copyright (C) 2008-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef TWAINIFACE_H
#define TWAINIFACE_H

#include "twain.h"

// Windows includes.
#include <windows.h>

#include <QWidget>

namespace KSaneIface
{


class KSaneWidgetPrivate : public QWidget
{
    Q_OBJECT

public:

    KSaneWidgetPrivate();
    ~KSaneWidgetPrivate();

    /**
     * Hook-in. See class documentation for details!
     * @result    One should return false to get the message being
     *            processed by the application (should return false by default!)
     */
    bool winEvent(MSG* pMsg, long *result);

    void CloseDSM();
    QString SelectSource();
    bool OpenSource(const QString &device);


public Q_SLOTS:
    bool OpenDialog();
    bool ReOpenDialog();


Q_SIGNALS:
    void ImageReady(QByteArray &data, int width, int height, int bytes_per_line, int format);

private:

    bool InitTwain();
    void ReleaseTwain();

    bool ProcessMessage(MSG msg);

    bool SetImageCount(TW_INT16 nCount=1);
    bool DSOpen() const;


    bool CallTwainProc(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest,
                       TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG,
                       TW_MEMREF pData);

    bool EnableSource(bool showUI);
    void CloseDS();

    bool GetCapability(TW_CAPABILITY& twCap, TW_UINT16 cap, TW_UINT16 conType=TWON_DONTCARE16);
    bool GetCapability(TW_UINT16 cap, TW_UINT32& value);
    bool SetCapability(TW_UINT16 cap, TW_UINT16 value, bool sign=false);
    bool SetCapability(TW_CAPABILITY& twCap);

    bool GetImageInfo(TW_IMAGEINFO& info);

    void TranslateMessage(TW_EVENT& twEvent);
    void TransferImage();
    bool EndTransfer();
    void CancelTransfer();
    bool GetImage(TW_IMAGEINFO& info);

    void ImageData(TW_MEMREF pdata, TW_IMAGEINFO& info);

protected:

    bool         m_bDSMOpen;
    bool         m_bDSOpen;
    bool         m_bSourceEnabled;
    bool         m_bModalUI;

    int          m_nImageCount;

    HINSTANCE    m_hTwainDLL;
    DSMENTRYPROC m_pDSMProc;

    TW_IDENTITY  m_AppId;
    TW_IDENTITY  m_Source;
    TW_STATUS    m_Status;
    TW_INT16     m_returnCode;
    HWND         m_hMessageWnd;
};

}

#endif /* TWAINIFACE_H */
