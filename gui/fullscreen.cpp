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
#include <QMainWindow>

#include "fullscreen.h"

#ifdef Q_WS_MAC
#include "macosx/macfullscreen.h"
#endif

namespace utils {

void addFullScreenWidget(QMainWindow * window)
{
#ifdef Q_WS_MAC
    macAddFullScreen(window);
#endif
}

void toggleFullScreen(QMainWindow * window)
{
#ifdef Q_WS_MAC
    macToggleFullScreen(window);
#else
    if(window->isFullScreen())
        window->showNormal();
    else
        window->showFullScreen();
#endif
}

bool isFullScreen(QMainWindow * window)
{
#ifdef Q_WS_MAC
    return macIsFullScreen(window);
#else
    return window->isFullScreen();
#endif
}

}
