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

#include <QtGlobal>
#include <QSettings>
#include <QDir>
#include <QDebug>

#include "iconutils.h"
#include "settingskeys.h"

QStringList availableIconThemes()
{
    QDir icon_dir(":/icons");
    return icon_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
}

static void setDefaultIconTheme()
{
#ifdef Q_WS_MAC
    QIcon::setThemeName("faenza");
#else
    QIcon::setThemeName("");
#endif
}

void iconThemeSetup()
{
    QSettings settings;

    QVariant theme = settings.value(KEY_ICON_THEME);
    if(theme.isValid()) {
        QIcon::setThemeName(theme.toString());
        if(!QIcon::hasThemeIcon("zoom-in"))
            setDefaultIconTheme();
    }
    else {
        setDefaultIconTheme();
    }
}

QIcon iconFromTheme(const QString& name)
{
    return QIcon::fromTheme(name, QIcon(QString(":/icons/%1.png").arg(name)));
}

