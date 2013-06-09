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

#import <AppKit/NSWorkspace.h>

#include "macopenfile.h"
#include "macstring.h"
#include "macpool.h"

namespace utils {

bool macOpenFile(const QString& fullPath)
{
    const MacPool pool;
    const MacString file(fullPath);

    BOOL rc = [[NSWorkspace sharedWorkspace]
            openFile: (NSString *)(CFStringRef) file
    ];

    return rc;
}

bool macOpenFileWithApplication(const QString& fullPath, const QString& appName)
{
    const MacPool pool;

    BOOL rc = [[NSWorkspace sharedWorkspace]
            openFile: MacString::toNSString(fullPath)
            withApplication: MacString::toNSString(appName)
     ];

    return rc;
}

bool macLaunchApplication(const QString& appName)
{
    const MacPool pool;

    BOOL rc = [[NSWorkspace sharedWorkspace]
            launchApplication: MacString::toNSString(appName)
     ];

    return rc;
}

}
