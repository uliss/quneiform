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

#ifndef LABELED_CHECKBOX_H
#define LABELED_CHECKBOX_H

#include <qframe.h>
#include <qcheckbox.h>
/**
  *@author Kåre Särs
  */

/**
 * A wrapper for a checkbox
 */
class LabeledCheckbox : public QFrame
{
    Q_OBJECT

public:
   /**
    * Create the checkbox.
    *
    * \param parent parent widget
    * \param text is the text describing the checkbox.
    */
    LabeledCheckbox(QWidget *parent, const QString& text);
    ~LabeledCheckbox();
    void setChecked(bool);

private slots:
    void prToggled(bool);

signals:
    void toggled(bool);

private:
    QCheckBox *chbx;
};


#endif
