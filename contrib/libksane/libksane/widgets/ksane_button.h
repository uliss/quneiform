/* ============================================================
 *
 * This file is part of the KDE project
 *
 * Copyright (C) 2008-2011 by Kare Sars <kare.sars@iki .fi>
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

#ifndef KSANE_BUTTON_H
#define KSANE_BUTTON_H

#include "ksane_option_widget.h"

// Qt includes.
#include <QPushButton>
#include <QGridLayout>

namespace KSaneIface
{

/**
  *@author Kåre Särs
  */

/**
 * A wrapper for a checkbox
 */
class KSaneButton : public KSaneOptionWidget
{
    Q_OBJECT

public:
   /**
    * Create the checkbox.
    *
    * \param parent parent widget
    * \param text is the text describing the checkbox.
    */
    KSaneButton(QWidget *parent, const QString& text);
    ~KSaneButton();

Q_SIGNALS:
    void clicked();

private:
    QPushButton *m_button;
};

}  // NameSpace KSaneIface

#endif // KSANE_BUTTON_H
