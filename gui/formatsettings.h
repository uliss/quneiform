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

#ifndef FORMATSETTINGS_H
#define FORMATSETTINGS_H

#include <QFlags>
#include <QChar>

namespace cf {
class FormatOptions;
}

class QDataStream;

class FormatSettings
{
public:
    FormatSettings();
    FormatSettings(const cf::FormatOptions& opts);

    enum Option {
        USE_BOLD = 0x1,
        USE_ITALIC = 0x2,
        USE_UNDERLINED = 0x4,
        USE_FONT_SIZE = 0x8,
        PRESERVE_LINE_BREAKS = 0x10,
        SHOW_ALTERNATIVES = 0x20
    };

    Q_DECLARE_FLAGS(Options, Option);

    /**
      * Saves format settings to cf::FormatOptions
      */
    void exportTo(cf::FormatOptions& opts) const;

    /**
      * Returns true if bold font used while formatting
      * @see isItalicUsed(), isUnderlinedUsed()
      */
    bool isBoldUsed() const;

    /**
      * Returns true if font size used while formatting
      * @see isBoldUsed(), iseUnderlinedUsed(), isItalicUsed()
      */
    bool isFontSizeUsed() const;

    /**
      * Returns true if italic font style used while formatting
      * @see isBoldUsed(), isUnderlinedUsed()
      */
    bool isItalicUsed() const;

    /**
      * Returns true if underlined font decoration used while formatting
      * @see isBoldUsed(), isItalicUsed()
      */
    bool isUnderlinedUsed() const;

    /**
      * Returns true if line breaks preserved while formatting
      * @see setPreserveLineBreaks()
      */
    bool preserveLineBreaks() const;

    /**
      * Sets using bold font while formatting
      * @see isBoldUsed()
      */
    void setBoldUsed(bool value);

    /**
      * Sets using font size while formatting
      * @see isFontSizeUsed()
      */
    void setFontSizeUsed(bool value);

    /**
      * Sets using italic font style while formatting
      * @see isItalicUsed()
      */
    void setItalicUsed(bool value);

    /**
      * Sets preserving of line breaks while formatting
      * @see preserveLineBreaks()
      */
    void setPreserveLineBreaks(bool value);

    /**
      * Sets displaying character alternatives
      * @see showAlternatives()
      */
    void setShowAlternatives(bool value);

    /**
      * Sets using underlined font decoration while formatting
      * @see isUnderlinedUsed()
      */
    void setUnderlinedUsed(bool value);

    /**
      * Sets unrecognized char replacement
      * @param c - symbol, that replace unrecognized characters in text output
      * @see unrecognizedChar()
      */
    void setUnrecognizedChar(QChar c);

    /**
      * Returns true if recognition alternatives shown
      * @see setShowAlternatives()
      */
    bool showAlternatives() const;

    /**
      * Returns unrecognized character replacement
      * @see unrecognizedChar()
      */
    QChar unrecognizedChar() const;
private:
    Options flags_;
    QChar unrecognized_char_;
    friend QDataStream& operator<<(QDataStream& stream, const FormatSettings& s);
    friend QDataStream& operator>>(QDataStream& stream, FormatSettings& s);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FormatSettings::Options)



#endif // FORMATSETTINGS_H
