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

#include "testhistogramcreator.h"
#include "rimage/histogramcreator.h"
#include "rdib/magickimageloader.h"
#include "common/imageurl.h"

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(TestHistogramCreator);

#define IMAGE(name) cf::ImageURL(LOADER_TEST_IMAGE_DIR "/" name)

using namespace cf;

void TestHistogramCreator::brightnessHistogram()
{
    MagickImageLoader l;
    ImagePtr img = l.load(IMAGE("color_24.bmp"));
    CTDIB image;
    image.setBitmap(img->data());
    Histogram res(0);
    CPPUNIT_ASSERT(HistogramCreator::grayBrighness(res, image));
    CPPUNIT_ASSERT_EQUAL(size_t(272), res.sum());
    CPPUNIT_ASSERT_EQUAL(size_t(49045), res.weightedSum());
    CPPUNIT_ASSERT_EQUAL(size_t(256), res.size());
}
