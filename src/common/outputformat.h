/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef OUTPUTFORMAT_H_
#define OUTPUTFORMAT_H_

#include <string>
#include <list>
#include "puma/pumadef.h"

namespace CIF
{

typedef std::list<puma_format_t> OutputFormatList;

class OutputFormat
{
    public:
        OutputFormat(puma_format_t format);
        ~OutputFormat();

        std::string description() const;
        std::string extension() const;
        puma_format_t get() const;
        std::string name() const;
    public:
        static std::string description(puma_format_t format);
        static std::string extension(puma_format_t format);
        static OutputFormatList formats();
        static std::string name(puma_format_t format);
    private:
        puma_format_t format_;
};

inline puma_format_t OutputFormat::get() const
{
    return format_;
}

}

#endif /* OUTPUTFORMAT_H_ */
