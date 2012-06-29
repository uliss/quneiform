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

#include <QLabel>

#include "openprogressdialog.h"
#include "imageutils.h"

OpenProgressDialog::OpenProgressDialog(QWidget * parent) :
    QProgressDialog(parent),
    page_(0),
    label_(NULL)
{
    setMinimum(0);
    setMaximum(0);
    setMinimumDuration(1000);

    label_ = new QLabel(this);
    label_->setAlignment(Qt::AlignLeft);
    label_->setTextFormat(Qt::PlainText);
    setLabel(label_);

    setFixedWidth(450);
    setWindowTitle(tr("Quneiform OCR - opening images"));
    setWindowModality(Qt::ApplicationModal);
    setModal(true);
}

void OpenProgressDialog::loadDone()
{
    setValue(value() + 1);
}

void OpenProgressDialog::start(const QStringList& files)
{
    reset();
    setMaximum(utils::imageCount(files));
    show();
}

void OpenProgressDialog::load(const QString& path, int page)
{
    current_path_ = path;
    page_ = page;

    updateLabel();
}

void OpenProgressDialog::setPath(const QString& path)
{
    current_path_ = path;
}

void OpenProgressDialog::setPage(int page)
{
    page_ = page;
}

QString OpenProgressDialog::elidedPath() const
{
    if(!label_)
        return QString();

    QFontMetrics fm = label_->fontMetrics();
    QString res = fm.elidedText(current_path_, Qt::ElideMiddle, width() / 1.5);
    return res;
}

void OpenProgressDialog::updateLabel()
{
    if(page_ < 0)
        setLabelText(tr("Opening image \"%1\"").arg(elidedPath()));
    else
        setLabelText(tr("Opening image \"%1\"\nPage: %2").arg(elidedPath()).arg(page_ + 1));
}


void OpenProgressDialog::stop()
{
    reset();
    hide();
}
