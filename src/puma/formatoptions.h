/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef FORMATOPTIONS_H_
#define FORMATOPTIONS_H_

#include <string>
#include <iostream>
#include <cstddef>

#include "pumadef.h"
#include "lang_def.h"

namespace CIF {

class FormatOptions
{
public:
    FormatOptions();
    virtual ~FormatOptions();

    puma_format_mode_t formatMode() const;
    bool isBoldUsed() const;
    bool isFontSizeUsed() const;
    bool isItalicUsed() const;
    language_t language() const;
    std::string monospaceName() const;
    std::string sansSerifName() const;
    std::string serifName() const;
    void setFormatMode(puma_format_mode_t format);
    void setLanguage(language_t lang);
    void setMonospaceName(const std::string& name);
    void setSansSerifName(const std::string& name);
    void setSerifName(const std::string& name);
    void setUnrecognizedChar(wchar_t ch);
    void setUnrecognizedChar(char ch);
    wchar_t unrecognizedChar() const;
    void useBold(bool val = true);
    void useFontSize(bool val = true);
    void useItalic(bool val = true);
private:
    std::string serif_name_;
    std::string sans_serif_name_;
    std::string monospace_name_;
    bool use_bold_;
    bool use_italic_;
    bool use_font_size_;
    puma_format_mode_t format_mode_;
    wchar_t unrecognized_char_;
    language_t language_;
};

std::ostream& operator<<(std::ostream& os, const FormatOptions& fmt);

}

#endif /* FORMATOPTIONS_H_ */
