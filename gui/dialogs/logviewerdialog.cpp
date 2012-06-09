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

#include <QDebug>
#include <QFile>

#include "logviewerdialog.h"
#include "ui_logviewerdialog.h"

#include "guilog.h"

LogViewerDialog::LogViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::LogViewerDialog)
{
    ui_->setupUi(this);
    initLogList();

    ui_->logContent->setDisabled(true);
    ui_->logContent->setReadOnly(true);
    ui_->logContent->setCenterOnScroll(true);
    ui_->logContent->setLineWrapMode(QPlainTextEdit::NoWrap);
}

LogViewerDialog::~LogViewerDialog()
{
    delete ui_;
}

void LogViewerDialog::handleLogShow(QListWidgetItem * current)
{
    QVariant data = current->data(Qt::UserRole);

    if(!data.isValid())
        return;

    QFile file(data.toString());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui_->logContent->setPlainText(QString(file.readAll()));
    ui_->logContent->setEnabled(true);

    file.close();
}

void LogViewerDialog::initLogList()
{
    addLogItem(guiDebugLogPath(), "Debug", "messagebox_info.png");
    addLogItem(guiWarningLogPath(), "Warning", "messagebox_warning.png");
    addLogItem(guiCriticalLogPath(), "Critical", "messagebox_critical.png");
    addLogItem(guiFatalLogPath(), "Fatal", "messagebox_critical.png");

    connect(ui_->logList,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            SLOT(handleLogShow(QListWidgetItem*)));


    ui_->logList->setFixedWidth(100);
}

void LogViewerDialog::addLogItem(const QString& path, const QString& title, const QString& icon)
{
    if(!QFile::exists(path))
        return;

    QListWidgetItem * item = new QListWidgetItem;
    item->setText(title);
    item->setIcon(QIcon(":/img/oxygen/32x32/" + icon));
    item->setToolTip(path);
    item->setData(Qt::UserRole, path);
    ui_->logList->addItem(item);
}
