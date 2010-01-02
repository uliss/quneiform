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

#ifndef IMAGEFORMATDETECTOR_H_
#define IMAGEFORMATDETECTOR_H_

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>

#include "imageformats.h"

namespace CIF
{

class ImageFormatDetector: public boost::noncopyable
{
    public:
        static ImageFormatDetector& instance();
        image_format_t detect(const std::string& filename) const;
        image_format_t detect(std::istream& stream) const;
        std::vector<std::string> knownExtensions() const;
    private:
        ImageFormatDetector();
    private:
        typedef std::map<std::string, image_format_t> FormatMap;
        FormatMap extension_format_map_;
        typedef std::map<std::string, image_format_t> MagickMap;
        MagickMap magick_format_map_;
};

}

#endif /* IMAGEFORMATDETECTOR_H_ */
