/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#import "scanwindowcontroller.h"

@interface ScanWindowController ()

@end

@implementation ScanWindowController

-(id) init
{
    if(![super initWithWindowNibName:@"ScanWindow"])
        return nil;
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    NSLog(@"Nib file is loaded");
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

@end
