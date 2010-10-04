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

#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "document.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
	connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openImages()));

	doc_ = new Document(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::about() {
	QMessageBox::about(0, "About", "Cuneiform GUI");
}

void MainWindow::openImages() {
	QStringList files = QFileDialog::getOpenFileNames(NULL, "Open Dialog", "", "Images (*.gif *.png *.xpm *.jpg)");
}
