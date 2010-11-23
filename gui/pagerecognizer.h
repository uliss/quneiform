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

#include <QThread>
#include <QString>
#include <QImage>
#include <QMutex>

class Page;

class PageRecognizer : public QThread
{
    Q_OBJECT
public:
    PageRecognizer(Page * p, QObject * parent);

    /**
      * Abortes page recognition process
      * @note abort is not immidiate - only next recognition stage aborted
      */
    void abort();

    /**
      * Returns true if recognizer is stopped; otherwise returns false.
      */
    bool isPaused() const;

    /**
      * Returns pointer to recogized page
      */
    Page * page();

    /**
      * Locks recognition thread
      */
    void pause();

    /**
      * Unlocks recognition thread
      */
    void resume();

    /**
      * Sets recognition language
      */
    void setLanguage(int language);

    /**
      * Sets recognized page
      */
    void setPage(Page * p);
protected:
    void run();
signals:
    /**
      * Emitted if page recognition failed
      * @param msg - error message
      */
    void failed(const QString& msg);

    /**
      * Emitted when page formatting is done.
      */
    void formatted();

    /**
      * Emmitted when page image is opened and loaded
      */
    void opened();

    /**
      * Emitted when page is recognized
      */
    void recognized();
private:
    void doRecognize();
    void formatResult();
    QImage loadImage() const;
    void openImage();
    void recognize();
    void saveOcrText();
    void setRecognizeOptions();
private:
    Page * page_;
    int language_;
    QMutex pause_;
    volatile bool paused_;
    volatile bool aborted_;
};

#endif // PAGERECOGNIZER_H
