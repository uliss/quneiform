/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef CF_IMAGEURL_H
#define CF_IMAGEURL_H

#include <string>
#include <iosfwd>

#include "globus.h"
#include "common/serialize.h"

namespace cf
{

class CLA_EXPO ImageURL
{
public:
    ImageURL();
    explicit ImageURL(const std::string& path, int imageNumber = 0);

    /**
     * Returns true if image path is empty
     */
    bool empty() const;

    /**
     * Returns true if image path exists
     */
    bool exists() const;

    /**
     * Returns image filename extension in lower case
     */
    std::string extension() const;

    /**
     * Returns image number if file contains severeval images inside,
     * like PDF, TIFF and other formats.
     * @see setImageNumber()
     */
    int imageNumber() const;

    bool operator==(const ImageURL& url) const;
    bool operator!=(const ImageURL& url) const { return !operator==(url); }

    /**
     * Returns image file path
     * @see setPath()
     */
    std::string path() const;

    /**
     * Sets image number
     * @see imageNumber()
     */
    void setImageNumber(int number);

    /**
     * Sets image path
     * @param path - new image path
     * @see path()
     */
    void setPath(const std::string& path);

    /**
     * Returns true if image url is simple, with image number == 0
     */
    bool simple() const;
private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("path", path_);
            ar & make_nvp("image-number", image_number_);
        }
#endif
private:
    std::string path_;
    int image_number_;
};

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const cf::ImageURL& url);

}


#endif // CF_IMAGEURL_H
