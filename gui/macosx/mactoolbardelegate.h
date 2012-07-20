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

#ifndef MACTOOLBARDELEGATE_H
#define MACTOOLBARDELEGATE_H

#import <AppKit/NSToolbar.h>
#import <AppKit/NSToolbarItem.h>

#include <QList>

class QObject;
class QIcon;
class QAction;

@interface MacToolbarDelegate : NSObject<NSToolbarDelegate>
{
@public
    NSToolbar * toolbar;
    NSMutableArray * items;
    QList<QAction*> actions;
}

-(id) init;
-(NSToolbarItem*) toolbar: (NSToolbar*) toolbar
    itemForItemIdentifier: (NSString*) itemId
    willBeInsertedIntoToolbar:(BOOL) flag;

-(NSArray*) toolbarDefaultItemIdentifiers:(NSToolbar*)toolbar;
-(NSArray*) toolbarAllowedItemIdentifiers:(NSToolbar*)toolbar;
-(BOOL) addAction:(QAction*)action;
-(IBAction) itemClicked:(id)sender;
@end


#endif // MACTOOLBARDELEGATE_H
