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

#ifndef BMPIMAGEEXPORTER_H_
#define BMPIMAGEEXPORTER_H_

#include "imageexporter.h"

namespace cf
{

class BmpImageExporter : public ImageExporter
{
    public:
        BmpImageExporter();

        /**
          * Returns  "image/x-ms-bmp"
          */
        std::string mime() const;

        FormatList supportedFormats() const;
    protected:
        void saveToStream(const ImageRawData& image, std::ostream& os);
};

}

#endif /* BMPIMAGEEXPORTER_H_ */
