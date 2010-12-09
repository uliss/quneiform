/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#ifndef PAGE_H_
#define PAGE_H_

#include <stdexcept>
#include <QString>
#include <QObject>
#include <QRect>
#include <QSize>
#include <QColor>
#include <QTransform>
#include <QMutex>
#include "recognitionsettings.h"

class QDataStream;

class Page: public QObject {
    Q_OBJECT
public:
    Page(const QString& image_path);

    typedef std::runtime_error Exception;

    /**
      * Returns page rotation angle (0, 90, 180 or 270 degrees)
      */
    int angle() const;

    /**
      * Returns image path
      */
    QString imagePath() const;

    /**
      * Returns image size
      */
    QSize imageSize() const;

    /**
      * Returns true if points to valid image
      * otherwise returns false
      */
    bool isNull() const;

    /**
      * Returns true if image recognized
      */
    bool isRecognized() const;

    /**
      * Returns true if image already saved
      */
    bool isSaved() const;

    /**
      * Returns true if page is selected
      */
    bool isSelected() const;

    /**
      * Returns page number
      */
    unsigned int number() const;

    /**
      * Returns OCR text
      */
    QString ocrText() const;

    /**
      * Returns page area on image
      */
    const QRect& pageArea() const;

    /**
      * Returns page recognize options
      */
    const RecognitionSettings& recognitionSettings() const;

    /**
      * Resets page scaling, but saves rotate
      * Emits signals changed() and transformed()
      */
    void resetScale();

    /**
      * Rotates page
      * Emits signals changed() and rotated()
      */
    void rotate(int angle);

    /**
      * Saves ocr result
      * @throw Exception if page is not recognized
      * Emits signal saved()
      */
    void save(const QString& file);

    /**
      * Scales page
      * Emits signals changed() and transformed()
      */
    void scale(qreal factor);

    /**
      * Sets page number
      * Emits signal changed()
      */
    void setNumber(unsigned int number);

    /**
      * Sets page ocr text
      * Emits signal changed() and recognized()
      */
    void setOcrText(const QString& text);

    /**
      * Sets page area on image
      * Emits signal changed()
      */
    void setPageArea(const QRect& area);

    /**
      * Sets page recognize options
      * emits signal changed()
      */
    void setRecognizeOptions(const RecognitionSettings& opts);

    /**
      * Selects page
      * Emits signal changed()
      */
    void setSelected(bool value);

    /**
      * Sets page transform
      * Emits signals changed() and transformed()
      */
    void setTransform(const QTransform& t);

    /**
      * Sets page view scroll
      */
    void setViewScroll(const QPoint& pt);

    /**
      * Returns page transform
      */
    QTransform transform() const;

    /**
      * Returns page view scroll
      */
    QPoint viewScroll() const;
signals:
    void changed();
    /**
      * Emmited when page is transformed
      */
    void transformed();

    /**
      * Emitted when page is recognized
      */
    void recognized();

    /**
      * Emmitted when page is rotated
      */
    void rotated(int angle);

    /**
      * Emitted when page saved
      */
    void saved();
private:
    QString image_path_;
    QSize image_size_;
    QString ocr_text_;
    unsigned int number_;
    bool is_recognized_;
    bool is_saved_;
    bool is_selected_;
    QRect page_area_;
    QTransform transform_;
    QPoint view_scroll_;
    bool is_null_;
    mutable QMutex mutex_;
    RecognitionSettings rec_settings_;
public:
    friend QDataStream& operator<<(QDataStream& stream, const Page& page);
    friend QDataStream& operator>>(QDataStream& stream, Page& page);
    friend class PageRecognizer;
};

QDataStream& operator<<(QDataStream& stream, const Page& page);
QDataStream& operator>>(QDataStream& stream, Page& page);

#endif /* PAGE_H_ */
