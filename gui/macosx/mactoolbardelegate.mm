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

#include <QAction>
#include <QDebug>

#include "mactoolbardelegate.h"
#include "macstring.h"
#include "macpool.h"

static NSImage * toNSImage(const QPixmap& pixmap)
{
    NSBitmapImageRep * bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:pixmap.toMacCGImageRef()];
    NSImage * image = [[NSImage alloc] init];
    [image addRepresentation:bitmapRep];
    [bitmapRep release];
    return image;
}

@implementation MacToolbarDelegate

-(id) init
{
    self = [super init];
    items = [[NSMutableArray alloc] init];
    return self;
}

-(void) dealloc
{
    [toolbar dealloc];
    [items dealloc];
    [super dealloc];
}

-(NSArray*) toolbarDefaultItemIdentifiers: (NSToolbar*) tb
{
    Q_UNUSED(tb);
    NSMutableArray * res = [[NSMutableArray alloc] init];

    for(NSString * s in items) {
        [res addObject: s];
    }

    return res;
}

-(NSArray*) toolbarAllowedItemIdentifiers:(NSToolbar*) tb
{
    return [self toolbarDefaultItemIdentifiers: tb];
}

-(IBAction) itemClicked:(id)sender
{
    NSToolbarItem * item = reinterpret_cast<NSToolbarItem*>(sender);
    QString id = MacString::toQString([item itemIdentifier]);

    foreach(QAction * a, actions) {
        if(a->text() == id) {
            a->activate(QAction::Trigger);
            break;
        }
    }
}

-(NSToolbarItem*) toolbar: (NSToolbar *) tb
    itemForItemIdentifier: (NSString *) itemIdentifier
    willBeInsertedIntoToolbar:(BOOL) flag
{
    Q_UNUSED(tb);
    Q_UNUSED(flag);

    NSToolbarItem * toolbarItem = nil;
    foreach(QAction * a, actions) {
        CFStringRef itemName = MacString::toCFStringRef(a->text());

        if([itemIdentifier isEqualTo:(NSString*) itemName]) {
            toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier: (NSString*) itemIdentifier];
            [toolbarItem setLabel:itemIdentifier];
            [toolbarItem setPaletteLabel:[toolbarItem label]];

            if(!a->icon().isNull()) {
                QPixmap pixmap = a->icon().pixmap(64, 64);
                [toolbarItem setImage: toNSImage(pixmap)];
            }

            if(!a->toolTip().isEmpty()) {
                CFStringRef toolTip = MacString::toCFStringRef(a->toolTip());
                [toolbarItem setToolTip:(NSString*) toolTip];
            }

            [toolbarItem setTarget:self];
            [toolbarItem setAction:@selector(itemClicked:)];
            break;
        }
    }

    return [toolbarItem autorelease];
}

-(BOOL) addAction: (QAction*) action
{
    if(!action) {
        qWarning() << Q_FUNC_INFO << "NULL action pointer given";
        return NO;
    }

    const MacPool pool;

    CFStringRef itemName = MacString::toCFStringRef(action->text());
    [items addObject: (NSString*) itemName];

    actions.append(action);
    return YES;
}

@end
