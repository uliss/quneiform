/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavsky                                 *
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

#include <QMainWindow>
#import <AppKit/AppKit.h>

#include "macfullscreen.h"

void macAddFullScreen(QMainWindow * window)
{
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    NSView *nsview = (NSView *) window->winId();
    NSWindow *nswindow = [nsview window];
    [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
#endif
}


void macToggleFullScreen(QMainWindow * window)
{
    if(!window)
        return;
    NSView *nsview = (NSView *) window->winId();
    NSWindow *nswindow = [nsview window];
    [nswindow toggleFullScreen:nil];
}

bool macIsFullScreen(QMainWindow * window)
{
    if(!window)
        return false;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7

    NSView *nsview = (NSView *) window->winId();
    NSWindow *nswindow = [nsview window];
    NSUInteger masks = [nswindow styleMask];
    return masks & NSFullScreenWindowMask;
#else
    return window->isFullScreen();
#endif
}
