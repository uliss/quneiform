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

#ifndef PACKET_H_
#define PACKET_H_

#include <QList>
#include <QObject>
#include "language.h"

class Page;
typedef QList<Page*>    PageList;

class Packet: public QObject
{
    Q_OBJECT
public:
    Packet(QObject * parent = 0);

    /**
      * Appends page
      * @param allowDuplication if true - emit imageDuplicated and do nothing
      * emits pageAdded(Page*) and changed() signals
      */
    void append(Page * page, bool allowDuplication = false);

    /**
      * Clears packet. Removes all pages
      * emits pageRemoved(Page*) when each page removed
      * and changed if at least one page was removed
      * @see remove(), removeSelected()
      */
    void clear();

    /**
      * Returns packet filename
      * if document is not saved returns default filename: "untitled.qfp"
      */
    QString fileName() const;

    /**
      * Returns pointer to first page in packet
      * if empty returns NULL
      * @see pageAt()
      */
    Page * firstPage();

    /**
      * Checks if packet contains page with given image file path
      * @return true if such page exists
      */
    bool hasPage(const QString& path) const;

    /**
      * Returns true if packet changed
      */
    bool isChanged() const;

    /**
      * Returns true if packet has no pages
      */
    bool isEmpty() const;

    /**
      * Returns true if packet is new and never was saved
      */
    bool isNew() const;

    void move(int pos, Page * page);
    void moveBefore(Page * page, Page * target);

    /**
      * Opens packet from filename
      * current document replaced by new one
      * @return true if packet opened with success
      * @see save()
      */
    bool open(const QString& filename);

    /**
      * Returns page at given position
      * @return NULL if page not found
      * @see pageCount()
      */
    Page * pageAt(int index);

    /**
      * Returns number of pages in document
      * @see page()
      */
    int pageCount() const;

    /**
      * Saves packet to file
      * emits signal saved()
      * @return true on success, false on saving error
      * @see open()
      */
    bool save(const QString& filename);

    PageList pages() const { return pages_;  }
signals:
    /**
      * Emitted when packet changed
      */
    void changed();

    /**
      * Emitted when page with such imagePath already exists in document
      */
    void imageDuplicated(const QString& imagePath);

    /**
      * Emitted while loading packet
      * @param percent - percent done
      */
    void loaded(int percent);

    /**
      * Emitted when packet was opened
      */
    void opened();

    /**
      * Emitted when new page added
      */
    void pageAdded(Page * page);

    /**
      * Emitted when page removed
      */
    void pageRemoved(Page * page);

    /**
      * Emitted on page reorder
      */
    void reordered();

    /**
      * Emitted when packet saved
      */
    void saved();
public  slots:
    /**
      * Removes page from list
      * If page not found do nothing
      */
    void remove(Page * page);
private slots:
    void pageChange();
private:
    PageList pages_;
    QString filename_;
    bool changed_;
    bool is_new_;
public:
    friend QDataStream& operator<<(QDataStream& stream, const Packet& doc);
    friend QDataStream& operator>>(QDataStream& stream, Packet& doc);
};

QDataStream& operator<<(QDataStream& stream, const Packet& doc);
QDataStream& operator>>(QDataStream& stream, Packet& doc);


#endif /* PACKET_H_ */
