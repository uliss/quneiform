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
#include <QFileInfo>
#include <QPushButton>

#include "logviewerdialog.h"
#include "ui_logviewerdialog.h"

#include "guilog.h"
#include "iconutils.h"

LogViewerDialog::LogViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::LogViewerDialog)
{
    ui_->setupUi(this);
    QPushButton * clear_btn = ui_->buttonBox->addButton(tr("Clear"), QDialogButtonBox::ResetRole);
    connect(clear_btn, SIGNAL(clicked()), SLOT(handleLogClear()));

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

    ui_->logContent->setPlainText(QString::fromUtf8(file.readAll()));
    ui_->logContent->setEnabled(true);

    file.close();
}

void LogViewerDialog::handleLogClear()
{
    clearLog(guiDebugLogPath());
    clearLog(guiWarningLogPath());
    clearLog(guiWarningLogPath());
    clearLog(guiFatalLogPath());

    updateCurrentLog();
}

bool LogViewerDialog::clearLog(const QString& path)
{
    QFile log(path);
    if(!log.exists())
        return false;

    if(!log.open(QFile::Truncate | QFile::WriteOnly))
        return false;

    log.close();
    return true;
}

void LogViewerDialog::initLogList()
{
    addLogItem(guiDebugLogPath(), "Debug", "dialog-information");
    addLogItem(guiWarningLogPath(), "Warning", "dialog-warning");
    addLogItem(guiCriticalLogPath(), "Critical", "dialog-error");
    addLogItem(guiFatalLogPath(), "Fatal", "dialog-fatal");

    connect(ui_->logList,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            SLOT(handleLogShow(QListWidgetItem*)));


    ui_->logList->setFixedWidth(100);
}

void LogViewerDialog::addLogItem(const QString& path, const QString& title, const QString& iconName)
{
    QFileInfo info(path);
    if(!info.exists())
        return;

    if(info.size() == 0)
        return;

    QListWidgetItem * item = new QListWidgetItem;
    item->setText(title);
    item->setIcon(iconFromTheme(iconName));
    item->setToolTip(path);
    item->setData(Qt::UserRole, path);
    ui_->logList->addItem(item);
}

void LogViewerDialog::updateCurrentLog()
{
    if(ui_->logList->count() == 0)
        return;

    handleLogShow(ui_->logList->currentItem());
}
