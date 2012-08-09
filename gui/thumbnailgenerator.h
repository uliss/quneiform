/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QList>

class Page;

class ThumbnailGenerator : public QObject
{
    Q_OBJECT
public:
    ThumbnailGenerator(QObject * parent = 0);
    ~ThumbnailGenerator();

    QList<Page*> pages() const;
    void setPages(const QList<Page*>& pages);

    /**
     * Cancels thumb generation thread
     * @see run()
     */
    void cancel();

    /**
     * Returns ture if thumb generation thread is running
     * @see run()
     */
    bool isRunning() const;

    /**
     * Starts thumb generation thread
     * @see isRunning()
     */
    void run();
signals:
    void finished();
    void started();
private:
    QList<Page*> pages_;
    QFuture<void> job_;
    QFutureWatcher<void> watcher_;
};

#endif // THUMBNAILGENERATOR_H
