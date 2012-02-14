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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>

#include "cimageview.h"
#include "cimagestorage.h"

CImageView::CImageView(QWidget *parent) :
    QDialog(parent),
    storage_(new CImageStorage)
{
    setWindowTitle("CImage view");
    init();

    QVBoxLayout * lv = new QVBoxLayout;
    QHBoxLayout * lh = new QHBoxLayout;
    lv->addLayout(lh);
    lh->addWidget(image_list_);
    lh->addWidget(image_);

    QDialogButtonBox * buttons = new QDialogButtonBox;
    QPushButton * ok = buttons->addButton(QDialogButtonBox::Ok);
    connect(ok, SIGNAL(clicked()), SLOT(close()));
    lv->addWidget(buttons);
    setLayout(lv);
}

CImageView::~CImageView()
{
    delete storage_;
}

void CImageView::init()
{
    image_list_ = new QListWidget;
    foreach(QString s, storage_->images()) {
        image_list_->addItem(s);
    }

    connect(image_list_, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(showImage(QListWidgetItem*)));

    image_ = new QLabel;
}

void CImageView::showImage(QListWidgetItem * item)
{
    qDebug() << Q_FUNC_INFO << item->text();

    if(item->text().isEmpty())
        return;

    image_->setPixmap(storage_->pixmap(item->text()));
}
