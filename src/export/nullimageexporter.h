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

#ifndef NULLIMAGEEXPORTER_H_
#define NULLIMAGEEXPORTER_H_

#include "imageexporter.h"

namespace CIF
{

class NullImageExporter: public ImageExporter
{
    public:
        NullImageExporter();

        /**
         * Returns empty string
         */
        std::string mime() const;

        void save(const ImageRawData& image, std::ostream&);

        /**
         * Returns empty list
         * @return
         */
        FormatList supportedFormats() const;
};

}

#endif /* NULLIMAGEEXPORTER_H_ */
