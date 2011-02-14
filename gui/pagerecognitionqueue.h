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


#ifndef PAGERECOGNITIONQUEUE_H
#define PAGERECOGNITIONQUEUE_H

#include <QObject>
#include <QQueue>
#include <QMap>

class Packet;
class Page;
class RecognitionProgressDialog;
class PageRecognizer;

class PageRecognitionQueue : public QObject
{
    Q_OBJECT
public:
    explicit PageRecognitionQueue(QObject * parent = 0);

    /**
      * Adds all packett pages to queue
      */
    void add(Packet * packet);

    /**
      * Adds page to recognition queue
      * if page already in queue - do nothing
      */
    void add(Page * p);

    /**
      * Returns recogition error for given page imagePath
      */
    QString getPageFault(const QString& imagePath) const;

    /**
      * Returns true is queue is empty
      * @see pageCount()
      */
    bool isEmpty() const;

    /**
      * Returns page count in queue
      * @see isEmpty()
      */
    int pageCount() const;

    /**
      * Sets recognition language
      */
    void setLanguage(int language);
public slots:
    void start();
signals:
    void finished();
    void started();
private slots:
    void pageFault(const QString& msg);
private:
    void clearPageFault(const QString& page);
    void setPageFault(const QString& page, const QString& msg);
    void setupProgressDialog();
    void setupPageRecognizer();
private:
    QQueue<Page*> pages_;
    RecognitionProgressDialog * progress_;
    PageRecognizer * recognizer_;
    QMap<QString, QString> page_fault_log_;
};

#endif // PAGERECOGNITIONQUEUE_H
