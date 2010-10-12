/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef IMAGE_VIEW_H_
#define IMAGE_VIEW_H_

#include <QGraphicsView>
#include <QGraphicsScene>

class QGraphicsScene;
class Page;

class ImageView : public QGraphicsView {
	Q_OBJECT
public:
	ImageView(QWidget * parent);
	void clear();
	void showPage(Page * page);
	void setPage(Page * page);
public slots:
	void deletePage();
	void fitPage();
    void fitWidth();
    void originalSize();
    void zoomIn();
    void zoomOut();
private slots:
    void updatePage();
private:
    void connectPage();
    void disconnectPage();
    void saveTransform();
private:
    Page * page_;
    QGraphicsScene scene_;
};

#endif
