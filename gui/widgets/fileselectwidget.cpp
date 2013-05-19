/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>

#include "fileselectwidget.h"

FileSelectWidget::FileSelectWidget(QWidget *parent, const QString& dir) :
    QWidget(parent),
    line_edit_(NULL),
    dir_(dir),
    title_(tr("Select file"))
{
    line_edit_ = new QLineEdit;
    line_edit_->setMinimumWidth(200);

    QPushButton * btn_select = new QPushButton(tr("Select"));

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(line_edit_);
    layout->addWidget(btn_select);
    setLayout(layout);

    connect(line_edit_, SIGNAL(textChanged(QString)), this, SIGNAL(textChanged(QString)));
    connect(btn_select, SIGNAL(pressed()), this, SLOT(showSelectDialog()));
}

QString FileSelectWidget::directory() const
{
    return dir_;
}

void FileSelectWidget::setDirectory(const QString& dir)
{
    dir_ = dir;
}

QString FileSelectWidget::path() const
{
    Q_CHECK_PTR(line_edit_);
    return line_edit_->text();
}

void FileSelectWidget::setPath(const QString& path)
{
    Q_CHECK_PTR(line_edit_);
    line_edit_->setText(path);
}

QString FileSelectWidget::title() const
{
    return title_;
}

void FileSelectWidget::setTitle(const QString& title)
{
    title_ = title;
}

void FileSelectWidget::showSelectDialog()
{
    QString path = QFileDialog::getOpenFileName(this, title_, dir_);

    activateWindow();

    if(!path.isNull())
        line_edit_->setText(path);
}
