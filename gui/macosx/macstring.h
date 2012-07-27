/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef MACSTRING_H
#define MACSTRING_H

#include <QString>
#include <string>
#import <Foundation/NSString.h>

#include "maccftype.h"

class MacString : public MacCFType<CFStringRef>
{
public:
    MacString(const char * str);
    MacString(const QString& str);
    MacString(const std::string& str);
    MacString(const CFStringRef str = 0);
    MacString(const MacCFType<CFStringRef>& str);

    operator QString() const;
    operator CFStringRef() const;
    static QString toQString(CFStringRef cfstr);
    static QString toQString(NSString * str);
    static NSString * toNSString(const QString& str);
    static CFStringRef toCFStringRef(const QString& str);
    static CFStringRef toCFStringRef(const std::string& str);
    static CFStringRef toCFStringRef(const char * str);
private:
    QString str_;
};

#endif // MACSTRING_H
