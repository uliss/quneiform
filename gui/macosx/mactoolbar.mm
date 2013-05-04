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

#include <QWidget>
#include <QDebug>
#include <QAction>
#import <AppKit/NSToolbar.h>
#import <AppKit/NSView.h>
#include <AppKit/NSWindow.h>

#include "mactoolbar.h"
#include "mactoolbardelegate.h"
#include "macpool.h"

static NSToolbar * makeNSToolbar()
{
    NSToolbar * toolbar = [[NSToolbar alloc] initWithIdentifier:@"MacToolbar"];
    [toolbar setAllowsUserCustomization:NO];
    [toolbar setAutosavesConfiguration:NO];
    [toolbar setDisplayMode:NSToolbarDisplayModeIconAndLabel];
    [toolbar setSizeMode:NSToolbarSizeModeRegular];
    return toolbar;
}

static NSWindow * widgetNSWindow(QWidget * widget)
{
    if(!widget)
        return nil;

    NSView * view = (NSView*) widget->winId();
    return [view window];
}

static NSToolbar * widgetNSToolbar(QWidget * widget)
{
    NSWindow * window = widgetNSWindow(widget);
    if(!window)
        return nil;

    return [window toolbar];
}

bool macAddToolBar(QWidget* widget, const QList<QAction*> actions)
{
    if(!widget) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return false;
    }

    if(!widget->isWindow()) {
        qWarning() << Q_FUNC_INFO << "not a window widget" << widget;
        return false;
    }

    NSWindow * window = widgetNSWindow(widget);
    if([window toolbar] != nil) {
        qWarning() << Q_FUNC_INFO << "tool bar already exists";
        return false;
    }

    NSToolbar * toolbar = makeNSToolbar();
    MacToolbarDelegate * delegate = [[MacToolbarDelegate alloc] init];

    foreach(QAction * a, actions) {
        [delegate addAction: a];
    }

    [toolbar setDelegate:delegate];
    [window setToolbar:toolbar];

    [toolbar release];
    return true;
}

bool macToolBarSelect(QWidget * widget, int idx)
{
    if(!widget) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return false;
    }

    if(!widget->isWindow()) {
        qWarning() << Q_FUNC_INFO << "not a window widget" << widget;
        return false;
    }

    NSToolbar * tb = widgetNSToolbar(widget);
    if(tb == nil) {
        qWarning() << Q_FUNC_INFO << "toolbar not exists";
        return false;
    }

    if(idx >= 0 && idx < [[tb items] count]) {
        NSToolbarItem * item = [[tb items] objectAtIndex:idx];
        [tb setSelectedItemIdentifier:[item itemIdentifier]];
    }
    else {
        NSLog(@"invalid toolbar index: %@", idx);
        return false;
    }

    return true;
}

bool macRemoveToolBar(QWidget * widget)
{
    if(!widget) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return false;
    }

    if(!widget->isWindow()) {
        qWarning() << Q_FUNC_INFO << "not a window widget" << widget;
        return false;
    }

    NSWindow * window = widgetNSWindow(widget);
    NSToolbar * tb = widgetNSToolbar(widget);
    if(tb == nil) {
        qWarning() << Q_FUNC_INFO << "toolbar not exists";
        return false;
    }

    [window setToolbar: nil];
    return true;
}

