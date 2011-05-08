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
#include <QMutex>

class Packet;
class Page;
class PageRecognizer;

class PageRecognitionQueue : public QObject
{
    Q_OBJECT
public:
    explicit PageRecognitionQueue(QObject * parent = NULL);

    /**
      * Adds all packet pages to queue
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
    QString pageError(const QString& imagePath) const;

    /**
      * Returns true is queue is empty
      * @see pageCount()
      */
    bool isEmpty() const;

    /**
      * Returns true if page recognition failed
      * @see failed()
      */
    bool isFailed(const QString& path) const;

    /**
      * Returns current page count in queue
      * @see isEmpty()
      */
    int pageCount() const;

    /**
      * Returns numbers of error during recognition
      */
    int pageErrorNum() const;

    /**
      * Returns pointer to page recognizer
      */
    PageRecognizer * recognizer();
public slots:
    /**
      * Tries to abort recognition process
      * @see start()
      */
    void abort();

    /**
      * Runs recognition process
      * @see abort(), started()
      */
    void start();
signals:
    /**
      * Emmitted on page recognition error
      * @param msg - error message
      */
    void failed(const QString& msg);

    /**
      * Emitted when recognition queue is finished
      * @param number - number of processed pages
      */
    void finished(int number);

    /**
      * Emitted when each page recognition in queue started
      * @param name - page name
      */
    void pageStarted(const QString& name);

    /**
      * Emitted when some part of job is done
      * @param perc - percent of work (from 0 to 100)
      */
    void percentDone(int perc);

    /**
      * Emitted when page queue started
      */
    void started();
private slots:
    void handleFail(const QString& msg);
    void handlePagePercents(int perc);
private:
    void clearPageFault(const QString& page);
    void handlePageStep(int pageStep);
    void setPageFault(const QString& page, const QString& msg);
private:
    /* queue pages */
    QQueue<Page*> pages_;
    QMap<QString, QString> page_errors_;
    /* lock for whole queue */
    QMutex queue_lock_;
    /* lock for abort only */
    QMutex abort_lock_;
    PageRecognizer * recognizer_;
    /* initial page count right before recognition start */
    int page_count_;
    /* abort flag */
    volatile bool abort_;
    /* number of errors during recognition */
    int page_error_num_;
};

#endif // PAGERECOGNITIONQUEUE_H
