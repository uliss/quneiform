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

#include <qlayout.h>

#include "labeled_entry.h"

LabeledEntry::LabeledEntry(QWidget *parent, const QString& ltext)
   : QFrame(parent)
{
    QHBoxLayout *hb = new QHBoxLayout(this);
    hb->setSpacing(2);
    hb->setMargin(0);
    label = new QLabel(ltext, this);
    entry = new QLineEdit(this);
    reset = new QPushButton(this);
    reset->setText("Reset");
    set = new QPushButton(this);
    set->setText("Set");

    hb->addWidget(label);
    hb->addWidget(entry);
    hb->addWidget(reset);
    hb->addWidget(set);
    hb->activate();

    connect(reset, SIGNAL(clicked(void)), this, SLOT(resetClicked(void)));
    connect(set, SIGNAL(clicked(void)), this, SLOT(setClicked(void)));
}

LabeledEntry::~LabeledEntry()
{
}

void LabeledEntry::setText(const QString& text)
{
    e_text = text;
    entry->setText(text);
}

void LabeledEntry::resetClicked(void)
{
    entry->setText(e_text);
}

void LabeledEntry::setClicked(void)
{
    e_text = entry->text();
    emit entryEdited(e_text);
}

