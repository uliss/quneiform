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

#ifndef PAGERECOGNIZER_H
#define PAGERECOGNIZER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QMutex>
#include <QMap>

class Page;

class PageRecognizer : public QObject
{
    Q_OBJECT
public:
    PageRecognizer(QObject * parent = NULL);

    enum StageType {
        LOAD = 0,
        OPEN,
        RECOGNIZE,
        FORMAT
    };

    /**
      * Returns current page path
      */
    QString pagePath() const;

    /**
      * Sets recognized page
      */
    void setPage(Page *p);

    /**
      * Sleep stage
      */
    void setStageSleep(StageType t, int msec = 1000);
public slots:
    /**
      * Tries to abort recognition process
      */
    void abort();

    /**
      * Starts page recognition thread
      * @return true on success or false on abort or error
      */
    bool recognize();
signals:
    /**
      * Emitted when recognition is aborted
      */
    void aborted();

    /**
      * Emitted if recognition done
      */
    void done();

    /**
      * Emitted if page recognition failed
      * @param msg - error message
      */
    void failed(const QString& msg);

    /**
      * Emitted after page formatting is done.
      */
    void formatted();

    /**
      * Emitted after image is loaded into memory
      */
    void loaded();

    /**
      * Emmitted after page image is opened and loaded
      */
    void opened();

    /**
      * Emitted after some part of recognition process is done
      */
    void percentsDone(int done);

    /**
      * Emitted after page is recognized
      */
    void recognized();
private:
    void doRecognize();
    void formatResult();
    QImage loadImage();
    void open();
    void setFormatOptions();
    void setRecognizeOptions();
    void stageSleep(StageType t);
private:
    Page * page_;
    QMutex lock_;
    volatile bool abort_;
    QVector<int> stage_sleep_;
};

#endif // PAGERECOGNIZER_H
