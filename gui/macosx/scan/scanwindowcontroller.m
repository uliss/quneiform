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

@implementation ScanDelegate

- (void)scannerDeviceView:(IKScannerDeviceView *)scannerDeviceView didEncounterError:(NSError *)error
{
    NSLog(@"%@", error);
}

- (void)scannerDeviceView:(IKScannerDeviceView *)scannerDeviceView didScanToURL:(NSURL *)url fileData:(NSData *)data error:(NSError *)error
{
    if(url)
        NSLog(@"Scanned to %@", url);

    if(error)
        NSLog(@"%@", error);
}

@end

@implementation ScanWindowController

-(void)dealloc
{
    [super dealloc];
    NSLog(@"dealloc called");
}


-(id) init
{
    if(![super initWithWindowNibName:@"ScanWindow"])
        return nil;

//    windowDelegate = [[ScanWindowDelegate alloc] init];
//    [[self window] setDelegate:windowDelegate];
    
    return self;
}

- (void) windowDidLoad
{
    [super windowDidLoad];

    [devView setMode:IKScannerDeviceViewDisplayModeSimple];
    [devView setScanControlLabel:NSLocalizedString(@"Scan", @"scan buttton label")];
    [devView setDocumentName:NSLocalizedString(@"Page", @"scanned page name")];
    [devView setDisplaysPostProcessApplicationControl:NO];
    [devView setDisplaysDownloadsDirectoryControl:YES];
    [devView setDownloadsDirectory:[NSURL URLWithString:@"/Users/serj/Pictures"]];
    [devView setTransferMode:IKScannerDeviceViewTransferModeFileBased];
    [devView setPostProcessApplication:[NSURL URLWithString:@""]];


    scanDelegate = [[ScanDelegate alloc] init];
    [devView setDelegate: scanDelegate];
}

@end
