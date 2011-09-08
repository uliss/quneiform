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

#ifndef FORMATTER_H_
#define FORMATTER_H_

#include <cstdio>
#include <string>

#include "globus.h"
#include "common/formatoptions.h"
#include "ced/cedpageptr.h"

namespace cf
{

class CRtfPage;

class CLA_EXPO Formatter
{
    public:
        Formatter(const FormatOptions& opt = FormatOptions());
        ~Formatter();
        /**
         * Formats file
         * @return pointer to created CEDPage document
         * @note caller should free return value
         */
        CEDPagePtr format(const std::string& fileName);

        /**
         * Returns format options
         */
        FormatOptions options() const;

        /**
         * Returns pointer to formatted page tree
         */
        CRtfPage * page();

        /**
         * Sets format options
         */
        void setOptions(const FormatOptions& opts);
    private:
        CEDPage * readFormatFile(const std::string& imageName, FILE * fp);
        void setFontOptions() const;
        void setInnerOptions() const;
        void setupPage();
        bool writeFormatFile(FILE * fp);
    private:
        FormatOptions opts_;
        CRtfPage * page_;
    public:
        static bool extended_mode_;
        static char unrecognized_char;
};

}

#endif /* FORMATTER_H_ */
