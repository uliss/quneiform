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

#include <QMenu>
#include <QContextMenuEvent>

#include "imagegraphicsview.h"

ImageGraphicsView::ImageGraphicsView(QWidget * parent) :
    QGraphicsView(parent)
{
    setBackgroundRole(QPalette::Dark);
}

void ImageGraphicsView::contextMenuEvent(QContextMenuEvent * event) {
    Q_CHECK_PTR(event);

    QMenu menu;
    menu.addAction(QIcon(":/img/oxygen/22x22/document_rotate_left.png"),
                   tr("Select recognize area"),
                   this, SLOT(selectPageArea()));
    menu.exec(event->globalPos());
}
