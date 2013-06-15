/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2009-11-13
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

#ifndef KSANE_PREVIEW_THREAD_H
#define KSANE_PREVIEW_THREAD_H

// Sane includes.
extern "C"
{
    #include <sane/saneopts.h>
    #include <sane/sane.h>
}

#include <QThread>
#include <QMutex>
#include <QImage>

#define PREVIEW_READ_CHUNK_SIZE 100000

namespace KSaneIface
{
    class KSanePreviewThread: public QThread
    {
        Q_OBJECT
        public:
            typedef enum
            {
                READ_ON_GOING,
                READ_ERROR,
                READ_CANCEL,
                READ_READY
            } ReadStatus;
            
            KSanePreviewThread(SANE_Handle handle, QImage *img);
            void run();
            void setPreviewInverted(bool);
            void cancelScan();
            int scanProgress();
            bool saneStartDone();
            bool imageResized();

            SANE_Status status;
            QMutex imgMutex;

        private:
            void readData();
            void copyToPreviewImg(int readBytes);
            
            SANE_Byte       m_readData[PREVIEW_READ_CHUNK_SIZE];
            int             m_frameSize;
            int             m_frameRead;
            int             m_dataSize;
            int             m_frame_t_count;
            int             m_pixel_x;
            int             m_pixel_y;
            int             m_px_colors[3];
            int             m_px_c_index;
            SANE_Parameters m_params;
            QImage          *m_img;
            SANE_Handle     m_saneHandle;
            bool            m_invertColors;
            ReadStatus      m_readStatus;
            int             m_scanProgress;
            bool            m_saneStartDone;
            bool            m_imageResized;
    };
}


#endif
