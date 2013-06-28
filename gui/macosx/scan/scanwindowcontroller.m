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

@implementation ScanWindowController

-(id) initWithCallback:(scannedCallback) callback
{
    if(![super initWithWindowNibName:@"ScanWindow"])
        return nil;

    callback_ = callback;

    return self;
}

- (void) windowDidLoad
{
    [super windowDidLoad];

    [devView setScanControlLabel:NSLocalizedString(@"Scan", @"scan buttton label")];
    [devView setOverviewControlLabel:NSLocalizedString(@"Overview", @"overview buttton label")];
    [devView setDocumentName:NSLocalizedString(@"Page", @"scanned document name")];
    [devView setDisplaysDownloadsDirectoryControl:YES];
    [devView setDownloadsDirectory:[self picturesDirectory]];
    [devView setDelegate:self];

    [[self window] setTitle:NSLocalizedString(@"Image scanning", @"scanning dialog title")];
    [[self window] setReleasedWhenClosed:NO];
    [[self window] setDelegate:self];
}

- (NSURL*)picturesDirectory
{
    NSArray * possibleURLs = [[NSFileManager defaultManager] URLsForDirectory:NSPicturesDirectory inDomains:NSUserDomainMask];

    if ([possibleURLs count] >= 1)
        return [possibleURLs objectAtIndex:0];

    return [[NSBundle mainBundle] bundleURL];
}

- (void)windowWillClose:(NSNotification *)notification
{

}

- (void)showDialog
{
    [self showWindow:nil];
}

- (void)scannerDeviceView:(IKScannerDeviceView *)scannerDeviceView didScanToURL:(NSURL *)url fileData:(NSData *)data error:(NSError *)error
{
    if(url) {
        const char * path = [[url path] UTF8String];
        callback_(path);
    }

    if(error)
        NSLog(@"%@", error);
}

@end
