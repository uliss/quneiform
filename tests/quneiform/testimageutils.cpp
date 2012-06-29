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

#include <QTest>
#include <QDebug>
#include <QtPlugin>

#include "testimageutils.h"
#include "imageutils.h"

#ifndef CF_IMAGE_DIR
#define CF_IMAGE_DIR
#endif

void TestImageUtils::testImageCount()
{
    QStringList files;
    files << CF_IMAGE_DIR "/german.png"    // 1
          << CF_IMAGE_DIR "/quneiform.pdf" // 2
          << CF_IMAGE_DIR "/multipage.tif" // 2
          << CF_IMAGE_DIR "/english.png";  // 1

    QCOMPARE(utils::imageCount(files), 6);
}

Q_IMPORT_PLUGIN(multitiff_imageplugin)
Q_IMPORT_PLUGIN(pdf_imageplugin)

QTEST_MAIN(TestImageUtils)
