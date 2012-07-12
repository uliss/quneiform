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

#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H

namespace utils
{

/**
 * This function does many things,
 * first, it checks if Image Capture plugin is installed
 * in user home directory (~/Library/Workflow/Applications/Image Capture).
 * If no plugin found, it creates it.
 *
 * Then, it checks if plugin application is same as current application.
 * Doing this allowes application bundle to be moved easily, while Image Capture
 * will scan images directly in right application.
 * If current application path differs from plugin - it updated.
 *
 * After that, it runs Image Capture in separate process
 *
 * @brief open Image Capture program
 * @return false if some stages failed
 */
bool openImageCapture();

}

#endif // IMAGECAPTURE_H
