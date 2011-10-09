/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include <QGraphicsSceneEvent>
#include <QMimeData>
#include <QUrl>
#include <QDebug>

#include "thumbscene.h"

void ThumbScene::dragEnterEvent(QGraphicsSceneDragDropEvent * event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();

    qDebug() << Q_FUNC_INFO;
}

void ThumbScene::dragMoveEvent(QGraphicsSceneDragDropEvent * event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }

    qDebug() << Q_FUNC_INFO;
}

void ThumbScene::dropEvent(QGraphicsSceneDragDropEvent * event) {
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();

        QStringList paths;
        foreach(QUrl url, event->mimeData()->urls()) {
            paths << url.toLocalFile();
        }

        emit dropImages(paths);
    }
}
