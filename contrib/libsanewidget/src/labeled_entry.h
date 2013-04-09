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

#ifndef LABELED_ENTRY_H
#define LABELED_ENTRY_H

#include <qframe.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>

/**
 *@author Kåre Särs
 */

/**
 * A text entry field with a set and reset button
 */
class LabeledEntry : public QFrame
{
    Q_OBJECT

public:
   /**
    * Create the entry.
    *
    * \param parent parent widget
    * \param text is the text describing the entry.
    */
    LabeledEntry(QWidget *parent, const QString& text);
    ~LabeledEntry();

    void setText(const QString& text);

private slots:
    void setClicked(void);
    void resetClicked(void);

signals:
    void entryEdited(const QString& text);

private:
    QLabel *label;
    QLineEdit *entry;
    QPushButton *set;
    QPushButton *reset;
    QString e_text;
};


#endif
