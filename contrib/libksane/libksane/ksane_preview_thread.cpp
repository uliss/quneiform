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

// Local includes.
#include "ksane_preview_thread.h"

#include <QDebug>
#include <QMutexLocker>


namespace KSaneIface
{
    KSanePreviewThread::KSanePreviewThread(SANE_Handle handle, QImage *img):
    QThread(),
    status(SANE_STATUS_GOOD),
    m_frameSize(0),
    m_frameRead(0),
    m_dataSize(0),
    m_frame_t_count(0),
    m_pixel_x(0),
    m_pixel_y(0),
    m_px_c_index(0),
    m_img(img),
    m_saneHandle(handle),
    m_invertColors(false),
    m_readStatus(READ_READY),
    m_scanProgress(0),
    m_saneStartDone(false),
    m_imageResized(false)
    {
        m_px_colors[0] = 0;
        m_px_colors[1] = 0;
        m_px_colors[2] = 0;
    }
    
    void KSanePreviewThread::setPreviewInverted(bool inverted)
    {
        m_invertColors = inverted;
    }
    
    void KSanePreviewThread::cancelScan()
    {
        m_readStatus = READ_CANCEL;
    }
    
    void KSanePreviewThread::run()
    {
        m_dataSize = 0;
        m_readStatus = READ_ON_GOING;
        m_saneStartDone = false;

        // Start the scanning with sane_start
        status = sane_start(m_saneHandle);
        
        if (status != SANE_STATUS_GOOD) {
            qDebug() << "sane_start=" << sane_strstatus(status);
            sane_cancel(m_saneHandle);
            m_readStatus = READ_ERROR;
            return;
        }
        
        // Read image parameters
        status = sane_get_parameters(m_saneHandle, &m_params);
        if (status != SANE_STATUS_GOOD) {
            qDebug() << "sane_get_parameters=" << sane_strstatus(status);
            sane_cancel(m_saneHandle);
            m_readStatus = READ_ERROR;
            return;
        }

        // calculate data size
        m_frameSize  = m_params.lines * m_params.bytes_per_line;
        if ((m_params.format == SANE_FRAME_RED) ||
            (m_params.format == SANE_FRAME_GREEN) ||
            (m_params.format == SANE_FRAME_BLUE))
        {
            // this is unfortunately calculated again for every frame....
            m_dataSize = m_frameSize*3;
        }
        else {
            m_dataSize = m_frameSize;
        }
        
        // create a new image if necessary
        if ((m_img->height() != m_params.lines) ||
            (m_img->width()  != m_params.pixels_per_line))
        {
            // just hope that the frame size is not changed between different frames of the same image.
            if (m_params.lines > 0) {
                *m_img = QImage(m_params.pixels_per_line, m_params.lines, QImage::Format_RGB32);
            }
            else {
                // handscanners have the number of lines -1 -> make room for something
                *m_img = QImage(m_params.pixels_per_line, m_params.pixels_per_line, QImage::Format_RGB32);
            }
            m_img->fill(0xFFFFFFFF);
        }
        m_imageResized = false;
        m_pixel_x     = 0;
        m_pixel_y     = 0;
        m_frameRead   = 0;
        m_px_c_index  = 0;
        m_frame_t_count = 0;

        // set the m_saneStartDone here so the new QImage gets allocated before updating the preview.
        m_saneStartDone = true;
        
        
        while (m_readStatus == READ_ON_GOING) {
            readData();
        }
    }
    
    int KSanePreviewThread::scanProgress()
    {
        // handscanners have negative data size
        if (m_dataSize <= 0) return 0;
        
        int bytesRead;
        
        if (m_frameSize < m_dataSize) {
            bytesRead = m_frameRead + (m_frameSize * m_frame_t_count);
        }
        else {
            bytesRead = m_frameRead;
        }
        
        return (int)(((float)bytesRead * 100.0)/m_dataSize);
    }
    
    void KSanePreviewThread::readData()
    {
        SANE_Int readBytes;
        status = sane_read(m_saneHandle, m_readData, PREVIEW_READ_CHUNK_SIZE, &readBytes);
        
        switch (status) 
        {
            case SANE_STATUS_GOOD:
                // continue to parsing the data
                break;
                
            case SANE_STATUS_EOF:
                // (handscanners have negative frame size) 
                if (m_frameRead < m_frameSize) {
                    qDebug() << "frameRead =" << m_frameRead  << ", frameSize =" << m_frameSize;
                    m_readStatus = READ_ERROR;
                    return;
                }
                if (m_params.last_frame == SANE_TRUE) {
                    // this is where it all ends well :)
                    m_readStatus = READ_READY;
                    return;
                }
                else {
                    // start reading next frame
                    SANE_Status status = sane_start(m_saneHandle);
                    if (status != SANE_STATUS_GOOD) {
                        qDebug() << "sane_start =" << sane_strstatus(status);
                        m_readStatus = READ_ERROR;
                        return;
                    }
                    status = sane_get_parameters(m_saneHandle, &m_params);
                    if (status != SANE_STATUS_GOOD) {
                        qDebug() << "sane_get_parameters =" << sane_strstatus(status);
                        m_readStatus = READ_ERROR;
                        sane_cancel(m_saneHandle);
                        return;
                    }
                    //kDebug() << "New Frame";
                    m_frameRead = 0;
                    m_pixel_x     = 0;
                    m_pixel_y     = 0;
                    m_px_c_index  = 0;
                    m_frame_t_count++;
                    break;
                }
            default:
                qDebug() << "sane_read=" << status << "=" << sane_strstatus(status);
                m_readStatus = READ_ERROR;
                sane_cancel(m_saneHandle);
                return;
        }
        
        copyToPreviewImg(readBytes);
    }
    
    #define inc_pixel(x,y,ppl) { x++; if (x>=ppl) { y++; x=0;} }
    #define inc_color_index(index) { index++; if (index==3) index=0;}
    
    #define index_red8_to_argb8(i)   (i*4 + 2)
    #define index_red16_to_argb8(i)  (i*2 + 2)
    
    #define index_green8_to_argb8(i)  (i*4 + 1)
    #define index_green16_to_argb8(i) (i*2 + 1)
    
    #define index_blue8_to_argb8(i)  (i*4)
    #define index_blue16_to_argb8(i) (i*2)
    
    void KSanePreviewThread::copyToPreviewImg(int read_bytes)
    {
        QMutexLocker locker(&imgMutex);
        int index;
        uchar *imgBits = m_img->bits();
        if (m_invertColors) {
            if (m_params.depth >= 8) {
                for(int i=0; i<read_bytes; i++) {
                    m_readData[i] = 255 - m_readData[i];
                }
            }
            if (m_params.depth == 1) {
                for(int i=0; i<read_bytes; i++) {
                    m_readData[i] = ~m_readData[i];
                }
            }
        }
        switch (m_params.format)
        {
            case SANE_FRAME_GRAY:
                if (m_params.depth == 1) {
                    int i, j;
                    for (i=0; i<read_bytes; i++) {
                        if (m_pixel_y >= m_img->height()) {
                            // resize the image
                            *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                            m_imageResized = true;
                        }
                        for (j=7; j>=0; --j) {
                            if ((m_readData[i] & (1<<j)) == 0) {
                                m_img->setPixel(m_pixel_x,
                                                m_pixel_y,
                                                qRgb(255,255,255));
                            }
                            else {
                                m_img->setPixel(m_pixel_x,
                                                m_pixel_y,
                                                qRgb(0,0,0));
                            }
                            m_pixel_x++;
                            if(m_pixel_x >= m_params.pixels_per_line) {
                                m_pixel_x = 0;
                                m_pixel_y++;
                                break;
                            }
                            if (m_pixel_y >= m_params.lines) break;
                        }
                        m_frameRead++;
                    }
                    return;
                }
                else if (m_params.depth == 8) {
                    for (int i=0; i<read_bytes; i++) {
                        index = m_frameRead * 4;
                        if ((index + 2) >m_img->numBytes()) {
                            // resize the image
                            *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                            imgBits = m_img->bits();
                            m_imageResized = true;
                        }
                        imgBits[index    ] = m_readData[i];
                        imgBits[index + 1] = m_readData[i];
                        imgBits[index + 2] = m_readData[i];
                        m_frameRead++;
                    }
                    return;
                }
                else if (m_params.depth == 16) {
                    for (int i=0; i<read_bytes; i++) {
                        if (m_frameRead%2 == 0) {
                            index = m_frameRead * 2;
                            if ((index + 2) > m_img->numBytes()) {
                                // resize the image
                                *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                imgBits = m_img->bits();
                                m_imageResized = true;
                            }
                            imgBits[index    ] = m_readData[i+1];
                            imgBits[index + 1] = m_readData[i+1];
                            imgBits[index + 2] = m_readData[i+1];
                        }
                        m_frameRead++;
                    }
                    return;
                }
                break;
                
                case SANE_FRAME_RGB:
                    if (m_params.depth == 8) {
                        for (int i=0; i<read_bytes; i++) {
                            m_px_colors[m_px_c_index] = m_readData[i];
                            inc_color_index(m_px_c_index);
                            m_frameRead++;
                            if (m_px_c_index == 0) {
                                if (m_pixel_y >= m_img->height()) {
                                    // resize the image
                                    *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                    m_imageResized = true;
                                }
                                m_img->setPixel(m_pixel_x,
                                                m_pixel_y,
                                                qRgb(m_px_colors[0],
                                                     m_px_colors[1],
                                                     m_px_colors[2]));
                                                     inc_pixel(m_pixel_x, m_pixel_y, m_params.pixels_per_line);
                            }
                        }
                        return;
                    }
                    else if (m_params.depth == 16) {
                        for (int i=0; i<read_bytes; i++) {
                            m_frameRead++;
                            if (m_frameRead%2==0) {
                                m_px_colors[m_px_c_index] = m_readData[i];
                                inc_color_index(m_px_c_index);
                                if (m_px_c_index == 0) {
                                    if (m_pixel_y >= m_img->height()) {
                                        // resize the image
                                        *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                        m_imageResized = true;
                                    }
                                    m_img->setPixel(m_pixel_x,
                                                    m_pixel_y,
                                                    qRgb(m_px_colors[0],
                                                         m_px_colors[1],
                                                         m_px_colors[2]));
                                                         inc_pixel(m_pixel_x, m_pixel_y, m_params.pixels_per_line);
                                }
                            }
                        }
                        return;
                    }
                    break;
                    
                case SANE_FRAME_RED:
                    if (m_params.depth == 8) {
                        for (int i=0; i<read_bytes; i++) {
                            if (index_red8_to_argb8(m_frameRead) > m_img->numBytes()) {
                                // resize the image
                                *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                imgBits = m_img->bits();
                                m_imageResized = true;
                            }
                            imgBits[index_red8_to_argb8(m_frameRead)] = m_readData[i];
                            m_frameRead++;
                        }
                        return;
                    }
                    else if (m_params.depth == 16) {
                        for (int i=0; i<read_bytes; i++) {
                            if (m_frameRead%2 == 0) {
                                if (index_red16_to_argb8(m_frameRead) > m_img->numBytes()) {
                                    // resize the image
                                    *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                    imgBits = m_img->bits();
                                    m_imageResized = true;
                                }
                                imgBits[index_red16_to_argb8(m_frameRead)] = m_readData[i+1];
                            }
                            m_frameRead++;
                        }
                        return;
                    }
                    break;

                case SANE_FRAME_GREEN:
                    if (m_params.depth == 8) {
                        for (int i=0; i<read_bytes; i++) {
                            if (index_green8_to_argb8(m_frameRead) > m_img->numBytes()) {
                                // resize the image
                                *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                imgBits = m_img->bits();
                                m_imageResized = true;
                            }
                            imgBits[index_green8_to_argb8(m_frameRead)] = m_readData[i];
                            m_frameRead++;
                        }
                        return;
                    }
                    else if (m_params.depth == 16) {
                        for (int i=0; i<read_bytes; i++) {
                            if (m_frameRead%2 == 0) {
                                if (index_green16_to_argb8(m_frameRead) > m_img->numBytes()) {
                                    // resize the image
                                    *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                    imgBits = m_img->bits();
                                    m_imageResized = true;
                                }
                                imgBits[index_green16_to_argb8(m_frameRead)] = m_readData[i+1];
                            }
                            m_frameRead++;
                        }
                        return;
                    }
                    break;

                case SANE_FRAME_BLUE:
                    if (m_params.depth == 8) {
                        for (int i=0; i<read_bytes; i++) {
                            if (index_blue8_to_argb8(m_frameRead) > m_img->numBytes()) {
                                // resize the image
                                *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                imgBits = m_img->bits();
                                m_imageResized = true;
                            }
                            imgBits[index_blue8_to_argb8(m_frameRead)] = m_readData[i];
                            m_frameRead++;
                        }
                        return;
                    }
                    else if (m_params.depth == 16) {
                        for (int i=0; i<read_bytes; i++) {
                            if (m_frameRead%2 == 0) {
                                if (index_blue16_to_argb8(m_frameRead) > m_img->numBytes()) {
                                    // resize the image
                                    *m_img = m_img->copy(0, 0, m_img->width(), m_img->height() + m_img->width());
                                    imgBits = m_img->bits();
                                    m_imageResized = true;
                                }
                                imgBits[index_blue16_to_argb8(m_frameRead)] = m_readData[i+1];
                            }
                            m_frameRead++;
                        }
                        return;
                    }
                    break;
        }

        qWarning() << "Format" << m_params.format
        << "and depth" << m_params.format
        << "is not yet suppoeted by libksane!";
        m_readStatus = READ_ERROR;
        return;
    }

    bool KSanePreviewThread::saneStartDone()
    {
        return   m_saneStartDone;
    }

    bool KSanePreviewThread::imageResized()
    {
        if (m_imageResized) {
            m_imageResized = false;
            return true;
        }
        return false;
    }

}  // NameSpace KSaneIface
