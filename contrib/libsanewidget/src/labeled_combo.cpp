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

#include <QComboBox>
#include <QLabel>

#include "labeled_combo.h"

LabeledCombo::LabeledCombo(QWidget *parent, const QString& ltext, const QStringList& list)
: QFrame(parent)
{
    layout = new QGridLayout(this);
    layout->setSpacing(3);
    layout->setMargin(0);
    label = new QLabel(ltext, this);
    combo = new QComboBox(this);
    combo->addItems(list);

    connect(combo, SIGNAL(activated(const QString &)),
            this, SLOT(prActivated(const QString &)));
    connect(combo, SIGNAL(activated(int)),
            this, SLOT(prActivated(int)));

    layout->addWidget(label, 0, 0);
    layout->addWidget(combo, 0, 1);
    layout->setColumnStretch(0, 50);
    layout->setColumnStretch(1, 0);
}


void LabeledCombo::setCurrentText(const QString &t)
{
    int i;
    for (i=0; i<combo->count(); i++) {
        if (combo->itemText(i) == t) {
            combo->setCurrentIndex(i);
        }
    }
}

bool LabeledCombo::setIcon(const QIcon &icon, const QString& str)
{
    for (int i=0; i < combo->count(); i++)
    {
        if (combo->itemText(i) == str)
        {
            combo->setItemIcon(i, icon);
            return true;
        }
    }
    return false;
}

QString LabeledCombo::currentText(void) const
{
    return(combo->currentText());
}


QString LabeledCombo::text(int i) const
{
    return(combo->itemText(i));
}

void LabeledCombo::setCurrentIndex(int i)
{
    combo->setCurrentIndex(i);
}


void LabeledCombo::wigetSizeHints(int *lab_w, int *cmb_w)
{
    if (lab_w != 0) *lab_w = label->sizeHint().width();
    if (cmb_w != 0) *cmb_w = combo->sizeHint().width();
    //printf("lab_w=%d, cmb_w=%d\n", *lab_w, *cmb_w);
}

void LabeledCombo::setColumnWidths(int lab_w, int cmb_w)
{
    layout->setColumnMinimumWidth(0, lab_w);
    layout->setColumnMinimumWidth(1, cmb_w);
}

int LabeledCombo::count(void) const
{
    return(combo->count());
}

void LabeledCombo::prActivated(const QString &t)
{
    emit activated(t);
}

void LabeledCombo::prActivated(int i)
{
    emit activated(i);
}

