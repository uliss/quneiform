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

#include <QDataStream>
#include "formatsettings.h"
#include "common/formatoptions.h"

FormatSettings::FormatSettings() :
        flags_(0),
        unrecognized_char_('~')
{
    flags_ |= USE_BOLD;
    flags_ |= USE_ITALIC;
    flags_ |= USE_UNDERLINED;
    flags_ |= PRESERVE_LINE_BREAKS;
}

FormatSettings::FormatSettings(const cf::FormatOptions& opts)
{
    if(opts.isBoldUsed())
        flags_ |= USE_BOLD;
    if(opts.isItalicUsed())
        flags_ |= USE_ITALIC;
    if(opts.isUnderlinedUsed())
        flags_ |= USE_UNDERLINED;
    if(opts.isFontSizeUsed())
        flags_ |= USE_FONT_SIZE;
    if(opts.preserveLineBreaks())
        flags_ |= PRESERVE_LINE_BREAKS;
    if(opts.showAlternatives())
        flags_ |= SHOW_ALTERNATIVES;

    unrecognized_char_ = opts.unrecognizedChar();
}

void FormatSettings::exportTo(cf::FormatOptions& opts) const {
    opts.useBold(isBoldUsed());
    opts.useItalic(isItalicUsed());
    opts.useUnderlined(isUnderlinedUsed());
    opts.useFontSize(isFontSizeUsed());
    opts.setPreserveLineBreaks(preserveLineBreaks());
    opts.setUnrecognizedChar(unrecognized_char_.toAscii());
    opts.setShowAlternatives(showAlternatives());
}

bool FormatSettings::isBoldUsed() const {
    return flags_ & USE_BOLD;
}

bool FormatSettings::isFontSizeUsed() const {
    return flags_ & USE_FONT_SIZE;
}

bool FormatSettings::isItalicUsed() const {
    return flags_ & USE_ITALIC;
}

bool FormatSettings::isUnderlinedUsed() const {
    return flags_ & USE_UNDERLINED;
}

bool FormatSettings::preserveLineBreaks() const {
    return flags_ & PRESERVE_LINE_BREAKS;
}

void FormatSettings::setBoldUsed(bool value) {
    if(value)
        flags_ |= USE_BOLD;
    else
        flags_ &= (~USE_BOLD);
}

void FormatSettings::setFontSizeUsed(bool value) {
    if(value)
        flags_ |= USE_FONT_SIZE;
    else
        flags_ &= (~USE_FONT_SIZE);
}

void FormatSettings::setItalicUsed(bool value) {
    if(value)
        flags_ |= USE_ITALIC;
    else
        flags_ &= (~USE_ITALIC);
}

void FormatSettings::setPreserveLineBreaks(bool value) {
    if(value)
        flags_ |= PRESERVE_LINE_BREAKS;
    else
        flags_ &= (~PRESERVE_LINE_BREAKS);
}

void FormatSettings::setShowAlternatives(bool value) {
    if(value)
        flags_ |= SHOW_ALTERNATIVES;
    else
        flags_ &= (~SHOW_ALTERNATIVES);
}

void FormatSettings::setUnderlinedUsed(bool value) {
    if(value)
        flags_ |= USE_UNDERLINED;
    else
        flags_ &= (~USE_UNDERLINED);
}

void FormatSettings::setUnrecognizedChar(QChar c) {
    unrecognized_char_ = c;
}

bool FormatSettings::showAlternatives() const {
    return flags_ & SHOW_ALTERNATIVES;
}

QChar FormatSettings::unrecognizedChar() const {
    return unrecognized_char_;
}

QDataStream& operator<<(QDataStream& os, const FormatSettings& s) {
    os << int(s.flags_)
            << s.unrecognized_char_;
    return os;
}

QDataStream& operator>>(QDataStream& is, FormatSettings& s) {
    int fl = 0;
    is >> fl;
    s.flags_ &= 0;
    s.flags_ = static_cast<FormatSettings::Options>(fl);
    is >> s.unrecognized_char_;
    return is;
}
