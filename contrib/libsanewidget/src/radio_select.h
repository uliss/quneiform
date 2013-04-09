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

#ifndef RADIO_SELECT_H
#define RADIO_SELECT_H

#include <QGroupBox>
#include <QDialog>
#include <QStringList>

class RadioSelect : public QDialog
{
    Q_OBJECT

    public:
        RadioSelect(QWidget *parent=0);
        ~RadioSelect(void) {};

        int getSelectedIndex(QWidget *parent,
                             const QString& group_name,
                             const QStringList& items,
                             int default_index=0);

    private:
        QGroupBox *radio_group;
};

#endif

