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
#include <QScopedPointer>

class Page;

namespace cf {
class AbstractRecognitionServer;
class PercentCounter;
class RecognitionState;
}

class PageRecognizer : public QObject
{
    Q_OBJECT
public:
    enum WorkerType {
        RUNTIME,  // worker can be local or process, depends from QSettings at runtime moment
        LOCAL,    // worker runs in the same process and thread as calling process
        PROCESS   // worker runs in separate process
    };
public:
    PageRecognizer(QObject * parent = NULL, WorkerType type = LOCAL);
    ~PageRecognizer();

    /**
      * Returns current page path
      */
    QString pagePath() const;

    /**
      * Sets recognized page
      */
    void setPage(Page *p);

    /**
     * Sets worker type
     * @see workerType()
     */
    void setWorkerType(WorkerType t);
public slots:
    /**
      * Tries to abort recognition process
      */
    void abort();

    /**
     * Analyzes page layout and makes page markup
     */
    bool analyze();

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
private slots:
    void handleFail(const QString& msg);
private:
    void exportPageText();
    bool exportPageLayout(cf::AbstractRecognitionServer * server);
    bool importPageLayout(const cf::AbstractRecognitionServer * server);
    void handleRecognitionProgress(unsigned char percentsDone);
    void handleRecognitionState(int);
    bool loadImage();
    void setConfigOptions();
    void saveResolutionHeightHistogram(const std::vector<int>& hist);
    void saveResolutionWidthHistogram(const std::vector<int>& hist);
    WorkerType workerType() const;
private:
    Page * page_;
    QScopedPointer<cf::PercentCounter> counter_;
    QScopedPointer<cf::RecognitionState> recog_state_;
    QMutex lock_;
    volatile bool abort_;
    WorkerType worker_type_;
    QImage image_;
};

#endif // PAGERECOGNIZER_H
