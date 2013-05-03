/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Date        : 2007-09-13
 * Description : Sane interface for KDE
 *
 * Copyright (C) 2007-2010 by Kare Sars <kare dot sars at iki dot fi>
 * Copyright (C) 2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef KSANE_H
#define KSANE_H

// Qt includes.
#include <QtGui/QWidget>

// Local includes.
#include "libksane_export.h"

/** This namespace collects all methods and classes in LibKSane. */
namespace KSaneIface
{

class KSaneWidgetPrivate;

/**
 * This class provides the widget containing the scan options and the preview.
 * @author Kare Sars <kare.sars@iki.fi>
 */
class LIBKSANE_EXPORT KSaneWidget : public QWidget
{
    Q_OBJECT
    friend class KSaneWidgetPrivate;

public:
    /** This enumeration describes the type of the returned data.
     * The number of formats might grow, so it is wise to be prepared fro more.*/
    typedef enum
    {
        FormatBlackWhite,   /**< One bit per pixel 1 = black 0 = white */
        FormatGrayScale8,   /**< Grayscale with one byte per pixel 0 = black 255 = white */
        FormatGrayScale16,  /**< Grayscale withtTwo bytes per pixel.
                             * The byte order is the one provided by libsane. */
        FormatRGB_8_C,      /**< Every pixel consists of three colors in the order Read,
                             * Grean and Blue, with one byte per color (no alpha channel). */
        FormatRGB_16_C,     /**< Every pixel consists of three colors in the order Read,
                             * Grean and Blue, with two bytes per color(no alpha channel).
                             * The byte order is the one provided by libsane. */
        FormatBMP,          /**< The image data  is returned as a BMP. */
        FormatNone = 0xFFFF /**< This enumeration value should never be returned to the user */
    } ImageFormat;

    /** @note There might come more enumerations in the future. */
    typedef enum
    {
        NoError,            /**< The scanning was finished successfully.*/
        ErrorCannotSegment, /**< If this error status is returned libksane can not segment the
                             * returned data. Scanning without segmentation should work.
                             * @note segmentation is not implemented yet.*/
        ErrorGeneral,        /**< The error string should contain an error message. */
        Information          /**< There is some information to the user. */
    } ScanStatus;
    
    struct DeviceInfo
    {
        QString name;     /* unique device name */
        QString vendor;   /* device vendor string */
        QString model;    /* device model name */
        QString type;     /* device type (e.g., "flatbed scanner") */
    };

    /** This constructor initializes the private class variables, but the widget is left empty.
     * The options and the preview are added with the call to openDevice(). */
    KSaneWidget(QWidget* parent=0);

    /** Standard destructor */
    ~KSaneWidget();

    /** This helper method displays a dialog for selecting a scanner. The libsane
     * device name of the selected scanner device is returned. */
    QString selectDevice(QWidget* parent=0);

    /**
     * Get the list of available scanning devices. Connect to availableDevices()
     * which is fired once these devices are known.
     */
    void initGetDeviceList() const;

    /** This method opens the specified scanner device and adds the scan options to the
     * KSane widget.
     * @param device_name is the libsane device name for the scanner to open.
     * @return 'true' if all goes well and 'false' if the specified scanner can not be opened. */
    bool openDevice(const QString &device_name);

    /** This method closes the currently open scanner device.
    * @return 'true' if all goes well and 'false' if no device is open. */
    bool closeDevice();
    
    KDE_DEPRECATED bool makeQImage(const QByteArray &, int, int, int, ImageFormat, QImage &);
     
    /**
     * This is a convenience method that can be used to create a QImage from the image data
     * returned by the imageReady(...) signal. 
     * @note: If the image data has 16 bits/color the * data is truncated to 8 bits/color 
     * since QImage does not support 16 bits/color. A warning message will be shown.
     *
     * @param data is the byte data containing the image.
     * @param width is the width of the image in pixels.
     * @param height is the height of the image in pixels.
     * @param bytes_per_line is the number of bytes used per line. This might include padding
     * and is probably only relevant for 'FormatBlackWhite'.
     * @param format is the KSane image format of the data.
     * @return This function returns the provided image data as a QImage. */
    QImage toQImage(const QByteArray &data,
                    int width,
                    int height,
                    int bytes_per_line,
                    ImageFormat format);

    /**
     * This is a convenience method that can be used to create a QImage from the image data
     * returned by the imageReady(...) signal. 
     * @note: If the image data has 16 bits/color the * data is truncated to 8 bits/color, but
     * unlike toQImage() this function will not give a warning.
     *
     * @param data is the byte data containing the image.
     * @param width is the width of the image in pixels.
     * @param height is the height of the image in pixels.
     * @param bytes_per_line is the number of bytes used per line. This might include padding
     * and is probably only relevant for 'FormatBlackWhite'.
     * @param format is the KSane image format of the data.
     * @return This function returns the provided image data as a QImage. */
    QImage toQImageSilent(const QByteArray &data,
                    int width,
                    int height,
                    int bytes_per_line,
                    ImageFormat format);

    /** This method returns the vendor name of the scanner (Same as make). */
    QString vendor() const;
    /** This method returns the make name of the scanner. */
    QString make() const;
    /** This methos returns the model of the scanner. */
    QString model() const;

    /** This method returns the current resolution of the acquired image,
    * in dots per inch.
    * @note This function should be called from the slot connected
    * to the imageReady signal. The connection should not be queued.
    * @return the resolution used for scanning or 0.0 on failure. */
    float currentDPI();

    /** This method returns the scan area's width in mm
    * @return Width of the scannable area in mm */
    float scanAreaWidth();

    /** This method returns the scan area's height in mm
    * @return Height of the scannable area in mm */
    float scanAreaHeight();

    /** This method sets the selection according to the given points
    * @note The points are defined with respect to the scan areas top-left corner in mm
    * @param topLeft Upper left corner of the selection (in mm)
    * @param bottomRight Lower right corner of the selection (in mm) */
    void setSelection(QPointF topLeft, QPointF bottomRight);
    
    /** This function is used to set the preferred resolution for scanning the preview.
    * @param dpi is the wanted scan resolution for the preview
    * @note if the set value is not supported, the cloasest one is used
    * @note setting the value 0 means that the default calculated value should be used */
    void setPreviewResolution(float dpi);

    /** This method reads the available parameters and their values and
     * returns them in a QMap (Name, value)
     * @param opts is a QMap with the parameter names and values. */
    void getOptVals(QMap <QString, QString> &opts);

    /** This method can be used to write many parameter values at once.
     * @param opts is a QMap with the parameter names and values.
     * @return This function returns the number of successful writes. */
    int setOptVals(const QMap <QString, QString> &opts);

    /** This function reads one parameter value into a string.
     * @param optname is the name of the parameter to read.
     * @param value is the string representation of the value.
     * @return this function returns true if the read was successful. */
    bool getOptVal(const QString &optname, QString &value);

    /** This function writes one parameter value into a string.
     * @param optname is the name of the parameter to write.
     * @param value is the string representation of the value.
     * @return this function returns true if the write was successful. */
    bool setOptVal(const QString &optname, const QString &value);

    /** This function sets the label on the final scan button
    * @param scanLabel is the new label for the button. */
    void setScanButtonText(const QString &scanLabel);

    /** This function sets the label on the preview button
    * @param previewLabel is the new label for the button. */
    void setPreviewButtonText(const QString &previewLabel);

    /** This function can be used to enable/disable automatic selections on previews.
    * The default state is enabled.
    * @param enable specifies if the auto selection should be turned on or off. */
    void enableAutoSelect(bool enable);

    /** This function is used to programatically collapse/restore the options.
    * @param collapse defines the state to set. */
    void setOptionsCollapsed(bool collapse);

    /** This function is used hide/show the final scan button.
    * @param hidden defines the state to set. */
    void setScanButtonHidden(bool hidden);

public Q_SLOTS:
    /** This method can be used to cancel a scan or prevent an automatic new scan. */
    void scanCancel();

    /** This method can be used to start a scan (if no GUI is needed).
    * @note libksane may return one or more images as a result of one invocation of this slot.
    * If no more images are wanted scanCancel should be called in the slot handling the
    * imageReady signal. */
    void scanFinal();

Q_SIGNALS:
    /**
     * This Signal is emitted when a final scan is ready.
     * @param data is the byte data containing the image.
     * @param width is the width of the image in pixels.
     * @param height is the height of the image in pixels.
     * @param bytes_per_line is the number of bytes used per line. This might include padding
     * and is probably only relevant for 'FormatBlackWhite'.
     * @param format is the KSane image format of the data. */
    void imageReady(QByteArray &data, int width, int height,
                    int bytes_per_line, int format);

    /**
     * This signal is emitted when the scanning has ended.
     * @param status contains a ScanStatus status code.
     * @param strStatus If an error has occurred this string will contain an error message.
     * otherwise the string is empty. */
    void scanDone(int status, const QString &strStatus);

    /**
     * This signal is emitted when the user is to be notified about something.
     * @note If no slot is connected to this signal the message will be displayed in a KMessageBox.
     * @param type contains a ScanStatus code to identify the type of message (error/info/...).
     * @param msg is the message to the user.
     * otherwise the string is empty. */
    void userMessage(int type, const QString &strStatus);

    /**
     * This Signal is emitted for progress information during a scan.
     * The GUI already has a progress bar, but if the GUI is hidden,
     * this can be used to display a progress bar.
     * @param percent is the percentage of the scan progress (0-100). */
    void scanProgress(int percent);

    /**
     * This signal is emitted every time the device list is updated or
     * after initGetDeviceList() is called.
     * @param deviceList is a QList of KSaneWidget::DeviceInfo that contain the
     * device name, model, vendor and type of the attached scanners.
     * @note The list is only a snapshot of the current available devices. Devices
     * might be added or removed/opened after the signal is emitted.
     */
    void availableDevices(const QList<KSaneWidget::DeviceInfo> &deviceList);

    /**
     * This Signal is emitted when a hardware button is pressed.
     * @param optionName is the untranslated technical name of the sane-option.
     * @param optionLabel is the translated user visible label of the sane-option.
     * @param pressed indicates if the value is true or false.
     * @note The SANE standard does not specify hardware buttons and their behaviors,
     * so this signal is emitted for sane-options that behave like hardware buttons.
     * That is the sane-options are read-only and type boolean. The naming of hardware
     * buttons also differ from backend to backend.
     */
    void buttonPressed(const QString &optionName, const QString &optionLabel, bool pressed);

private:

    KSaneWidgetPrivate * const d;
};

}  // NameSpace KSaneIface

#endif // SANE_WIDGET_H
