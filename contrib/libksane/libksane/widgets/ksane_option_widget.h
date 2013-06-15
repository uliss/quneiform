/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Description : Base class for option widgets
 *
 * Copyright (C) 2011 by Kare Sars <kare.sars@iki.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================ */

#ifndef KSANE_OPTION_WIDGET_H
#define KSANE_OPTION_WIDGET_H

// Qt includes.
#include <QWidget>
#include <QLabel>
#include <QGridLayout>

namespace KSaneIface
{

/**
  *@author Kåre Särs
  */

/**
 * A wrapper for a checkbox
 */
class KSaneOptionWidget : public QWidget
{
    Q_OBJECT

public:
   /**
    * Create the Wodget.
    * \param parent parent widget
    * \param Label text.
    */
    KSaneOptionWidget(QWidget *parent, const QString &labelText);
    ~KSaneOptionWidget();

    void setLabelText(const QString &text);

    int labelWidthHint();
    void setLabelWidth(int labelWidth);

protected:
    QLabel      *m_label;
    QGridLayout *m_layout;
};

}  // NameSpace KSaneIface

#endif // KSANE_OPTION_WIDGET_H
