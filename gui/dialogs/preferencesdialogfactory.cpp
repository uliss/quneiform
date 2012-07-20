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

#include <QSettings>

#include "preferencesdialogfactory.h"
#include "tabpreferencesdialog.h"
#include "listpreferencesdialog.h"
#include "preferenceswidget.h"
#include "generalpreferences.h"
#include "editorpreferences.h"
#include "debugpreferences.h"
#include "iconutils.h"
#include "settingskeys.h"

#ifdef Q_WS_MAC
#include "macpreferencesdialog.h"
#endif

void PreferencesDialogFactory::initPreferences(AbstractPreferencesDialog * dlg)
{
    if(!dlg)
        return;

    PreferencesWidget * general = new GeneralPreferences;
    PreferencesWidget * debug = new DebugPreferences;
    PreferencesWidget * editor = new EditorPreferences;

    PreferencesList prefs;
    prefs.append(general);
    prefs.append(editor);
    prefs.append(debug);

    dlg->setPages(prefs);
}

AbstractPreferencesDialog * PreferencesDialogFactory::make()
{
    AbstractPreferencesDialog * res = NULL;

#ifdef Q_WS_MAC
    res = new MacPreferencesDialog(NULL);
#else
    res = new ListPreferencesDialog(NULL);
#endif

    initPreferences(res);
    return res;
}
