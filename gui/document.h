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

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <QtCore/QList>
#include <QtCore/QObject>


class Page;
typedef QList<Page*> PageList;

class Document: public QObject
{
    Q_OBJECT
public:
    Document(QObject * parent = 0);
    ~Document();

    /**
      * Appends page
      * @param allowDuplication if true - emit imageDuplicated and do nothing
      * emits pageAdded(Page*) and changed() signals
      */
    void append(Page * page, bool allowDuplication = false);

    /**
      * Clears document. Removes all pages
      * emits pageRemoved(Page*) when each page removed
      * and changed if at least one page was removed
      * @see remove(), removeSelected()
      */
    void clear();

    /**
      * Returns numbers of selected pages
      */
    int countSelected() const;

    /**
      * Returns document filename
      * if document is not saved returns default filename: "untitled.qfp"
      */
    QString fileName() const;

    /**
      * Checks if document contains page with given image file path
      * @return true if such page exists
      */
    bool hasPage(const QString& path) const;

    /**
      * Returns true if document changed
      */
    bool isChanged() const;

    /**
      * Returns true if document is new and never was saved
      */
    bool isNew() const;

    /**
      * Returns document language
      * @see setLanguage()
      */
    int language() const;

    /**
      * Opens document from filename
      * current document replaced by new one
      * @return true if document opened with success
      * @see save()
      */
    bool open(const QString& filename);

    /**
      * Returns page at given position
      * @return NULL if page not found
      * @see pageCount()
      */
    Page * page(int index);

    /**
      * Returns number of pages in document
      * @see page()
      */
    int pageCount() const;

    /**
      * Saves document to file
      * emits signal saved()
      * @return true on success, false on saving error
      * @see open()
      */
    bool save(const QString& filename);

    /**
      * Sets document language
      * emits signal changed()
      * @see language()
      */
    void setLanguage(int lang);
signals:
    /**
      * Emitted when document changed
      */
    void changed();

    /**
      * Emitted when page with such imagePath already exists in document
      */
    void imageDuplicated(const QString& imagePath);

    /**
      * Emitted when new page added
      */
    void pageAdded(Page * page);

    /**
      * Emitted when page removed
      */
    void pageRemoved(Page * page);

    /**
      * Emitted when document saved
      */
    void saved();
public  slots:
    /**
      * Removes page from list
      * If page not found do nothing
      */
    void remove(Page * page);

    /**
      * Removes selected pages
      */
    void removeSelected();
private slots:
    void pageChange();
    void pageSave();
private:
    PageList pages_;
    QString filename_;
    int language_;
    bool changed_;
    bool is_new_;
public:
    friend QDataStream& operator<<(QDataStream& stream, const Document& doc);
    friend QDataStream& operator>>(QDataStream& stream, Document& doc);
};

QDataStream& operator<<(QDataStream& stream, const Document& doc);
QDataStream& operator>>(QDataStream& stream, Document& doc);


#endif /* DOCUMENT_H_ */
