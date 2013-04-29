/*
   Copyright (C) 2007 Kåre Särs <kare.sars@kolumbus.fi>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <QGroupBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLayout>
#include <QDebug>

#include "radio_select.h"

RadioSelect::RadioSelect(QWidget * parent) :
    QDialog(parent)
{
    QVBoxLayout * layout = new QVBoxLayout;
    this->setLayout(layout);

    radio_group_ = new QGroupBox(this);
    layout->addWidget(radio_group_, 100);

    QDialogButtonBox * btn_box = new QDialogButtonBox(this);
    QPushButton * ok = btn_box->addButton(QDialogButtonBox::Ok);
    QPushButton * cancel = btn_box->addButton(QDialogButtonBox::Cancel);
    connect (ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect (cancel, SIGNAL(clicked()), this, SLOT(reject()));

    layout->addWidget(btn_box);
}

int RadioSelect::getSelectedIndex(QWidget * parent,
                                  const QString& group_name,
                                  const QStringList& items,
                                  int default_index)
{
    if(items.isEmpty())
        return -1;

    this->setParent(parent);
    radio_group_->setTitle(group_name);

    // Create the RadioButton list
    QList<QRadioButton *> btn_list;
    for (int i = 0; i < items.size(); i++) {
        btn_list.append(new QRadioButton(items.at(i), radio_group_));
    }

    // Add the device list to the layout
    QVBoxLayout * radio_layout = new QVBoxLayout(radio_group_);
    for (int i = 0; i < btn_list.size(); i++) {
        radio_layout->addWidget(btn_list.at(i));
    }

    int radio_index = default_index;
    if (radio_index >= btn_list.size())
        radio_index = btn_list.size() - 1;
    if (radio_index < 0)
        radio_index = 0;

    btn_list.at(radio_index)->toggle();

    // show the dialog and get the selection
    if (this->exec()) {
        // check which one is selected
        int i = 0;
        for (; i < btn_list.size(); i++) {
            if (btn_list.at(i)->isChecked())
                break;
        }

        if (i == btn_list.size()) {
            qWarning() << Q_FUNC_INFO << QString("This is a bad index: %1").arg(i);
            return -1;
        }
        return i;
    }

    return -1;
}


