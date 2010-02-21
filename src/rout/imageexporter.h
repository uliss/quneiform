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

#ifndef IMAGEEXPORTER_H_
#define IMAGEEXPORTER_H_

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include "common/exception.h"

namespace CIF
{

class ImageExporter
{
    public:
        virtual ~ImageExporter();

        typedef RuntimeExceptionImpl<ImageExporter> Exception;

        std::string outputFilename() const;
        virtual void save(void * data, size_t dataSize, const std::string& path);
        virtual void save(void * data, size_t dataSize, std::ostream& os) = 0;
        void setOutputFilename(const std::string& filename);
    private:
        std::string output_filename_;
};

typedef boost::shared_ptr<ImageExporter> ImageExporterPtr;

}

#endif /* IMAGEEXPORTER_H_ */
