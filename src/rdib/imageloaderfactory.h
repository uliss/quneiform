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

#ifndef IMAGELOADERFACTORY_H_
#define IMAGELOADERFACTORY_H_

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>

#include "common/image.h"
#include "common/imageformats.h"
#include "imageloader.h"
#include "globus.h"

namespace cf
{

class CLA_EXPO ImageLoaderFactory : boost::noncopyable
{
    public:
        static ImageLoaderFactory& instance();
    public:
        typedef ImageLoader * (*loaderCreate)();
        ImagePtr load(const std::string& filename);
        ImagePtr load(std::istream& stream);
        ImageLoader& loader(image_format_t format);
        bool registerCreator(image_format_t format, int gravity, loaderCreate creator);
    private:
        typedef std::pair<int, loaderCreate> LoaderEntry;
        typedef std::multimap<image_format_t, LoaderEntry> LoaderMap;
        ImageLoader& unknownLoader();
        LoaderMap loader_map_;
        typedef boost::shared_ptr<ImageLoader> ImageLoaderPtr;
        typedef std::vector<ImageLoaderPtr> ImageLoadersList;
        ImageLoadersList loaders_list_;
    private:
        ImageLoaderFactory();
        void checkImageExists(const std::string& filename);
};

}

#endif /* IMAGELOADERFACTORY_H_ */
