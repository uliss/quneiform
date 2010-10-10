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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScopedPointer>
#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsScene>

namespace Ui {
class MainWindow;
}

class Document;
class LanguageSelect;
class Page;

class MainWindow: public QMainWindow {
Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void closeEvent(QCloseEvent *event);
public slots:
	void about();
    void openImage(const QString& path);
    void openImages();
    void recognizeAll();
    void recognizePage(Page * page);
    void rotate(int factor);
    void rotateLeft();
    void rotateRight();
    void showPageImage(Page * page);
    void showPageText(Page * page);
private:
    void clearScene();
    void createActions();
    void readSettings();
    void setupUi();
    void writeSettings();
    void setZoomEnabled(bool value);
private:
    QScopedPointer<Ui::MainWindow> ui_;
    Document * doc_;
    QGraphicsScene scene_;
    LanguageSelect * lang_select_;
};

#endif // MAINWINDOW_H
