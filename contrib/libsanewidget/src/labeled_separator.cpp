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
#include <qlabel.h>

#include "labeled_separator.h"

LabeledSeparator::LabeledSeparator(QWidget *parent, const QString& ltext)
   : QFrame( parent )
{
    QHBoxLayout *hb = new QHBoxLayout(this);
    hb->setSpacing(2);
    hb->setMargin(0);
    QFrame *line1 = new QFrame(this);
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );

    QLabel *label = new QLabel(ltext, this);
    QFrame *line2 = new QFrame(this);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    hb->addWidget(line1,50);
    hb->addWidget(label);
    hb->addWidget(line2,50);
    //hb->activate();

}

LabeledSeparator::~LabeledSeparator()
{
}
