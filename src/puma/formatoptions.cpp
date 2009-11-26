/*
 * formatoptions.cpp
 *
 *  Created on: 18.10.2009
 *      Author: uliss
 */

#include <iomanip>

#include "formatoptions.h"

namespace CIF
{

FormatOptions::FormatOptions() :
        serif_name_("Times New Roman"), sans_serif_name_("Arial"), monospace_name_("Courier New"),
        use_bold_(true), use_italic_(true), use_font_size_(true),
        format_mode_(PUMA_FORMAT_ALL), unrecognized_char_('~'), language_(LANG_RUSENG)
{
}

FormatOptions::~FormatOptions()
{
}

puma_format_mode_t FormatOptions::formatMode() const
{
    return format_mode_;
}

bool FormatOptions::isBoldUsed() const
{
    return use_bold_;
}

bool FormatOptions::isFontSizeUsed() const
{
    return use_font_size_;
}

bool FormatOptions::isItalicUsed() const
{
    return use_italic_;
}

language_t FormatOptions::language() const
{
    return language_;
}

std::string FormatOptions::monospaceName() const
{
    return monospace_name_;
}

std::string FormatOptions::sansSerifName() const
{
    return sans_serif_name_;
}

std::string FormatOptions::serifName() const
{
    return serif_name_;
}

void FormatOptions:: setFormatMode(puma_format_mode_t format)
{
    format_mode_ = format;
}

void FormatOptions::setLanguage(language_t lang)
{
    language_ = lang;
}

void FormatOptions::setMonospaceName(const std::string & name)
{
    monospace_name_ = name;
}

void FormatOptions::setSansSerifName(const std::string & name)
{
    sans_serif_name_ = name;
}

void FormatOptions::setSerifName(const std::string & name)
{
    serif_name_ = name;
}

void FormatOptions::setUnrecognizedChar(wchar_t ch)
{
    unrecognized_char_ = ch;
}

void FormatOptions::setUnrecognizedChar(char ch)
{
    unrecognized_char_ = ch;
}

wchar_t FormatOptions::unrecognizedChar() const
{
    return unrecognized_char_;
}

void FormatOptions::useBold(bool val)
{
    use_bold_ = val;
}

void FormatOptions::useFontSize(bool val)
{
    use_font_size_ = val;
}

void FormatOptions::useItalic(bool val)
{
    use_italic_ = val;
}

std::ostream & operator <<(std::ostream & os, const FormatOptions & fmt)
{
    using namespace std;
    os << "FormatOptions:\n";
    os << setw(20) << "   SerifName:" << fmt.serifName() << "\n";
    os << setw(20) << "   SansSerifName:" << fmt.sansSerifName() << "\n";
    os << setw(20) << "   Monospace Name:" << fmt.monospaceName() << "\n";
    os << setw(20) << "   Use bold:" << fmt.isBoldUsed() << "\n";
    os << setw(20) << "   Use Italic: " << fmt.isItalicUsed() << "\n";
    os << setw(20) << "   Use font size: " << fmt.isFontSizeUsed() << "\n";
    os << setw(20) << "   Unrecognized char: " << (char) (fmt.unrecognizedChar()) << "\n";
    return os;
}

}
