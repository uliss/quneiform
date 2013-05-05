#include "maclocale.h"
#include "macstring.h"

#import <Cocoa/Cocoa.h>

namespace utils {

void macSetLocale(const QString& locale)
{
    [[NSUserDefaults standardUserDefaults]
            setObject:[NSArray arrayWithObject: MacString::toNSString(locale)] forKey:@"AppleLanguages"];

    [[NSUserDefaults standardUserDefaults] setObject:MacString::toNSString(locale) forKey:@"AppleLocale"];
}

}
