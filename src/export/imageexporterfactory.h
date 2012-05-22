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

#ifndef IMAGEEXPORTERFACTORY_H_
#define IMAGEEXPORTERFACTORY_H_

#include <map>

#include "common/singleton.h"
#include "imageexporter.h"

namespace cf
{

class ImageExporterFactoryImpl
{
public:
    ~ImageExporterFactoryImpl();
    ImageExporterPtr make();
    void registerCreator(image_format_t format, ImageExporter::Creator creator, int gravity);

    typedef std::pair<ImageExporter::Creator, int> Creator;
    typedef std::multimap<image_format_t, Creator> CreatorMap;
    typedef CreatorMap::iterator Iterator;
    typedef std::pair<Iterator, Iterator> Range;
    typedef CreatorMap::value_type Value;
private:
    Iterator findBestCreator(image_format_t format);
private:
    CreatorMap map_;
};

typedef Singleton<ImageExporterFactoryImpl> ImageExporterFactory;

}

#endif /* IMAGEEXPORTERFACTORY_H_ */
