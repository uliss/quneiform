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

#include <QIcon>
#include <QLabel>

#include "preferencesdialogfactory.h"
#include "tabpreferencesdialog.h"
#include "iconutils.h"

static void initPreferences(AbstractPreferencesDialog * dlg)
{
    if(!dlg)
        return;

    dlg->addCategoryWidget(iconFromTheme("settings"), "General", new QLabel("QLabel"));
    dlg->addCategoryWidget(iconFromTheme("debug"), "Debug", new QLabel("Debug"));
}

AbstractPreferencesDialog * PreferencesDialogFactory::make(QWidget * parent)
{
    AbstractPreferencesDialog * res = new TabPreferencesDialog(parent);
    initPreferences(res);
    return res;
}
