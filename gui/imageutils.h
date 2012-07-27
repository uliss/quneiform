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

#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

class QStringList;
class QString;

namespace utils
{

/**
 * Returns number of images in file list.
 * @note for multipage formats, like PDF or TIFF, returned value
 * can be more than file list size.
 * @param files - list of full paths to images
 * @return image count
 */
int imageCount(const QStringList& files);

/**
 * Returns true if given file looks like multi page image
 * @param path - full path to image
 * @return true on success
 */
bool looksLikeMultiPageDocument(const QString& path);

/**
 * Returns QImageReader format for given file
 */
const char * imagePluginFormat(const QString& fullPath);

}

#endif // IMAGEUTILS_H
