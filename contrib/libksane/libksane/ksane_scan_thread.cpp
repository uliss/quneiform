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
#include "ksane_scan_thread.h"

#include <QDebug>

namespace KSaneIface
{

KSaneScanThread::KSaneScanThread(SANE_Handle handle, QByteArray *data):
    QThread(),
    m_data(data),
    m_saneHandle(handle),
    m_frameSize(0),
    m_frameRead(0),
    m_frame_t_count(0),
    m_dataSize(0),
    m_saneStatus(SANE_STATUS_GOOD),
    m_readStatus(READ_READY),
    m_saneStartDone(false)
{}

void KSaneScanThread::setImageInverted(bool inverted)
{
    m_invertColors = inverted;
}

SANE_Status KSaneScanThread::saneStatus()
{
    return m_saneStatus;
}

KSaneScanThread::ReadStatus KSaneScanThread::frameStatus()
{
    return m_readStatus;
}

void KSaneScanThread::cancelScan()
{
    m_readStatus = READ_CANCEL;
}

int KSaneScanThread::scanProgress()
{
    if (m_dataSize == 0) return 0;

    int bytesRead;

    if (m_frameSize < m_dataSize) {
        bytesRead = m_frameRead + (m_frameSize * m_frame_t_count);
    }
    else {
        bytesRead = m_frameRead;
    }
    return (int)(((float)bytesRead * 100.0)/m_dataSize);
}

SANE_Parameters KSaneScanThread::saneParameters()
{
    return m_params;
}

void KSaneScanThread::run()
{
    m_dataSize = 0;
    m_readStatus = READ_ON_GOING;
    m_saneStartDone = false;

    // Start the scanning with sane_start
    m_saneStatus = sane_start(m_saneHandle);

    m_saneStartDone = true;

    if (m_readStatus == READ_CANCEL) {
        return;
    }

    if (m_saneStatus != SANE_STATUS_GOOD) {
        qDebug() << "sane_start=" << sane_strstatus(m_saneStatus);
        m_readStatus = READ_ERROR;
        // oneFinalScanDone() does the sane_cancel()
        return;
    }

    // Read image parameters
    m_saneStatus = sane_get_parameters(m_saneHandle, &m_params);
    if (m_saneStatus != SANE_STATUS_GOOD) {
        qDebug() << "sane_get_parameters=" << sane_strstatus(m_saneStatus);
        m_readStatus = READ_ERROR;
        // oneFinalScanDone() does the sane_cancel()
        return;
    }

    // calculate data size
    m_frameSize  = m_params.lines * m_params.bytes_per_line;
    if ((m_params.format == SANE_FRAME_RED) ||
            (m_params.format == SANE_FRAME_GREEN) ||
            (m_params.format == SANE_FRAME_BLUE))
    {
        m_dataSize = m_frameSize * 3;
    }
    else {
        m_dataSize = m_frameSize;
    }

    m_data->clear();
    if (m_dataSize > 0) {
        m_data->reserve(m_dataSize);
    }

    m_frameRead     = 0;
    m_frame_t_count = 0;
    m_readStatus    = READ_ON_GOING;
    while (m_readStatus == READ_ON_GOING) {
        readData();
    }
}

void KSaneScanThread::readData()
{
    SANE_Int readBytes = 0;
    m_saneStatus = sane_read(m_saneHandle, m_readData, SCAN_READ_CHUNK_SIZE, &readBytes);

    switch (m_saneStatus)
    {
    case SANE_STATUS_GOOD:
        // continue to parsing the data
        break;

    case SANE_STATUS_EOF:
        if (m_frameRead < m_frameSize) {
            qDebug() << "frameRead =" << m_frameRead  << ", frameSize =" << m_frameSize << "readBytes =" << readBytes;
            if ((readBytes > 0) && ((m_frameRead + readBytes) <= m_frameSize)) {
                qDebug() << "This is not a standard compliant backend";
                copyToScanData(readBytes);
            }
            m_readStatus = READ_READY; // It is better to return a broken image than nothing
            return;
        }
        if (m_params.last_frame == SANE_TRUE) {
            // this is where it all ends well :)
            m_readStatus = READ_READY;
            return;
        }
        else {
            // start reading next frame
            m_saneStatus = sane_start(m_saneHandle);
            if (m_saneStatus != SANE_STATUS_GOOD) {
                qDebug() << "sane_start =" << sane_strstatus(m_saneStatus);
                m_readStatus = READ_ERROR;
                return;
            }
            m_saneStatus = sane_get_parameters(m_saneHandle, &m_params);
            if (m_saneStatus != SANE_STATUS_GOOD) {
                qDebug() << "sane_get_parameters =" << sane_strstatus(m_saneStatus);
                m_readStatus = READ_ERROR;
                sane_cancel(m_saneHandle);
                return;
            }

            m_frameRead = 0;
            m_frame_t_count++;
            break;
        }
    default:
        qDebug() << "sane_read=" << m_saneStatus << ":" << sane_strstatus(m_saneStatus);
        m_readStatus = READ_ERROR;
        sane_cancel(m_saneHandle);
        return;
    }

    copyToScanData(readBytes);
}

#define index_red8_to_rgb8(i)     (i*3)
#define index_red16_to_rgb16(i)   ((i/2)*6 + i%2)

#define index_green8_to_rgb8(i)   (i*3 + 1)
#define index_green16_to_rgb16(i) ((i/2)*6 + i%2 + 2)

#define index_blue8_to_rgb8(i)    (i*3 + 2)
#define index_blue16_to_rgb16(i)  ((i/2)*6 + i%2 + 4)

void KSaneScanThread::copyToScanData(int readBytes)
{
    if (m_invertColors) {
        if (m_params.depth == 16) {
            quint16 *u16ptr = reinterpret_cast<quint16*>(m_readData);
            for(int i=0; i<readBytes/2; i++) {
                u16ptr[i] = 0xFFFF - u16ptr[i];
            }
        }
        else if (m_params.depth == 8) {
            for(int i=0; i<readBytes; i++) {
                m_readData[i] = 0xFF - m_readData[i];
            }
        }
        else if (m_params.depth == 1) {
            for(int i=0; i<readBytes; i++) {
                m_readData[i] = ~m_readData[i];
            }
        }
    }
    switch (m_params.format)
    {
    case SANE_FRAME_GRAY:
        m_data->append((const char*)m_readData, readBytes);
        m_frameRead += readBytes;
        return;
    case SANE_FRAME_RGB:
        if (m_params.depth == 1) {
            break;
        }
        m_data->append((const char*)m_readData, readBytes);
        m_frameRead += readBytes;
        return;

    case SANE_FRAME_RED:
        if (m_params.depth == 8) {
            for (int i=0; i<readBytes; i++) {
                (*m_data)[index_red8_to_rgb8(m_frameRead)] = m_readData[i];
                m_frameRead++;
            }
            return;
        }
        else if (m_params.depth == 16) {
            for (int i=0; i<readBytes; i++) {
                (*m_data)[index_red16_to_rgb16(m_frameRead)] = m_readData[i];
                m_frameRead++;
            }
            return;
        }
        break;

    case SANE_FRAME_GREEN:
        if (m_params.depth == 8) {
            for (int i=0; i<readBytes; i++) {
                (*m_data)[index_green8_to_rgb8(m_frameRead)] = m_readData[i];
                m_frameRead++;
            }
            return;
        }
        else if (m_params.depth == 16) {
            for (int i=0; i<readBytes; i++) {
                (*m_data)[index_green16_to_rgb16(m_frameRead)] = m_readData[i];
                m_frameRead++;
            }
            return;
        }
        break;

    case SANE_FRAME_BLUE:
        if (m_params.depth == 8) {
            for (int i=0; i<readBytes; i++) {
                (*m_data)[index_blue8_to_rgb8(m_frameRead)] = m_readData[i];
                m_frameRead++;
            }
            return;
        }
        else if (m_params.depth == 16) {
            for (int i=0; i<readBytes; i++) {
                (*m_data)[index_blue16_to_rgb16(m_frameRead)] = m_readData[i];
                m_frameRead++;
            }
            return;
        }
        break;
    }

    qDebug() << Q_FUNC_INFO << "Format" << m_params.format
             << "and depth" << m_params.format
             << "is not yet suppoeted by libksane!";

    m_readStatus = READ_ERROR;
    return;
}

bool KSaneScanThread::saneStartDone()
{
    return   m_saneStartDone;
}

}  // NameSpace KSaneIface
