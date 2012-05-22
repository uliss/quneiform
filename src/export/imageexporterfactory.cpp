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

#include <limits>

#include "imageexporterfactory.h"
#include "bmpimageexporter.h"
#include "common/debug.h"

namespace cf
{

ImageExporterFactoryImpl::~ImageExporterFactoryImpl()
{}

ImageExporterPtr ImageExporterFactoryImpl::make()
{
    image_format_t format = FORMAT_JPEG;
    Iterator it = findBestCreator(format);

    if(it == map_.end()) {
        Debug() << "[WARNING] " << BOOST_CURRENT_FUNCTION
                << " creator not found for format: "
                << format << "\n";

        return ImageExporterPtr(new BmpImageExporter);
    }

    ImageExporter::Creator creator = it->second.first;
    assert(creator);
    return creator();
}

void ImageExporterFactoryImpl::registerCreator(image_format_t format, ImageExporter::Creator creator, int gravity)
{
    map_.insert(Value(format, Creator(creator, gravity)));
}

static inline bool creatorCompare(ImageExporterFactoryImpl::Value first, ImageExporterFactoryImpl::Value second)
{
    return first.second < second.second;
}

ImageExporterFactoryImpl::Iterator ImageExporterFactoryImpl::findBestCreator(image_format_t format)
{
    Range range = map_.equal_range(format);

    if(range.first == range.second)
        return map_.end();

    Iterator best = std::max_element(range.first, range.second, creatorCompare);

    if(best == range.second)
        return map_.end();

    return best;
}

}
