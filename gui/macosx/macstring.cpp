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

#include <CoreFoundation/CoreFoundation.h>
#include <QVarLengthArray>
#include "macstring.h"

MacString::MacString(const char * str) : str_(str)
{
}

MacString::MacString(const QString& str)
    : MacCFType<CFStringRef>(0), str_(str)
{
}

MacString::MacString(const std::string& str) : str_(str.c_str())
{
}

MacString::MacString(const CFStringRef str) : MacCFType<CFStringRef>(str)
{
}

MacString::MacString(const MacCFType<CFStringRef>& str) : MacCFType<CFStringRef>(str)
{
}

MacString::operator QString() const {
    if (str_.isEmpty() && t_)
        const_cast<MacString*>(this)->str_ = toQString(t_);

    return str_;
}

MacString::operator CFStringRef() const {
    if (!t_)
        const_cast<MacString*>(this)->t_ = toCFStringRef(str_);
    return t_;
}

QString MacString::toQString(CFStringRef str) {
    if(!str)
        return QString();

    CFIndex length = CFStringGetLength(str);
    const UniChar * chars = CFStringGetCharactersPtr(str);
    if (chars)
        return QString(reinterpret_cast<const QChar *>(chars), length);

    QVarLengthArray<UniChar> buffer(length);
    CFStringGetCharacters(str, CFRangeMake(0, length), buffer.data());
    return QString(reinterpret_cast<const QChar *>(buffer.constData()), length);
}

CFStringRef MacString::toCFStringRef(const QString& str) {
    return CFStringCreateWithCharacters(0,
                                        reinterpret_cast<const UniChar *>(str.unicode()),
                                        str.length());
}

CFStringRef MacString::toCFStringRef(const std::string& str) {
    return CFStringCreateWithCString(0, str.c_str(), kCFStringEncodingUTF8);
}

CFStringRef MacString::toCFStringRef(const char * str) {
    return CFStringCreateWithCString(0, str, kCFStringEncodingUTF8);
}
